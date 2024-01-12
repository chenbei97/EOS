/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-01 15:08:16
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-13 15:54:24
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
#include "qtextcodec.h"

enum class SQLType {Mysql,Sqlite};
Q_ENUMS(SQLType)
enum class MirrorType {NoMirror,HorMirror,VerMirror,AllMirror};
Q_ENUMS(MirrorType)

static const int KB = 1024;
static const int MB = 1024*1024;
static const int GB = 1024 * 1024 * 1024;
static const int DefaultPrecisionBit = 6;
static const int DefaultDrawPointWidth = 8;
static const int DefaultPainterPenWidth = 2;
static const int DefaultDispersedMaskSize = 100;
static const int DefaultImageQuality = 100;
static const int DefaultThreadSleepMs = 100;
#define PointToIDCoefficient 1000 // kx1+y1=kx2+y2=> k=(y2-y1)/(x1-x2),k取什么值?找不到这样的2个坐标能等于k,由于wellsize最大384,实际上k>384就能保证id唯一了
#define LOG (qDebug()<<"["<<QTime::currentTime().toString("h:mm:ss:zzz")<<__FUNCTION__<<"] ")
#define SqlExecFailedLOG (qDebug()<<"[" \
    <<QTime::currentTime().toString("h:mm:ss:zzz")<<__FUNCTION__<<"] sql exec failed! error is ")
#define UserScreenWidth (QApplication::desktop()->screenGeometry().width())
#define UserScreenHeight (QApplication::desktop()->screenGeometry().height())
#define WarningChinese tr("Warning")
#define InformationChinese tr("Information")
#define CriticalChinese tr("Critical")
#define DefaultDateTimeFormat "yyyy-MM-dd hh:mm"
#define DefaultImageDateTimeFormat "yyyy-MM-dd-hh-mm-ss"
#define DefaultImageSaveDateTimeFormat "yyyy_MM_dd_hh_mm_ss"
#define TableFlagDateTimeFormat "yyMMddhhmmss"
#define CURRENT_PATH (QDir::currentPath())
#define CURRENT_THREAD (QThread::currentThread())
#define DefaultFontSize 10
#define DefaultFontFamily ("Times New Roman")
#define DefaultWindowSize QSize(400,300)
#define DefaultPainterFont QFont(DefaultFontFamily,DefaultFontSize,QFont::Bold)
#define DefaultPainterMetric (QFontMetrics(DefaultPainterFont))
#define DefaultPainterPixelHeight (DefaultPainterMetric.height()) //指定font下文字像素的高度
#define DefaultColorAlpha 125
#define SocketPort 3000
#define LocalHost "localhost"
#define SocketWaitTime 60000
#define DefaultNativeColor QColor("#F0F0F0")
//#define UseSqlcipher true
#define TakePhotoTempPath (CURRENT_PATH+"/temp/photo/")
#define MergeChannelTempPath (CURRENT_PATH+"/temp/merge/")
#define RecordVideoDefaultTempPath (CURRENT_PATH+"/temp/video/")
#define AutoFocusTempPath (CURRENT_PATH+"/temp/autofocus/")
static const char* PairArgsField = "(%1,%2)";
static const char* ApplyField = "apply";
static const char* AllChannelsField = "allchannels";
static const char* CustomField = "custom";
static const char* PurpleEA3FF7 = "#EA3FF7";
static const char* PointModeField = "Point Mode";
static const char* AreaModeField = "Area Mode";
static const char* WholeModeField = "Whole Mode";
static const char* RollBackField = "rollback";
static const char* OriginalField = "original";
static const char* ExportField = "export";
static const char* SqliteField = "sqlite";
static const char* QSQLITEField = "QSQLITE";
static const char* SqlcipherField = "sqlcipher";
static const char* SQLITECIPHERField = "SQLITECIPHER";
static const char* MysqlField = "mysql";
static const char* QMYSQLField = "QMYSQL";
static const char* DB3Suffix = ".db3";
static const char* DBSuffix = ".db";
static const char* IBDSuffix = ".ibd";
static const char* EOSField = "eos";
static const char* EOSSuffix = ".eos";
static const char* MatchEOSSuffix = "*.eos";
static const char * UserField = "user";
static const char * PwdField = "pwd";
static const char * LastLoginField = "last_login";
static const char * IDField = "id";
static const char * DateTimeField = "datetime";
static const char * FlagField = "flag";
static const char * AutoFocusField = "auto focus";
static const char * SaveFocusField = "save focus";
static const char * RotateField = "rotate";
static const char * ResetField = "reset";
static const char * NoMirrorField = "no mirror";
static const char * HorizontalMirrorField = "horizontal mirror";
static const char * VerticalMirrorField = "vertical mirror";
static const char * AllMirrorField = "all mirror";

static const char * AdminField = "admin";
static const char * DefalutPwdField = "123456";
static const char*  SqliteDataBasePassword = "gaofenshengwu";
static const char*  MysqlDataBaseConnections = "MYSQL_OPT_CONNECT_TIMEOUT=1;";
static const char*  SqliteDataBaseConnectionsFirst =
        "QSQLITE_USE_CIPHER=sqlcipher; ""SQLCIPHER_LEGACY=1; "
        "SQLCIPHER_LEGACY_PAGE_SIZE=4096;QSQLITE_CREATE_KEY";
static const char*  SqliteDataBaseConnectionsAgain =
        "QSQLITE_USE_CIPHER=sqlcipher; ""SQLCIPHER_LEGACY=1; "
        "SQLCIPHER_LEGACY_PAGE_SIZE=4096";
static const char* FrameField = "frame";
static const char* SeparateField = "@@@";
static const char* PathField = "path";
static const char* EquipField = "equip";
static const char* EstimatedSpaceField = "estimateSpace";
static const char* OkField = "ok";
static const char* HourField = "hour";
static const char* MinuteField = "minute";
static const char* HoursFieldSuffix = " hours";
static const char* MinutesFieldSuffix = " minutes";
static const char* JPGSuffix = ".jpg";
static const char* JPGField = "jpg";
static const char* JPGSuffixFilter = "*.jpg";
static const char* PNGSuffixFilter = "*.png";
static const char* TIFSuffixFilter = "*.tif";
static const char* TIFSuffix = ".tif";
static const char* StateField = "state";
static const char* ImageField = "image";
static const char* ImageFieldUpper = "Image";
static const char* TimeStampFieldUpper = "TimeStamp";
static const char* GridSizeField = "gridsize";
static const char* NameFieldUpper = "Name";
static const char* StatusFieldUpper = "Status";
static const char* StartTimeFieldUpper = "Start Time";
static const char* CoordinateFieldUpper = "Coordinate";
static const char* InfoFieldUpper = "Info";
static const char* ObjectiveFieldUpper = "Objective";
static const char* ChannelsFieldUpper = "Channels";
static const char* CompleteFieldUpper = "Complete";
static const char* RunningFieldUpper = "Running";
static const char* FocusFieldUpper = "Focus";
static const char* StitchFieldUpper = "Stitch";
static const char* ZStackFieldUpper = "ZStack";
static const char* StopField = "stop";
static const char* ExperFinishedField = "exper_finished";
static const char* ActivateCodeField = "activate_code";
static const char* TurnOffLight = "turnoff_light";
static const char* IsCheckedField = "isChecked";
static const char* DirectionField = "direction";
static const char* DishField = "dish";
static const char* DishFieldLabel = "dish: ";
static const char* FlaskField = "flask";
static const char* FlaskFieldLabel = "flask: ";
static const char* SlideField = "slide";
static const char* SlideFieldLabel = "slide: ";
static const char* VideoRecordLabelField = "video record: ";
static const char* VideoPlayLabelField = "video play: ";
static const char* SelectVideoField = "select video";
static const char* SelectSaveDirField = "Select Save Dir";
static const char* SaveSettingToFileField = "save settings to file?";
static const char* PleaseInputFileNameField = "please input filename";
static const char* SelectSavePathField = "select save path";
static const char* StartField = "start";
static const char* PlayField = "play";
static const char* PauseField = "pause";
static const char* MergeField = "merge";
static const char* SaveCurrentChannelSettingLabelField = "save current channel settings";
static const char* ApplyHistoryConfigLabelField = "apply historical config?";
static const char* SelectConfigLabelField = "select config file";
static const char* StartExperField = "start experiment";
static const char* StopExperField = "stop experiment";
static const char* Z_StackField = "z_stack";
static const char* StitchImageField = "stitch_image";

static const char* PreviewDockHoleTitle = "Select Hole Inside View";
static const char* PreviewWellDockTitle = "Hole(%1,%2)-Group(%3)";
static const char* PreviewNoGroupWellDockTitle = "Group not set";
static const char* WellPatternField = "wellpattern";
static const char* ViewModeBoxTitle = "Group/View";
static const char* WellBoxTitle = "Well";
static const char* ChannelBoxTitle = "Channel";
static const char* RecordBoxTitle = "Record";
static const char* CameraFocusBoxTitle ="Camera/Focus";
static const char * FocusBoxTitle = "Focus";
static const char * ObjectiveBoxTitle = "Objective";
static const char * SaveBoxTitle = "Save";
static const char * TimeBoxTitle = "Experiment";
static const char * ExperimentTabTitle = "Experiment";
static const char * PreviewTabTitle = "Preview";
static const char * ZStackBoxField = "ZStack/Stitch";

static const char*  SaveViewActTitle = "Select View";
static const char*  RemoveViewActTitle = "Remove View";
static const char*  RemoveHoleActTitle = "Remove Hole";
static const char*  EnterViewActTitle = "Come into View";
static const char*  GroupHoleActTitle = "Grouping Hole";
static const char*  ApplyHoleActTitle = "Apply To Hole";
static const char*  ApplyGroupActTitle = "Apply To Group";
static const char*  ApplyAllActTitle = "Apply To All";
static const char*  QuitViewActTitle = "Quit View";
static const char*  MoveMachineActTitle = "Move Machine";

#define AppendField(s,field) (s+field)
#define AppendSeparateField(s) (s+SeparateField)
#define PrependField(s,field) (field+s)
#define PrependSeparateField(s) (SeparateField+s)

static const QStringList TimeBoxTimeUnitFields = {
        HourField,MinuteField,
};
//static const QFieldList DishFields = {
//        "dish1","dish2","dish3"
//};
//static const QFieldList FlaskFields = {
//        "flask1","flask2","flask3"
//};
//static const QFieldList SlideFields = {
//        "slide1","slide2","slide3"
//};

#endif //EOSI_CONSTANT_H
