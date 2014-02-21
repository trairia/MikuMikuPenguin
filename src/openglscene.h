#ifndef GL_SCENE_H
#define GL_SCENE_H

#include <QtQuick/QQuickItem>
#include <QtGui/QOpenGLShaderProgram>

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
    void itemChange(ItemChange change, const ItemChangeData &);

public slots:
    void paint();
    void cleanup();
    void sync();

private:
    QOpenGLShaderProgram *shaderProgram;

    qreal gui_t; //value of 't' in the GUI thread
    qreal render_t; //value of 't' in the Rendering thread
};

#endif
