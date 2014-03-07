#ifndef LOGORENDERER_H
#define LOGORENDERER_H

#include <QtGui/qvector3d.h>
#include <QtGui/qmatrix4x4.h>
#include <QtGui/qopenglshaderprogram.h>

#include <QTime>
#include <QVector>

#include <QWindow>

#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

#define BUFFER_OFFSET(offset) ((void *) (offset))

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

enum VAO_IDs { Vertices, NumVAOs };
enum Buffer_IDs { VertexArrayBuffer, VertexIndexBuffer, RecordBuffer, NumBuffers };
enum Attrib_IDs { vPosition, vUV, vNormal, vBoneIndices, vBoneWeights, vWeightFormula };
enum Uniform_IDs { 
	uAmbient,uDiffuse,uSpecular,uShininess,
	uIsEdge,uEdgeColor,uEdgeSize,
	uHalfVector,uLightDirection,
	uSphereMode,
	uTextureSampler,uSphereSampler,uToonSampler,
	NumUniforms };

namespace ClosedMMDFormat
{
	struct PMXInfo;
	struct VMDInfo;
}
class VMDMotionController;
class BulletPhysics;
class MMDPhysics;
class VertexData;

class LogoRenderer
{

public:
    LogoRenderer(QWindow *window);
    ~LogoRenderer();

    void render();
    void initialize();

private:
	void init();
	void loadTextures();
	void initBuffers();
	void setVertexAttributes();
	void unsetVertexAttributes();
	void initUniformVarLocations();
	
	void handleEvents();
	void handleLogic();
	void drawStuff();
	
	void setCamera(GLuint MVPLoc);
	
	void drawModel(bool drawEdges);

	qreal   m_fAngle;
	qreal   m_fScale;

	void paintQtLogo();
	void createGeometry();
	void quad(qreal x1, qreal y1, qreal x2, qreal y2, qreal x3, qreal y3, qreal x4, qreal y4);
	void extrude(qreal x1, qreal y1, qreal x2, qreal y2);

    QVector<QVector3D> vertices;
    QVector<QVector3D> normals;
    QOpenGLShaderProgram program1;
    int vertexAttr1;
    int normalAttr1;
    int matrixUniform1;
    
    
    //QOpenGLFunctions_4_3_Core *glFuncs;
    
    GLuint VAOs[NumVAOs];
	GLuint Buffers[NumBuffers];
	GLuint uniformVars[NumUniforms];
	
	GLuint MVP_loc;
	
    
	ClosedMMDFormat::PMXInfo *pmxInfo;
	ClosedMMDFormat::VMDInfo *vmdInfo;
	VMDMotionController *motionController;
	
	BulletPhysics *bulletPhysics;
	MMDPhysics *mmdPhysics;
	
	float theta=3.0f*M_PI/2.0f;
	float zenith=0.0;
	float radius=10.0f;
	float t=radius; //distances to center after rotating up/down
	
	glm::vec3 cameraPosition;
	glm::vec3 cameraTarget;
	glm::vec3 modelTranslate;
	
	
	std::vector<GLuint> textures;
	
	std::string vertShaderPath,fragShaderPath;
	GLuint shaderProgram;
	std::string bulletVertPath,bulletFragPath;
};
#endif
