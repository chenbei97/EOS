/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-30 14:11:53
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-30 14:27:49
 * @FilePath: \EOS\interface\include\preview\lensbox.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_LENSBOX_H
#define EOSI_LENSBOX_H

#include "window.h"

class INTERFACE_IMEXPORT LensBox: public GroupBox
{
Q_OBJECT
public:
    explicit LensBox(QWidget*parent= nullptr);
    LensInfo lensInfo() const;
private:
    TriangleMove * pattern;
signals:
    void leftMove();
    void rightMove();
    void topMove();
    void bottomMove();
    void directionMove(int option);
};


#endif //EOSI_LENSBOX_H
