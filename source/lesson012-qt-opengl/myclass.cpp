#include "myclass.h"

MyClass::MyClass(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{
    ui.setupUi(this);

    _context.setup(winId(),getDC());
    QObject::connect(&_renderTimer,SIGNAL(timeout()),this,SLOT(render()));
    _renderTimer.setInterval(16);
    _renderTimer.start();
}

MyClass::~MyClass()
{
    _context.shutdown();
}

void MyClass::paintEvent( QPaintEvent* evt )
{
}

void MyClass::render()
{
    glClearColor(1,0,0,01);
    glClear(GL_COLOR_BUFFER_BIT);
    _context.swapBuffer();
}
