#include "openglscene.h"

#include <QtQuick/qquickwindow.h>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLContext>

//#include <QOpenGLVertexArrayObject> 
//#include <QGLTexture2D>

#include <QOpenGLFunctions_4_3_Core>

#include <iostream>
#include <SOIL/SOIL.h>

#include "motioncontroller.h"
#include "pmx.h"
#include "vmd.h"
#include "shader.h"


using namespace ClosedMMDFormat;
using namespace std;

OpenGLScene::OpenGLScene()
    : shaderProgram(0)
    , gui_t(0)
    , render_t(0)
{	
	pmxInfo=&readPMX(DATA_PATH"/model/tdamiku/","tdamiku.pmx");
	vmdInfo=&readVMD(DATA_PATH"/motion/私の時間/私の時間_short_Lat式ミク.vmd");
}

void OpenGLScene::init()
{
	QOpenGLFunctions_4_3_Core glFuncs;
	
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
	
	//linkShaders(shaderProgram);
	//glFuncs.glUseProgram(shaderProgram);
	
	
	loadTextures();
	//initBuffers();
}

void OpenGLScene::setT(qreal t)
{
	if (t == gui_t)
		return;
	gui_t = t;
	emit tChanged();
	if (window())
		window()->update();
}

void OpenGLScene::itemChange(ItemChange change, const ItemChangeData &)
{
    //The ItemSceneChange event is sent when we are first attached to a window.
    if(change == ItemSceneChange)
    {
		QQuickWindow *win = window();
		if (!win)
			return;

		//Connect the beforeRendering signal to our paint function.
		//Since this call is executed on the rendering thread it must be a Qt::DirectConnection
		connect(win, SIGNAL(beforeRendering()), this, SLOT(paint()), Qt::DirectConnection);
		connect(win, SIGNAL(beforeSynchronizing()), this, SLOT(sync()), Qt::DirectConnection);

		//If we allow QML to do the clearing, they would clear what we paint and nothing would show.
		win->setClearBeforeRendering(false);
	}
}

void OpenGLScene::loadTextures()
{
	//Thought about replacing SOIL with FreeImage, but for now sticking with SOIL. Don't fix what isn't broken.
	//Warning: Commented-out FreeImage code below is buggy (DOES NOT WORK)
	
	//FreeImage_Initialise();	
	QOpenGLFunctions_4_3_Core glFuncs;        
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
			
			glFuncs.glActiveTexture( GL_TEXTURE0 );
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
			
			glFuncs.glActiveTexture( GL_TEXTURE0 );
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
			
			glFuncs.glActiveTexture(GL_TEXTURE0);
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

void OpenGLScene::initBuffers()
{
	QOpenGLFunctions_4_3_Core glFuncs;
	
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
	glFuncs.glGenBuffers(NumBuffers,Buffers);
	exit(EXIT_SUCCESS);
	
	//init Element Buffer Object
	/*glFuncs.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[VertexIndexBuffer]);
	glFuncs.glBufferData(GL_ELEMENT_ARRAY_BUFFER, pmxInfo->face_continuing_datasets*sizeof(GLuint)*3, vertexIndices, GL_STATIC_DRAW);
	
	free(vertexIndices);
	
	
	//Init Vertex Array Buffer Object
	glFuncs.glGenVertexArrays(NumVAOs, VAOs);
	glFuncs.glBindVertexArray(VAOs[Vertices]);
	
	glFuncs.glBindBuffer(GL_ARRAY_BUFFER, Buffers[VertexArrayBuffer]);
	
	//Intialize Vertex Attribute Pointers
	glFuncs.glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), BUFFER_OFFSET(0)); //4=number of components updated per vertex
	glFuncs.glBindAttribLocation(shaderProgram, vPosition, "vPosition"); //Explicit vertex attribute index specification for older OpenGL version support. (Newer method is layout qualifier in vertex shader)
	//glFuncs.glEnableVertexAttribArray(vPosition);

	glFuncs.glVertexAttribPointer(vUV, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), BUFFER_OFFSET(sizeof(glm::vec4)));
	glFuncs.glBindAttribLocation(shaderProgram, vUV, "vUV");
	//glFuncs.glEnableVertexAttribArray(vUV);

	glFuncs.glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), BUFFER_OFFSET(sizeof(glm::vec4)+sizeof(glm::vec2)));
	glFuncs.glBindAttribLocation(shaderProgram, vNormal, "vNormal");
	//glFuncs.glEnableVertexAttribArray(vNormal);

	glFuncs.glVertexAttribPointer(vBoneIndices, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), BUFFER_OFFSET(sizeof(glm::vec4)+sizeof(glm::vec2)+sizeof(glm::vec3)+sizeof(GLfloat)));
	glFuncs.glBindAttribLocation(shaderProgram, vBoneIndices, "vBoneIndices");
	//glFuncs.glEnableVertexAttribArray(vBoneIndices);

	glFuncs.glVertexAttribPointer(vBoneWeights, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), BUFFER_OFFSET(sizeof(glm::vec4)+sizeof(glm::vec2)+sizeof(glm::vec3)+sizeof(GLfloat)*5));
	glFuncs.glBindAttribLocation(shaderProgram, vBoneWeights, "vBoneWeights");
	//glFuncs.glEnableVertexAttribArray(vBoneWeights);

	glFuncs.glVertexAttribPointer(vWeightFormula, 1, GL_FLOAT, GL_FALSE, sizeof(VertexData), BUFFER_OFFSET(sizeof(glm::vec4)+sizeof(glm::vec2)+sizeof(glm::vec3)));
	glFuncs.glBindAttribLocation(shaderProgram, vWeightFormula, "vWeightFormula");
	//glFuncs.glEnableVertexAttribArray(vWeightFormula);
	
	
	glFuncs.glBindBuffer(GL_ARRAY_BUFFER,0);
	glFuncs.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	
	glFuncs.glBindVertexArray(0);*/
	
	//glFuncs.glDisableVertexAttribArray(vWeightFormula);
}

void OpenGLScene::paint()
{
	if(window())
	{
		//QOpenGLFunctions glFunctions(QOpenGLContext::currentContext());
		
		if(textures.size()==0)
		{
			init();
		}
	}
	
	
	/*if(!shaderProgram)
    {
		shaderProgram = new QOpenGLShaderProgram();
		shaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex,
											"attribute highp vec4 vertices;"
											"varying highp vec2 coords;"
											"void main()"
											"{"
											"    gl_Position = vertices;"
											"    coords = vertices.xy;"
											"}");
		shaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment,
											"uniform lowp float t;"
											"varying highp vec2 coords;"
											"void main()"
											"{"
											"    lowp float i = 1. - (pow(abs(coords.x), 4.) + pow(abs(coords.y), 4.));"
											"    i = smoothstep(t - 0.8, t + 0.8, i);"
											"    i = floor(i * 20.) / 20.;"
											"    gl_FragColor = vec4(coords * .5 + .5, i, i);"
											"}");

		shaderProgram->bindAttributeLocation("vertices", 0);
		shaderProgram->link();

		connect(window()->openglContext(), SIGNAL(aboutToBeDestroyed()),
				this, SLOT(cleanup()), Qt::DirectConnection);
	}
	shaderProgram->bind();

	shaderProgram->enableAttributeArray(0);

	float values[]=
	{
        -1, -1,
        1, -1,
        -1, 1,
        1, 1
    };
		
	shaderProgram->setAttributeArray(0, GL_FLOAT, values, 2);
	shaderProgram->setUniformValue("t", (float) render_t);

	glViewport(0, 0, window()->width(), window()->height());

	glDisable(GL_DEPTH_TEST);

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	shaderProgram->disableAttributeArray(0);
	shaderProgram->release();*/
}

void OpenGLScene::cleanup()
{
    /*if(shaderProgram)
    {
		delete shaderProgram;
		shaderProgram = 0;
    }*/
}

void OpenGLScene::sync()
{
    render_t = gui_t;
}

