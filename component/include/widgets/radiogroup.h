/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-12-20 10:20:28
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-20 10:26:08
 * @FilePath: \EOS\component\include\widgets\radiogroup.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_RADIOGROUP_H
#define EOS_RADIOGROUP_H

#include "widgets.h"
#include "radiobutton.h"

class COMPONENT_IMEXPORT RadioGroup: public QWidget
{
    Q_OBJECT
public:
    explicit RadioGroup(QWidget*parent= nullptr);
    explicit RadioGroup(unsigned num,QWidget*parent= nullptr);
    void setText(int id,const QString& text = QString());
    void setText(const QStringList& texts);
    void setChecked(int id,bool checked);
    void setEnabled(int id);
    void setEnabled(int id,bool enabled);
    unsigned checkedID() const;
    QString checkedText() const;
    RadioButton* checkedButton() const;
    RadioButton* button(int id) const;
    void emitSignals(int id);
private:
    QButtonGroup * group;
    unsigned radioNum = 2;
signals:
    void radioClicked(int id);
};

#endif //EOS_RADIOGROUP_H
