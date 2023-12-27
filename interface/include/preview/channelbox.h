
#ifndef EOS_CHANNELBOX_H
#define EOS_CHANNELBOX_H

#include "window.h"

#define use_channelnotifier 1
#ifdef use_channelnotifier
#include "channelnotifier.h"
#endif

class INTERFACE_IMEXPORT ChannelBox: public GroupBox
{
    Q_OBJECT
public:
    explicit ChannelBox(QWidget*parent= nullptr);
    ChannelInfo channelInfo() const;
    QString currentChannel() const;
    void disableChannel(const QString& obj);
    void openDefaultChannel();
    void takePhoto(const QImage& img,const QString& channel);
private:
    PushButton * brchannel;
    PushButton * phchannel;
    PushButton * gfpchannel;
    PushButton * rfpchannel;
    PushButton * dapichannel;
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
    void channelClosed(int option);
};

#endif //EOS_CHANNELBOX_H
