/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-18 11:43:37
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-18 11:46:46
 * @FilePath: \EOS\interface\include\window\mainwindow.h
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
#include "startpython.h"
#include "startpythonx.h"
#include "pythonprocess.h"

#define use_testSocket 0

class INTERFACE_IMEXPORT MainWindow: public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget*parent= nullptr);
private:
    NavigBar * navigbar;
    AppSelect * appselect;
    Preview * preview;
    Setting * setting;
    QStackedWidget * stack;
    PythonProcess * process;
private:
    void navigbarSelect(int id);
};
#endif //EOSI_MAINWINDOW_H
