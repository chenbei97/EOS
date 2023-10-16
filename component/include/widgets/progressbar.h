/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 14:44:35
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-16 15:09:11
 * @FilePath: \EOS\component\include\widgets\progressbar.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_PROGRESSBAR_H
#define EOSI_PROGRESSBAR_H

#include "widgets.h"

class COMPONENT_IMEXPORT ProgressBar : public QProgressBar
{
    Q_OBJECT
public:
    explicit ProgressBar(QWidget * parent = nullptr);
};

#endif //EOSI_PROGRESSBAR_H
