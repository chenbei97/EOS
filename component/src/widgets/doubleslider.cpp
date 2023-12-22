/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-24 10:35:33
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-24 10:36:27
 * @FilePath: \EOS\component\src\widgets\doubleslider.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "doubleslider.h"

void DoubleSlider::setScaleFactor(int factor)
{ // factor只能大于等于1
    // 为了模拟滑动条浮点数的效果,例如把0-10的所有浮点数换算到0-10000即可
    if (factor<1)
        scalefactor = 1;
    else scalefactor = factor;
    slider->setMaximum(scalefactor*slider->maximum());
}

void DoubleSlider::onSliderMoved(int val)
{
    accumulateval = (double)val / scalefactor;
    //tmpval = accumulateval; // 滑动是一定会改变的
    QString s = QString("%1").arg(accumulateval);
    if (!s.contains(".")) s+=".0";
    auto text = QString("%1 %2").arg(s).arg(suffixtext);
    suffix->setText(text);
    emit sliderMoved(accumulateval);
}

void DoubleSlider::onSliderReleased()
{// 引入tmpval原因是: 单击slider的滑块似乎release信号会发送2次导致相同的值重复发送
    if (accumulateval != tmpval) // 注意tmpval初始值用负数,避免更改到0.0时不触发
    {
        emit sliderReleased(accumulateval); // 释放不一定会改变值
        //LOG<<"last val ="<<tmpval<<" now ="<<accumulateval <<"has changed";
        tmpval = accumulateval;
    } else {
        //LOG<<"val not changed!";
    }

}

void DoubleSlider::setValue(double val)
{ // 后缀名义上的值
    accumulateval = val;
    QString s = QString("%1").arg(accumulateval);
    if (!s.contains(".")) s+=".0";
    auto text = QString("%1 %2").arg(s).arg(suffixtext);
    suffix->setText(text);
    slider->blockSignals(true);
    setValue_(accumulateval);
    slider->blockSignals(false);
}

void DoubleSlider::setValue_(double value)
{
    value = value * scalefactor;
    if (value<=slider->minimum())// 浮点数可能会累计超过最值
        slider->setValue(slider->minimum());
    else if (value>=slider->maximum())
        slider->setValue(slider->maximum());
    else slider->setValue(value);// 给滑动条设置值会自动取整
}

void DoubleSlider::addValue(double val)
{ // 浮点数很大的时候会出现被系统丢去精度
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
    auto tmp = accumulateval * scalefactor;
    if (tmp >= (double)slider->maximum() || qAbs(tmp - (double)slider->maximum())<1e-5)
        accumulateval = (double)slider->maximum() / scalefactor;
    QString s = QString("%1").arg(accumulateval);
    if (!s.contains(".")) s+=".0";
    auto text = QString("%1 %2").arg(s).arg(suffixtext); // 数特别大的话用num
#endif
    suffix->setText(text);
    slider->blockSignals(true); // 防止触发slidermove信号,这个是int的会重新设置suffix
    setValue_(accumulateval);
    slider->blockSignals(false);

    if (tmpval != accumulateval) {
        emit sliderMoved(accumulateval);
        emit sliderReleased(accumulateval);
        tmpval = accumulateval;
    }
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
    auto tmp = accumulateval * scalefactor;
    if (qAbs(tmp - (double)slider->minimum())<1e-5 || tmp<=(double)slider->minimum())
        accumulateval = (double)slider->minimum() / scalefactor; // 有可能差值是个很小的正数,也要视为0
    QString s = QString("%1").arg(accumulateval);
    if (!s.contains(".")) s+=".0";
    auto text = QString("%1 %2").arg(s).arg(suffixtext);
#endif
    suffix->setText(text);
    slider->blockSignals(true);
    setValue_(accumulateval);
    slider->blockSignals(false);

    if (tmpval != accumulateval) {
        emit sliderMoved(accumulateval);
        emit sliderReleased(accumulateval);
        tmpval = accumulateval;
    }
}

DoubleSlider::DoubleSlider(QWidget *parent) : QWidget(parent)
{
    slider = new Sliderx(Qt::Horizontal);
    suffix = new Label;
    prefix = new Label;

    suffix->setText("0.000");

    auto hlay = new QHBoxLayout;
    hlay->addWidget(prefix);
    hlay->addWidget(slider);
    hlay->addWidget(suffix);
    hlay->setMargin(0);
    hlay->setSpacing(0);
    setLayout(hlay);

    connect(slider,&QSlider::sliderMoved,this,&DoubleSlider::onSliderMoved);
    connect(slider,&QSlider::sliderReleased,this,&DoubleSlider::onSliderReleased);
}

void DoubleSlider::setDirection(Qt::Orientation orientation)
{
    slider->setOrientation(orientation);
}

void DoubleSlider::setMouseEvent(bool enabled)
{
    slider->setMouseEvent(enabled);
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
    return accumulateval;
}

void DoubleSlider::setPrefixVisible(bool visible)
{
    prefix->setVisible(visible);
}

void DoubleSlider::setSuffixVisible(bool visible)
{
    suffix->setVisible(visible);
}