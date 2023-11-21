/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-20 17:08:54
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-20 17:31:15
 * @FilePath: \EOS\component\include\widgets\channelpicture.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_CHANNELPICTURE_H
#define EOS_CHANNELPICTURE_H

#include "widgets.h"

#define channel_use_picture 0
#ifdef channel_use_picture
#include "picture.h"
#else
#include "label.h" // label.h没有鼠标事件和2个动作按钮
#endif

class COMPONENT_IMEXPORT ChannelPicture: public QWidget
{
    Q_OBJECT
public:
    explicit ChannelPicture(QWidget*parent= nullptr);
    void setPixmap(int index,const QPixmap& pix);
    void setImage(int index,const QImage& img);
    void initAttributes();
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
private:
#ifdef channel_use_picture
    Picture * brcanvas;
    Picture * phcanvas;
    Picture * gfpcanvas;
    Picture * rfpcanvas;
    Picture * dapicanvas;
#else
    Label * brcanvas;
    Label * phcanvas;
    Label * gfpcanvas;
    Label * rfpcanvas;
    Label * dapicanvas;
#endif
};
#endif //EOS_CHANNELPICTURE_H
