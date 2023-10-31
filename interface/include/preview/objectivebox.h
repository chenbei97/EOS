/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-23 13:54:55
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-23 14:13:51
 * @FilePath: \EOS\interface\include\window\objectivebox.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_OBJECTIVEBOX_H
#define EOSI_OBJECTIVEBOX_H

#include "window.h"

class INTERFACE_IMEXPORT ObjectiveBox: public GroupBox
{
    Q_OBJECT
public:
    explicit ObjectiveBox(QWidget*parent= nullptr);
    ObjectiveInfo objectiveInfo() const;
private:
    QRadioButton *  rbtn4;
    QRadioButton *  rbtn10;
    QRadioButton *  rbtn20;
    QRadioButton *  rbtn40;
    void onClicked();
signals:
    void objectiveChanged(int option);
};

#endif //EOSI_OBJECTIVEBOX_H
