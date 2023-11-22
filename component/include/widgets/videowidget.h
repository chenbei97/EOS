/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-21 14:30:15
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-22 11:37:44
 * @FilePath: \EOS\component\include\widgets\videowidget.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_MAIN_CORE_PY_VIDEOWIDGET_H
#define EOS_MAIN_CORE_PY_VIDEOWIDGET_H

#include "widgets.h"

class COMPONENT_IMEXPORT VideoWidget: public QWidget
{
    Q_OBJECT
public:
    explicit VideoWidget(QWidget*parent= nullptr);
    void setMedia(const QString& path);
    void play();
    void stop();
    void pause();
    QString error() const;
private:
    QVideoWidget * canvs;
    QMediaPlayer * player;
    QMediaPlaylist * playlist;
    QString mediapath;
private:
    void errorOccurred(QMediaPlayer::Error err);
    QString translateError(QMediaPlayer::Error err) const;
};

#endif //EOS_MAIN_CORE_PY_VIDEOWIDGET_H
