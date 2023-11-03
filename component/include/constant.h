/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-01 15:08:16
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-01 15:09:22
 * @FilePath: \EOS\component\include\constant.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOSI_CONSTANT_H
#define EOSI_CONSTANT_H

#include "qdebug.h"
#include "qdatetime.h"
#include "qscreen.h"
#include "qapplication.h"
#include "qdesktopwidget.h"

enum class SQLType {Mysql,Sqlite};
Q_ENUMS(SQLType)

#define LOG (qDebug()<<"["<<QTime::currentTime().toString("h:mm:ss:zzz")<<__FUNCTION__<<"] ")
#define SqlExecFailedLOG (qDebug()<<"[" \
    <<QTime::currentTime().toString("h:mm:ss:zzz")<<__FUNCTION__<<"] sql exec failed! error is ")
#define UserScreenWidth (QApplication::desktop()->screenGeometry().width())
#define UserScreenHeight (QApplication::desktop()->screenGeometry().height())
#define WarningChinese tr("警告")
#define InformationChinese tr("消息")
#define CriticalChinese tr("错误")
#define DefaultDateTimeFormat "yyyy_MM_dd hh::mm"
#define CURRENT_PATH (QDir::currentPath())
#define CURRENT_THREAD (QThread::currentThread())
#define DefaultFontSize 10
#define DefaultFontFamily ("Times New Roman")
#define DefaultPainterFont QFont(DefaultFontFamily,DefaultFontSize,QFont::Bold)
#define DefaultPainterMetric (QFontMetrics(DefaultPainterFont))
#define DefaultPainterPixelHeight (DefaultPainterMetric.height()) //指定font下文字像素的高度
#define DefaultColorAlpha 125
#define SocketPort 3000
#define LocalHost "localhost"
#define SocketWaitTime 3000
//#define UseSqlcipher true

static const char* SqliteDriver = "sqlite";
static const char* QSqliteDriver = "QSLITE";
static const char* SqlcipherDriver = "sqlcipher";
static const char* QSqlcipherDriver = "SQLITECIPHER";
static const char* MysqlDriver = "mysql";
static const char* QMysqlDriver = "QMYSQL";
static const char* SqliteFileSuffix = ".db3";
static const char* ConfigFileSuffix = ".eos";
static const char* MatchConfigFileSuffix = "*.eos";
static const char * LoginInfoUserField = "user";
static const char * LoginInfoPwdField = "pwd";
static const char * LoginInfoDefaultUser = "admin";
static const char * LoginInfoDefaultPwd = "123456";
static const char*  SqliteDataBasePassword = "cb199791";
static const char*  MysqlDataBaseConnections = "MYSQL_OPT_CONNECT_TIMEOUT=1;";
static const char*  SqliteDataBaseConnectionsFirst =
        "QSQLITE_USE_CIPHER=sqlcipher; ""SQLCIPHER_LEGACY=1; "
        "SQLCIPHER_LEGACY_PAGE_SIZE=4096;QSQLITE_CREATE_KEY";
static const char*  SqliteDataBaseConnectionsAgain =
        "QSQLITE_USE_CIPHER=sqlcipher; ""SQLCIPHER_LEGACY=1; "
        "SQLCIPHER_LEGACY_PAGE_SIZE=4096";
static const char* FrameField = "frame";
static const char* SeparateField = "@@@";
static const char* HourField = "hour";
static const char* MinuteField = "minute";
static const char* HoursFieldSuffix = " hours";
static const char* MinutesFieldSuffix = " minutes";
static const char* StateField = "state";
static const char* ActivateCodeField = "activate_code";
static const char* TurnOffLight = "turnoff_light";
static const char* IsCheckedField = "isChecked";
static const char* DishField = "dish";
static const char* DishFieldLabel = "dish: ";
static const char* FlaskField = "flask";
static const char* FlaskFieldLabel = "flask: ";
static const char* SlideField = "slide";
static const char* SlideFieldLabel = "slide: ";

#define AppendSeparateField(s) (s+SeparateField)

static const QStringList TimeBoxTimeUnitFields = {
        HourField,MinuteField,
};
static const QFieldList ImageFormatFields = {
        "jpg","png","ico","bmp"
};
static const QFieldList VideoFormatFields = {
        "avi","wmv"
};
static const QFieldList DishFields = {
        "dish1","dish2","dish3"
};
static const QFieldList FlaskFields = {
        "flask1","flask2","flask3"
};
static const QFieldList SlideFields = {
        "slide1","slide2","slide3"
};

#endif //EOSI_CONSTANT_H
