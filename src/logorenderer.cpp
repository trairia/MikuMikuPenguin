#include <GL/glew.h>

#include "logorenderer.h"
#include <QPainter>
#include <QPaintEngine>
#include <math.h>

#include "motioncontroller.h"
#include "pmx.h"
#include "vmd.h"
#include "shader.h"

#include <SOIL/SOIL.h>

#include "bulletphysics.h"
#include "mmdphysics.h"

using namespace ClosedMMDFormat;
using namespace std;

LogoRenderer::LogoRenderer(QWindow *window)
{
	//this->glFuncs=(QOpenGLFunctions_4_3_Core*)glFuncs;
	/*QOpenGLContext *context;
	QAbstractOpenGLFunctions *funcs =  context->versionFunctions();
	if ( !funcs )
	{
		qWarning("Could not obtain OpenGL versions object");
		exit( 1 );
	}
	funcs->initializeOpenGLFunctions();
	
	glFuncs=(QOpenGLFunctions_4_3_Core*) funcs;*/
	glewInit();
	
	init();
}

void LogoRenderer::init()
{
	pmxInfo=&readPMX(DATA_PATH"/model/tdamiku/","tdamiku.pmx");
	vmdInfo=&readVMD(DATA_PATH"/motion/私の時間/私の時間_short_Lat式ミク.vmd");
	
	ifstream test("shaders/model.vert");
	if(!test.is_open())
	{
		shaderProgram=compileShaders(DATA_PATH"/shaders/model.vert",DATA_PATH"/shaders/model.frag");
	}
	else
	{
		shaderProgram=compileShaders("shaders/model.vert","shaders/model.frag");
	}
	test.close();
	
	ifstream test2("shaders/bulletDebug.vert");
	if(!test2.is_open())
	{
		//bulletVertPath=DATA_PATH"/shaders/bulletDebug.vert";
		//bulletFragPath=DATA_PATH"/shaders/bulletDebug.frag";
	}
	else
	{
		//bulletVertPath="shaders/bulletDebug.vert";
		//bulletFragPath="shaders/bulletDebug.frag";
	}
	
	loadTextures();
	initBuffers();
	setVertexAttributes();
	linkShaders(shaderProgram);
	glUseProgram(shaderProgram);
	
	initUniformVarLocations();
	
	MVP_loc = glGetUniformLocation(shaderProgram, "MVP");	
	
	
	//Set OpenGL render settings
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPointSize(5.0);
	//glClearDepth(1.0f);
	
	//glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
	//glClearColor(1,1,1,1);
	glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
	//glDisable(GL_MULTISAMPLE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//Setup MotionController, Physics
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[VertexArrayBuffer]);
	motionController=new VMDMotionController(*pmxInfo,*vmdInfo,shaderProgram);
	
	bulletPhysics = new BulletPhysics(bulletVertPath,bulletFragPath);
	glUseProgram(shaderProgram); //restore GL shader program binding to Viewer's shader program after initializing BulletPhysic's debugDrawer
	mmdPhysics = new MMDPhysics(*pmxInfo,motionController,bulletPhysics);
	
	motionController->updateVertexMorphs();
	motionController->updateBoneAnimation();
	
	modelTranslate=glm::vec3(0.0f,-10.0f,-15.0f);
	
	cameraPosition=glm::vec3(0.0f,0.0f,radius*sin(theta));
	cameraTarget=glm::vec3(0.0f,0,0.0f);
}

void LogoRenderer::loadTextures()
{
	for(int i=0; i<pmxInfo->texture_continuing_datasets; ++i)
	{
		cout<<"Loading "<<pmxInfo->texturePaths[i]<<"...";
		if(pmxInfo->texturePaths[i].substr(pmxInfo->texturePaths[i].size()-3)=="png" || pmxInfo->texturePaths[i].substr(pmxInfo->texturePaths[i].size()-3)=="spa")
		{
			GLuint texture;
			int width, height;
			unsigned char* image;
			string loc=pmxInfo->texturePaths[i];
			
			ifstream test(loc);
			if(!test.is_open())
			{
				cerr<<"Texture file could not be found: "<<loc<<endl;
				//exit(EXIT_FAILURE);
			}
			test.close();
			
			glActiveTexture( GL_TEXTURE0 );
			glGenTextures( 1, &texture );
			glBindTexture( GL_TEXTURE_2D, texture );
				image = SOIL_load_image( loc.c_str(), &width, &height, 0, SOIL_LOAD_RGBA );
			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image );
			SOIL_free_image_data( image );

			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			
			if(texture==0)
			{
				cerr<<"Texture failed to load: "<<pmxInfo->texturePaths[i]<<endl;
				cerr<<glGetError()<<endl;
				printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
				exit(EXIT_FAILURE);
			}
			
			cout<<"done"<<endl;
			
			textures.push_back(texture);
		}
		else if(pmxInfo->texturePaths[i].substr(pmxInfo->texturePaths[i].size()-3)=="tga")
		{
			//cerr<<"WARNING: TGA files only mildly tested"<<endl;
			
			GLuint texture;
			int width, height, channels;
			unsigned char* image;
			string loc=pmxInfo->texturePaths[i];
			
			ifstream test(loc);
			if(!test.is_open())
			{
				cerr<<"Texture file could not be found: "<<loc<<endl;
				//exit(EXIT_FAILURE);
			}
			test.close();
			
			glActiveTexture( GL_TEXTURE0 );
			glGenTextures( 1, &texture );
			glBindTexture( GL_TEXTURE_2D, texture );
				image = SOIL_load_image( loc.c_str(), &width, &height, &channels, SOIL_LOAD_RGBA );
			
			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image );
			SOIL_free_image_data( image );

			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			
			if(glIsTexture(texture)==GL_FALSE)
			{
				cerr<<"Texture failed to load: "<<pmxInfo->texturePaths[i]<<endl;
				cerr<<glGetError()<<endl;
				printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
				exit(EXIT_FAILURE);
			}
			
			cout<<"done"<<endl;
			
			textures.push_back(texture);
		}
		else
		{
			GLuint texture;
			int width, height;
			unsigned char* image;
			string loc=pmxInfo->texturePaths[i];
			
			ifstream test(loc);
			if(!test.is_open())
			{
				cerr<<"Texture file could not be found: "<<loc<<endl;
				//exit(EXIT_FAILURE);
			}
			test.close();
			
			glActiveTexture(GL_TEXTURE0);
			glGenTextures( 1, &texture );
			glBindTexture( GL_TEXTURE_2D, texture );
			image = SOIL_load_image( loc.c_str(), &width, &height, 0, SOIL_LOAD_RGB );
			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
			SOIL_free_image_data( image );
			
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
 
			if(texture == 0)
			{
				cerr<<"Texture failed to load: "<<pmxInfo->texturePaths[i]<<endl;
				cerr<<glGetError()<<endl;
				printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
				exit(EXIT_FAILURE);
			}
			
			textures.push_back(texture);
			
			cout<<"done"<<endl;
		}
	}
	
	/*for(int i=1; i<=10; ++i)
	{
		//cout<<"Loading toon"<<i<<".bmp...";
		
		GLuint texture;
		int width, height, channels;
		unsigned char* image;
		stringstream loc;
		if(i!=10) loc<<"data/share/toon0"<<i<<".bmp";
		else loc<<"data/share/toon10.bmp";
		
		ifstream test(loc.str());
		if(!test.is_open())
		{
			//cerr<<"Texture file could not be found: "<<loc.str()<<endl;
			//exit(EXIT_FAILURE);
			
			loc.str(std::string()); //clear ss
			if(i!=10) loc<<DATA_PATH<<"/textures/toon0"<<i<<".bmp";
			else loc<<DATA_PATH<<"/textures/toon10.bmp";
			
			ifstream test2(loc.str());
			if(!test2.is_open())
			{
				cerr<<"Texture file could not be found: "<<loc.str()<<endl;
			}
			
		}
		test.close();
		
		glActiveTexture( GL_TEXTURE0 );
		glGenTextures( 1, &texture );
		glBindTexture( GL_TEXTURE_2D, texture );
			image = SOIL_load_image( loc.str().c_str(), &width, &height, &channels, SOIL_LOAD_RGBA );
		
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image );
		SOIL_free_image_data( image );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		
		if(texture==0)
		{
			cerr<<"Toon Texture failed to load: "<<i<<endl;
			printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
			exit(EXIT_FAILURE);
		}
		
		//cout<<"done"<<endl;
		
		textures.push_back(texture);
	}*/
}

void LogoRenderer::initBuffers()
{
	#ifdef MODELDUMP
	ofstream modeldump("modeldump.txt");
	modeldump << "indices:" << endl;
	#endif
	
	//Note: vertex indices are loaded statically, since they do not change.
	//The actual vertex data is loaded dynamically each frame, so its memory is managed by the MotionController.
	GLuint *vertexIndices= (GLuint*) calloc(pmxInfo->face_continuing_datasets,sizeof(GLuint)*3);
	for(int i=0; i<pmxInfo->faces.size(); ++i) //faces.size()
	{
		int j=i*3;
		vertexIndices[j]=pmxInfo->faces[i]->points[0];
		vertexIndices[j+1]=pmxInfo->faces[i]->points[1];
		vertexIndices[j+2]=pmxInfo->faces[i]->points[2];
		
		#ifdef MODELDUMP
		modeldump << vertexIndices[j] << " " << vertexIndices[j+1] << " " << vertexIndices[j+2] << endl;
		#endif
	}
	
	
	#ifdef MODELDUMP
	modeldump << "vertices:" << endl;
	#endif
	
	#ifdef MODELDUMP
	modeldump.close();
	#endif
	
	//Generate all Viewer Buffers
	glGenBuffers(NumBuffers,Buffers);
	
	//init Element Buffer Object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[VertexIndexBuffer]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, pmxInfo->face_continuing_datasets*sizeof(GLuint)*3, vertexIndices, GL_STATIC_DRAW);
	
	free(vertexIndices);
	
	
	//Init Vertex Array Buffer Object
	glGenVertexArrays(NumVAOs, VAOs);
	glBindVertexArray(VAOs[Vertices]);
	
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[VertexArrayBuffer]);
}

void LogoRenderer::setVertexAttributes()
{	
	//Intialize Vertex Attribute Pointers
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), BUFFER_OFFSET(0)); //4=number of components updated per vertex
	glBindAttribLocation(shaderProgram, vPosition, "vPosition"); //Explicit vertex attribute index specification for older OpenGL version support. (Newer method is layout qualifier in vertex shader)
	glEnableVertexAttribArray(vPosition);

	glVertexAttribPointer(vUV, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), BUFFER_OFFSET(sizeof(glm::vec4)));
	glBindAttribLocation(shaderProgram, vUV, "vUV");
	glEnableVertexAttribArray(vUV);

	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), BUFFER_OFFSET(sizeof(glm::vec4)+sizeof(glm::vec2)));
	glBindAttribLocation(shaderProgram, vNormal, "vNormal");
	glEnableVertexAttribArray(vNormal);

	glVertexAttribPointer(vBoneIndices, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), BUFFER_OFFSET(sizeof(glm::vec4)+sizeof(glm::vec2)+sizeof(glm::vec3)+sizeof(GLfloat)));
	glBindAttribLocation(shaderProgram, vBoneIndices, "vBoneIndices");
	glEnableVertexAttribArray(vBoneIndices);

	glVertexAttribPointer(vBoneWeights, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), BUFFER_OFFSET(sizeof(glm::vec4)+sizeof(glm::vec2)+sizeof(glm::vec3)+sizeof(GLfloat)*5));
	glBindAttribLocation(shaderProgram, vBoneWeights, "vBoneWeights");
	glEnableVertexAttribArray(vBoneWeights);

	glVertexAttribPointer(vWeightFormula, 1, GL_FLOAT, GL_FALSE, sizeof(VertexData), BUFFER_OFFSET(sizeof(glm::vec4)+sizeof(glm::vec2)+sizeof(glm::vec3)));
	glBindAttribLocation(shaderProgram, vWeightFormula, "vWeightFormula");
	glEnableVertexAttribArray(vWeightFormula);
}

void LogoRenderer::unsetVertexAttributes()
{
	glDisableVertexAttribArray(vPosition);
	glDisableVertexAttribArray(vUV);
	glDisableVertexAttribArray(vNormal);
	glDisableVertexAttribArray(vBoneIndices);
	glDisableVertexAttribArray(vBoneWeights);
	glDisableVertexAttribArray(vWeightFormula);
}

void LogoRenderer::initUniformVarLocations()
{
	uniformVars[uAmbient]=glGetUniformLocation(shaderProgram, "ambient");
	uniformVars[uDiffuse]=glGetUniformLocation(shaderProgram, "diffuse");
	uniformVars[uSpecular]=glGetUniformLocation(shaderProgram, "specular");
	
	uniformVars[uShininess]=glGetUniformLocation(shaderProgram, "shininess");
	
	uniformVars[uIsEdge]=glGetUniformLocation(shaderProgram, "isEdge");
	uniformVars[uEdgeColor]=glGetUniformLocation(shaderProgram, "edgeColor");
	uniformVars[uEdgeSize]=glGetUniformLocation(shaderProgram, "edgeSize");
	
	uniformVars[uHalfVector]=glGetUniformLocation(shaderProgram, "halfVector");
	
	uniformVars[uLightDirection]=glGetUniformLocation(shaderProgram, "lightDirection");
	
	uniformVars[uSphereMode]=glGetUniformLocation(shaderProgram, "fSphereMode");
	
	//WARNING: Last time this code was known to be working, this was written as glGetUniformLocationARB()
	uniformVars[uTextureSampler]=glGetUniformLocationARB(shaderProgram,"textureSampler");
	uniformVars[uSphereSampler]=glGetUniformLocationARB(shaderProgram,"sphereSampler");
	uniformVars[uToonSampler]=glGetUniformLocationARB(shaderProgram,"toonSampler");
}

LogoRenderer::~LogoRenderer()
{
}

void LogoRenderer::handleEvents()
{
	glUseProgram(shaderProgram);
	glBindVertexArray(VAOs[Vertices]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[VertexIndexBuffer]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[VertexArrayBuffer]);
	
	setVertexAttributes();
	
	/*glfwPollEvents();
	if(glfwGetKey(GLFW_KEY_UP)==GLFW_PRESS)
	{
		modelTranslate.y-=0.1;
	}
	else if(glfwGetKey(GLFW_KEY_DOWN)==GLFW_PRESS)
	{
		modelTranslate.y+=0.1;
	}*/
}

void LogoRenderer::handleLogic()
{
	bool doPhysics=true;
	/*if(glfwGetKey('Q')==GLFW_PRESS)
	{
		doPhysics=false;
	}*/
	
	//if(glfwGetKey('A')==GLFW_RELEASE)
	{
		/*if(!motionController->advanceTime())
		{
			motionController->updateVertexMorphs();
			motionController->updateBoneAnimation();
		}
		//Debug- hold model in bind pose
		//holdModelInBindPose();
		
		mmdPhysics->updateBones(doPhysics);*/
		
		/*glUseProgram(bulletPhysics->debugDrawer->shaderProgram);
		setCamera(bulletPhysics->debugDrawer->MVPLoc);*/
		glUseProgram(shaderProgram);
		setCamera(MVP_loc);
	}
}

void LogoRenderer::drawStuff()
{	
	glUseProgram(shaderProgram); //Restore shader program and buffer's to Viewer's after drawing Bullet debug
	glBindVertexArray(VAOs[Vertices]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[VertexArrayBuffer]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[VertexIndexBuffer]);
	
	//if(glfwGetKey('A')==GLFW_RELEASE)
	{
		//drawModel(true); //draw model edges
		drawModel(false); //draw model
	}
	if(1==0) //if(glfwGetKey('S')==GLFW_PRESS)
	{
		bulletPhysics->SetDebugMode(btIDebugDraw::DBG_DrawWireframe);
		bulletPhysics->DebugDrawWorld();
	}
	else if(1==0) //else if(glfwGetKey('D')==GLFW_PRESS)
	{
		bulletPhysics->SetDebugMode(btIDebugDraw::DBG_DrawAabb);
		bulletPhysics->DebugDrawWorld();
	}
	else if(1==0) //if(glfwGetKey('F')==GLFW_PRESS)
	{
		bulletPhysics->SetDebugMode(btIDebugDraw::DBG_DrawConstraints);
		bulletPhysics->DebugDrawWorld();
	}
	else if(1==0) //if(glfwGetKey('G')==GLFW_PRESS)
	{
		bulletPhysics->SetDebugMode(btIDebugDraw::DBG_DrawConstraintLimits);
		bulletPhysics->DebugDrawWorld();
	}
	glUseProgram(shaderProgram); //Restore shader program and buffer's to Viewer's after drawing Bullet debug
	glBindVertexArray(VAOs[Vertices]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[VertexArrayBuffer]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[VertexIndexBuffer]);
	
	//glFinish();
	//glDrawBuffer(RecordBuffer,Buffers);
	//glReadPixels(0,0,1920,1080,GL_RGB,
	
	
	unsetVertexAttributes();
	glUseProgram(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void LogoRenderer::drawModel(bool drawEdges)
{
	glUseProgram(shaderProgram);
	
	//Bind VAO and related Buffers
	glBindVertexArray(VAOs[Vertices]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[VertexArrayBuffer]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[VertexIndexBuffer]);
	
	setVertexAttributes();
	initUniformVarLocations();
	
	if(drawEdges)
	{
		glDisable(GL_BLEND);
		glCullFace(GL_FRONT);
        glUniform1i(uniformVars[uIsEdge], 1);
        
        glDisable(GL_DEPTH_TEST);
	}
	else
	{        
        glEnable(GL_BLEND);
		glCullFace(GL_BACK);
		glUniform1i(uniformVars[uIsEdge], 0);
		
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA, GL_DST_ALPHA);
	}
	
	
	glm::vec3 halfVector=glm::normalize(cameraPosition-cameraTarget);
	halfVector.z=-halfVector.z;
	//glm::vec3 halfVector=glm::vec3(0,0,0);
	
	glm::vec3 lightDirection=glm::normalize(glm::vec3(0.3,1.0,2.0));
	
	int faceCount=0;
	for(int m=0; m<pmxInfo->material_continuing_datasets; ++m) //pmxInfo->material_continuing_datasets
	{
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,textures[pmxInfo->materials[m]->textureIndex]);
		glUniform1iARB(uniformVars[uTextureSampler], 0); //WARNING: ARB change
		
		if((int)pmxInfo->materials[m]->sphereMode>0)
		{
			glActiveTexture(GL_TEXTURE1);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D,textures[pmxInfo->materials[m]->sphereIndex]);
			glUniform1iARB(uniformVars[uSphereSampler], 1);
		}
		
		if((int)pmxInfo->materials[m]->shareToon==0)
		{
			glActiveTexture(GL_TEXTURE2);
			glEnable(GL_TEXTURE_2D);
			
			glBindTexture(GL_TEXTURE_2D,textures[pmxInfo->materials[m]->toonTextureIndex]);
			glUniform1iARB(uniformVars[uToonSampler], 2);
		}
		else if((int)pmxInfo->materials[m]->shareToon==1)
		{
			glActiveTexture(GL_TEXTURE2);
			glEnable(GL_TEXTURE_2D);
			
			glBindTexture(GL_TEXTURE_2D,textures[textures.size()-11+pmxInfo->materials[m]->shareToonTexture]);
			glUniform1iARB(uniformVars[uToonSampler], 2);
		}
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,textures[pmxInfo->materials[m]->textureIndex]);
		
		
		glUniform3fv(uniformVars[uAmbient],1,(GLfloat*)&pmxInfo->materials[m]->ambient);
		glUniform4fv(uniformVars[uDiffuse],1,(GLfloat*)&pmxInfo->materials[m]->diffuse);
		glUniform3fv(uniformVars[uSpecular],1,(GLfloat*)&pmxInfo->materials[m]->specular);
		
		glUniform1f(uniformVars[uShininess],glm::normalize(pmxInfo->materials[m]->shininess));
		glUniform3f(uniformVars[uHalfVector],halfVector.x,halfVector.y,halfVector.z);
		glUniform3f(uniformVars[uLightDirection],lightDirection.x,lightDirection.y,lightDirection.z);
		
		glUniform4fv(uniformVars[uEdgeColor],1,(GLfloat*)&pmxInfo->materials[m]->edgeColor);
		glUniform1f(uniformVars[uEdgeSize],glm::normalize(pmxInfo->materials[m]->edgeSize));
		
		glUniform1f(uniformVars[uSphereMode],pmxInfo->materials[m]->sphereMode);
        
		glDrawElements(GL_TRIANGLES, (pmxInfo->materials[m]->hasFaceNum), GL_UNSIGNED_INT, BUFFER_OFFSET(sizeof(GLuint)*faceCount));
		faceCount+=pmxInfo->materials[m]->hasFaceNum;
	}
	
	//glBindTexture(GL_TEXTURE_2D,0);
	
	glDisableVertexAttribArray(vPosition);
	glDisableVertexAttribArray(vUV);
	glDisableVertexAttribArray(vNormal);
	glDisableVertexAttribArray(vBoneIndices);
	glDisableVertexAttribArray(vBoneWeights);
	glDisableVertexAttribArray(vWeightFormula);
}

void LogoRenderer::setCamera(GLuint MVPLoc)
{	
	glm::mat4 Projection = glm::perspective(45.0f, 16.0f/9.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View       = glm::lookAt(
		glm::vec3(cameraPosition.x,cameraPosition.y,-cameraPosition.z), // Camera is at (4,3,3), in World Space
		cameraTarget, // and looks at the origin
		glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	//View= glm::rotate(0.0f,0.0f,0.0f,1.0f)* View;
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::translate(modelTranslate.x, modelTranslate.y, modelTranslate.z);
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = Projection * View * Model;
	
	glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, &MVP[0][0]);
}


void LogoRenderer::paintQtLogo()
{
    program1.enableAttributeArray(normalAttr1);
    program1.enableAttributeArray(vertexAttr1);
    program1.setAttributeArray(vertexAttr1, vertices.constData());
    program1.setAttributeArray(normalAttr1, normals.constData());
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    program1.disableAttributeArray(normalAttr1);
    program1.disableAttributeArray(vertexAttr1);
}


void LogoRenderer::initialize()
{
    /*glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QOpenGLShader *vshader1 = new QOpenGLShader(QOpenGLShader::Vertex, &program1);
    const char *vsrc1 =
        "attribute highp vec4 vertex;\n"
        "attribute mediump vec3 normal;\n"
        "uniform mediump mat4 matrix;\n"
        "varying mediump vec4 color;\n"
        "void main(void)\n"
        "{\n"
        "    vec3 toLight = normalize(vec3(0.0, 0.3, 1.0));\n"
        "    float angle = max(dot(normal, toLight), 0.0);\n"
        "    vec3 col = vec3(0.40, 1.0, 0.0);\n"
        "    color = vec4(col * 0.2 + col * 0.8 * angle, 1.0);\n"
        "    color = clamp(color, 0.0, 1.0);\n"
        "    gl_Position = matrix * vertex;\n"
        "}\n";
    vshader1->compileSourceCode(vsrc1);

    QOpenGLShader *fshader1 = new QOpenGLShader(QOpenGLShader::Fragment, &program1);
    const char *fsrc1 =
        "varying mediump vec4 color;\n"
        "void main(void)\n"
        "{\n"
        "    gl_FragColor = color;\n"
        "}\n";
    fshader1->compileSourceCode(fsrc1);

    program1.addShader(vshader1);
    program1.addShader(fshader1);
    program1.link();

    vertexAttr1 = program1.attributeLocation("vertex");
    normalAttr1 = program1.attributeLocation("normal");
    matrixUniform1 = program1.uniformLocation("matrix");

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    m_fAngle = 0;
    m_fScale = 1;
    createGeometry();*/
}

void LogoRenderer::render()
{	
	//glClearColor(0.5f, 0.5f, 0.7f, 1.0f);
	glClearColor(0, 0, 0, 1);
	
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
    glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPointSize(5.0);
	//glClearDepth(1.0f);
	
	glDisable(GL_SCISSOR_TEST);
    glEnable(GL_STENCIL_TEST);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	handleEvents();
	handleLogic();
	
	drawStuff();
	
	glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
	
	return;
	
    glDepthMask(true);

    glClearColor(0.5f, 0.5f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glFrontFace(GL_CW);
    glCullFace(GL_FRONT);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    QMatrix4x4 modelview;
    modelview.rotate(m_fAngle, 0.0f, 1.0f, 0.0f);
    modelview.rotate(m_fAngle, 1.0f, 0.0f, 0.0f);
    modelview.rotate(m_fAngle, 0.0f, 0.0f, 1.0f);
    modelview.scale(m_fScale);
    modelview.translate(0.0f, -0.2f, 0.0f);

    program1.bind();
    program1.setUniformValue(matrixUniform1, modelview);
    paintQtLogo();
    program1.release();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    m_fAngle += 1.0f;
}

void LogoRenderer::createGeometry()
{
    vertices.clear();
    normals.clear();

    qreal x1 = +0.06f;
    qreal y1 = -0.14f;
    qreal x2 = +0.14f;
    qreal y2 = -0.06f;
    qreal x3 = +0.08f;
    qreal y3 = +0.00f;
    qreal x4 = +0.30f;
    qreal y4 = +0.22f;

    quad(x1, y1, x2, y2, y2, x2, y1, x1);
    quad(x3, y3, x4, y4, y4, x4, y3, x3);

    extrude(x1, y1, x2, y2);
    extrude(x2, y2, y2, x2);
    extrude(y2, x2, y1, x1);
    extrude(y1, x1, x1, y1);
    extrude(x3, y3, x4, y4);
    extrude(x4, y4, y4, x4);
    extrude(y4, x4, y3, x3);

    const qreal Pi = 3.14159f;
    const int NumSectors = 100;

    for (int i = 0; i < NumSectors; ++i) {
        qreal angle1 = (i * 2 * Pi) / NumSectors;
        qreal x5 = 0.30 * sin(angle1);
        qreal y5 = 0.30 * cos(angle1);
        qreal x6 = 0.20 * sin(angle1);
        qreal y6 = 0.20 * cos(angle1);

        qreal angle2 = ((i + 1) * 2 * Pi) / NumSectors;
        qreal x7 = 0.20 * sin(angle2);
        qreal y7 = 0.20 * cos(angle2);
        qreal x8 = 0.30 * sin(angle2);
        qreal y8 = 0.30 * cos(angle2);

        quad(x5, y5, x6, y6, x7, y7, x8, y8);

        extrude(x6, y6, x7, y7);
        extrude(x8, y8, x5, y5);
    }

    for (int i = 0;i < vertices.size();i++)
        vertices[i] *= 2.0f;
}

void LogoRenderer::quad(qreal x1, qreal y1, qreal x2, qreal y2, qreal x3, qreal y3, qreal x4, qreal y4)
{
    vertices << QVector3D(x1, y1, -0.05f);
    vertices << QVector3D(x2, y2, -0.05f);
    vertices << QVector3D(x4, y4, -0.05f);

    vertices << QVector3D(x3, y3, -0.05f);
    vertices << QVector3D(x4, y4, -0.05f);
    vertices << QVector3D(x2, y2, -0.05f);

    QVector3D n = QVector3D::normal
        (QVector3D(x2 - x1, y2 - y1, 0.0f), QVector3D(x4 - x1, y4 - y1, 0.0f));

    normals << n;
    normals << n;
    normals << n;

    normals << n;
    normals << n;
    normals << n;

    vertices << QVector3D(x4, y4, 0.05f);
    vertices << QVector3D(x2, y2, 0.05f);
    vertices << QVector3D(x1, y1, 0.05f);

    vertices << QVector3D(x2, y2, 0.05f);
    vertices << QVector3D(x4, y4, 0.05f);
    vertices << QVector3D(x3, y3, 0.05f);

    n = QVector3D::normal
        (QVector3D(x2 - x4, y2 - y4, 0.0f), QVector3D(x1 - x4, y1 - y4, 0.0f));

    normals << n;
    normals << n;
    normals << n;

    normals << n;
    normals << n;
    normals << n;
}

void LogoRenderer::extrude(qreal x1, qreal y1, qreal x2, qreal y2)
{
    vertices << QVector3D(x1, y1, +0.05f);
    vertices << QVector3D(x2, y2, +0.05f);
    vertices << QVector3D(x1, y1, -0.05f);

    vertices << QVector3D(x2, y2, -0.05f);
    vertices << QVector3D(x1, y1, -0.05f);
    vertices << QVector3D(x2, y2, +0.05f);

    QVector3D n = QVector3D::normal
        (QVector3D(x2 - x1, y2 - y1, 0.0f), QVector3D(0.0f, 0.0f, -0.1f));

    normals << n;
    normals << n;
    normals << n;

    normals << n;
    normals << n;
    normals << n;
}
