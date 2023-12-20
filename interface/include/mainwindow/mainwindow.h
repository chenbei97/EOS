/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-18 11:43:37
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-14 14:26:10
 * @FilePath: \EOS\interface\include\mainwindow\mainwindow.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_MAINWINDOW_H
#define EOSI_MAINWINDOW_H

#include "window.h"
#include "navigbar.h"
#include "appselect.h"
#include "preview.h"
#include "setting.h"
#include "toupcam.h"

class INTERFACE_IMEXPORT MainWindow: public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget*parent= nullptr);
    void closeEvent(QCloseEvent*event);
private:
    NavigBar * navigbar;
    AppSelect * appselect;
    Preview * preview;
    Setting * setting;
    QStackedWidget * stack;
    QWidget* mDataWidget;
private:
    void navigbarSelect(int id);
};
#endif //EOSI_MAINWINDOW_H
