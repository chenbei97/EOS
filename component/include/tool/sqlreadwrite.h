/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 16:47:13
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-16 16:47:43
 * @FilePath: \EOS\component\include\tool\sqlreadwrite.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOSI_SQLREADWRITE_H
#define EOSI_SQLREADWRITE_H

#include "tool.h"

class COMPONENT_IMEXPORT SqlReadWrite : public QObject
{
    Q_OBJECT
protected:
    QSqlDatabase mDB;
    QSqlQuery mQuery;
    SQLType mType;
public:
    explicit SqlReadWrite(QObject *parent = nullptr);

    virtual QDateTime lastVisitTime(QCString table);
    virtual QDateTime lastUpdateTime(QCString table);
    virtual QDateTime createTime(QCString table);

    void setType(SQLType type);
    SQLType type() const;
    void close();

    const QSqlDatabase database() const;
    QString currentDataSource();
    bool isValid() const;
    bool isOpen() const;

    QString dbName() const;
    QString ip() const;
    QString username() const;
    QString password() const;
    int port() const;

    bool createTable(QCString statement);
    bool createTable(QCString statement,QCString init);
    bool createEmptyTable(QCString table);
    bool dropTable(QCString table);
    bool haveTable(QCString table);
    bool containTable(QCString table);

    int tableRows(QCString table);
    virtual int tableColumns(QCString table);

    bool haveField(QCString table,QCString field);
    bool containField(QCString table,QCString field);
    bool fieldsRight(QCString table,QCFieldList fieldNames);

    QString fieldName(QCString table,int index);
    QFieldList fieldNames(QCString table);
    int fieldIndex(QCString table,QCString field);

    QString fieldValue(QCString table,int row, int col);
    QString fieldValue(QCString table,int row, const QString&field);

    bool query(QCString q);
    QString dbError() const;
    QString lastError() const;
    QSqlRecord lastRecord() const;
    QSqlQuery lastQuery() const;

    bool addRecord(QCString table,QCFieldList fieldList, QCValueList values); // 多值
    bool addRecord(QCString table,QCFieldList fieldList, QCString value);//单组值
    bool addRecord(QCString table,QCFieldList fieldList, QCValueVector values); // 单组值
    QBoolVector addRecord(QCString table,QCFieldList fieldList, QCValue2DVector map); // 多值

    bool haveRecord(QCString table,QCString condition);
    bool haveRecord(QCString table,QCString field,QCString condition);
    bool haveRecord(QCString table,int row);

    bool removeRecord(QCString table, QCString condition);
    bool removeRecord(QCString table, int row);
    bool rollback(QCString flag);

    bool updateRecord(QCString table,QCString dict, QCString condition);
    bool updateRecord(QCString table,QCString dict, int row);
    bool updateRecord(QCString table,QCString key, QCString value,QCString condition);
    bool updateRecord(QCString table,QCString key, QCString value,int row);
    bool updateRecord(QCString table,QFieldList keys, QValueList values,QCString condition);
    bool updateRecord(QCString table,QFieldList keys, QValueList values,int row);
signals:

};
#endif //EOSI_SQLREADWRITE_H
