/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 16:47:24
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-16 16:47:34
 * @FilePath: \EOS\component\src\tool\sqlreadwrite.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "sqlreadwrite.h"

SqlReadWrite::SqlReadWrite(QObject *parent) : QObject(parent)
{
}

void SqlReadWrite::setType(SQLType type)
{ // 如果不使用子类,需要设置类型,这样可以使用haveTable,createEmptyTable等函数
    mType = type;
}

SQLType SqlReadWrite::type() const
{
    return mType;
}

void SqlReadWrite::close()
{
    mDB.close();
}

bool SqlReadWrite::isValid() const
{
    return mDB.isValid();
}

bool SqlReadWrite::isOpen() const
{
    return mDB.isOpen();
}

QString SqlReadWrite::dbName() const
{
    return mDB.databaseName();
}

QString SqlReadWrite::ip() const
{
    return mDB.hostName();
}

QString SqlReadWrite::username() const
{
    return mDB.userName();
}

QString SqlReadWrite::password() const
{
    return mDB.password();
}

int SqlReadWrite::port() const
{
    return mDB.port();
}

bool SqlReadWrite::dropTable(QCString table)
{
    if (!haveTable(table)) {
        LOG<<table<<" table is not existed, can't drop!";
        return true;
    }
    auto statement = QString(DropTable).arg(table);
    mQuery.exec(statement);
    if (mQuery.isActive()) {
        LOG<<"drop table "<<table<<" successful!";
        return true;
    } else {
        //SqlExecFailedLOG<<lastError();
        LOG<<statement<<" failed!";
    }
    LOG<<"drop table "<<table<<" failed!";
    return false;
}

bool SqlReadWrite::haveTable(QCString table)
{
    QString statement;
    if (mType==SQLType::Sqlite)
        statement = QString(SelectTableExistedFromSqliteMaster).arg(table);
    else if (mType == SQLType::Mysql)
        statement = QString(ShowTablesLike).arg(table);
    else return false;

    mQuery.exec(statement);
    if (mQuery.isActive()){ // 执行语句成功前提下
        mQuery.first();
        auto rec = lastRecord();
        if (mQuery.isValid() && rec.count() == 1) { // 查询有效时只会有1条记录
            auto t = rec.value(0).toString(); // 会返回这个表名称
            if (t == table) {
                LOG<<"table"<<t<<" is already exist!";
                return true;
            }
        }
    } else { // 语句没执行成功
        //SqlExecFailedLOG<<lastError();
        LOG<<statement<<" failed!";
    }
    LOG<<"table"<<table<<" is not exist!";
    return false;
}

bool SqlReadWrite::createTable(QCString statement)
{ // 完全灵活的创建表表达式
    mQuery.exec(statement);
    if (!mQuery.isActive()) {
        //SqlExecFailedLOG<<lastError();
        LOG<<statement<<" failed!";
        return false;
    }
    LOG<<"create table is successful";
    return true;
}

bool SqlReadWrite::createTable(QCString create,QCString init)
{ // create指定创建语句,init指定表的具体语句 (create) ... init
    auto statement = QString(create).arg(init);
    return createTable(statement);
}

bool SqlReadWrite::createEmptyTable(QCString table)
{
    QString statement;
    if (mType == SQLType::Mysql)
        statement = QString(CreateEmptyTableMySql).arg(table);
    else if (mType == SQLType::Sqlite)
        statement = QString(CreateEmptyTableSqlite).arg(table);
    else return false;

    mQuery.exec(statement);
    if (!mQuery.isActive()) {
        //SqlExecFailedLOG<<lastError();
        LOG<<statement<<" failed!";
        return false;
    }
    LOG<<"create empty table is successful";
    return true;
}

bool SqlReadWrite::containTable(QCString table)
{
    return mDB.tables().contains(table);
}

const QSqlDatabase SqlReadWrite::database() const
{
    return mDB;
}

QString SqlReadWrite::currentDataSource()
{
    QString statement;
    if (mType == SQLType::Mysql)
        statement = CurrentDataSourceMysql;
    else if ( mType == SQLType::Sqlite)
        statement = CurrentDataSourceSqlite;
    else return "";

    mQuery.exec(statement);
    if (!mQuery.isActive()){
        //SqlExecFailedLOG<<lastError();
        LOG<<statement<<" failed!";
        return "";
    }

    mQuery.first();
    if (!mQuery.isValid())
        return "";

    if (mType == SQLType::Mysql)
        return mQuery.value(0).toString(); // mysql第1个值返回名称
    else return mQuery.value(2).toString();// sqlite是第3列返回名称
}

int SqlReadWrite::tableRows(QCString table)
{
    auto statements = QString(SelectTableRows).arg(table);
    mQuery.exec(statements);
    mQuery.first();
    if (!mQuery.isValid()) return 0; // 可能是无效的没有查到结果
    auto rows = mQuery.value(0).toInt();
    LOG<<"table's rows = "<<rows;
    return rows;
}

int SqlReadWrite::tableColumns(QCString table)
{ // 默认实现通过查询表后,record本身提供的功能返回
    // 对于mysql还可以使用其他命令得到
    auto statements = QString(SelectXFromTable).arg(table);
    mQuery.exec(statements);
    auto cols = lastRecord().count();
    LOG<<"table's cols = "<<cols;
    return cols;
}

bool SqlReadWrite::haveField(QCString table,QCString field)
{ // 直接使用查询字段的sql命令
    auto statement = QString(SelectFieldFromTable)
            .arg(field).arg(table);
    mQuery.exec(statement);

    if (mQuery.isActive()) { // 查询字段只要没执行失败就说明有这个字段
        LOG<<"field ["<<field<<"] is exist!";
        return true;
    } else {
        //SqlExecFailedLOG<<lastError();
        LOG<<statement<<" failed!";
    }
    LOG<<"field ["<<field<<"] is not exist!";
    return false;
}

bool SqlReadWrite::containField(QCString table,QCString field)
{ // 不直接查询字段,查表后利用record实现
    auto statements = QString(SelectXFromTable).arg(table);
    mQuery.exec(statements);
    if (lastRecord().contains(field)) {
        LOG<<"field ["<<field<<"] is contains!";
        return true;
    }
    LOG<<"field ["<<field<<"] is not contains!";
    return false;
}

bool SqlReadWrite::fieldsRight(QCString table,QCFieldList fields)
{ // 查询表中的字段,和事先规定的字段是否匹配,fields来自sqlstatements.h
    // 这样确定表没有被修改过,不过表应该一开始就加密并不允许被修改,只是一层验证
    auto tableFields = fieldNames(table);

    // 1.检查数量
    auto count = tableFields.count();
    if (count != fields.count()) {
        LOG<<"field's count not right, expect count = "<<fields.count()
           <<" infact count = "<<tableFields.count();
        return false;
    }

    // 2.逐顺序比较字段是否对应正确
    LOG<<"field's count is right, count = "<<count;
    for (int i = 0; i < count; ++i ) {
        if (tableFields.at(i) != fields.at(i)) {
            LOG<<"field's count is right, but field "
               <<tableFields.at(i)<<" "<<fields.at(i) <<" dismatch";
            return false;
        }
    }
    LOG<<"table's field is right";
    return true;
}

QString SqlReadWrite::fieldName(QCString table,int index)
{// 查询指定表某列的字段名称
    auto statements = QString(SelectXFromTable).arg(table);
    mQuery.exec(statements);
    auto name = lastRecord().fieldName(index);
    LOG<<"field name is "<<name<<"  with column "<<index;
    return name;
}

QFieldList SqlReadWrite::fieldNames(QCString table)
{ // 把表所有的字段列出来
    auto statements = QString(SelectXFromTable).arg(table);
    mQuery.exec(statements);
    QStringList ret;
    auto rec = lastRecord();
    for (int i = 0; i < rec.count(); ++i)
        ret.append(rec.fieldName(i));
    LOG<<table<<"'s allfield is "<<ret;
    return ret;
}

int SqlReadWrite::fieldIndex(QCString table,QCString field)
{ // 查询指定字段所在的列索引
    auto statements = QString(SelectXFromTable).arg(table);
    mQuery.exec(statements);
    auto idx = lastRecord().indexOf(field);
    LOG<<"the "<<field<<"'s column index is "<<idx;
    return idx;
}

QString SqlReadWrite::fieldValue(QCString table,int row, int col)
{ // 查询指定行指定列的值,要求查询的表必须是有序的,按照id查询
    if (row<0 || row > tableRows(table)-1) {
        LOG<<"row is out of range";
        return "row is out of range";
    }
    if (col<0 || col > tableColumns(table) -1) {
        LOG<<"column is out of range";
        return "column is out of range";
    }

    auto statements = QString(SelectXFromTableOrderById).arg(table);
    mQuery.exec(statements);
    int r = row;
    mQuery.first();
    while (row-- && mQuery.next()) {}; // 移动到指定行
    auto value = mQuery.value(col).toString(); // 拿到指定列的值
    LOG<<table<<" ["<<r<<","<<col<<"] 's value = "<<value;
    return value;
}

QString SqlReadWrite::fieldValue(QCString table,int row, QCString field)
{// 查询指定行指定列的值,要求查询的表必须是有序的,按照id查询
    if (row<0 || row > tableRows(table)-1) {
        LOG<<"row is out of range";
        return "row is out of range";
    }
    if (!haveField(table,field)) {
        LOG<<field<<" is not exists!";
        return field+" is not exists!";
    }

    auto statements = QString(SelectXFromTableOrderById).arg(table);
    mQuery.exec(statements);
    mQuery.first();
    int r = row;
    while (row-- && mQuery.next()) {}; // 移动到指定行
    auto value = mQuery.value(field).toString();
    LOG<<table<<" ["<<r<<","<<field<<"] 's value = "<<value;
    return value;
}

bool SqlReadWrite::query(QCString q)
{ // 使用方式仅限于验证查询某个语句是否成功
    auto r = mQuery.exec(q);
    if (!r) {
        //SqlExecFailedLOG<<lastError();
        LOG<<q<<" failed!";
    }
    return r;
}

QString SqlReadWrite::dbError() const
{
    return mDB.lastError().text();
}

QString SqlReadWrite::lastError() const
{
    return  mQuery.lastError().text();
}

QSqlRecord SqlReadWrite::lastRecord() const
{
    return mQuery.record();
}

QSqlQuery SqlReadWrite::lastQuery() const
{
    return mQuery;
}

bool SqlReadWrite::addRecord(QCString table,QCFieldList fieldList, QCValueList values)
{/*使用方式说明如下:
    * values是个列表,每个元素都是小括号包裹起来逗号分隔的字符串,需要单引号 "('','','')","('','','')"
    * fieldList是所有字段的列表,要拆成逗号分隔的,不需要单引号
    * QFieldsList fields = {"a1","a2","a3"};
    * QFieldsList values = {"('0.5','0.6','chenbei')","('0.86','0.34','chbi')","('0.98','0.16','cb')"};
    * instance->addRecord(table,fields,values);
    * 本函数可以一次性添加多组记录,字段数量和值的元素包含的值个数要一致
*/
    QString field;
    foreach(auto f,fieldList){
        field.append(f);
        field.append(',');
    }
    field.chop(1);// 拆解字段列表变为字段字符串'a1,a2,a3'

    QString value;
    foreach(auto v,values) {
        value.append(v);
        value.append(',');
    }
    value.chop(1); // 拆解值列表变为值字符串('0.5','0.6','chenbei'),('0.86','0.34','chbi')

    auto statement = QString(InsertRecord).arg(table).arg(field).arg(value);
    mQuery.exec(statement);
    if (mQuery.isActive()) {
        LOG<<statement<<" successful!";
        return true;
    } else {
        //SqlExecFailedLOG<<lastError();
    }
    LOG<<statement<<" failed!";
    return false;
}

bool SqlReadWrite::addRecord(QCString table,QCFieldList fieldList, QCString value)
{ /*使用方式说明如下:
    * value是个小括号和单引号包裹起来的值
    * fieldList是所有字段的列表,要拆成逗号分隔的,不需要单引号
    * QFieldsList fields = {"a1","a2","a3"};
    * QFieldsList values = "('0.5','0.6','chenbei')"
    * 本函数用于添加1组记录,字段数量和值的元素包含的值个数要一致
*/
    QCValueList values = {value}; // 转换成列表,借助添加多记录函数直接完成
    return addRecord(table,fieldList,values);
}

bool SqlReadWrite::addRecord(QCString table,QCFieldList fieldList, QCValueVector values)
{ /*使用方式说明如下:
    * values是个列表,为了函数重载改用QCValueVector,每个值和fieldList对应,长度相同,不是单引号形式
    * fieldList是所有字段的列表,要拆成逗号分隔的,不需要单引号
    * QFieldsList fields = {"a1","a2","a3"};
    * QFieldsList values = {"0.5","0.34","cb"};
    * 本函数用于添加1组记录,字段数量和值的元素包含的值个数要一致
*/
    if (fieldList.count() != values.count()){
        LOG<<"values'count = "<<values.count()<<" fields'count = "<<fieldList.count()<<" dismatch!";
        return false;
    }

    // 借助addRecord(QCString table,QCFieldList fieldList, QCString value)来实现
    // 所以需要改造成小括号和单引号修饰的格式
    QString value = "("; // 加括号
    foreach(auto v,values) {
        value.append("\'"); // 加单引号
        value.append(v);
        value.append("\'");
        value.append(',');
    }
    value.chop(1);
    value.append(")");// 将{"0.5","0.34","cb"}改造成('0.5','0.34','cb')的格式

    return addRecord(table,fieldList,value);
}

QBoolVector SqlReadWrite::addRecord(QCString table,QCFieldList fieldList, QCValue2DVector values)
{ /*使用方式说明如下:
    * values是个二维列表,每个一维列表和fieldList对应,长度相同,不是单引号形式
    * fieldList是所有字段的列表,要拆成逗号分隔的,不需要单引号
    * QFieldsList fields = {"a1","a2","a3"};
    * QFieldsList values = {{"0.5","0.34","cb"},{"0.5","0.34","cb"},{"0.5","0.34","cb"}};
    * 本函数可以一次性添加多组记录,字段数量和值的一维列表长度要一致
*/
    auto count = values.count(); // 添加记录的条数
    QBoolVector list(count); // 记录每条记录添加是否成功
    for(int i = 0 ; i < count; ++i) {
        QValueVector vals = values.at(i); // 该条记录
        if (vals.count() != fieldList.count()){ // 和字段长度不匹配
            list[i] = false; // 键值对个数必须匹配
            continue;
        }
        else { // 调用addRecord(QCString,QCFieldList, QCValueVector)
            list[i] = addRecord(table,fieldList,vals);
        }
    }
    LOG<<"add multi record's ret is "<<list;
    return list;
}

bool SqlReadWrite::haveRecord(QCString table,QCString condition)
{ // 是否存在某条记录,condition是where后面跟随的语句
    auto statement = QString(SelectXFromTableWhere).arg(table).arg(condition);
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

bool SqlReadWrite::haveRecord(QCString table,QCString field,QCString condition)
{// 是否存在某条记录,condition是where后面跟随的语句,field是查找特定字段的条件
    auto statement = QString(SelectFieldFromTableWhere).arg(field).arg(table).arg(condition);
    mQuery.exec(statement);
    if (!mQuery.isActive()) {
        //SqlExecFailedLOG<<lastError();
        LOG<<statement<<" failed!";
    } else {
        mQuery.first();
        if (mQuery.isValid()) {
            LOG<<"have record where "<<field<<condition;
            return true;
        }
    }
    LOG<<"don't have record where "<<field<<condition;
    return false;
}

bool SqlReadWrite::haveRecord(QCString table,int row)
{// 一个快捷方法检查是否存在指定行记录,id是有序的,指定行就是指定id
    auto condition = QString(FilterID).arg(row);
    return haveRecord(table,condition);
}

bool SqlReadWrite::removeRecord(QCString table, QCString condition)
{ // 删掉指定条件的某条记录
    if (!haveRecord(table,condition)) return true;

    auto statement = QString(RemoveRecord).arg(table).arg(condition);
    mQuery.exec(statement);
    if (!mQuery.isActive()) {
        //SqlExecFailedLOG<<lastError();
        LOG<<statement<<" failed!";
        return false;
    }
    LOG<<statement<<" successful!";
    return true;
}

bool SqlReadWrite::removeRecord(QCString table, int row)
{ // 一个快捷方法删除指定行的记录,id是有序的,指定行就是指定id
    auto condition = QString(FilterID).arg(row);
    return removeRecord(table,condition);
}

bool SqlReadWrite::rollback(QCString table,QCString flag)
{ /*
 * 本回滚函数只限于取消添加之前添加的记录,不能用于删除的记录恢复,这里flag是每条记录独有的值
 * 一般用于启动实验失败,那么这条记录应该回滚,取消储存之前的记录
 * 如果是删除的记录恢复,那就需要在addRecord时还要记录上次的操作,再把那次添加的记录删除
 * addRecord有个实现是把多组值拆成单组值循环添加,所以回滚比较麻烦,暂时不做这个功能
 * */

    removeRecord(table,QString(FilterFlag).arg(flag));
    auto r = haveRecord(table,QString(FilterFlag).arg(flag));

    LOG<<"rollback table? "<<table<<!r; // 确实不存在这条记录回滚成功
    return r;

    // 下方代码是特定业务的会耦合,都注释掉,在其他地方去写
//    removeRecord(ExperGeneralConfigTableName,QString(FilterFlag).arg(flag));
//    bool r1 = haveRecord(ExperGeneralConfigTableName,QString(FilterFlag).arg(flag));
//    LOG<<"rollback table1? "<<!r1; // 确实不存在这条记录回滚成功
//    removeRecord(GroupPointTableName,QString(FilterFlag).arg(flag));
//    bool r2 = haveRecord(GroupPointTableName,QString(FilterFlag).arg(flag));
//    LOG<<"rollback table2? "<<!r2;
//    return r1&&r2;
}

bool SqlReadWrite::updateRecord(QCString table,QCString values, QCString condition)
{/*使用方式说明如下:
    * dict是个字符串,由多对key='val'组合,"a1='0.5',a2="0.02",a3='abc'"
    * condition是条件,例如"id='3'"
    * 用于更新多对值
*/
    auto statement = QString(UpdateRecord)
            .arg(table).arg(values).arg(condition);
    mQuery.exec(statement);
    if (!mQuery.isActive()) {
        //SqlExecFailedLOG<<lastError();
        LOG<<statement<<" failed!";
        return false;
    }
    LOG<<statement<<" successful!";
    return true;
}

bool SqlReadWrite::updateRecord(QCString table,QCString values, int row)
{// 一个快捷方法更新指定行的记录,指定行就是指定id,id是有序的,更新指定行的多对值
    if (!haveRecord(table,row)) return false;
    auto condition = QString(FilterID).arg(row);
    return updateRecord(table,values,condition);
}

bool SqlReadWrite::updateRecord(QCString table,QCString key, QCString value,QCString condition)
{ /*使用方式说明如下:
    * 把dict是其实拆成了key,value就是,需要手动组合"key = 'value'"
    * condition是条件,例如"id='3'"
    * 用于更新一对值
*/
    auto dict = QString("%1 = '%2' ").arg(key).arg(value);
    return updateRecord(table,dict,condition);
}

bool SqlReadWrite::updateRecord(QCString table,QCString key, QCString value,int row)
{// 一个快捷方法更新指定行的记录,指定行就是指定id,id是有序的,更新指定行的一对值
    if (!haveRecord(table,row)) return false;
    auto condition = QString(FilterID).arg(row);
    return updateRecord(table,key,value,condition);
}

bool SqlReadWrite::updateRecord(QCString table,QFieldList keys, QValueList values,QCString condition)
{ /*使用方式说明如下:
    * 把dict是其实拆成了key,value就是,需要手动组合"key = 'value'",现在又变成了多对
    * condition是条件,例如"id='3'"
    * 用于更新多对值
*/
    if (keys.count() != values.count()) return false;

    QString dict;
    auto count = keys.count();
    for(int i = 0; i < count; ++i) {
        auto key = keys.at(i);
        auto val = values.at(i);
        dict += key + " = " + "\'" +val + "\'"+','; // 得到"A='1',B='1',..."的字符串形式
    }
    dict.chop(1);//去掉逗号
    return updateRecord(table,dict,condition);
}

bool SqlReadWrite::updateRecord(QCString table,QFieldList keys, QValueList values,int row)
{ // 更新指定行的字段值,指定行就是指定id,id是有序的,更新指定行的多对值
    if (!haveRecord(table,row)) return false;
    auto condition = QString(FilterID).arg(row);
    return updateRecord(table,keys,values,condition);
}

QDateTime SqlReadWrite::lastVisitTime(QCString)
{
    return QDateTime();
}

QDateTime SqlReadWrite::lastUpdateTime(QCString)
{
    return QDateTime();
}

QDateTime SqlReadWrite::createTime(QCString)
{
    return QDateTime();
}
