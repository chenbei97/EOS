/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-16 16:33:34
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-16 17:29:12
 * @FilePath: \EOS\component\src\widgets\picture.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "picture.h"

Picture::Picture(QWidget *parent) : QWidget(parent)
{
    pix = new PhotoCanvas;
    info = new Label;
    menu = new QMenu(this);
    experact = new QAction(tr(ExportField),this);
    originact = new QAction(tr(OriginalField),this);

    pix->setStrategy(PhotoCanvas::SinglePixmap);
    info->setAlignment(Qt::AlignCenter);
    info->setScaledContents(true);
    info->setMaximumHeight(PictureDescriptionMaxHeight);
    showDescribtion(false);
    menu->addAction(experact);
    menu->addAction(originact);
    setContextMenuPolicy(Qt::CustomContextMenu);

    QVBoxLayout * lay = new QVBoxLayout(this);
    lay->addWidget(pix);
    lay->addWidget(info);
    lay->setSpacing(0);

    connect(experact,&QAction::triggered,this,&Picture::exportImage);
    connect(originact,&QAction::triggered,this,&Picture::showOriginalPixmap);
    connect(pix,&PhotoCanvas::doubleMouseClicked,this,&Picture::showOriginalPixmap);
    connect(this,&Picture::customContextMenuRequested,[=]{menu->exec(QCursor::pos());});

    resize(DefaultWindowSize);
    /*
     *  QImage img(CURRENT_PATH+"/images/cell.png");
     *  Picture w;
     *  w.setImage(img,CURRENT_PATH+"/images/cell.png");
     *  w.setDescription("a cell png");
     *  w.setDescriptionHeight(50);
     *  setWindowAlignCenter(&w);
     *  w.show();
     * */
}

void Picture::exportImage()
{
    auto img = pix->image();
    auto dir = QFileDialog::getExistingDirectory(this,tr("Select Save Dir"));
    if (dir.isEmpty()) return ;

    img.save(dir +"/"+ QDateTime::currentDateTime().
        toString(DefaultImageDateTimeFormat)+JPGSuffix);
}

void Picture::showOriginalPixmap()
{
    if (pixpath.isEmpty()) return;

    PhotoCanvas * dlg = new PhotoCanvas;
    dlg->setStrategy(PhotoCanvas::SinglePixmap);
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->setImage(QImage(pixpath));
    dlg->show();
}

void Picture::setPixmap(const QPixmap& pixmap,const QString& path)
{
    pixmap.isNull()? menu->setEnabled(false):menu->setEnabled(true);
    pixpath = path;
    pix->setImage(pixmap.toImage());
}

void Picture::setImage(const QImage &img,const QString& path)
{
    img.isNull()? menu->setEnabled(false):menu->setEnabled(true);
    pixpath = path;
    pix->setImage(img);
}

QPixmap Picture::pixmap() const
{
    return QPixmap::fromImage(pix->image());
}

QImage Picture::image() const
{
    return pix->image();
}

void Picture::setDescription(const QString &description)
{
    if (info->isHidden()) info->show();
    info->setText(description);
    info->setToolTip(description);
}

QString Picture::description() const
{
    return info->text();
}

void Picture::showDescribtion(bool show)
{
    info->setVisible(show);
}

void Picture::setDescriptionHeight(int height)
{
    info->setMaximumHeight(height);
}