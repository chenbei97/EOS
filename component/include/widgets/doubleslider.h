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

class COMPONENT_IMEXPORT DoubleSlider: public QWidget
{
    Q_OBJECT
public:
    explicit DoubleSlider(QWidget*parent= nullptr);
    void setRange(int min,int max);
    void setPrefix(const QString& p);
    void setSuffix(const QString& s);
    void setSingleStep(int step);
    void setMouseEvent(bool enabled);
    double value() const;
    void setValue(double val);

    void addValue(double val);
    void subtractValue(double val);

    void setDirection(Qt::Orientation orientation);
private:
    Sliderx * slider;
    Label * prefix;
    Label * suffix;
    QString suffixtext;
    double accumulateval = 0.0;
    double fracval = 0.0;
    double intval = 0.0;
    void setValue(int val);
private slots:
    void onSliderChanged(int val);
signals:
    void valueChanged(int val);
};

#endif //EOSI_DOUBLESLIDER_H
