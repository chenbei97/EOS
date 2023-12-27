/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-23 13:54:55
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-20 10:09:51
 * @FilePath: \EOS\interface\include\preview\objectivebox.h
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
    void importExperConfig(const QString&objectiveDescrip);
    ObjectiveInfo objectiveInfo() const;
    int currentObjective() const;
    QString currentObjectiveDescription() const;
public slots:
    void onObjectiveSettingChanged(const LocationObjectiveInfo& m);
private:
    RadioButton *  rbtn_loc1;
    RadioButton *  rbtn_loc2;
    RadioButton *  rbtn_loc3;
    RadioButton *  rbtn_loc4;
    QMap<QString,RadioButton*> location_button;
    void onClicked();
    QString convertTo(const QString& text) const;
    QString convertFrom(const QString& text) const;
signals:
    void objectiveChanged(const QString& obj);
    void objectiveToggled(int objective,int objective_loc,int isPh);
};

#endif //EOSI_OBJECTIVEBOX_H
