/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-25 08:10:44
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-25 08:25:09
 * @FilePath: \EOS\interface\src\preview\previewpattern.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "previewpattern.h"

PreviewPattern::PreviewPattern(int rows, int cols,QWidget*parent): Pattern(rows,cols,parent)
{
    mMouseEvent = true;
}

void PreviewPattern::paintEvent(QPaintEvent *event)
{
    Pattern::paintEvent(event);

    auto cell_size = getInnerRectSize();
    auto cell_w = cell_size.width();
    auto cell_h = cell_size.height();
    auto radius = cell_w>=cell_h? cell_h/2: cell_w/2; // 选较小的确保圆在矩形内
    auto  centerPts = getCenterPoints();

    QPainter painter(this);
    QPainterPath path;

    for(int row = 0 ; row < mrows; ++ row) {
        for (int col = 0; col < mcols; ++col) {
            auto center = centerPts[row][col];
            //启用了鼠标事件mMousePos才会被赋值,否则是(-1,-1),所以这里不用再判断是否启用了鼠标事件
            if (mMousePos.x() == row && mMousePos.y() == col) {
                path.clear();
                path.moveTo(center);
                path.addEllipse(center, radius * 0.75, radius * 0.75);
                painter.fillPath(path, mMouseClickColor);
            }
        }
    }
    event->accept();
}

void PreviewPattern::mouseReleaseEvent(QMouseEvent *event)
{
    if (mMouseEvent) {
        //LOG<<"mouse point = "<<mMousePos;
        if (mMousePos == QPoint(-1,-1))
            return; // 可能会点到边缘位置导致下方获取颜色越界
    }
    event->accept();
}
