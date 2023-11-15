/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-15 10:44:05
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-15 10:51:10
 * @FilePath: \EOS\interface\include\preview\skipbox.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_MAIN_CORE_PY_SKIPBOX_H
#define EOS_MAIN_CORE_PY_SKIPBOX_H

#include "window.h"

class INTERFACE_IMEXPORT SkipBox: public GroupBox
{
    Q_OBJECT
public:
    explicit SkipBox(QWidget*parent= nullptr);
    int skipHoles() const;
    int skipViews() const;
private:
    SpinBox * skipholes;
    SpinBox * skipviews;
signals:
    
};

#endif //EOS_MAIN_CORE_PY_SKIPBOX_H
