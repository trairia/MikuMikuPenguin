#include <QGuiApplication>
#include <QtQuick/QQuickView>
#include "openglscene.h"
#include "fboinsgrenderer.h"

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<OpenGLScene>("OpenGLUnderQML", 1, 0, "OpenGLScene");
	qmlRegisterType<FboInSGRenderer>("SceneGraphRendering", 1, 0, "Renderer");

    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl::fromLocalFile(DATA_PATH"/qml/main.qml"));
    view.show();

    return app.exec();
}
