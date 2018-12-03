/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_4;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QPushButton *config_file_button;
    QHBoxLayout *horizontalLayout_3;
    QLineEdit *dir_lineedit;
    QPushButton *browser_button_;
    QPushButton *clear_button;
    QHBoxLayout *horizontalLayout_2;
    QSpinBox *spinBox;
    QPushButton *download_button_;
    QTextEdit *content_editor;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->resize(799, 631);
        gridLayout = new QGridLayout(Widget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(Widget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        lineEdit = new QLineEdit(Widget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        horizontalLayout->addWidget(lineEdit);


        horizontalLayout_4->addLayout(horizontalLayout);

        config_file_button = new QPushButton(Widget);
        config_file_button->setObjectName(QStringLiteral("config_file_button"));

        horizontalLayout_4->addWidget(config_file_button);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        dir_lineedit = new QLineEdit(Widget);
        dir_lineedit->setObjectName(QStringLiteral("dir_lineedit"));
        dir_lineedit->setReadOnly(true);

        horizontalLayout_3->addWidget(dir_lineedit);

        browser_button_ = new QPushButton(Widget);
        browser_button_->setObjectName(QStringLiteral("browser_button_"));
        browser_button_->setEnabled(true);
        browser_button_->setAutoDefault(true);

        horizontalLayout_3->addWidget(browser_button_);

        clear_button = new QPushButton(Widget);
        clear_button->setObjectName(QStringLiteral("clear_button"));
        clear_button->setCursor(QCursor(Qt::OpenHandCursor));

        horizontalLayout_3->addWidget(clear_button);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        spinBox = new QSpinBox(Widget);
        spinBox->setObjectName(QStringLiteral("spinBox"));

        horizontalLayout_2->addWidget(spinBox);

        download_button_ = new QPushButton(Widget);
        download_button_->setObjectName(QStringLiteral("download_button_"));

        horizontalLayout_2->addWidget(download_button_);


        horizontalLayout_3->addLayout(horizontalLayout_2);


        verticalLayout->addLayout(horizontalLayout_3);

        content_editor = new QTextEdit(Widget);
        content_editor->setObjectName(QStringLiteral("content_editor"));

        verticalLayout->addWidget(content_editor);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(Widget);

        browser_button_->setDefault(true);


        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", nullptr));
        label->setText(QApplication::translate("Widget", "URL", nullptr));
        config_file_button->setText(QApplication::translate("Widget", "ConfigFile", nullptr));
        browser_button_->setText(QApplication::translate("Widget", "Browse", nullptr));
        clear_button->setText(QApplication::translate("Widget", "Clear", nullptr));
        download_button_->setText(QApplication::translate("Widget", "Download", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
