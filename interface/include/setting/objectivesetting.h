/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-31 14:38:19
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-20 10:11:28
 * @FilePath: \EOS\interface\include\setting\objectivesetting.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_OBJECTIVESETTING_H
#define EOSI_OBJECTIVESETTING_H

#include "window.h"

class INTERFACE_IMEXPORT ObjectiveSetting: public GroupBox
{
    Q_OBJECT
public:
    explicit ObjectiveSetting(QWidget *parent = nullptr);
    void emitSignals();
private:
    void initObjects();
    void initAttributes();
    void initLayout();
    void moveMachine();
    void saveSetting();
    void updateTipInfo();
    void updateCheckedState();
private:
    Label * tipinfo;
    QString tip;
    LocationObjectiveInfo info;
    QMap<QString,RadioButton*> objective_button;
    RadioGroup * locationgroup;
    PushButton * locationbtn;
    ButtonGroup * objectivegroup;
    PushButton * savebtn;
signals:
    void objectiveSettingChanged(const LocationObjectiveInfo& m);
};
#endif //EOSI_OBJECTIVESETTING_H
