/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-31 14:38:19
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-31 14:41:48
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
    void onMoveMachine(const QString& f,const QVariant& d);
private:
    void initObjects();
    void initAttributes();
    void initLayout();
    QButtonGroup * buttongroup1;
    QButtonGroup * buttongroup2;
    void moveMachine();
    void saveSetting();
    void updateTipInfo();
    void updateCheckedState();
    Label * tipinfo;
    QString tip;
    LocationObjectiveInfo info;
    QMap<QString,QRadioButton*> objective_button;
private:
    QRadioButton * location_one;
    QRadioButton * location_two;
    QRadioButton * location_three;
    QRadioButton * location_four;
    PushButton * locationbtn;
private:
    QRadioButton * br4x;
    QRadioButton * ph4x;
    QRadioButton * br10x;
    QRadioButton * ph10x;
    QRadioButton * br20x;
    QRadioButton * ph20x;
    QRadioButton * br40x;
    QRadioButton * ph40x;
    QRadioButton * none;
    PushButton * savebtn;
signals:
    void objectiveSettingChanged(const LocationObjectiveInfo& m);
};
#endif //EOSI_OBJECTIVESETTING_H
