#ifndef FBOINSGRENDERER_H
#define FBOINSGRENDERER_H

#include <QQuickItem>


#include <QtQuick/QQuickFramebufferObject>

class LogoRenderer;

class FboInSGRenderer : public QQuickFramebufferObject
{
    Q_OBJECT
public:
    Renderer *createRenderer() const;
};

/*class FboInSGRenderer : public QQuickItem
{
    Q_OBJECT

public:
    FboInSGRenderer();

protected:
    QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *);
    
    void mouseMoveEvent(QMouseEvent *event);

};*/

#endif
