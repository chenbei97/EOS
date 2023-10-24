/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-18 15:40:35
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-18 15:42:12
 * @FilePath: \EOS\interface\include\window\preview.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_PREVIEW_H
#define EOSI_PREVIEW_H

#include "window.h"
#include "previewtool.h"
#include "cameramode.h"

class INTERFACE_IMEXPORT Preview : public QWidget
{
    Q_OBJECT
public:
    explicit Preview(QWidget*parent= nullptr);
private:
    PreviewTool * toolbar;
    Pattern * pattern;

    void onManufacturerChanged(int option);
    void onInfoChanged();
};
#endif //EOSI_PREVIEW_H
