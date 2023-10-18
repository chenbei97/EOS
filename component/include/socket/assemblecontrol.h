/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-17 17:26:29
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-17 17:27:50
 * @FilePath: \EOS\component\include\socket\assemblecontrol.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_ASSEMBLECONTROL_H
#define EOSI_ASSEMBLECONTROL_H

#include "assemble.h"
#include "eventloop.h"

class COMPONENT_IMEXPORT AssemblerController : public QObject
{
    Q_OBJECT
public:
    explicit AssemblerController(QObject *parent = nullptr);
    ~AssemblerController();
    QByteArray message() const;
private:
    QThread assemblethread;
    Assembler * assembler;
    QByteArray msg;
    EventLoop loop;
signals:
    void assemble(const QString&frame, const QVariantMap& m);
    void assembleResult(const QByteArray& m);
};
#endif //EOSI_ASSEMBLECONTROL_H
