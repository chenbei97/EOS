/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-19 08:52:14
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-19 09:01:48
 * @FilePath: \EOS\interface\include\window\cameratool.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_CAMERATOOL_H
#define EOSI_CAMERATOOL_H

#include "window.h"

class INTERFACE_IMEXPORT CameraTool : public QWidget
{
    Q_OBJECT
public:
    explicit CameraTool(QWidget*parent= nullptr);

    void setTitle(const QString& title);

    void setBright(int bright);
    void setGain(int gain);
    void setExposure(int exposure);

    void setEnabled(int tool,bool enable);

    QString exposure() const;
    QString gain() const;
    QString bright() const;
//    QSize sizeHint() const override;
//    QSize minimumSizeHint() const;
private:
    Label * label;
    Slider * expslider;
    Slider * gainslider;
    Slider * brightslider;
signals:
    void exposureChanged(int ms); // 1ms-15s
    void gainChanged(int percent); // 100%-5000%
    void brightChanged(int bright); // 0-100
};

#endif //EOSI_CAMERATOOL_H
