
#ifndef EOS_MAIN_CORE_PY_VIEWSELECT_H
#define EOS_MAIN_CORE_PY_VIEWSELECT_H

#include "window.h"

class INTERFACE_IMEXPORT ViewSelect: public GroupBox
{
    Q_OBJECT
public:
    explicit ViewSelect(QWidget*parent= nullptr);
private:
    QRadioButton * pointMode;
    QRadioButton * rectMode;
    QRadioButton * wholeMode;
signals:
    void modeSelected(int mode);
};

#endif //EOS_MAIN_CORE_PY_VIEWSELECT_H
