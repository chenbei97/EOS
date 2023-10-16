/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 17:14:39
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-16 17:15:18
 * @FilePath: \EOS\component\include\tool\sqlitereadwrite.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOSI_SQLITEREADWRITE_H
#define EOSI_SQLITEREADWRITE_H

#include "sqlreadwrite.h"
#include "utilities.h"

class COMPONENT_IMEXPORT SqliteReadWrite : public SqlReadWrite
{
    Q_OBJECT
public:
    static SqliteReadWrite& instance();
    bool open(QCString db,QCString path = QString());
    bool haveDataSource(QCString source,QCString path = QString());
    bool createDataSource(QCString source,QCString path = QString());
    bool dropDataSource(QCString source,QCString path = QString());
    virtual QDateTime lastVisitTime(QCString table = "") override;
    virtual QDateTime lastUpdateTime(QCString table = "") override;
    virtual QDateTime createTime(QCString table = "") override;
private:
    explicit SqliteReadWrite(QObject*parent = nullptr);
    ~SqliteReadWrite();
    QString mStandardPath;
};
#endif //EOSI_SQLITEREADWRITE_H
