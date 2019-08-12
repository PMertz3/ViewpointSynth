/********************************************************************************
** Form generated from reading UI file 'ViewpointSynthesis.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIEWPOINTSYNTHESIS_H
#define UI_VIEWPOINTSYNTHESIS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ViewpointSynthesisClass
{
public:

    void setupUi(QWidget *ViewpointSynthesisClass)
    {
        if (ViewpointSynthesisClass->objectName().isEmpty())
            ViewpointSynthesisClass->setObjectName(QString::fromUtf8("ViewpointSynthesisClass"));
        ViewpointSynthesisClass->resize(600, 400);

        retranslateUi(ViewpointSynthesisClass);

        QMetaObject::connectSlotsByName(ViewpointSynthesisClass);
    } // setupUi

    void retranslateUi(QWidget *ViewpointSynthesisClass)
    {
        ViewpointSynthesisClass->setWindowTitle(QApplication::translate("ViewpointSynthesisClass", "ViewpointSynthesis", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ViewpointSynthesisClass: public Ui_ViewpointSynthesisClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIEWPOINTSYNTHESIS_H
