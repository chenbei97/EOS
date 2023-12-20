
#ifndef EOS_BUTTONGROUP_H
#define EOS_BUTTONGROUP_H

#include "datastructure.h"
#include "radiobutton.h"
#include "pushbutton.h"

class COMPONENT_IMEXPORT ButtonGroup: public QObject
{
    Q_OBJECT
public:
    enum ButtonType {RadioBtn,PushBtn};
    explicit ButtonGroup(unsigned num,ButtonType type = RadioBtn,QObject*parent= nullptr);
    explicit ButtonGroup(QObject*parent= nullptr);
    void setText(int id,const QString& text = QString());
    void setText(const QStringList& texts);
    void setChecked(int id,bool checked);
    unsigned checkedID() const;
    QString checkedText() const;
    QAbstractButton* checkedButton() const;
    QAbstractButton* button(int id) const;
    ButtonType type() const;
    void emitSignals(int id);
private:
    ButtonType buttonType = RadioBtn;
    unsigned buttonNum = 2;
    QButtonGroup * group;
signals:
    void buttonClicked(int id);
};

#endif //EOS_BUTTONGROUP_H
