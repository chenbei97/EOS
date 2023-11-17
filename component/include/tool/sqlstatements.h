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
#include "constant.h"
#include "fields.h"

// (1) 业务的表信息和特定快捷命令
// (1.1) 登陆表
static const QFieldList LoginTableFieldList = {
        UserField,PwdField,LastLoginField
};
static const char* SelectUserAndPwd = "select user,pwd from logininfo";
static const char* FilterAdmin = "user = 'admin' ";
static const char * CreateLoginTable =
        "create table if not exists logininfo ("
        "user varchar(100) not null default '', "
        "pwd varchar(100) not null default '',"
        "last_login datetime not null default '1899/12/30 00:00:00' ); ";
// (1.2) 实验表
static const QFieldList ExperTableFieldList = {
        IDField,DateTimeField,UserField,FlagField,
        WellInfoField,
        ObjectiveInfoField,
        ChannelInfoField,
        FocusInfoField,
        ExperInfoField,
        OtherInfoField,
        GroupInfoField,
};
#define ExperTableFieldIndex(field) (ExperTableFieldList.indexOf(field))
static const char * CreateExperTable_Common = // 公共部分,mysql/sqlite共用
        "datetime datetime not null default '1899/12/30 00:00:00',"// now(),不支持.zzz
        "flag varchar(100) not null default '', "
        "user varchar(100) not null default '', "
        "wellinfo varchar(100) not null default '', "
        "objectiveinfo varchar(100) not null default '', "
        "channelinfo varchar(100) not null default '', "
        "focusinfo varchar(100) not null default '', "
        "experinfo varchar(100) not null default '', "
        "otherinfo varchar(100) not null default '', "
        "groupinfo varchar(500) not null default '', "
        "xxx varchar(20) not null default '' ";
static const char* CreateExperTableMysql = "create table if not exists experinfo ( "
                                           "id integer primary key auto_increment not null unique, "
                                           "%1 );";
static const char* CreateExperTableSqlite = "create table if not exists experinfo ( "
                                            "id integer primary key autoincrement not null unique, "
                                            "%1 );";

struct ExperTableField {
    const QString id = IDField;
    const QString datetime = DateTimeField;
    const QString user = UserField;
    const QString flag = FlagField;

    const QString manufacturer = ManufacturerField;
    const QString brand = BrandField;
    const QString wellsize = WellsizeField;

    const QString camera_location = CameraLocationField;
    const QString objective = ObjectiveField;

    const QString br = BRField;
    const QString ph = PHField;
    const QString gfp = RFPField;
    const QString rfp = GFPField;
    const QString dapi = DAPIField;

    const QString focus = FocusField;
    const QString focus_step = FocusStepField;

    const QString zstack = ZStackField;
    const QString stitch = StitchField;

    const QString is_schedule = IsScheduleField;
    const QString total_time = TotalTimeField;
    const QString duration_time = DurationTimeField;
    const QString start_time = StartTimeField;

    const QString channel = ChannelField;


    const QString wellinfo = WellInfoField;
    const QString objectiveinfo = ObjectiveInfoField;
    const QString channelinfo = ChannelInfoField;
    const QString focusinfo = FocusInfoField;
    const QString experinfo = ExperInfoField;
    const QString otherinfo = OtherInfoField;
    const QString groupinfo = GroupInfoField;
};
static const ExperTableField ExperTableFieldName;
// (1.3) 空表
static const char* CreateEmptyTableMySql = "create table if not exists %1( "
                                           "id integer primary key auto_increment not null unique, "
                                           "datetime datetime not null default '1899/12/30 00:00:00', "
                                           "flag varchar(100) not null default '', "
                                           "user varchar(100) not null default '' );"; // 空表
static const char* CreateEmptyTableSqlite = "create table if not exists %1( "
                                            "id integer primary key autoincrement not null unique, "
                                            "datetime datetime not null default '1899/12/30 00:00:00', "
                                            "flag varchar(100) not null default '', "
                                            "user varchar(100) not null default '' );"; // 空表

// （2）通用命令
static const char* CreateTable = "create table if not exists %1(%2) ";
static const char* DropTable = "drop table %1";
static const char* InsertRecord = "insert into %1 (%2) values %3";
// insert into table (a,b,c) values ('a','2','6'),('b','4','4'),('c','c','8')
static const char* RemoveRecord = "delete from %1 where %2";
static const char* UpdateRecord = "update %1 set %2 where %3";

static const char* SelectXFromTable = "select * from %1 ";
static const char* SelectXFromTableWhere = "select * from %1 where %2 ";
static const char* SelectXFromTableWhereLike = "select * from %1 where %2 like '%%3%';";
static const char* SelectXFromTableWhereBinary = "select * from %1 where binary %2 "; // 大小写敏感mysql特有,sqlite本身就是大小写敏感无需设置
static const char* SelectFieldFromTable = "select %1 from %2";
static const char* SelectDoubleFieldsFromTable = "select %1,%2 from %3";
static const char* SelectFieldFromTableWhere = "select %1 from %2 where %3";
static const char* SelectTableRows = "select count(*) from %1 "; // 查询行数

static const char* SelectXFromTableWhereDateTime = "select * from %1 where datetime between '%2' and '%3';"; // 注意时间必须是2023/01/02
static const char* SelectXFromTableOrderById = "select * from %1 order by id"; // id保证都i有
static const char* FilterID = "id = '%1' "; // 查询指定行是否
static const char* FilterUser = "user = '%1' "; // QSqlTableModel中filter函数使用
static const char* FilterFlag = "flag = '%1' "; // rollback函数使用

// (3) mysql支持的命令
static const char*  InformationSchema = "information_schema";
static const char*  ShowDataBaseLike = "show database like '%1' ";
static const char*  ShowDataBasesLike = "show databases like %1";
static const char*  CreateDataBase = "create database %1";
static const char*  ShowTablesLike = "show tables like '%1' ";
static const char*  CreateDataBaseUtf8 = "create database if not exists %1 character set 'utf8mb4'";
static const char*  DropDataBase = "drop database %1";
static const char*  SelectTableFromInformationSchemaWhere =
        "select table_name from information_schema.tables where table_schema= '%1' and table_name = '%2' ";
static const char* SelectTableColumnsFromInformationSchema =
        "select count(column_name) from information_schema.columns where  table_name = '%1' ";// 查询表格行数
static const char* SelectCountFromInformationSchemaWhere=
        "select count(1) from information_schema.schemata where schema_name = '%1' ";
static const char* SelectDataBaseMysql = "select database() ";
static const char* PrimaryKeyReorderMysql =
        "alter table %1 drop id;"
        "alter table %1 add id integer not null first;"
        "alter table %1 modify column id integer not null auto_increment, add primary key(id);";//主键重排序
static const char* ShowDataDir = "show global variables like \"%datadir%\";"; // 数据源存储路径会返回2列,1个是变量名称一个是值
static const char* SelectDataDir = "select @@datadir;"; // 直接返回数据库存储路径，更快捷
static const char*  SelectUpdateTime = // 查询创建时间和上次更新时间
        "select update_time from information_schema.tables where table_schema = '%1' and  table_name = '%2'; ";
static const char*  SelectCreateTime =
        "select create_time from information_schema.tables where table_schema = '%1' and  table_name = '%2'; ";

// (4) sqlite支持的命令
static const char* SelectTableFromSqliteMasterWhere = "select name from sqlite_master where name = '%1' ";
static const char* SelectDataBaseSqlite = "PRAGMA database_list";
static const char* PrimaryKeyReorderSqlite =
        "update %1 set id=id-1 where id > %2;"; // 主键重排序,删除记录时要拿到删除的行,不过这个方法可能会执行错误

#endif //EOSI_SQLSTATEMENTS_H
