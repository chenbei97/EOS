/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 16:49:58
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-16 16:50:16
 * @FilePath: \EOS\component\include\tool\sqlstatements.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_SQLSTATEMENTS_H
#define EOSI_SQLSTATEMENTS_H

#include "alias.h"

enum class SQLType {Mysql,Sqlite};
Q_ENUMS(SQLType)

// 驱动的相关设置
static const char* SqliteDriver = "sqlite";
static const char* QSqliteDriver = "QSLITE";
static const char* SqlcipherDriver = "sqlcipher";
static const char* QSqlcipherDriver = "SQLITECIPHER";
static const char* MysqlDriver = "mysql";
static const char* QMysqlDriver = "QMYSQL";
static const char* ConfigFileSuffix = ".eos";
static const char* MatchConfigFileSuffix = "*.eos";
static const char* SqliteFileSuffix = ".db3";
//#define UseSqlcipher true
static const char*  SqliteDataBasePassword = "cb199791";
static const char*  MysqlDataBaseConnections = "MYSQL_OPT_CONNECT_TIMEOUT=1;";
static const char*  SqliteDataBaseConnectionsFirst =
        "QSQLITE_USE_CIPHER=sqlcipher; ""SQLCIPHER_LEGACY=1; "
        "SQLCIPHER_LEGACY_PAGE_SIZE=4096;QSQLITE_CREATE_KEY";
static const char*  SqliteDataBaseConnectionsAgain =
        "QSQLITE_USE_CIPHER=sqlcipher; ""SQLCIPHER_LEGACY=1; "
        "SQLCIPHER_LEGACY_PAGE_SIZE=4096";

// （1）表信息，表架构等
// （1.1）数据源固定名称
static const char*  DataBaseName = "eosi";
// （1.2）表的名称
static const char * LoginInfoTableName = "logininfo";
static const char* ExperGeneralConfigTableName = "experconfig";
static const char* GroupPointTableName = "grouppoint";
// （1.3）每张表的所有字段，按照顺序存储
static const char * LoginInfoUserField = "user";
static const char * LoginInfoPwdField = "pwd";
static const char * LoginInfoPLoginField = "last_login";
static const char * LoginInfoDefaultUser = "admin";
static const char * LoginInfoDefaultPwd = "123456";

static const QFieldList LoginInfoConfigFields = {
        "user","pwd","last_login"
};
static const QFieldList ExperGeneralConfigFields = {
        "id", "datetime",
        "user","flag",
        "channel","brand","imageformat","videoformat",
        "exper_description","exper_person","exper_type",
        "group_names",
        "objective",
        "count","duration","end_time","is_schedule","start_time","total",
        "view","wellsize"
};

struct ExperConfigTableField {
    const QString id = "id";
    const QString datetime = "datetime";
    const QString user = "user";
    const QString flag = "flag";
    const QString channel = "channel";
    const QString brand = "brand";
    const QString imageformat = "imageformat";
    const QString videoformat = "videoformat";

    const QString exper_description = "exper_description";
    const QString exper_person = "exper_person";
    const QString exper_type = "exper_type";
    const QString group_names = "group_names";
    const QString objective = "objective";
    const QString count = "count";
    const QString duration = "duration";
    const QString end_time = "end_time";
    const QString is_schedule = "is_schedule";
    const QString start_time = "start_time";
    const QString total = "total";
    const QString view = "view";
    const QString wellsize = "wellsize";
    struct OtherField {
        const QString experiment = "experiment"; // 4个json用的组字段
        const QString time = "time";
        const QString group = "group";
        const QString x = "x";
        const QString y = "y";
        const QString order = "order";
        const QString pname = "pname";
        const QString datetime_iso1 = "yyyy/MM/dd hh:mm:ss";
        const QString datetime_iso2 = "yyyy/MM/dd hh:mm"; // 用于start_time和end_time
        const QString datetime_num = "yyMMddhhmmss";
        const QString schedule = "schedule";
        const QString once = "once";
    };
    OtherField other_fields;
};

static const ExperConfigTableField ExperConfigTableFields;

#define ExperTableFlagColumn  (ExperGeneralConfigFields.indexOf("flag"))
#define ExperTableDateTimeColumn  (ExperGeneralConfigFields.indexOf("datetime"))
#define ExperTableChannelColumn  (ExperGeneralConfigFields.indexOf("channel"))
#define ExperTableBrandColumn  (ExperGeneralConfigFields.indexOf("brand"))
#define ExperTableImageFormatColumn  (ExperGeneralConfigFields.indexOf("imageformat"))
#define ExperTableVideoFormatColumn  (ExperGeneralConfigFields.indexOf("videoformat"))
#define ExperTableObjectiveColumn  (ExperGeneralConfigFields.indexOf("objective"))
#define ExperTableChannelColumn  (ExperGeneralConfigFields.indexOf("channel"))
#define ExperTableWellSizeColumn  (ExperGeneralConfigFields.indexOf("wellsize"))
#define ExperTableExperTypeColumn  (ExperGeneralConfigFields.indexOf("exper_type"))
#define ExperTableViewColumn  (ExperGeneralConfigFields.indexOf("view"))
#define ExperTableTotalColumn  (ExperGeneralConfigFields.indexOf("total"))
#define ExperTableDurationColumn  (ExperGeneralConfigFields.indexOf("duration"))
#define ExperTableScheduleColumn  (ExperGeneralConfigFields.indexOf("is_schedule"))
#define ExperTableGroupColumn  (ExperGeneralConfigFields.indexOf("group_names"))

#define ExperTableCountColumn  (ExperGeneralConfigFields.indexOf("count"))
#define ExperTableStartTimeColumn  (ExperGeneralConfigFields.indexOf("start_time"))
#define ExperTableEndTimeColumn  (ExperGeneralConfigFields.indexOf("end_time"))

static const QFieldList WellsizeFields = {
        "6","24","96","384",
};

static const QFieldList ObjectiveFields = {
        "4x","10x","20x","40x",
};

static const QFieldList ChannelFields = {
        "bright","phase","red","green","blue"
};

static const QFieldList ImageFormatFields = {
        "jpg","png","ico","bmp"
};

static const QFieldList VideoFormatFields = {
        "avi","wmv"
};

static const QFieldList BrandFields = {
        "brand1","brand2","brand3"
};

static const QFieldList  GroupPointFields = {
        "id","user","flag",
        "group_name","point_pos","point_name"
};

static const QList<QFieldList> SocketNeedMapFields{
        BrandFields,VideoFormatFields,ImageFormatFields,
        ChannelFields,ObjectiveFields,WellsizeFields
};

static QString getIndexFromFields (QCString field)
{ // 用于把这些字段统一映射为0,1,2,3方便下位机读取
    // 例如4x,jpg,bright,avi都应该映射为0
    QString idx = "";
    foreach(auto fields, SocketNeedMapFields) { // 仅限于从这些字段查找
        auto index = fields.indexOf(field);
        if (index>=0){
            idx = QString::number(index);
            break; // 找到
        }
    }
    return idx;
}

static const char* QueryGroupPointAndName =
        "select point_pos,point_name from grouppoint "
        "where flag = '%1' and group_name = '%2' ";

struct GroupPointTableField {
    const QString id = "id";
    const QString user = "user";
    const QString flag = "flag";
    const QString group_name = "group_name";
    const QString point_pos = "point_pos";
    const QString point_name = "point_name";
};

static const GroupPointTableField GroupPointTableFields;

// （2）通用命令
static const char* SelectUserAndPwd = "select user,pwd from logininfo";
static const char* RollBack = "rollback";
static const char* FilterAdmin = "user = 'admin' ";
static const char* FilterUser = "user = '%1' "; // QSqlTableModel中filter函数使用
static const char* FilterFlag = "flag = '%1' "; // rollback函数使用
static const char* CreateTable = "create table if not exists %1(%2) ";
static const char* SelectXFromTable = "select * from %1 ";
static const char* SelectXFromTableWhere = "select * from %1 where %2 ";
static const char* SelectXFromTableWhereBinary = "select * from %1 where binary %2 "; // 大小写敏感mysql特有,sqlite本身就是大小写敏感无需设置
static const char* SelectTableRows = "select count(*) from %1 "; // 查询行数
static const char* SelectXFromTableOrderById = "select * from %1 order by id"; // id保证都i有
static const char* SelectFieldFromTable = "select %1 from %2";
static const char* SelectDoubleFieldsFromTable = "select %1,%2 from %3";
static const char* SelectFieldFromTableWhere = "select %1 from %2 where %3";
static const char* DropTable = "drop table %1";
static const char* FilterTable = "select * from %1 where %2 like '%%3%';";
static const char* FilterTableByTime = "select * from %1 where datetime between '%2' and '%3';"; // 注意时间必须是2023/01/02
static const char* CreateEmptyTableMySql = "create table if not exists %1( "
                                           "id integer primary key auto_increment not null unique, "
                                           "datetime datetime not null default '1899/12/30 00:00:00', "
                                           "user varchar(100) not null default '' );"; // 空表
static const char* CreateEmptyTableSqlite = "create table if not exists %1( "
                                            "id integer primary key autoincrement not null unique, "
                                            "datetime datetime not null default '1899/12/30 00:00:00', "
                                            "user varchar(100) not null default '' );"; // 空表
//insert into experconfig (channel,view,wellsize)
//values ('phase','2','96'),('red','4','24'),('green','8','384')
// 通用型,%3可以是多个()括起来的值,%2可以是ExperGeneralConfigFields每个表的每个字段逗号隔开
static const char* InsertRecord = "insert into %1 (%2) values %3";
static const char* RemoveRecord = "delete from %1 where %2";
static const char* UpdateRecord = "update %1 set %2 where %3";

// （3）数据库特有命令
// （3.1）查询指定表是否存在
static const char*  InformationSchema = "information_schema";// 仅限于mysql
static const char*  ShowDataBaseLike = "show database like '%1' ";// 仅限于mysql
static const char*  CreateDataBase = "create database %1";// 仅限于mysql
static const char*  ShowTablesLike = "show tables like '%1' "; // 仅限于mysql
static const char*  SelectTableExistedFromInformationSchema =
        "select table_name from information_schema.tables where table_schema= '%1' and table_name = '%2' ";// 仅限于mysql
static const char* SelectTableExistedFromSqliteMaster = "select name from sqlite_master where name = '%1' ";// 仅限于sqlite
// （3.2）查询表格列数
static const char* SelectTableColumnsFromInformationSchema =
        "select count(column_name) from information_schema.columns where  table_name = '%1' ";// 仅限于mysql
// （3.2）创建表
//  （3.2.1）登录信息表
static const char * CreateLoginInfoConfigTable =
        "create table if not exists logininfo ("
        "user varchar(100) not null default '', "
        "pwd varchar(100) not null default '',"
        "last_login datetime not null default '1899/12/30 00:00:00' ); ";

// （3.2.2）实验流程通用配置表 时间和ID一定存在
static const char * CreateExperGeneralConfigTableInit =
        "user varchar(100) not null default '', "
        "flag varchar(100) not null default '',"
        "channel varchar(100) not null default '', "
        "brand varchar(100) not null default '', "
        "imageformat varchar(100) not null default '', "
        "videoformat varchar(100) not null default '', "
        "exper_description varchar(5000) not null default '', "
        "exper_person varchar(100) not null default '', "
        "exper_type varchar(100) not null default '', "
        "group_names varchar(100) not null default '', "
        "objective varchar(100) not null default '', "
        "count varchar(100) not null default '', "
        "duration varchar(100) not null default '', "
        "end_time varchar(100) not null default '', "
        "is_schedule varchar(100) not null default '', "
        "start_time varchar(100) not null default '', "
        "total varchar(100) not null default '', "
        "view varchar(20) not null default '', "
        "wellsize varchar(20) not null default '' ";

static const char * CreateGroupPointTableMysql =
        "create table if not exists grouppoint ("
        "id integer primary key auto_increment not null unique, "
        "user varchar(100) not null default '', "
        "flag varchar(100) not null default '',"
        "group_name varchar(100) not null default '', "
        "point_pos varchar(100) not null default '', "
        "point_name varchar(100) not null default '' ); ";

static const char * CreateGroupPointTableSqlite =
        "create table if not exists grouppoint ("
        "id integer primary key autoincrement not null unique, "
        "user varchar(100) not null default '', "
        "flag varchar(100) not null default '',"
        "group_name varchar(100) not null default '', "
        "point_pos varchar(100) not null default '', "
        "point_name varchar(100) not null default '' ); ";

static const char* CreateExperGeneralConfigTableMysql = "create table if not exists experconfig ( "
                                                        "id integer primary key auto_increment not null unique, "
                                                        "datetime datetime not null default '1899/12/30 00:00:00',"// now(),不支持.zzz
                                                        "%1 );";
static const char* CreateExperGeneralConfigTableSqlite = "create table if not exists experconfig ( "
                                                         "id integer primary key autoincrement not null unique, "
                                                         "datetime datetime not null default '1899/12/30 00:00:00',"
                                                         "%1 );";

// （3.3）数据源是否存在
static const char* ShowDataSourceLike = "show databases like %1";// 仅限于mysql
static const char* SelectDataSourceExistedFromInformationSchema=
        "select count(1) from information_schema.schemata where schema_name = '%1' ";// 仅限于mysql
// （3.4）创建数据源
static const char*  CreateDataSource = "create database if not exists %1 character set 'utf8mb4'";// 仅限于mysql
// （3.5）删除数据源
static const char*  DropDataSource = "drop database %1";// 仅限于mysql
// （3.6）列出数据源
static const char* CurrentDataSourceMysql = "select database() ";
static const char* CurrentDataSourceSqlite = "PRAGMA database_list";
// （3.7）主键重排序
static const char* PrimaryKeyReorderMysql =
        "alter table %1 drop id;"
        "alter table %1 add id integer not null first;"
        "alter table %1 modify column id integer not null auto_increment, add primary key(id);";
static const char* PrimaryKeyReorderSqlite =
        "update %1 set id=id-1 where id > %2;"; // 删除记录时要拿到删除的行,不过这个方法可能会执行错误
// （3.8）mysql的数据源路径
static const char* ShowDataDir =
        "show global variables like \"%datadir%\";"; // 会返回2列,1个是变量名称一个是值
static const char* SelectDataDir =
        "select @@datadir;"; // 直接返回路径，更快捷
// （3.9）mysql查询创建时间和上次更新时间
static const char*  UpdateTime =
        "select update_time from information_schema.tables where table_schema = '%1' and  table_name = '%2'; ";
static const char*  CreateTime =
        "select create_time from information_schema.tables where table_schema = '%1' and  table_name = '%2'; ";

#endif //EOSI_SQLSTATEMENTS_H
