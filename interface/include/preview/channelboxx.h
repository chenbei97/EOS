
#ifndef EOS_CHANNELBOXX_H
#define EOS_CHANNELBOXX_H

#include "window.h"

//#define use_channelnotifier 0
#ifdef use_channelnotifier
#include "channelnotifier.h"
#endif

class INTERFACE_IMEXPORT ChannelBoxx: public GroupBox
{
    Q_OBJECT
public:
    explicit ChannelBoxx(QWidget*parent= nullptr);
    ChannelInfo channelInfo() const;
    void disableChannel(const QString& obj);
    void takePhoto(const QImage& img,const QString& channel);
private:
    PushButton * brbtn;
    PushButton * phbtn;
    PushButton * gfpbtn;
    PushButton * rfpbtn;
    PushButton * dapibtn;
    ChannelPicture * canvas;
#ifdef use_channelnotifier
    ChannelNotifier * notifier;
    void toggleChannel(int option);
#else
    void clickBr();
    void clickPh();
    void clickGfp();
    void clickRfp();
    void clickDapi();
#endif
signals:
    void channelChanged(int option);
};

#endif //EOS_CHANNELBOXX_H
