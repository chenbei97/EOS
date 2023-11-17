/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-31 14:11:59
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-31 14:13:53
 * @FilePath: \EOS\interface\include\setting\setting.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_SETTING_H
#define EOSI_SETTING_H

#include "window.h"
#include "objectivesetting.h"
#include "wellsetting.h"
#include "settingnotifier.h"

class INTERFACE_IMEXPORT Setting : public QWidget
{
    Q_OBJECT
public:
    explicit Setting(QWidget*parent= nullptr);
    void emitSignals();
private:
    void initObjects();
    void initAttributes();
    void initLayout();
    void initConnections();
private:
    QRadioButton * loginbtn;
    GroupBox * b1;
    QRadioButton * databasebtn;
    GroupBox * b2;
    QRadioButton * objectivebtn;
    QRadioButton * wellbtn;
private:
    ObjectiveSetting * objectivesetting;
    WellSetting * wellsetting;
    QButtonGroup * buttongroup;
    SettingNotifier * notifier;
private:
    void toggleSetting(int option);
signals:
    void objectiveSettingChanged(const LocationObjectiveInfo& m);
};
#endif //EOSI_SETTING_H
