/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-12-04 13:47:39
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-20 10:11:41
 * @FilePath: \EOS\interface\include\preview\viewmode.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_MAIN_CORE_PY_VIEWMODE_H
#define EOS_MAIN_CORE_PY_VIEWMODE_H

#include "window.h"

class INTERFACE_IMEXPORT ViewModeBox: public GroupBox
{
    Q_OBJECT
public:
    explicit ViewModeBox(QWidget*parent= nullptr);
    void setViewEnabled(int option);
    ViewMode viewMode() const;
    ViewModeInfo viewModeInfo() const;
private:
    RadioGroup * groupMode;
    void initObjects();
    void initAttributes();
    void initLayout();
signals:
    void modeSelected(int mode);
};

#endif //EOS_MAIN_CORE_PY_VIEWMODE_H
