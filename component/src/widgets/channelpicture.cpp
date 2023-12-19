/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-20 17:09:07
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-20 17:31:43
 * @FilePath: \EOS\component\src\widgets\channelpicture.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "channelpicture.h"

ChannelPicture::ChannelPicture(QWidget *parent) : QWidget(parent)
{
#ifdef channel_use_picture
    brcanvas = new Picture;
    phcanvas = new Picture;
    gfpcanvas = new Picture;
    rfpcanvas = new Picture;
    dapicanvas = new Picture;
#else
    brcanvas = new Label;
    phcanvas = new Label;
    gfpcanvas = new Label;
    rfpcanvas = new Label;
    dapicanvas = new Label;
    initAttributes();
#endif

    auto lay = new QHBoxLayout;
    lay->addWidget(brcanvas);
    lay->addWidget(phcanvas);
    lay->addWidget(gfpcanvas);
    lay->addWidget(rfpcanvas);
    lay->addWidget(dapicanvas);

    //lay->setSpacing(0);
    lay->setMargin(0);
    setLayout(lay);

    setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Minimum);
    resize(ChannelPictureLabelDefaultWidth,ChannelPictureLabelMinHeight);
}

void ChannelPicture::setPixmap(int index,const QPixmap& pix)
{
    switch (index) {
        case 0: brcanvas->setPixmap(pix.scaled(brcanvas->size()));
            break;
        case 1: phcanvas->setPixmap(pix.scaled(phcanvas->size()));
            break;
        case 2: gfpcanvas->setPixmap(pix.scaled(gfpcanvas->size()));
            break;
        case 3: rfpcanvas->setPixmap(pix.scaled(rfpcanvas->size()));
            break;
        case 4: dapicanvas->setPixmap(pix.scaled(dapicanvas->size()));
            break;
    }
}

void ChannelPicture::setImage(int index,const QImage& img)
{
    switch (index) {
        case 0: brcanvas->setPixmap(QPixmap::fromImage(img).scaled(brcanvas->size()));
            break;
        case 1: phcanvas->setPixmap(QPixmap::fromImage(img).scaled(phcanvas->size()));
            break;
        case 2: gfpcanvas->setPixmap(QPixmap::fromImage(img).scaled(gfpcanvas->size()));
            break;
        case 3: rfpcanvas->setPixmap(QPixmap::fromImage(img).scaled(rfpcanvas->size()));
            break;
        case 4: dapicanvas->setPixmap(QPixmap::fromImage(img).scaled(dapicanvas->size()));
            break;
    }
}

QSize ChannelPicture::sizeHint() const
{
    return QSize(ChannelPictureLabelDefaultWidth,ChannelPictureLabelMinHeight);
}


QSize ChannelPicture::minimumSizeHint() const
{
    return QSize(ChannelPictureLabelDefaultWidth,ChannelPictureLabelMinHeight);
}


#ifndef channel_use_picture
void ChannelPicture::initAttributes()
{
    brcanvas->setAlignment(Qt::AlignCenter);
    phcanvas->setAlignment(Qt::AlignCenter);
    gfpcanvas->setAlignment(Qt::AlignCenter);
    rfpcanvas->setAlignment(Qt::AlignCenter);
    dapicanvas->setAlignment(Qt::AlignCenter);

    brcanvas->setFrameShape(Label::WinPanel);
    phcanvas->setFrameShape(Label::WinPanel);
    gfpcanvas->setFrameShape(Label::WinPanel);
    rfpcanvas->setFrameShape(Label::WinPanel);
    dapicanvas->setFrameShape(Label::WinPanel);

    brcanvas->setFrameShadow(Label::Plain);
    phcanvas->setFrameShadow(Label::Plain);
    gfpcanvas->setFrameShadow(Label::Plain);
    rfpcanvas->setFrameShadow(Label::Plain);
    dapicanvas->setFrameShadow(Label::Plain);

    brcanvas->setScaledContents(true);
    phcanvas->setScaledContents(true);
    gfpcanvas->setScaledContents(true);
    rfpcanvas->setScaledContents(true);
    dapicanvas->setScaledContents(true);

    brcanvas->setMaximumWidth(ChannelPictureLabelMaxWidth);
    phcanvas->setMaximumWidth(ChannelPictureLabelMaxWidth);
    gfpcanvas->setMaximumWidth(ChannelPictureLabelMaxWidth);
    rfpcanvas->setMaximumWidth(ChannelPictureLabelMaxWidth);
    dapicanvas->setMaximumWidth(ChannelPictureLabelMaxWidth);
}
#endif