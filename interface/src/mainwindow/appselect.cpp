/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-18 12:59:27
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-18 12:59:53
 * @FilePath: \EOS\interface\src\window\appselect.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "appselect.h"

AppSelect::AppSelect(QWidget*parent): QWidget(parent)
{
    foreach(auto text, AppFields)
       pixelwidths<< (AppSelectButtonWidth-NavigPainterMetric.width(text))/2;
}

void AppSelect::paintEvent(QPaintEvent*event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    auto pen = painter.pen();
    pen.setWidth(2);
    painter.setPen(pen);

    //drawRoundRectV1(painter); // 不使用循环的写法,简单易懂
    drawRoundRectV2(painter); // 循环写法,代码简洁

    event->accept();
}

void AppSelect::mousePressEvent(QMouseEvent *event)
{
    auto pos = event->pos();
    auto rects = getRects();

    for(int c = 0; c < rects.count(); ++c) {
        if (rects[c].contains(pos)) {
            //LOG<<"click app index = "<<c;
            switch (c) {
                case 0: emit proliferationClicked();break;
                case 1: emit scratchClicked();break;
                case 2: emit invasionClicked();break;
                case 3: emit transfectionClicked();break;
                case 4: emit morphologyClicked();break;
                case 5: emit organoidClicked();break;
                case 6: emit pharmtoxiClicked();break;
                case 7: emit customClicked();break;
                default:break;
            }
            emit appClicked(c);
            emit appClicked(AppFields[c]);
        }
    }
    event->accept();
}

QRectVector AppSelect::getRects() const
{// 获取所有按钮的小正方形区域,从左到右从上到下
    QRectVector vec;

    for(int r = 0; r < AppSelectButtonCountPerRow; ++ r) {
        for (int c = 0; c < AppSelectButtonCountPerCol; ++c) {
            auto rect_x = AppSelectHorGap+AppSelectMoveHorDistance*c;
            auto rect_y = AppSelectVerGap+AppSelectMoveVerDistance*r;
            vec.append(QRect(rect_x,rect_y,AppSelectButtonWidth,AppSelectButtonHeight));
        }
    }

    return vec;
}

void AppSelect::drawRoundRectV2(QPainter &painter)
{ // 使用循环的写法,目前是2行4列
    QPainterPath path;
    for(int r = 0; r < AppSelectButtonCountPerRow; ++ r) {
        for(int c = 0; c < AppSelectButtonCountPerCol; ++c) {
            path.clear();
            auto rect_x = AppSelectHorGap+AppSelectMoveHorDistance*c;
            auto rect_y = AppSelectVerGap+AppSelectMoveVerDistance*r;
            path.addRoundedRect(rect_x,rect_y,
                                AppSelectButtonWidth,AppSelectButtonHeight,
                                AppSelectButtonRoundRadius,AppSelectButtonRoundRadius);
            auto idx = r * AppSelectButtonCountPerCol + c; // 例如(1,0)应该1x4+0=4,一维列表的索引位置4 r*4+c=idx
            auto pixwidth = pixelwidths[idx]; // (AppSelectButtonWidth-PainterMetric.width(texts[idx]))/2;
            auto x = AppSelectHorGap+(AppSelectMoveHorDistance)*c+pixwidth; // 到左侧距离+水平移动距离*0+文字相对按钮的水平距离
            auto y = AppSelectVerGap+(AppSelectMoveVerDistance)*r+AppSelectButtonFontHeight; // 到顶端距离+垂直移动+文字相对按钮垂直距离
            path.addText(x,y,NavigPainterFont,AppFields[idx]);
            painter.fillPath(path,AppSelectColor);
        }
    }
}

void AppSelect::drawRoundRectV1(QPainter &painter)
{
    QPainterPath path;
    // (1,1)
    path.clear();
    path.addRoundedRect(AppSelectHorGap+AppSelectMoveHorDistance*0,
                        AppSelectVerGap+AppSelectMoveVerDistance*0,
                        AppSelectButtonWidth,AppSelectButtonHeight,
                        AppSelectButtonRoundRadius,AppSelectButtonRoundRadius);
    auto pixwidth = (AppSelectButtonWidth-NavigPainterMetric.width("增值"))/2;
    auto x = AppSelectHorGap+(AppSelectMoveHorDistance)*0+pixwidth; // 到左侧距离+水平移动距离*0+文字相对按钮的水平距离
    auto y = AppSelectVerGap+(AppSelectMoveVerDistance)*0+AppSelectButtonFontHeight; // 到顶端距离+垂直移动+文字相对按钮垂直距离
    path.addText(x,y,NavigPainterFont,"增殖");
    painter.fillPath(path,AppSelectColor);

    //(1,2)
    path.clear();
    path.addRoundedRect(AppSelectHorGap+AppSelectMoveHorDistance*1,
                        AppSelectVerGap+AppSelectMoveVerDistance*0,// y不需要动
                        AppSelectButtonWidth,AppSelectButtonHeight,
                        AppSelectButtonRoundRadius,AppSelectButtonRoundRadius);
    pixwidth = (AppSelectButtonWidth-NavigPainterMetric.width("划痕"))/2;
    x = AppSelectHorGap+(AppSelectMoveHorDistance)*1+pixwidth;
    y = AppSelectVerGap+(AppSelectMoveVerDistance)*0+AppSelectButtonFontHeight;// y不需要动
    path.addText(x,y,NavigPainterFont,"划痕");
    painter.fillPath(path,AppSelectColor);

    //(1,3)
    path.clear();
    path.addRoundedRect(AppSelectHorGap+AppSelectMoveHorDistance*2,
                        AppSelectVerGap+AppSelectMoveVerDistance*0,// y不需要动
                        AppSelectButtonWidth,AppSelectButtonHeight,
                        AppSelectButtonRoundRadius,AppSelectButtonRoundRadius);
    pixwidth = (AppSelectButtonWidth-NavigPainterMetric.width("侵袭"))/2;
    x = AppSelectHorGap+(AppSelectMoveHorDistance)*2+pixwidth;
    y = AppSelectVerGap+(AppSelectMoveVerDistance)*0+AppSelectButtonFontHeight;// y不需要动
    path.addText(x,y,NavigPainterFont,"侵袭");
    painter.fillPath(path,AppSelectColor);

    //(1,4)
    path.clear();
    path.addRoundedRect(AppSelectHorGap+AppSelectMoveHorDistance*3,
                        AppSelectVerGap+AppSelectMoveVerDistance*0,// y不需要动
                        AppSelectButtonWidth,AppSelectButtonHeight,
                        AppSelectButtonRoundRadius,AppSelectButtonRoundRadius);
    pixwidth = (AppSelectButtonWidth-NavigPainterMetric.width("转染"))/2;
    x = AppSelectHorGap+(AppSelectMoveHorDistance)*3+pixwidth;
    y = AppSelectVerGap+(AppSelectMoveVerDistance)*0+AppSelectButtonFontHeight;// y不需要动
    path.addText(x,y,NavigPainterFont,"转染");
    painter.fillPath(path,AppSelectColor);

    //(2,1)
    path.clear();
    path.addRoundedRect(AppSelectHorGap+AppSelectMoveHorDistance*0,// x回到0
                        AppSelectVerGap+AppSelectMoveVerDistance*1,// y+1个距离
                        AppSelectButtonWidth,AppSelectButtonHeight,
                        AppSelectButtonRoundRadius,AppSelectButtonRoundRadius);
    pixwidth = (AppSelectButtonWidth-NavigPainterMetric.width("形态学"))/2;
    x = AppSelectHorGap+(AppSelectMoveHorDistance)*0+pixwidth; // 回到0
    y = AppSelectVerGap+(AppSelectMoveVerDistance)*1+AppSelectButtonFontHeight;// y+1个距离
    path.addText(x,y,NavigPainterFont,"形态学");
    painter.fillPath(path,AppSelectColor);

    //(2,2)
    path.clear();
    path.addRoundedRect(AppSelectHorGap+AppSelectMoveHorDistance*1,// x回到1
                        AppSelectVerGap+AppSelectMoveVerDistance*1,// y+1个距离
                        AppSelectButtonWidth,AppSelectButtonHeight,
                        AppSelectButtonRoundRadius,AppSelectButtonRoundRadius);
    pixwidth = (AppSelectButtonWidth-NavigPainterMetric.width("类器官"))/2;
    x = AppSelectHorGap+(AppSelectMoveHorDistance)*1+pixwidth; // 回到1
    y = AppSelectVerGap+(AppSelectMoveVerDistance)*1+AppSelectButtonFontHeight;// y+1个距离
    path.addText(x,y,NavigPainterFont,"类器官");
    painter.fillPath(path,AppSelectColor);

    //(2,3)
    path.clear();
    path.addRoundedRect(AppSelectHorGap+AppSelectMoveHorDistance*2,// x回到2
                        AppSelectVerGap+AppSelectMoveVerDistance*1,// y+1个距离
                        AppSelectButtonWidth,AppSelectButtonHeight,
                        AppSelectButtonRoundRadius,AppSelectButtonRoundRadius);
    pixwidth = (AppSelectButtonWidth-NavigPainterMetric.width("药效和毒理"))/2;
    x = AppSelectHorGap+(AppSelectMoveHorDistance)*2+pixwidth; // 回到2
    y = AppSelectVerGap+(AppSelectMoveVerDistance)*1+AppSelectButtonFontHeight;// y+1个距离
    path.addText(x,y,NavigPainterFont,"药效和毒理");
    painter.fillPath(path,AppSelectColor);

    //(2,4)
    path.clear();
    path.addRoundedRect(AppSelectHorGap+AppSelectMoveHorDistance*3,// x回到3
                        AppSelectVerGap+AppSelectMoveVerDistance*1,// y+1个距离
                        AppSelectButtonWidth,AppSelectButtonHeight,
                        AppSelectButtonRoundRadius,AppSelectButtonRoundRadius);
    pixwidth = (AppSelectButtonWidth-NavigPainterMetric.width("自定义"))/2;
    x = AppSelectHorGap+(AppSelectMoveHorDistance)*3+pixwidth; // 回到3
    y = AppSelectVerGap+(AppSelectMoveVerDistance)*1+AppSelectButtonFontHeight;// y+1个距离
    path.addText(x,y,NavigPainterFont,"自定义");
    painter.fillPath(path,AppSelectColor);
}