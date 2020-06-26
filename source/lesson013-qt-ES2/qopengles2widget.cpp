#include "qopengles2widget.h"

QOpenGLES2Widget::QOpenGLES2Widget(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    

    QObject::connect(&_timer,SIGNAL(timeout()),this,SLOT(render()));
    _timer.setInterval(16);
    _timer.start();
}

QOpenGLES2Widget::~QOpenGLES2Widget()
{
    _content.shutdown();
}

void QOpenGLES2Widget::render()
{
    glClearColor(0,0,1,1);
    glClear(GL_COLOR_BUFFER_BIT);
    _content.swapBuffer();
}

void QOpenGLES2Widget::setup()
{
    _content.setup(winId(),getDC());
}
