#ifndef QOPENGLES2WIDGET_H
#define QOPENGLES2WIDGET_H

#include <QWidget>
#include <QTimer>
#include "ui_qopengles2widget.h"

#include "GLESContext.h"

class QOpenGLES2Widget : public QWidget
{
    Q_OBJECT

public:
    QOpenGLES2Widget(QWidget *parent = 0);
    ~QOpenGLES2Widget();


    void    setup();
public Q_SLOTS:
    void render();

protected:
    QTimer      _timer;
    GLESContext _content;
private:
    Ui::QOpenGLES2Widget ui;
};

#endif // QOPENGLES2WIDGET_H
