/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-12-25 16:40:29
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-25 17:08:52
 * @FilePath: \EOS\component\include\widgets\viewapply.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_VIEWAPPLY_H
#define EOS_VIEWAPPLY_H

#include "widgets.h"
#include "pushbutton.h"

class COMPONENT_IMEXPORT ViewApply: public QDialog
{
    Q_OBJECT
public:
    explicit ViewApply(QWidget*parent= nullptr);
    int mode() const;
private:
    PushButton * current;
    PushButton * group;
    PushButton * all;
    void apply();
    int mmode = 0;
};
#endif //EOS_VIEWAPPLY_H
