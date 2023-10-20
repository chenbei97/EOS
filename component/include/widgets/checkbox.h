#ifndef EOSI_CHECKBOX_H
#define EOSI_CHECKBOX_H

#include "widgets.h"

class COMPONENT_IMEXPORT CheckBox : public  QWidget
{
    Q_OBJECT
public:
    explicit CheckBox(QWidget*parent= nullptr);
    explicit CheckBox(const QString& text,QWidget*parent= nullptr);
    void setChecked(bool checked);
    void setText(const QString& text);
    bool isChecked() const;
//    QSize sizeHint() const override;
private:
    QCheckBox * checkbox;
signals:
    void checkedChanged(bool checked);

};
#endif //EOSI_CHECKBOX_H
