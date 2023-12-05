/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-12-05 16:49:05
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-05 16:51:02
 * @FilePath: \EOS\interface\include\preview\slidepattern.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_SLIDEPATTERN_H
#define EOS_SLIDEPATTERN_H

#include "window.h"

class INTERFACE_IMEXPORT SlidePattern : public QWidget
{
    Q_OBJECT
public:
    explicit SlidePattern(QWidget*parent= nullptr);
    void paintEvent(QPaintEvent *event) override;
private:


};
#endif //EOS_SLIDEPATTERN_H
