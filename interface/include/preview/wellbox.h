/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-19 11:16:07
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-19 13:36:07
 * @FilePath: \EOS\interface\include\window\wellbox.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_WELLBOX_H
#define EOSI_WELLBOX_H

#include "window.h"

class INTERFACE_IMEXPORT WellBox: public GroupBox
{
    Q_OBJECT
public:
    explicit WellBox(QWidget*parent= nullptr);
    void importWellInfo(int manufacturer,int brand);
    WellInfo wellInfo() const;
    int wellBrand() const;
    int wellManufacturer() const;
    int wellSize() const;
    QString wellBrandName() const;
    QString wellManufacturerName() const;
private:
    ComboBox * wellbrandCombo;
    ComboBox * manufacturerCombo;
    void onManufacturerChanged(int option);
    void onWellbrandChanged(int option);
signals:
    void welltypeChanged(int option);
    void wellbrandChanged(int option);
    void manufacturerChanged(int option);
};

#endif //EOSI_WELLBOX_H
