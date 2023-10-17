/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-17 17:27:15
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-17 17:27:47
 * @FilePath: \EOS\component\include\socket\assemble.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_ASSEMBLE_H
#define EOSI_ASSEMBLE_H

#include "socket.h"

class COMPONENT_IMEXPORT Assembler : public QObject
{
    Q_OBJECT
public:
    explicit Assembler(QObject *parent = nullptr);
    void assemble(const QString&frame,const QVariantMap& m);
private:
    QMutex mutex;
signals:
    void assembleResult(const QByteArray& d);
};
#endif //EOSI_ASSEMBLE_H
