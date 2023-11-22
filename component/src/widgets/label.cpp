/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 14:28:08
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-16 15:10:02
 * @FilePath: \EOS\component\src\widgets\label.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "label.h"

Label::Label(QWidget*parent):QLabel(parent)
{
    setFrameShape(QFrame::NoFrame);
}

Label::Label(const QString&text,QWidget*parent) :QLabel(parent)
{
    setText(text);
    setFrameShape(QFrame::NoFrame);
}

void Label::mousePressEvent(QMouseEvent *e)
{
    emit mouseClicked();
    e->accept();
}

void Label::setElidedText(const QString &text, Qt::TextElideMode elideMode, int pixelWidth)
{ // 使用本函数时拿结果必须从elidedText函数拿不可以从text去拿
    QFontMetrics fontWidth(font());//得到每个字符的宽度
    QString elideText = fontWidth.elidedText(text, elideMode, pixelWidth);
    setText(elideText);
    elidedtext = text; // 原来的信息要保存
}

QString Label::elidedText() const
{
    return elidedtext;
}