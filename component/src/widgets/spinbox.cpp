/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 14:39:01
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-16 15:10:22
 * @FilePath: \EOS\component\src\widgets\spinbox.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "spinbox.h"

SpinBox::SpinBox(QWidget*parent):QDoubleSpinBox(parent)
{
    setDecimals(0);
    setValue(1);
}

void SpinBox::wheelEvent(QWheelEvent *e)
{
    QPoint angle = e->angleDelta();
    if (angle.y() > 0) { // 鼠标前转,数值增加
        if (value()+singleStep() <= maximum()) // 例如359+1=360
            setValue(value()+singleStep()); // 360还是可以设置的
        else setValue(minimum()); // 再大超出范围直接变成最小值
    }
    else { // 鼠标后转,数值减少
        if (value() - singleStep() >= minimum()) // 1-1=0
            setValue(value()-singleStep()); // 0还可以设置
        else setValue(maximum());//倒退为最大值
    }
    e->accept();
}
