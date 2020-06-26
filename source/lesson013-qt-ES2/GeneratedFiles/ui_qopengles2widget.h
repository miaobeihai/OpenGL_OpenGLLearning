/********************************************************************************
** Form generated from reading UI file 'qopengles2widget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QOPENGLES2WIDGET_H
#define UI_QOPENGLES2WIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QOpenGLES2Widget
{
public:

    void setupUi(QWidget *QOpenGLES2Widget)
    {
        if (QOpenGLES2Widget->objectName().isEmpty())
            QOpenGLES2Widget->setObjectName(QString::fromUtf8("QOpenGLES2Widget"));
        QOpenGLES2Widget->resize(400, 300);

        retranslateUi(QOpenGLES2Widget);

        QMetaObject::connectSlotsByName(QOpenGLES2Widget);
    } // setupUi

    void retranslateUi(QWidget *QOpenGLES2Widget)
    {
        QOpenGLES2Widget->setWindowTitle(QApplication::translate("QOpenGLES2Widget", "QOpenGLES2Widget", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QOpenGLES2Widget: public Ui_QOpenGLES2Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QOPENGLES2WIDGET_H
