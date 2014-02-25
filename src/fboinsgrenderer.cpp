#include "fboinsgrenderer.h"
#include "logorenderer.h"

#include <QtGui/QOpenGLFramebufferObject>

#include <QtQuick/QQuickWindow>
#include <qsgsimpletexturenode.h>

#include <iostream>

using namespace std;

class LogoInFboRenderer : public QQuickFramebufferObject::Renderer
{
public:
    LogoInFboRenderer(QQuickWindow *window): window(window)
    {
		//Configure OpenGL context
		/*window->setSurfaceType( window->OpenGLSurface );

		// Specify the format and create platform-specific surface
		QSurfaceFormat format;
		format.setDepthBufferSize( 24 );
		format.setMajorVersion( 4 );
		format.setMinorVersion( 3 );
		format.setSamples( 4 );
		format.setProfile( QSurfaceFormat::CoreProfile );
		
		window->setFormat( format );
		window->create();

		// Create an OpenGL context
		context = new QOpenGLContext;
		context->setFormat( format );
		context->create();
		context->setShareContext(context);

		// Make the context current on this window
		context->makeCurrent( window );
		
		//window->openglContext()->makeCurrent(window);

		// Obtain a functions object and resolve all entry points
		// funcs is declared as: QOpenGLFunctions_4_3_Core* funcs
		funcs = context->versionFunctions();
		if ( !funcs )
		{
			qWarning("Could not obtain OpenGL versions object");
			exit( 1 );
		}
		funcs->initializeOpenGLFunctions();*/
				
		logo=new LogoRenderer(NULL);
        logo->initialize();
        
        //window->openglContext()->makeCurrent(window);
    }

    void render()
    {		
        logo->render();
        update();
        
        //window->openglContext()->makeCurrent(window);
    }

    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size)
    {
        QOpenGLFramebufferObjectFormat format;
        format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
        format.setSamples(4);
        return new QOpenGLFramebufferObject(size, format);
    }

    LogoRenderer *logo;

	QQuickWindow *window;
	
	QOpenGLContext *context;
	//QAbstractOpenGLFunctions* funcs;
};

QQuickFramebufferObject::Renderer *FboInSGRenderer::createRenderer() const
{
    return new LogoInFboRenderer(window());
}

/*class TextureNode : public QObject, public QSGSimpleTextureNode
{
	Q_OBJECT

public:
	TextureNode(QQuickWindow *window)
		: m_fbo(0)
		, m_texture(0)
		, m_window(window)
		, m_logoRenderer(0)
	{
		connect(m_window, SIGNAL(beforeRendering()), this, SLOT(renderFBO()));
		
		//Configure OpenGL context
		m_window->setSurfaceType( m_window->OpenGLSurface );

		// Specify the format and create platform-specific surface
		QSurfaceFormat format;
		format.setDepthBufferSize( 24 );
		format.setMajorVersion( 4 );
		format.setMinorVersion( 3 );
		format.setSamples( 4 );
		format.setProfile( QSurfaceFormat::CoreProfile );
		m_window->setFormat( format );
		m_window->create();

		// Create an OpenGL context
		m_context = new QOpenGLContext;
		m_context->setFormat( format );
		m_context->create();

		// Make the context current on this window
		m_context->makeCurrent( m_window );

		// Obtain a functions object and resolve all entry points
		// m_funcs is declared as: QOpenGLFunctions_4_3_Core* m_funcs
		m_funcs = m_context->versionFunctions();
		if ( !m_funcs ) {
			qWarning("Could not obtain OpenGL versions object");
			exit( 1 );
		}
		m_funcs->initializeOpenGLFunctions();
	}

	~TextureNode()
	{
		delete m_texture;
		delete m_fbo;
		delete m_logoRenderer;
	}

public slots:
	void renderFBO()
	{
		QSize size = rect().size().toSize();

		if (!m_fbo)
		{
			QOpenGLFramebufferObjectFormat format;
			format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
			m_fbo = new QOpenGLFramebufferObject(size, format);
			m_texture = m_window->createTextureFromId(m_fbo->texture(), size);
			m_logoRenderer = new LogoRenderer(m_funcs);
			m_logoRenderer->initialize();
			setTexture(m_texture);
		}

		m_fbo->bind();

		glViewport(0, 0, size.width(), size.height());

		m_logoRenderer->render();

		m_fbo->bindDefault();

		m_window->update();
	}

private:
	QOpenGLFramebufferObject *m_fbo;
	QSGTexture *m_texture;
	QQuickWindow *m_window;

	LogoRenderer *m_logoRenderer;
	
	QOpenGLContext *m_context;
	QAbstractOpenGLFunctions* m_funcs;
};

FboInSGRenderer::FboInSGRenderer()
{
	setFlag(ItemHasContents, true);
	
	//Enable mouse events
	setAcceptedMouseButtons(Qt::AllButtons);
	
	//setMouseTracking(true); //Uncomment to get mouseMoveEvents even when no mouse button is pressed
}

QSGNode *FboInSGRenderer::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
	// Don't bother with resize and such, just recreate the node from scratch
	// when geometry changes.
	if (oldNode)
		delete oldNode;

	TextureNode *node = new TextureNode(window());
	node->setRect(boundingRect());

	return node;
}

void FboInSGRenderer::mouseMoveEvent(QMouseEvent *event)
{
	if (event->buttons() & Qt::LeftButton)
	{
		QPoint pos=event->pos();
		cout<<pos.x()<<" "<<pos.y()<<endl;
	}
}*/

#include "fboinsgrenderer.moc"
