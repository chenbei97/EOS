/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-17 16:57:56
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-17 16:58:29
 * @FilePath: \EOS\component\include\widgets\socketpanel.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_SOCKETPANEL_H
#define EOSI_SOCKETPANEL_H

#include "socketpython.h"

#define sendText (tr("<strong><font color = #75FA8D>[%1] 发送:</font></strong> %2") \
            .arg(QDateTime::currentDateTime().toString("yyyy/MMdd hh:mm:ss")))
#define recvText (tr("<strong><font color = #75FA8D>[%1] 回复:</font></strong> %2") \
            .arg(QDateTime::currentDateTime().toString("yyyy/MMdd hh:mm:ss")))
#define errorText (tr("<strong><font color = #75FA8D>[%1] 错误:</font></strong> %2") \
            .arg(QDateTime::currentDateTime().toString("yyyy/MMdd hh:mm:ss")))

class SocketPanel: public QWidget
{
    Q_OBJECT
public:
    explicit SocketPanel(QWidget *parent = nullptr);
    QVariantMap getTestData1();
    QVariantMap getTestData2();
    QByteArray getTestData3();
    void testData1_2();
private:
    PushButton * btn;
    TextEdit * edit;
    void onSend();
    void askActivateCodeSync();
    void askActivateCodeASync(const QString& f,const QVariant& d);
};

static void test_socketPanel()
{
    auto w = new SocketPanel;
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->resize(1000,600);
    setWindowAlignCenter(w);
    w->show();
}

#endif //EOSI_SOCKETPANEL_H
