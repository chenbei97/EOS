/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-17 14:30:46
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-17 14:31:07
 * @FilePath: \EOS\component\src\widgets\colorbutton.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "colorbutton.h"

ColorButton::ColorButton(QWidget*parent): PushButton(parent)
{
    setFixedSize(btnsize,btnsize);//要固定,否则会在布局内被拉伸
    setStyleSheet(ColorButtonStyle.arg(btncolor.name()));
    connect(this,&ColorButton::clicked,this,&ColorButton::onClick);
}

QColor ColorButton::color() const
{
    return btncolor;
}

void ColorButton::setColor(const QColor& color)
{
    btncolor = color;
    setStyleSheet(ColorButtonStyle.arg(color.name()));
}

QColorDialog* ColorButton::colorDialog(const QColor&initColor)
{
    QColorDialog * dlg = new QColorDialog(initColor);
    dlg->setOptions(QColorDialog::ShowAlphaChannel|QColorDialog::DontUseNativeDialog);
    dlg->setWindowTitle(tr("Set Color"));
    dlg->setAttribute(Qt::WA_DeleteOnClose);

    connect(dlg,static_cast<void (QColorDialog::*) (const QColor&)>(&QColorDialog::colorSelected)
            ,this,[=](const QColor& color){
                btncolor = color; // 更新按钮背景颜色
                setStyleSheet(ColorButtonStyle.arg(btncolor.name()));
    });
    return dlg;
}

void ColorButton::onClick()
{
    QColorDialog * dlg = colorDialog(btncolor);
    dlg->exec();
}