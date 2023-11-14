/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-24 10:35:33
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-24 10:36:27
 * @FilePath: \EOS\component\src\widgets\doubleslider.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "doubleslider.h"

DoubleSlider::DoubleSlider(QWidget *parent) : QWidget(parent)
{
    slider = new Sliderx(Qt::Horizontal);
    suffix = new Label;
    prefix = new Label;

    suffix->setText("0.0");

    auto hlay = new QHBoxLayout;
    hlay->addWidget(prefix);
    hlay->addWidget(slider);
    hlay->addWidget(suffix);
    hlay->setMargin(0);
    hlay->setSpacing(0);
    setLayout(hlay);

    connect(slider,&QSlider::sliderMoved,this,&DoubleSlider::onSliderChanged);
    connect(slider,&QSlider::sliderMoved,this,&DoubleSlider::valueChanged);
}

void DoubleSlider::setDirection(Qt::Orientation orientation)
{
    slider->setOrientation(orientation);
}

void DoubleSlider::setMouseEvent(bool enabled)
{
    slider->setMouseEvent(enabled);
}

void DoubleSlider::onSliderChanged(int val)
{
    QString s = QString("%1").arg(val);
    if (!s.contains(".")) s+=".0";
    auto text = QString("%1 %2").arg(s).arg(suffixtext);
    accumulateval = val;
    suffix->setText(text);
}

void DoubleSlider::setRange(int min,int max)
{
    slider->setRange(min,max);
}

void DoubleSlider::setPrefix(const QString& p)
{
    prefix->setText(p);
}

void DoubleSlider::setSuffix(const QString& s)
{
    suffixtext = s;
    suffix->setText(s);
}

void DoubleSlider::setSingleStep(int step)
{
    slider->setSingleStep(step);
}

double DoubleSlider::value() const
{
    //return slider->sliderPosition();
    return accumulateval;
}

void DoubleSlider::setValue(double val)
{ // 后缀名义上的值
    accumulateval = val;
    QString s = QString("%1").arg(accumulateval);
    if (!s.contains(".")) s+=".0";
    auto text = QString("%1 %2").arg(s).arg(suffixtext);
    suffix->setText(text);
    slider->blockSignals(true);
    setValue((int)accumulateval-1);
    slider->blockSignals(false);
}

void DoubleSlider::setValue(int value)
{ // 给滑动条设置值
    slider->setValue(value);
    slider->setSliderPosition(value);
    if (value<=slider->minimum())
        slider->setValue(slider->minimum());
    if (value>=slider->maximum())
        slider->setValue(slider->maximum());
    slider->sliderMoved(value);
}

void DoubleSlider::addValue(double val)
{
#ifdef DoubleVeryBig
    double intpart, fracpart;
    fracpart = modf(val,&intpart);
    intval += intpart; // 取出每次步进的整数和小数部分
    fracval += fracpart;

    double frac_intpart,frac_fracpart;
    frac_fracpart = modf(fracval,&frac_intpart);
    if (frac_intpart>=1.0) { // 对于小数部分可能会累加到>=1.0,把这部分整数挪到整数部分
        intval += frac_intpart;
        fracval -= frac_intpart;
    }

    if (intval+fracval >= (double)slider->maximum()
        || qAbs(intval+fracval - (double)slider->maximum())<1e-5){
        intval = (double)slider->maximum();
        fracval = 0.0;
    }

    auto num = QString("%1.%2").arg(intval).arg(QString::number(fracval).remove("0."));
    auto text = QString("%1 %2").arg(num).arg(suffixtext); // 数特别大的话用num

#else
    accumulateval += val;
    if (accumulateval >= (double)slider->maximum() || qAbs(accumulateval - (double)slider->maximum())<1e-5)
        accumulateval = (double)slider->maximum();
    QString s = QString("%1").arg(accumulateval);
    if (!s.contains(".")) s+=".0";
    auto text = QString("%1 %2").arg(s).arg(suffixtext); // 数特别大的话用num
#endif
    suffix->setText(text);
    slider->blockSignals(true); // 防止触发slidermove信号,这个是int的会重新设置suffix
    setValue((int)accumulateval-1);
    slider->blockSignals(false);
}

void DoubleSlider::subtractValue(double val)
{
#ifdef DoubleVeryBig
    double intpart, fracpart;// 700.2-200.5=499.7
    fracpart = modf(val,&intpart);
    intval -= intpart;  // 700-200 = 500

    if (fracval < fracpart) { // 0.2-0.5
        intval -= 1; // 499
        fracval = (fracval-fracpart)+1.0; // 1.0+(0.2-0.5)=0.7
    } else fracval -= fracpart;

    if (intval+fracval <= (double)slider->minimum()
        || qAbs(intval+fracval - (double)slider->minimum())<1e-5){
        intval = (double)slider->minimum();
        fracval = 0.0;
    }
    auto num = QString("%1.%2").arg(intval).arg(QString::number(fracval).remove("0."));
    auto text = QString("%1 %2").arg(num).arg(suffixtext);
#else
    accumulateval -= val;
    if (qAbs(accumulateval - (double)slider->minimum())<1e-5 || accumulateval<=(double)slider->minimum())
        accumulateval = (double)slider->minimum(); // 有可能差值是个很小的正数,也要视为0
    QString s = QString("%1").arg(accumulateval);
    if (!s.contains(".")) s+=".0";
    auto text = QString("%1 %2").arg(s).arg(suffixtext);
#endif
    suffix->setText(text);
    slider->blockSignals(true);
    setValue((int)accumulateval+1);
    slider->blockSignals(false);
}