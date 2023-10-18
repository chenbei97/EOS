/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-18 10:42:26
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-18 10:43:09
 * @FilePath: \EOS\interface\src\navigbar.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "navigbar.h"

NavigBar::NavigBar(QWidget*parent): QWidget(parent)
{
    font.setFamily("Times New Roman");
    font.setBold(true);
    font.setPointSize(pointsize);

    QFontMetrics metric(font);
    texts <<"Logo"<<"Preview"<<"Protocol"<<"Data"<<"Analysis";
    foreach(auto text, texts)
        pixwidths << (rectwidth-metric.width(text))/2;
    pixheight = rectheight-metric.height();

    setFixedHeight(rectheight);
    setMinimumWidth(texts.count()*rectwidth
        +(texts.count()-1)*gap);
}

void NavigBar::paintEvent(QPaintEvent*event)
{
    QPainter painter(this);
    auto pen = painter.pen();
    pen.setWidth(2);
    painter.setPen(pen);

    QPainterPath path;
    for(int i = 0; i < texts.count(); ++i) {
        path.clear();
        path.addRect((gap+rectwidth)*i,0,rectwidth,rectheight);
        path.addText(pixwidths[i]+(gap+rectwidth)*i,pixheight,font,texts[i]);
        painter.fillPath(path,NavigBarColor);
    }

    event->accept();
}

void NavigBar::mousePressEvent(QMouseEvent *event)
{
    event->accept();
}
