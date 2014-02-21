#include <QGuiApplication>
#include <QtQuick/QQuickView>
#include "openglscene.h"

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<OpenGLScene>("OpenGLUnderQML", 1, 0, "OpenGLScene");

    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl::fromLocalFile("qml/main.qml"));
    view.show();

    return app.exec();
}
