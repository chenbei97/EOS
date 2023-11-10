/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-31 09:14:40
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-31 09:15:26
 * @FilePath: \EOS\interface\include\preview\savebox.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOSI_SAVEBOX_H
#define EOSI_SAVEBOX_H

#include "window.h"

class INTERFACE_IMEXPORT SaveBox: public GroupBox
{
    Q_OBJECT
public:
    explicit SaveBox(QWidget*parent= nullptr);
private:
    PushButton * loadbtn;
    PushButton * stopbtn;
    PushButton * exportallbtn;
    CheckBox * exportToFile;
    LineEdit * filenameedit;
private:
    void showExport(bool enable);
    void exportFile();
signals:
    void exportFilePath(const QString&path);
    void loadExper();
};
#endif //EOSI_SAVEBOX_H
