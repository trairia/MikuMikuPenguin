#ifndef GL_SCENE_H
#define GL_SCENE_H

#include <QtQuick/QQuickItem>
#include <QtGui/QOpenGLShaderProgram>
#include <QOpenGLFunctions_4_3_Core>

#define BUFFER_OFFSET(offset) ((void *) (offset))

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

class OpenGLScene : public QQuickItem
{
	Q_OBJECT

	Q_PROPERTY(qreal t READ t WRITE setT NOTIFY tChanged)

public:
	OpenGLScene();

	qreal t() const { return gui_t; }
	void setT(qreal t);

signals:
	void tChanged();

protected:
	void init();

	void itemChange(ItemChange change, const ItemChangeData &);
    
	void initBuffers();
	void loadTextures();

public slots:
	void paint();
	void cleanup();
	void sync();

private:
	//QOpenGLShaderProgram *shaderProgram;
	GLuint shaderProgram;

	qreal gui_t; //value of 't' in the GUI thread
	qreal render_t; //value of 't' in the Rendering thread

	ClosedMMDFormat::PMXInfo *pmxInfo;
	ClosedMMDFormat::VMDInfo *vmdInfo;
	VMDMotionController *motionController;
	
	GLuint VAOs[NumVAOs];
	GLuint Buffers[NumBuffers];
	GLuint uniformVars[NumUniforms];
	
	std::vector<GLuint> textures;
};

#endif
