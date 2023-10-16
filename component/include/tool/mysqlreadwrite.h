/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 17:10:07
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-16 17:10:32
 * @FilePath: \EOS\component\include\tool\mysqlreadwrite.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOSI_MYSQLREADWRITE_H
#define EOSI_MYSQLREADWRITE_H

#include "sqlreadwrite.h"

/*
 *  "Unknown database 'cell1' QMYSQL: Unable to connect"
 *  "Access denied for user 'roo'@'localhost' (using password: YES) QMYSQL: Unable to connect"
 * "Access denied for user 'root'@'localhost' (using password: YES) QMYSQL: Unable to connect"
 *  "Can't connect to MySQL server on '192.0.0.1:3306' (10060) QMYSQL: Unable to connect"
 *  "Can't connect to MySQL server on '127.0.0.1:3305' (10060) QMYSQL: Unable to connect"
*/
enum class MySqlLoginError {
    DataSourceUnFinded,
    UserOrPwdError,
    IPOrPortError,
    NoError
};

class COMPONENT_IMEXPORT MySqlReadWrite : public SqlReadWrite
{
    Q_OBJECT
public:
    static MySqlReadWrite& instance();
    MySqlLoginError open(QCString db, QCString user, QCString pwd,QCString ip, int port);
    virtual QDateTime lastVisitTime(QCString table) override;
    virtual QDateTime lastUpdateTime(QCString table) override;
    virtual QDateTime createTime(QCString table) override;

    bool haveRecordCaseSensitive(QCString table,QCString condition);
    bool haveDataSource(QCString source);
    bool createDataSource(QCString source);
    bool dropDataSource(QCString source);

    int tableColumns(QCString table) override;

    bool primaryKeyReorder(QCString table);
    QString globaldir();
    QString datadir();
    QStringList tablePaths();

private:
    explicit MySqlReadWrite(QObject*parent = nullptr);
    ~MySqlReadWrite();
};
#endif //EOSI_MYSQLREADWRITE_H
