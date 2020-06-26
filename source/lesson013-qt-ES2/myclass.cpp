#include "myclass.h"

#include <QHBoxLayout>


MyClass::MyClass(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{
    ui.setupUi(this);

    gles    =   new QOpenGLES2Widget(this);

    gles->resize(400,400);
    gles->show();

    QWidget*    center  =   centralWidget();
    QHBoxLayout* layout  =   new QHBoxLayout(center);
    layout->addWidget(gles);

    gles->setup();

   
    
}

MyClass::~MyClass()
{

}
