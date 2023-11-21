
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
private:
    QVideoWidget * canvs;
    QMediaPlayer * player;
    QMediaPlaylist * playlist;
};

#endif //EOS_MAIN_CORE_PY_VIDEOWIDGET_H
