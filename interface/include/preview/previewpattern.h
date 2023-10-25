/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-25 08:09:28
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-25 08:25:00
 * @FilePath: \EOS\interface\include\preview\previewpattern.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_PREVIEWPATTERN_H
#define EOSI_PREVIEWPATTERN_H

#include "window.h"
#include "pattern.h"

class INTERFACE_IMEXPORT PreviewPattern: public Pattern
{
    Q_OBJECT
public:
    explicit PreviewPattern(int rows, int cols,QWidget*parent= nullptr);
    void paintEvent(QPaintEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
private:

};

#endif //EOSI_PREVIEWPATTERN_H
