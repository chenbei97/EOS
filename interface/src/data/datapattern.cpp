/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2024-01-10 08:39:41
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2024-01-10 08:51:59
 * @FilePath: \EOS\interface\src\data\datapattern.cpp
 * @Copyright (c) 2024 by ${chenbei}, All Rights Reserved. 
 */

#include "datapattern.h"



void DataPattern::mousePressEvent(QMouseEvent *event)
{
    if (mMouseEvent) {
        if (event->button() == Qt::LeftButton) {
            mLastPos = event->pos();
            if (isValidPoint(mLastPos)) {
                QPoint tmp(-1,-1);
                auto rects = getAllInnerRects(); // 所有小正方形区域匹配这个坐标
                for(int row = 0; row < mrows; ++row)
                    for(int col = 0; col < mcols; ++col)
                        if (rects[row][col].contains(mLastPos))
                            tmp = {row,col}; // 找到孔的匹配坐标
                //LOG<<mLastPos<<mMousePos<<tmp;
                if (tmp != QPoint(-1,-1)) { // 点击的不是无效位置,确实是某个孔
                    if(tmp != mMousePos) { // mMousePos是上次点击的有效位置,而且不是重复点相同的孔
                        mMousePos = tmp;
                        emit holeClicked(mHoleInfo[mMousePos.x()][mMousePos.y()]); // 新位置和原来位置不同才需要更新
                    }
                }
            }
        }
    }
    update();
    event->accept();
}

void DataPattern::paintEvent(QPaintEvent *event)
{
    Pattern::paintEvent(event);

    if (!mMouseEvent) return;

    auto cell_size = getInnerRectSize();
    auto cell_w = cell_size.width();
    auto cell_h = cell_size.height();
    auto radius = cell_w>=cell_h? cell_h/2: cell_w/2; // 选较小的确保圆在矩形内

    QPainter painter(this);
    QPainterPath path;
    for(int row = 0 ; row < mrows; ++ row) {
        for (int col = 0; col < mcols; ++col) {
            auto center  = QPointF(mGap+cell_w/2+mSpace+(cell_w+mSpace)*col,
                                   mGap+cell_h/2+mSpace+(cell_h+mSpace)*row);

            // (1) 绘制确实选中的孔
            if (mHoleInfo[row][col].isSelected && !mDisableHoles[row][col]) //  绘制确定选中的点,不可选的孔不允许绘制
            {
                path.clear();
                path.moveTo(center);
                path.addEllipse(center,radius*0.75,radius*0.75);
                painter.fillPath(path,Qt::red);
            }

            // (2) 绘制鼠标点击的高亮
            //启用了鼠标事件mMousePos才会被赋值,否则是(-1,-1),所以这里不用再判断是否启用了鼠标事件
            if (mMousePos.x() == row && mMousePos.y() == col
                //&& !mHoleInfo[row][col].isSelected// 已绘制的点不要绘制鼠标选中高亮
                && !mDisableHoles[row][col]) { // 置灰不可选的点不要绘制鼠标高亮
                path.clear();
                path.moveTo(center);
                path.addEllipse(center, radius * 0.75, radius * 0.75);
                painter.fillPath(path, Qt::blue);
            }

            // (3) 绘制不可选置灰的点,将其高亮颜色变为灰色,并画个x
            if (mDisableHoles[row][col]) {
                path.clear();
                path.moveTo(center);
                path.addEllipse(center,radius,radius);
                painter.fillPath(path,mInnerCircleColor);

                auto p11 = center + QPointF(radius*sqrt(2)/2,-radius* sqrt(2)/2);
                auto p12 = center - QPointF(radius*sqrt(2)/2,-radius* sqrt(2)/2);
                auto p21 = center + QPointF(radius*sqrt(2)/2,radius* sqrt(2)/2);
                auto p22 = center - QPointF(radius*sqrt(2)/2,radius* sqrt(2)/2);
                painter.drawLine(p11,p12);
                painter.drawLine(p21,p22);
            }
        }
    }
}

void DataPattern::initHoleInfo(const PlateImageInfo &info)
{
    for(auto image: info.images) {
        auto hole_coordinate = image.view.hole.getCoordinate();
        //LOG<<hole_coordinate<<patternSize();
        mHoleInfo[hole_coordinate.x()][hole_coordinate.y()].isSelected = true;
        mHoleInfo[hole_coordinate.x()][hole_coordinate.y()].info = info;
        Q_ASSERT(mHoleInfo[hole_coordinate.x()][hole_coordinate.y()].hole == hole_coordinate);
    }
}

DataPattern::DataPattern(int rows, int cols, QWidget *parent) : Pattern(rows,cols,parent)
{
    mMouseEvent = true;
    initHoleInfo();
    initDisableHoles();
    setContextMenuPolicy(Qt::ActionsContextMenu);
}

void DataPattern::setPatternSize(int rows, int cols)
{
    Pattern::setPatternSize(rows,cols);
    initDisableHoles();
    initHoleInfo();
}


void DataPattern::initHoleInfo()
{
    mHoleInfo.clear();
    for(int row = 0 ; row < mrows; ++ row) {
        DataPatternHoleInfoVector vec;
        for (int col = 0; col < mcols; ++col){
            DataPatternHoleInfo info;
            info.isSelected = false;
            info.hole = QPoint(row,col);
            vec.append(info);
        }
        mHoleInfo.append(vec);
    }
    update();
}

void DataPattern::initDisableHoles()
{
    mDisableHoles.clear();
    for(int row = 0 ; row < mrows; ++ row) {
        QBoolVector var;
        for (int col = 0; col < mcols; ++col){
            var.append(false);
        }
        mDisableHoles.append(var);
    }
    update();
}