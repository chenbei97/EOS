/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-16 16:33:23
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-16 17:29:06
 * @FilePath: \EOS\component\include\widgets\picture.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_MAIN_CORE_PY_PICTURE_H
#define EOS_MAIN_CORE_PY_PICTURE_H

#include "widgets.h"
#include "label.h"
#include "photocanvas.h"

class COMPONENT_IMEXPORT Picture : public QWidget
{
    Q_OBJECT
public:
    explicit Picture(QWidget *parent = nullptr);

    void setPixmap(const QPixmap& pix,const QString& path = QString());
    void setImage(const QImage& img,const QString& path = QString());
    QPixmap pixmap() const;
    QImage image() const;

    void setDescription(const QString& description);
    QString description() const;
    void showDescribtion(bool show);
    void setDescriptionHeight(int height);
private:
    PhotoCanvas * pix;
    Label * info;
    QMenu * menu;
    QAction * experact;
    QAction * originact;
    QString pixpath;
private:
    void exportImage();
    void showOriginalPixmap();
signals:
};

#endif //EOS_MAIN_CORE_PY_PICTURE_H
