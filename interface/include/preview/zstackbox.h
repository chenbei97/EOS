/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-30 13:43:02
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-30 13:49:05
 * @FilePath: \EOS\interface\include\preview\zstackbox.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_ZSTACKBOX_H
#define EOSI_ZSTACKBOX_H

#include "window.h"

class INTERFACE_IMEXPORT ZStackBox: public GroupBox
{
    Q_OBJECT
public:
    explicit ZStackBox(QWidget*parent= nullptr);
    void importExperConfig(bool zstack,bool stitch);
    ZStackInfo zstackInfo() const;
private:
    CheckBox * zstack;
    CheckBox * stitchimage;
    ComboBox * videoformat;
    ComboBox * imageformat;
signals:
    void zstackChanged(bool isChecked);
    void stitchChanged(bool isChecked);
};

#endif //EOSI_ZSTACKBOX_H
