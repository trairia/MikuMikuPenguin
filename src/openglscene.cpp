#include "openglscene.h"

#include <QtQuick/qquickwindow.h>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLContext>

OpenGLScene::OpenGLScene()
    : shaderProgram(0)
    , gui_t(0)
    , render_t(0)
{
	
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

void OpenGLScene::paint()
{
    if(!shaderProgram)
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
	shaderProgram->release();
}

void OpenGLScene::cleanup()
{
    if(shaderProgram)
    {
		delete shaderProgram;
		shaderProgram = 0;
    }
}

void OpenGLScene::sync()
{
    render_t = gui_t;
}

