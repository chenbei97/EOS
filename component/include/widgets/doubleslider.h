/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-24 10:35:42
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-24 10:36:35
 * @FilePath: \EOS\component\include\widgets\doubleslider.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_DOUBLESLIDER_H
#define EOSI_DOUBLESLIDER_H

#include "widgets.h"
#include "sliderx.h"
#include "label.h"

#define DoubleNotBig 0
//#define DoubleVeryBig 0
// qslider本身不支持浮点数,需要1个中间double变量去代替保存值和显示到ui
// 也就是accumulateval,这个也要取整反馈给滑动条,实际值10.4,视觉上滑动条设置10不会影响
class COMPONENT_IMEXPORT DoubleSlider: public QWidget
{
    Q_OBJECT
public:
    explicit DoubleSlider(QWidget*parent= nullptr);
    void setRange(int min,int max);
    void setPrefix(const QString& p);
    void setSuffix(const QString& s);
    void setSingleStep(int step);
    void setScaleFactor(int factor);
    void setMouseEvent(bool enabled);

    void setSuffixVisible(bool visible);
    void setPrefixVisible(bool visible);

    double value() const;
    void setValue(double val); // 外部可以传double

    void addValue(double val);
    void subtractValue(double val);

    void setDirection(Qt::Orientation orientation);
private:
    Sliderx * slider;
    Label * prefix;
    Label * suffix;
    QString suffixtext;
    int scalefactor = 1;
    double accumulateval = 0.0;
    double tmpval = -1.0;
    void setValue_(double val);//内部使用转换到滑动条上

    double fracval = 0.0;
    double intval = 0.0;
private slots:
    void onSliderMoved(int val);
    void onSliderReleased();
signals:
    void sliderMoved(double val);
    void sliderReleased(double val);
};

#endif //EOSI_DOUBLESLIDER_H
