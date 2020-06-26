#ifndef MYCLASS_H
#define MYCLASS_H

#include <QtGui/QMainWindow>
#include "ui_myclass.h"

#include "qopengles2widget.h"

class MyClass : public QMainWindow
{
    Q_OBJECT

public:
    QOpenGLES2Widget*   gles;
public:
    MyClass(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~MyClass();

private:
    Ui::MyClassClass ui;
};

#endif // MYCLASS_H
