#ifndef MYCLASS_H
#define MYCLASS_H

#include <QtGui/QMainWindow>
#include <QTimer>
#include "ui_myclass.h"

#include "GLContext.h"

class MyClass : public QMainWindow
{
    Q_OBJECT

public:
    MyClass(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~MyClass();

    GLContext   _context;
    QTimer      _renderTimer;
public Q_SLOTS:

    void    render();


protected:
    void    paintEvent(QPaintEvent* evt);
private:
    Ui::MyClassClass ui;
};

#endif // MYCLASS_H
