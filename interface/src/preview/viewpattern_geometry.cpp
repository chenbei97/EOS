/***
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-18 15:40:45
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-18 15:42:19
 * @FilePath: \EOS\interface\src\window\viewpattern_geometry.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved.
 */

#include "viewpattern.h"

double ViewPattern::getCircleRadius() const
{ // 视野圆半径
    return width()>=height()?height()/2.0:width()/2.0;
}

double ViewPattern::getInnerRectWidth() const
{// 小矩形区域的宽度
    return 2 * getCircleRadius() / mrows *1.0;
}

double ViewPattern::getInnerRectHeight() const
{// 小矩形区域的高度
    return 2 * getCircleRadius() / mcols *1.0;
}

QPointF ViewPattern::getInnerRectTopLeftPoint() const
{// 外接正方形左上角顶点
    return QPointF(width()/2.0-getCircleRadius(),0);
}

QPointF ViewPattern::getInnerRectTopRightPoint() const
{// 外接正方形右上角顶点
    return QPointF(width()/2.0+getCircleRadius(),0);
}

QPointF ViewPattern::getInnerRectBottomLeftPoint() const
{// 外接正方形左下角顶点
    return QPointF(width()/2.0-getCircleRadius(),height());
}

QPointF ViewPattern::getInnerRectBottomRightPoint() const
{// 外接正方形右下角顶点
    return QPointF(width()/2.0+getCircleRadius(),height());
}

QPointFVector ViewPattern::getInnerVerticalLineTopPoints() const
{// 正方形顶侧的等分点
    QPointFVector vec;
    auto offset = getInnerRectWidth();
    auto topleft= getInnerRectTopLeftPoint();
    for (int i = 1; i <= mrows-1; ++i)
        vec.append(topleft+ QPoint(i*offset,0));
//    LOG<<"topleft = "<<topleft<<" offset = "<<offset<<" mrows = "<<mrows
//    <<" radius = "<<getExternalCircleRadius();
    return vec;
}

QPointFVector ViewPattern::getInnerVerticalLineBottomPoints() const
{// 正方形底侧的等分点
    QPointFVector vec;
    auto offset = getInnerRectWidth();
    auto bottomleft= getInnerRectBottomLeftPoint();
    for (int i = 1; i <= mrows-1; ++i)
        vec.append(bottomleft+ QPoint(i*offset,0));
    return vec;
}

QPointFVector ViewPattern::getInnerHorizonalLineLeftPoints() const
{// 正方形左侧的等分点
    QPointFVector vec;
    auto offset = getInnerRectHeight(); // 点之间y坐标相差的是小矩形高度
    auto topleft = getInnerRectTopLeftPoint();
    for (int i = 1; i <= mcols-1; ++i) // i = 1开始是不包含topleft
        vec.append(topleft+ QPoint(0,i*offset)); // x不变,y增加
    return vec;
}

QPointFVector ViewPattern::getInnerHorizonalLineRightPoints() const
{// 正方形右侧的等分点
    QPointFVector vec;
    auto offset = getInnerRectHeight(); // 点之间y坐标相差的是小矩形高度
    auto topright = getInnerRectTopRightPoint();
    for (int i = 1; i <= mcols-1; ++i) // i = 1开始是不包含topright
        vec.append(topright+ QPoint(0,i*offset)); // x不变,y增加
    return vec;
}

QRectF2DVector ViewPattern::getInnerRects() const
{
    QRectF2DVector m;
    auto hoffset = getInnerRectWidth();
    auto voffset = getInnerRectHeight();

    auto start = getInnerRectTopLeftPoint();
    for(int i = 0 ; i < mrows; ++i) {
        QRectFVector vec;
        for(int j = 0; j < mcols; ++j) { // j*offset加在x坐标也就是水平方向
            auto topleft = start + QPointF(j*hoffset,i*voffset); // x, x+d，j依次取0,1; y先不变在后
            auto bottomright = topleft + QPointF(hoffset,voffset);
            vec.append(QRectF(topleft,bottomright));
        }
        m.append(vec);
    }

    return m;
}

void ViewPattern::updatePatternUi()
{ // objective更新后视野窗口更新了,但是孔图案的ui绘制点还在,要刷新一下(本函数弃用,Preview::updateViewPatternUi内调用setStrategy和initHoeInfo更新了ui)

    // 1. 拿到当前孔的id
    auto coordinate = mCurrentViewInfo[HoleCoordinateField].toPoint();
    auto idx = coordinate.x()*PointToIDCoefficient+coordinate.y();// 保证索引唯一不重叠2k+y,每个孔对应唯一的idx

    // 2. 清除选择的视野信息
    for(int r = 0; r < mrows; ++r) {
        for(int c = 0; c < mcols; ++c) {//调用updateApplyGroup之前已经setStrategy更新了mrows,mcols并分配空间不会越界
            mViewSelectPoints[idx][r][c] = false;
        }
    }
    mTmpViewSelectPoints[idx].clear(); // 以前保存的临时信息清空,其实被setStrategy清空过了,视野尺寸不配的时候,加层保护

    // 3. 重新组装数据,除了viewpoint是空的其它不变(pattern的组颜色+名称+坐标这些是不用动的)
    QVariantMap m;
    m[HoleGroupNameField] = mCurrentViewInfo[HoleGroupNameField];// 组装组名称,方便pattern依据组名查找所有孔
    m[HoleGroupColorField] = mCurrentViewInfo[HoleGroupColorField]; // 组装组颜色,可以让pattern把同组内其他可能不相同的颜色全部统一
    m[HoleViewSizeField] = mCurrentViewInfo[HoleViewSizeField]; // 这是为了pattern画点使用
    m[HoleCoordinateField] = mCurrentViewInfo[HoleCoordinateField]; // 坐标信息顺带组装
    m[WellAllGroupsField] = mCurrentViewInfo[WellAllGroupsField]; // 所有组名信息顺带组装
    QVariant v;
    v.setValue(QPointVector());//去更新mViewInfo[row][col].viewpoints就是空的
    m[HoleViewPointsField] = v;
    emit applyGroupEvent(m);

    // 4. 还应该把其它组的所有孔信息也清除,不仅仅是本组的孔(触发应用到所有组的孔)
            foreach(auto groupname, m[WellAllGroupsField].value<QSet<QString>>()) {
            m[HoleGroupNameField] = groupname;
            emit applyGroupEvent(m);
        }
    // 这些工作已经在clearViewInfo做了

    // 5.同时更新同组其它孔的视野信息和临时信息也都重新分配空间并清空
    auto groupPoints = mCurrentViewInfo[HoleGroupCoordinatesField].value<QPointVector>();//拿到本组其它孔的所有坐标
            foreach(auto pt, groupPoints) {
            auto pt_idx = pt.x()*PointToIDCoefficient+pt.y(); // 本组其他孔的临时数据区更新为当前孔的视野信息
            mTmpViewSelectPoints[pt_idx] = mViewSelectPoints[idx]; //mViewSelectPoints[idx]已经分配过空间全是false
            mViewSelectPoints[pt_idx] = mViewSelectPoints[idx];
        }

    //applygroupact->trigger();// 如果没选择过视野不能触发,所以只能多写上述代码
}