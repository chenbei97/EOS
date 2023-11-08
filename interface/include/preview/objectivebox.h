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
    void importExperConfig(const QString&objective);
    ObjectiveInfo objectiveInfo() const;
public slots:
    void onObjectiveSettingChanged(const LocationObjectiveInfo& m);
private:
    QRadioButton *  rbtn_loc1;
    QRadioButton *  rbtn_loc2;
    QRadioButton *  rbtn_loc3;
    QRadioButton *  rbtn_loc4;
    QMap<QString,QRadioButton*> location_button;
    void onClicked();
signals:
    void objectiveChanged(const QString& obj);
};

#endif //EOSI_OBJECTIVEBOX_H
