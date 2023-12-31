/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 14:22:26
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-16 15:08:56
 * @FilePath: \EOS\component\include\widgets\groupbox.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_GROUPBOX_H
#define EOSI_GROUPBOX_H

#include "widgets.h"

class COMPONENT_IMEXPORT GroupBox : public QGroupBox
{
    Q_OBJECT
    Q_PROPERTY(int id READ getID WRITE setID MEMBER id NOTIFY idChanged)
public:
    explicit GroupBox(QWidget*parent=nullptr);
    explicit GroupBox(const QString&title,QWidget*parent=nullptr);
    explicit GroupBox(QLayout*lay,QWidget*parent=nullptr);
    explicit GroupBox(const QString&title,QLayout*lay,QWidget*parent=nullptr);
    void setID(int id);
    int getID() const;
protected:
    int id = -1;
signals:
    void idChanged(int id);
};


#endif //EOSI_GROUPBOX_H
