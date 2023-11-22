/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-21 14:31:56
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-22 11:38:08
 * @FilePath: \EOS\component\src\widgets\videowidget.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "videowidget.h"

VideoWidget::VideoWidget(QWidget *parent): QWidget(parent)
{
    canvs = new QVideoWidget;
    player = new QMediaPlayer;
    playlist = new QMediaPlaylist(player);

    player->setPlaylist(playlist);
    player->setVideoOutput(canvs);

    auto lay = new QVBoxLayout;
    lay->addWidget(canvs);
    lay->setMargin(0);
    //lay->setSpacing(0);
    setLayout(lay);
    resize(DefaultWindowSize);

    /*
     * VideoWidget w;
     * w.setMedia("test.mp4");
     * w.play();
     * */

    connect(player, QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error),this,&VideoWidget::errorOccurred);
}

void VideoWidget::setMedia(const QString &path)
{
    //playlist->clear();
    //playlist->load(QUrl::fromLocalFile(path));
    //playlist->addMedia(QUrl::fromLocalFile(path));
    //playlist->setCurrentIndex(0); // 比使用playlist直接

    if (path != mediapath) {
        mediapath = path; // 路径确实切换了再重设media
        player->setMedia(QUrl::fromLocalFile(path));
    } // 之所以这样做是为了播放时不至于每次都是从头开始,而是继续播放
}

void VideoWidget::play()
{
    //if (playlist->isEmpty()) return;
    player->play();
}

void VideoWidget::stop()
{
    player->stop();
}

void VideoWidget::pause()
{
    player->pause();
}

QString VideoWidget::error() const
{
    //return player->errorString(); // 这个好像不可以正确打印所以手动翻译
    return translateError(player->error());
}

void VideoWidget::errorOccurred(QMediaPlayer::Error err)
{
    //LOG<<player->errorString()<<player->error(); //  "" QMediaPlayer::ResourceError errorString没有打印出错误
    if (err == QMediaPlayer::NoError) return;
    QMessageBox::critical(this,CriticalChinese, translateError(err),QMessageBox::Ok);
}

QString VideoWidget::translateError(QMediaPlayer::Error err) const
{
    QString errorString = "UnknowError";
    switch (err) {
        case QMediaPlayer::NoError: errorString = "NoError";
            break;
        case QMediaPlayer::ResourceError: errorString = "ResourceError";
            break;
        case QMediaPlayer::FormatError: errorString = "FormatError";
            break;
        case QMediaPlayer::NetworkError: errorString = "NetworkError";
            break;
        case QMediaPlayer::AccessDeniedError: errorString = "AccessDeniedError";
            break;
        case QMediaPlayer::ServiceMissingError: errorString = "ServiceMissingError";
            break;
        default:
            break;
    }
    return errorString;
}