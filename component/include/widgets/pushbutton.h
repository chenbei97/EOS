/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 14:41:53
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-16 15:09:14
 * @FilePath: \EOS\component\include\widgets\pushbutton.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_PUSHBUTTON_H
#define EOSI_PUSHBUTTON_H

#include "widgets.h"

class COMPONENT_IMEXPORT PushButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(int id READ getID WRITE setID MEMBER id NOTIFY idChanged)
public:
    explicit PushButton(QWidget*parent=nullptr);
    explicit PushButton(const QString&text,QWidget*parent=nullptr);
    void setBackGroundColor(QCColor color);
    void resetBackGroundColor();
    bool isChecked() const;
    void setID(int id);
    int getID() const;
protected:
    int id = -1;
    bool checked = false;
signals:
    void idChanged(int id);
};


#endif //EOSI_PUSHBUTTON_H
