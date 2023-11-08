/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-07 16:21:45
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-08 11:10:43
 * @FilePath: \EOS\interface\include\preview\summarypanel.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_MAIN_CORE_PY_SUMMARYPANEL_H
#define EOS_MAIN_CORE_PY_SUMMARYPANEL_H

#include "window.h"

class INTERFACE_IMEXPORT SummaryPanel: public QWidget
{
    Q_OBJECT
public:
    explicit SummaryPanel(const QVariantMap &m = QVariantMap(),QWidget*parent= nullptr);
    void setData(const QVariantMap &m);
private:
    TextEdit * edit;
};
#endif //EOS_MAIN_CORE_PY_SUMMARYPANEL_H
