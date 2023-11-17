/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 17:10:13
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-16 17:10:49
 * @FilePath: \EOS\component\src\tool\mysqlreadwrite.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "mysqlreadwrite.h"

MySqlReadWrite::MySqlReadWrite(QObject*parent):SqlReadWrite(parent)
{
    mType = SQLType::Mysql;
    mDB = QSqlDatabase::addDatabase(QMYSQLField,MysqlField);
    mDB.setConnectOptions(MysqlDataBaseConnections);
    mQuery = QSqlQuery(mDB);
}

MySqlReadWrite::~MySqlReadWrite()
{
    QSqlDatabase::removeDatabase(MysqlField);
}

MySqlReadWrite& MySqlReadWrite::instance()
{
    static MySqlReadWrite instance;
    return instance;
}

MySqlLoginError MySqlReadWrite::open(QCString db, QCString user, QCString pwd,QCString ip, int port)
{ // open就完成了检查数据源的功能,如果数据源不存在会返回DataSourceUnFinded
    if (isValid() && isOpen()) {
        LOG<<"mysql is already opened!";
        return MySqlLoginError::NoError;
    }
    mDB.setDatabaseName(db);
    mDB.setUserName(user);
    mDB.setPassword(pwd);
    mDB.setHostName(ip);
    mDB.setPort(port);
    mDB.open();

    if (!isOpen()) {
        auto error = dbError();
        LOG<<"mysql open failed! error is "<<error;
        if (error.contains("Unknown database"))
            return MySqlLoginError::DataSourceUnFinded;
        if (error.contains("Access denied for user"))
            return MySqlLoginError::UserOrPwdError;
        if (error.contains("Can't connect to MySQL server on"))
            return MySqlLoginError::IPOrPortError;
    }
    LOG<<"mysql is open successful!";
    return MySqlLoginError::NoError;
}

bool MySqlReadWrite::haveRecordCaseSensitive(QCString table,QCString condition)
{ /* 对于mysql有特定的查询记录的命令
 * 大小写敏感查询记录要增加binary关键字 select * from %1 where binary %2
 * 注意: 如果数据库存放的是加密值,那么查找条件也必须是加密过的
 * */
    auto statement = QString(SelectXFromTableWhereBinary).arg(table).arg(condition);
    mQuery.exec(statement);
    if (!mQuery.isActive()) {
        //SqlExecFailedLOG<<lastError();
        LOG<<statement<<" failed!";
    } else {
        mQuery.first();
        if (mQuery.isValid()) { // 记录有效就说明找到了
            LOG<<"have record where "<<condition;
            return true;
        }
    }
    LOG<<"don't have record where "<<condition;
    return false;
}

bool MySqlReadWrite::haveDataSource(QCString source)
{ /* mysql有查询是否存在指定源的命令,但必须mDB登录成功才能执行
 * 但是就是想在数据库没登陆前去检查是否有这个文件才行,那么本命令不适用
 * 只能使用QFileInfo和sqlite一样检查文件的存在性,但是mysql的表文件在特定安装目录下,相对特殊,需要指定source+dir
 * 但是mysql文件的目录一般无法显示指定,因电脑不同,所以只能借助open方法来判断是否存在,不存在会提示
 * "Unknown database 'eosi' QMYSQL: Unable to connect" 类似的错误
 * 注意: 如果数据库存放的是加密值,那么查找条件也必须是加密过的
 * */

    auto statement = QString(SelectCountFromInformationSchemaWhere).arg(source);
    //QSqlQueryModel model;
    //model.setQuery(statements);
    mQuery.exec(statement);
    if (mQuery.isActive()) {
        mQuery.first();
        if (mQuery.isValid() && mQuery.record().count() == 1) {
            if (mQuery.value(0).toInt()  == 1) { // 查询结果有效数量为1且值为1
                LOG<<"datasource is exist! source is "<<source;
                return true;
            }
        }
    } else {
        //SqlExecFailedLOG<<lastError();
        LOG<<statement<<" failed!";
    }
    LOG<<"datasource is not exist! source is "<<source;
    return false;
}

bool MySqlReadWrite::createDataSource(QCString source)
{ // 必须连接上mysql才能使用本函数
    if (haveDataSource(source)) return true;

    auto statement = QString(CreateDataBaseUtf8).arg(source);
    mQuery.exec(statement);
    if (mQuery.isActive()) {
        LOG<<"create datasource successful! source is "<<source;
        return true;
    }else {
        //SqlExecFailedLOG<<lastError();
        LOG<<statement<<" failed!";
    }
    LOG<<"create datasource failed! source is "<<source;
    return false;
}

bool MySqlReadWrite::dropDataSource(QCString source)
{// 必须连接上mysql才能使用本函数
    auto statement = QString(DropDataBase).arg(source);
    mQuery.exec(statement);
    if (mQuery.isActive()) {
        LOG<<"drop datasource successful! source is "<<source;
        return true;
    }else {
        //SqlExecFailedLOG<<lastError();
        LOG<<statement<<" failed!";
        // "Can't drop database 'eosi'; database doesn't exist QMYSQL: Unable to execute query"
    }
    LOG<<"drop datasource failed! source is "<<source;
    return false;
}

int MySqlReadWrite::tableColumns(QCString table)
{ // 对于mysql可以用自己的方法额外查询,sqlite只能借助query->record()来查询
    auto statement = QString(SelectTableColumnsFromInformationSchema).arg(table);
    mQuery.exec(statement);

    if (mQuery.isActive()) {
        mQuery.first();
        auto cols = mQuery.value(0).toInt();
        return cols;
    }else {
        //SqlExecFailedLOG<<lastError();
        LOG<<statement<<" failed!";
    }
    return 0;
}

bool MySqlReadWrite::primaryKeyReorder(QCString table)
{ // 主键重排序,添加删除记录后会导致数据库显示记录时乱序,会重排id
    return query(QString(PrimaryKeyReorderMysql).arg(table));
}

QString MySqlReadWrite::dataDir()
{ // mysql安装时数据存储的目录,登录成功后可用
  // 一般是C:\\ProgramData\\MySQL\\MySQL Server 8.0\\Data
    mQuery.exec(SelectDataDir);
    if (!mQuery.isActive()) {
        //SqlExecFailedLOG<<lastError();
        LOG<<SelectDataDir<<" failed!";
        return QString();
    }
    mQuery.first();
    auto dir = mQuery.value(0).toString();
    return dir;
}

QString MySqlReadWrite::sourcePath()
{ // 数据源存储的具体路径,这里其实可以拿去haveDataSource去判断
    // 但是dataDir的得知也是登陆成功才可用,所以其实还是不能直接用来判断数据源是否存在
    auto dir = dataDir();
    return dir+mDB.databaseName();
}

QStringList MySqlReadWrite::tablePaths()
{ // mysql返回指定数据源eosi所有表的存储路径
  // "C:\\ProgramData\\MySQL\\MySQL Server 8.0\\Data\\eosi\\experconfig.ibd"
    QStringList paths;
    auto dir = sourcePath(); // C:\\ProgramData\\MySQL\\MySQL Server 8.0\\Data\\eosi
        foreach(auto table, mDB.tables()) {
        QString path = dir + "\\"+table+IBDSuffix; // 会有ibd文件
        paths.append(path);
    }
    //LOG<<"tablePaths = "<<paths;
    return paths;
}

QDateTime MySqlReadWrite::lastVisitTime(QCString table)
{ // 上次访问时间,mysql没有相应代码查询,用qfileinfo的方法
    QString path;
    foreach(auto p,tablePaths())
        if (p.contains(table))
            path = p;// 找到这个表名对应的表路径

    if (path.isEmpty()) // 没有这个表
        return QDateTime();

    QFileInfo info(path);
    return info.lastRead();
}

QDateTime MySqlReadWrite::lastUpdateTime(QCString table)
{ // 上次更新时间可以通过查询来实现或者qfileinfo
    auto statement = QString(SelectUpdateTime)
            .arg(dbName()).arg(table);
    mQuery.exec(statement);

    if (!mQuery.isActive()) {
        //SqlExecFailedLOG<<lastError();
        LOG<<statement<<" failed!";
        return QDateTime();
    }

    mQuery.first();
    if (mQuery.isValid()) {
        auto datetime = mQuery.value(0).toString();
        return QDateTime::fromString(datetime,Qt::ISODate);
    }
    return QDateTime();
}

QDateTime MySqlReadWrite::createTime(QCString table)
{ // 表的创建时间
    auto statement = QString(SelectCreateTime).arg(dbName()).arg(table);
    mQuery.exec(statement);

    if (!mQuery.isActive()) {
        //SqlExecFailedLOG<<lastError();
        LOG<<statement<<" failed!";
        return QDateTime();
    }

    mQuery.first();
    if (mQuery.isValid()) {
        auto datetime = mQuery.value(0).toString();
        return QDateTime::fromString(datetime,Qt::ISODate);
    }
    return QDateTime();
}
