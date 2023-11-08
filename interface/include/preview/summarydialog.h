/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-08 10:48:39
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-08 11:10:33
 * @FilePath: \EOS\interface\include\preview\summarydialog.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_MAIN_CORE_PY_SUMMARYDIALOG_H
#define EOS_MAIN_CORE_PY_SUMMARYDIALOG_H

#include "window.h"
#include "summarypanel.h"

class INTERFACE_IMEXPORT SummaryDialog: public QDialog
{
    Q_OBJECT
public:
    explicit SummaryDialog(const QVariantMap &m = QVariantMap(),QWidget*parent = nullptr);
    void setData(const QVariantMap& m);
private:
    PushButton * btn;
    SummaryPanel * panel;
    void load();
};
#endif //EOS_MAIN_CORE_PY_SUMMARYDIALOG_H
