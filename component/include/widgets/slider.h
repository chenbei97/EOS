/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 15:24:08
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-16 16:01:19
 * @FilePath: \EOS\component\include\widgets\slider.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_SLIDER_H
#define EOSI_SLIDER_H

#include "widgets.h"
#include "label.h"

class COMPONENT_IMEXPORT Slider: public QWidget
{
    Q_OBJECT
public:
    explicit Slider(QWidget *parent = nullptr);
    void setRange(int min,int max);
    void setPrefix(const QString& p);
    void setSuffix(const QString& s);
private:
    QSlider * slider;
    Label * prefix;
    Label * suffix;
    QString suffixtext;
private slots:
     void onSliderChanged(int val);
};

#endif //EOSI_SLIDER_H