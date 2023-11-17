/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 17:14:35
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-17 11:05:33
 * @FilePath: \EOS\component\src\tool\sqlitereadwrite.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "sqlitereadwrite.h"

SqliteReadWrite::SqliteReadWrite(QObject*parent):SqlReadWrite(parent)
{
    mType = SQLType::Sqlite;
#ifdef UseSqlcipher
    mDB = QSqlDatabase::addDatabase(SQLITECIPHERField, SqlcipherField);
#else
    mDB = QSqlDatabase::addDatabase(QSQLITEField,SqliteField);
#endif
    mQuery = QSqlQuery(mDB);

    // 备用的数据库保存标准路径在../AppData/EOSI下
    CheckAndCreateStandardPath;
    mStandardPath = getAppStandardDirPath();
}

SqliteReadWrite& SqliteReadWrite::instance()
{
    static SqliteReadWrite instance;
    return instance;
}

SqliteReadWrite::~SqliteReadWrite()
{
#ifdef UseSqlcipher
    QSqlDatabase::removeDatabase(SqlcipherField);
#else
    QSqlDatabase::removeDatabase(SqliteField);
#endif
}

bool SqliteReadWrite::open(QCString filename,QCString dir)
{ /*
 * filename: 必须是完整带后缀的文件名称: eos.db3/eos.db不能是eos
 * path: 用户可更改的前缀保存目录,path默认是QString()空的
 * mysql调用open会有重复打开的问题,sqlite无需考虑
 * */
    QString fullPath;

    // 1. 检查指定的目录情况
    if (!dir.isEmpty()) { // 用户指定了数据库保存路径
        if (!pathExisted(dir)) { // 指定的路径不存在(多加层保护,指定时就不允许指定不存在的目录)
            //LOG<<dir<<" is not exists!";
            return false;
        } // 传递的路径存在.拼接为全路径
        fullPath = dir +"/"+filename;
    } else { // 使用标准路径
        fullPath = mStandardPath+"/"+filename; // 用户没指定默认是标准路径
    }

    // 2. 设置数据库属性
    mDB.setDatabaseName(fullPath);
    mDB.setPassword(SqliteDataBasePassword);
#ifdef UseSqlcipher
    mDB.setConnectOptions(SqliteDataBaseConnectionsFirst);
#endif
    mDB.open(); // 如果设置了加密,打开是可能失败的,要重新设置连接选项

    // 3. 检查打开情况
    if (!isOpen()) {
        LOG<<"sqlite not open! error is "<<dbError();
#ifdef UseSqlcipher
        mDB.setConnectOptions(SqliteDataBaseConnectionsAgain);
#endif
        mDB.open();
    }

    if (!isOpen())  { // 仍然没打开
        LOG<<"sqlite open failed again! error is "<<dbError(); // 一般不会发生
        return false;
    }
    LOG<<"sqlite is init and open! path is "<<mDB.databaseName();
    return true;
}

bool SqliteReadWrite::haveDataSource(QCString source,QCString dir)
{ // 相对于mysql没有相关命令,只能是文件操作: 检查是否有这个文件,文件就是源
    if (!source.endsWith(DB3Suffix) || !source.endsWith(DBSuffix)
     || !pathExisted(dir))
        return false;

    QString fullpath;

    if (!dir.isEmpty())  // 检查用户指定或者标准路径下的db文件是否存在
        fullpath = dir+"/"+source;
    else  // 检查标准路径
        fullpath = mStandardPath+"/"+source;

    LOG<<fullpath<<" is exists!";
    return true;
}

bool SqliteReadWrite::createDataSource(QCString source,QCString dir)
{ // 相对于mysql没有相关命令,只能是文件操作: 创建这个文件就是创建源

    if (!source.endsWith(DB3Suffix) || !source.endsWith(DBSuffix)
        || !pathExisted(dir))
        return false;

    if (haveDataSource(source,dir)) {
        return true; // 已存在不要再创建
    }

    QString fullpath;

    if (dir.isEmpty()) // 标准路径下创建数据源
        fullpath = mStandardPath+"/"+source;
    else {
        fullpath = dir + "/" + source;
    }

    QFile file(fullpath);
    file.open(QIODevice::ReadWrite);// sqlite的open本身就是创建文件
    file.close();
    LOG<<"create data source successful!";
    return true;
}

bool SqliteReadWrite::dropDataSource(QCString source,QCString dir)
{ // 相对于mysql没有相关命令,只能是文件操作: 删除这个文件就是删除源
    if (!source.endsWith(DB3Suffix) || !source.endsWith(DBSuffix)
        || !pathExisted(dir))
        return false;

    QString fullpath;

    if (dir.isEmpty()) // 标准路径下创建数据源
        fullpath = mStandardPath+"/"+source;
    else
        fullpath = dir + "/" + source;

    bool ok = QFile::remove(fullpath); // 路径存在时也不一定删除成功可能文件被占用
    if (ok)
        LOG<<"remove data source successful!";
    else LOG<<"remove data source failed!";
    return ok;
}

QDateTime SqliteReadWrite::lastVisitTime(QCString table)
{// sqlite没有特定表路径,mysql可以查询指定表的上次访问时间,sqlite只能是整体文件的访问
    Q_UNUSED(table);
    QFileInfo info(mDB.databaseName());
    return info.lastRead();
}

QDateTime SqliteReadWrite::lastUpdateTime(QCString table)
{ // 上次更新时间同理
    Q_UNUSED(table);
    QFileInfo info(mDB.databaseName());
    return info.lastModified();
}

QDateTime SqliteReadWrite::createTime(QCString table)
{ // 创建时间同理
    Q_UNUSED(table);
    QFileInfo info(mDB.databaseName());
    return info.birthTime();
}
