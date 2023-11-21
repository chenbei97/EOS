
#include "videowidget.h"

VideoWidget::VideoWidget(QWidget *parent): QWidget(parent)
{
    canvs = new QVideoWidget;
    player = new QMediaPlayer;
    playlist = new QMediaPlaylist(player);

    player->setPlaylist(playlist);
    player->setVideoOutput(canvs);
    //canvs->show();

    auto lay = new QVBoxLayout;
    lay->addWidget(canvs);
    lay->setMargin(0);
    //lay->setSpacing(0);
    setLayout(lay);
    resize(DefaultWindowSize);
}

void VideoWidget::setMedia(const QString &path)
{
//    playlist->clear();
//    //playlist->load(QUrl(path));
//    playlist->addMedia(QUrl::fromLocalFile(path));
//    playlist->setCurrentIndex(0);
//    LOG<<player->playlist()->media(0).isNull();
    player->setMedia(QUrl::fromLocalFile(path));
}

void VideoWidget::play()
{
    if (playlist->isEmpty()) return;

    player->play();

}
