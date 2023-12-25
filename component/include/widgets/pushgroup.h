/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-12-25 16:24:44
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-25 17:27:59
 * @FilePath: \EOS\component\include\widgets\pushgroup.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_PUSHGROUP_H
#define EOS_PUSHGROUP_H

#include "widgets.h"
#include "pushbutton.h"

class COMPONENT_IMEXPORT PushGroup: public QWidget
{
Q_OBJECT
public:
    explicit PushGroup(QWidget*parent= nullptr);
    explicit PushGroup(unsigned num,Qt::Orientation = Qt::Horizontal,QWidget*parent= nullptr);
    void setText(int id,const QString& text = QString());
    void setText(const QStringList& texts);
    void setChecked(int id,bool checked);
    void setEnabled(int id);
    void setEnabled(int id,bool enabled);
    unsigned checkedID() const;
    QString checkedText() const;
    PushButton* checkedButton() const;
    PushButton* button(int id) const;
    void emitSignals(int id);
private:
    QButtonGroup * group;
    unsigned btnNum = 2;
signals:
    void pushClicked(int id);
};

#endif //EOS_PUSHGROUP_H
