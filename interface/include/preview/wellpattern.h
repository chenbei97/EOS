/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-27 08:51:09
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-27 08:53:54
 * @FilePath: \EOS\interface\include\preview\wellpattern.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_WELLPATTERN_H
#define EOSI_WELLPATTERN_H

#include "window.h"
// 结合previewpattern的预览功能和protocolpattern分组功能
class INTERFACE_IMEXPORT WellPattern: public Pattern
{
    Q_OBJECT
public:
    explicit WellPattern(int rows,int cols,QWidget*parent= nullptr);
};
#endif //EOSI_WELLPATTERN_H
