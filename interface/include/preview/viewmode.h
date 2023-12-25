/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-12-04 13:47:39
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-20 10:11:41
 * @FilePath: \EOS\interface\include\preview\viewmode.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_MAIN_CORE_PY_VIEWMODE_H
#define EOS_MAIN_CORE_PY_VIEWMODE_H

#include "window.h"

class INTERFACE_IMEXPORT ViewModeBox: public GroupBox
{
    Q_OBJECT
public:
    explicit ViewModeBox(QWidget*parent= nullptr);
    void setViewEnabled(int option);
    ViewMode viewMode() const;
    ViewModeInfo viewModeInfo() const;
private:
    RadioGroup * groupMode;
    TriangleMove * triangleMove;
    PushButton * enableGroupBtn;
    PushButton * endGroupBtn;
    ComboBox * groupType;
    void initObjects();
    void initAttributes();
    void initLayout();
    void radioClick(int mode);
    void triangleClick(int option);
    void enableGroup();
    void endGroup();
    void toggleGroup(const QString& text);
signals:
    void modeSelected(int mode);
    void triangleClicked(int option);
    void enableWellPattern(bool enable);
    void groupTypeChanged(const QString& group);
    void groupColorChanged(const QColor& color);
};

static const QColorVector GroupTypeColors = { // 13x3 = 39个颜色
        "#8F8F8F","#BABABA","#E3E3E3", // 灰色
        "#942017","#C72B1E","#FF3727", // 红色
        "#9C5756","#D17573","#FF8F8D", // 褐色->玫瑰红
        "#817F26","#C4C13A","#EDE946", // 黄色 - 金色
        "#3E888A","#5AC5C7","#73FCFF", // 青色
        "#7A1C43","#BF2C69","#FF3B8B", // 深粉 => 淡粉 (255,59,139)
        "#57BA48","#67EB40","#70FF45",  // 浅绿(32,255,177)
        "#18B880","#1AD191","#20FFB1",   // 绿色
        "#741B7C","#B82BC4","#EF38FF",// 紫色 (239,56,255)
        "#1D4B8F","#2766C2","#3386FF", // 蓝色
        "#686B99","#8589C4","#ADB2FF", // 靛蓝 (173,178,255)
        "#3F1287","#5B1AC4","#7621FF",// 深蓝偏紫
        "#0018A8","#0021E8","#0024FF"// 深蓝偏蓝
};
#endif //EOS_MAIN_CORE_PY_VIEWMODE_H
