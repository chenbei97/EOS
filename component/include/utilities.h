/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 16:00:56
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-16 16:35:52
 * @FilePath: \EOS\component\include\utilities.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_UTILITIES_H
#define EOSI_UTILITIES_H

#include "alias.h"
#include "qdir.h"
#include "qstandardpaths.h"
#include "qpixmap.h"
#include "qimagereader.h"
#include "qmessagebox.h"
#include "qscreen.h"
#include "qapplication.h"
#include "qdesktopwidget.h"
#include "qmainwindow.h"
//#include "sysinfoapi.h"
//#include <windows.h>
/*获取windows内存情况*/

struct WindowsMemoryInfo
{
    QString memoryUsageRate; // 内存使用率
    QString totalPhysicalMemory;// 总物理内存
    QString freePhysicalMemory; // 可用物理内存
    QString totalNumberOfSystemSubmissions; // 系统提交总数
    QString numberOfSystemSubmissions;// 系统提交数
    QString totalVirtualMemory;// 总虚拟内存
    QString freeVirtualMemory;// 可用虚拟内存
    QString freeExtensionVirtualMemory;// 可用扩展虚拟内存
};

extern WindowsMemoryInfo getWindowsMemoryInfo();

/*获取环境列表*/
static auto getEnvironmentVariables(const QString& key = "Path")
{
    return  QString(qgetenv(key.toStdString().c_str()))
        .split(";",QString::SkipEmptyParts);
}

static auto addEnvironmentVariable(const QString& val,const QString& key = "Path")
{
    QString vals = val + ";" + qgetenv(key.toStdString().c_str());
    return qputenv(key.toStdString().c_str(), vals.toStdString().c_str());
}

/*w_chart*,char* 互转*/
static void charTowchar(const QString& src,wchar_t * dest, int size)
{
    swprintf(dest,size,L"%S",src.toStdString().c_str());
}

static void wcharTochar(wchar_t * src,char * dest, int size)
{
    //wcstombs(dest,src,size);
    sprintf(dest,"%S",src);
}

/*居中对齐窗口*/
#define UserScreenWidth (QApplication::desktop()->screenGeometry().width())
#define UserScreenHeight (QApplication::desktop()->screenGeometry().height())

static void setWindowAlignCenter(QWidget*w)
{
    auto x = (UserScreenWidth-w->width())/2;
    auto y = (UserScreenHeight-w->height())/4;
    //LOG<<UserScreenWidth<<UserScreenHeight<<w->width()<<w->height()<<x<<y;
    w->move(x,y);
}

#define WaitDialogMsg QObject::tr("请稍等,正在处理...")

/*对话框*/
//https://cloud.tencent.com/developer/ask/sof/427509
static QMessageBox* getWaitDialog(QWidget*parent = nullptr)
{ // 等待对话框
    QMessageBox * dlg = new QMessageBox(parent);
    dlg->setText(WaitDialogMsg);
    dlg->setWindowModality(Qt::WindowModal);
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->setStandardButtons(QMessageBox::NoButton);
    //dlg->setFixedSize(400,300);
    return dlg;
}

/*加密*/
// 利用toBase64加密
static QByteArray encry_base64(QCString string)
{
    return string.toLatin1().toBase64();
}

static QByteArray decry_base64(QCString string)
{
    return QByteArray::fromBase64(string.toLatin1());
}
// 自定义加密方式
static QString getAsciiChars() {
    QString s;
    for(int i = 0 ;i < 255; ++i)
        s += static_cast<QChar>(i);
    return s;
}

static QVector<int> getStringIndexs (QCString string)
{
    QVector<int> indexs;
    auto chars = getAsciiChars();
            foreach(auto s,string) {
            indexs.append(chars.indexOf(s));
        }
    return indexs;
}

static QString encry_color(QCString string)
{
    QString ret;
    auto idxs = getStringIndexs(string);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 255);
    QColor color;
            foreach(auto idx, idxs) {
            color.setRgb(dis(gen),dis(gen),idx);
            ret.append(color.name().remove("#"));
        }
    return ret;
}

static QString decry_color(QCString string)
{
    QString ret;
    auto chars = getAsciiChars();
    for(int i = 0; i < string.count(); i+=6)
    {
        auto s = string.mid(i,6);
        auto color = s.prepend("#");
        auto blue = QColor(color).blue();
        ret.append(chars.at(blue));
    }
    return ret;
}

/*加载图片*/
static QPixmap loadPixmapByImageReader(QCString imgpath, qreal scale_w, qreal scale_h)
{
    QImageReader reader;
    reader.setFileName(imgpath);
    reader.setAutoTransform(true); // reader.read()会自动进行元数据转换

    auto source_size = reader.size();
    auto target_size = source_size.scaled(scale_w,scale_h,Qt::KeepAspectRatio); // 原尺寸=>目标尺寸的保持纵横比尺寸

    reader.setScaledSize(target_size);
    auto pix = QPixmap::fromImageReader(&reader);
    return pix;
}

/*路径*/
static bool pathExisted(QCString path) noexcept
{
    QDir dir;
    if (dir.exists(path))
        return true;
    return false;
}

static QString getAppStandardDirPath() noexcept
{
    return QStandardPaths::standardLocations(QStandardPaths::DataLocation).at(0);
}

static QString getAppStandardFilePath(QCString filename) noexcept
{
    return getAppStandardDirPath()+"/"+filename;
}

static bool createPath(QCString path) noexcept
{
    QDir dir;
    dir.mkpath(path);
    return pathExisted(path);
}

static bool createAppStandardDir() noexcept
{
    return createPath(getAppStandardDirPath());
}

static bool appStandardDirExisted() noexcept
{
    return pathExisted(getAppStandardDirPath());
}

#define  CheckAndCreateStandardPath   {if (!appStandardDirExisted()) createAppStandardDir();}

/*随机颜色*/
static const QColorVector ClassicColors = { // 13x5 = 65个颜色
        "#474747","#6B6B6B","#8F8F8F","#BABABA","#E3E3E3", // 灰色
        "#400E0A","#6B1710","#942017","#C72B1E","#FF3727", // 红色
        "#422525","#693B3A","#9C5756","#D17573","#FF8F8D", // 褐色->玫瑰红
        "#2E2D0E","#545319","#817F26","#C4C13A","#EDE946", // 黄色 - 金色
        "#173233","#285859","#3E888A","#5AC5C7","#73FCFF", // 青色
        "#290916","#471027","#7A1C43","#BF2C69","#FF3B8B", // 深粉 => 淡粉 (255,59,139)
        "#1C4011","#377D22","#57BA48","#67EB40","#70FF45",  // 浅绿(32,255,177)
        "#094A33","#107D57", "#18B880","#1AD191","#20FFB1",   // 绿色
        "#2D0B30","#48114D","#741B7C", "#B82BC4","#EF38FF",// 紫色 (239,56,255)
        "#0E2445","#112E57","#1D4B8F","#2766C2","#3386FF", // 蓝色
        "#2D2E42","#454766","#686B99","#8589C4","#ADB2FF", // 靛蓝 (173,178,255)
        "#130529","#270B54","#3F1287","#5B1AC4", "#7621FF",// 深蓝偏紫
        "#000D59","#001282","#0018A8","#0021E8", "#0024FF"// 深蓝偏蓝
};

// Mersenne Twister伪随机数生成器 生成指定范围内的随机整数
static int generateRandomNumber(int min, int max) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

static QColorVector getClassicColors(int count)
{
    QColorVector colors;
    while (colors.count() < count) {
        auto idx = generateRandomNumber(0,ClassicColors.count()-1); // 这里不要越界
        auto color = ClassicColors.at(idx);
        if (!colors.contains(color))
            colors.append(color);
    }
    return colors;
}

/*点排序*/
static bool pointAscendFunc(const QPoint&p1,const QPoint&p2)
{
    if (p1.x()<p2.x()) return true;
    else if (p1.x() == p2.x()) return p1.y()<=p2.y();
    else return false;
}

static bool pointDescendFunc(const QPoint&p1,const QPoint&p2)
{
    if (p1.x()<p2.x()) return false;
    else if (p1.x() == p2.x()) return p1.y()>=p2.y();
    else return true;
}

static void sortPointAscending(QPointList& points)
{
    sort(points.begin(), points.end(), pointAscendFunc);
}

static void sortPointAscending(QPointVector& points)
{
    sort(points.begin(), points.end(), pointAscendFunc);
}

static void sortPointDescending(QPointList& points)
{
    sort(points.begin(), points.end(), pointDescendFunc);
}

static void sortPointDescending(QPointVector & points)
{
    sort(points.begin(), points.end(), pointDescendFunc);
}

#endif //EOSI_UTILITIES_H
