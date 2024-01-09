/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 16:00:56
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-16 16:24:21
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
#include "qstorageinfo.h"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/xfeatures2d.hpp"
#define LOG (qDebug()<<"["<<QTime::currentTime().toString("h:mm:ss:zzz")<<__FUNCTION__<<"] ")

static cv::Mat qimageToMat(const QImage& image)
{
    //LOG<<"image format:"<<image.format();
    cv::Mat mat;
    switch (image.format())
    {
        case QImage::Format_ARGB32:
        case QImage::Format_RGB32:
        case QImage::Format_ARGB32_Premultiplied:
            mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
            break;
        case QImage::Format_RGB888: // 图像是RGB888格式需要把mat先转为rgb,默认是bgr
            mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
            cv::cvtColor(mat, mat, cv::COLOR_BGR2RGB);
            break;
        case QImage::Format_Indexed8:
            mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
            break;
    }
    //LOG<<"mat type:"<<mat.type();
    return mat;
}

static QImage matToqimage(const cv::Mat& mat)
{
    //LOG<<"mat type:"<<mat.type();
    if (mat.type() == CV_8UC1)
    {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        image.setColorCount(256);
        for (int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }
        uchar *pSrc = mat.data;
        for (int row = 0; row < mat.rows; row++)
        {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        //LOG<<"image format:"<<image.format();
        return image;
    } else if (mat.type() == CV_8UC3) {
        auto img = QImage((const unsigned char*)mat.data, mat.cols, mat.rows,
                   mat.step,QImage::Format_RGB888);
        img = img.rgbSwapped(); // 交换红蓝2个通道,否则mat原来是BGR
        //<<"image format:"<<img.format();
        return img;
    } else if (mat.type() == CV_8UC4) {
        const uchar *pSrc = (const uchar*)mat.data;
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        //LOG<<"image format:"<<image.format();
        return image.copy();
    } else {
        //LOG<<"Error: Mat could not be converted to QImage";
        return QImage();
    }
}

static QImage getChannelImage(const QImage& img,Qt::GlobalColor color)
{
    auto mat = qimageToMat(img); // 转为mat

    std::vector<cv::Mat> channels;
    cv::split(mat,channels); // 把mat的3个通道分出来是BGR
    std::vector<cv::Mat> new_channels; // 新的通道图像,指定通道为0
    cv::Mat res;
    auto image = QImage();

    if (color == Qt::red) {
        new_channels = {
                        cv::Mat::zeros(mat.size(),CV_8UC1),
                        cv::Mat::zeros(mat.size(),CV_8UC1),
                        channels[2],
                        };
        cv::merge(new_channels,res);
        image = matToqimage(res);
    } else if (color == Qt::green) {
        new_channels = {cv::Mat::zeros(mat.size(),CV_8UC1),
                        channels[1],
                        cv::Mat::zeros(mat.size(),CV_8UC1)};
        cv::merge(new_channels,res);
        image = matToqimage(res);
    } else if (color == Qt::blue){
        new_channels = {channels[0],
                cv::Mat::zeros(mat.size(),CV_8UC1),
                cv::Mat::zeros(mat.size(),CV_8UC1),
                };
        cv::merge(new_channels,res);
        image = matToqimage(res);
    }

    return image;
}

/*拆分特定字符串转为坐标/区域函数*/
static QPointF convertToPointF(const QString& text)
{ // (0.835,0.33),(0,1)这样的字符串
    Q_ASSERT(text.count(",") == 1);
    Q_ASSERT(text.count("(") == 1);
    Q_ASSERT(text.count(")") == 1);

    auto tmp = text;
    tmp.remove(0,1);
    tmp.chop(1);

    auto x = tmp.section(",",0,0,QString::SectionSkipEmpty).toDouble();
    auto y = tmp.section(",",1,1,QString::SectionSkipEmpty).toDouble();

    return {x,y};
}

static Dimension2D convertToDimension2D(const QString& text)
{ // (0.835,0.33),(0,1)这样的字符串
    Q_ASSERT(text.count(",") == 1);
    Q_ASSERT(text.count("(") == 1);
    Q_ASSERT(text.count(")") == 1);

    auto tmp = text;
    tmp.remove(0,1);
    tmp.chop(1);

    auto rows = tmp.section(",",0,0,QString::SectionSkipEmpty).toDouble();
    auto cols = tmp.section(",",1,1,QString::SectionSkipEmpty).toDouble();

    return Dimension2D(rows,cols);
}

static QRectF convertToRectF(const QString& text)
{ // (0.71,0.205,0.1725,0.35)
    Q_ASSERT(text.count(",") == 3);
    Q_ASSERT(text.count("(") == 1);
    Q_ASSERT(text.count(")") == 1);

    auto tmp = text;
    tmp.remove(0,1);
    tmp.chop(1);

    auto values = tmp.split(",",QString::SkipEmptyParts);
    Q_ASSERT(values.count() == 4);

    return {values[0].toDouble(),values[1].toDouble(),
            values[2].toDouble(),values[3].toDouble()};
}

static QSet<QString> convertToSet(const QString& text)
{ // a,b,c
    QSet<QString> set;
    auto tmp = text;

    auto values = tmp.split(",",QString::SkipEmptyParts);
    for(auto val: values)
        set.insert(val);
    return set;
}

static QPointFVector convertToPointFVector(const QString& text)
{ // "(0.3075,0.4075),(0.27675,0.59175),(0.50175,0.36675),(0.50175,0.59175)"
    Q_ASSERT(text.count(",") % 2 == 1); //一定是奇数个
    QPointFVector points;

    int count = 0;
    for(int i = 0; i < text.count(); ++i) {
        if(text[i] == ",") {
            count++;
            if (count % 2 == 1) { // 奇数位置的","是2个数字之间的
                int s1 = i,s2 = i;
                while (text[s1] != "(") {
                    s1--;
                } //匹配时的s1就是"("的索引
                while (text[s2] != ")") {
                    s2++;
                }//匹配时的s2就是")"的索引
                auto x = text.midRef(s1+1,i-s1-1).toDouble();
                auto y = text.midRef(i+1,s2-i-1).toDouble();
                points.append(QPointF(x,y));
            }
        }
    }

    return points;
}

static QPointVector convertToPointVector(const QString& text)
{ // "(1,2),(4,5),(7,8),(3,9)"
    Q_ASSERT(text.count(",") % 2 == 1); //一定是奇数个
    QPointVector points;

    int count = 0;
    for(int i = 0; i < text.count(); ++i) {
        if(text[i] == ",") {
            count++;
            if (count % 2 == 1) { // 奇数位置的","是2个数字之间的
                int s1 = i,s2 = i;
                while (text[s1] != "(") {
                    s1--;
                } //匹配时的s1就是"("的索引
                while (text[s2] != ")") {
                    s2++;
                }//匹配时的s2就是")"的索引
                auto x = text.midRef(s1+1,i-s1-1).toInt();
                auto y = text.midRef(i+1,s2-i-1).toInt();
                points.append(QPoint(x,y));
            }
        }
    }

    return points;
}

static QPointF2DVector convertTo2DPointFVector(const QString& text)
{ // "(0,2),(1,2)|(0,0),(0,1),(1,0),(1,1)"
    Q_ASSERT(text.contains("|"));
    QPointF2DVector points;

    auto section = text.split("|",QString::SkipEmptyParts);
    for(auto sec: section)
        points.append(convertToPointFVector(sec));

    return points;
}

static QPoint2DVector convertTo2DPointVector(const QString& text)
{ // "(0,2),(1,2)|(0,0),(0,1),(1,0),(1,1)"
    Q_ASSERT(text.contains("|"));
    QPoint2DVector points;

    auto section = text.split("|",QString::SkipEmptyParts);
    for(auto sec: section)
        points.append(convertToPointVector(sec));

    return points;
}

/*精度转换函数*/
static QString convertPrecision(const QString& s,int precision = 6)
{
    int idx = s.indexOf(".");
    int count = s.mid(idx+1).count();
    QString res = s;
    if (count > precision) {
        res = s.left(idx+1+precision);
    } else if (count < precision) {
        auto bit = precision-count;
        while (bit--) {
            res.append("0");
        }
    }
    return res;
}

static QString convertPrecision(double n,int precision = 6)
{
    return convertPrecision(QString::number(n),precision);
}

static QString convertPrecision(int n,int precision = 6)
{
    return convertPrecision(QString::number(n)+".",precision);
}

/*获取电脑所有的盘符*/
static QStringList systemDrivers()
{ // ("C:/", "D:/")
    QStringList list;
    auto drivers = QDir::drives();
    foreach(auto info,drivers) {
        list.append(info.path());
    }
    return list;
}

/*获取电脑指定盘符的总空间或者剩余空间,getFree表示获取剩余空间*/
static double getDiskSpace(QString path, bool getFree = true) {
    double res = 0.0;
    double MB = 1024.0 * 1024.0;
    QList<QStorageInfo> storageInfoList = QStorageInfo::mountedVolumes();
            foreach (QStorageInfo storageInfo, storageInfoList) {
            if (path.startsWith(storageInfo.rootPath())) {
                getFree? res = (double )storageInfo.bytesAvailable() / MB:
                        res = (double)storageInfo.bytesTotal() / MB;
                break;
            }
        }
    return res;
}

/*将指定数字转为0x前缀的16进制字符串,且0x后边有*指定的位数*/
static QString convertToHex(quint32 number,int bits = 8) {
    // 例如指定8bits,1000,000和1000分别转换为"0x000f4240" "0x000003e8"
    QString hex = QString::number(number, 16);
    while (hex.length() < bits) {
        hex.prepend("0");
    }
    return "0x" + hex;
}


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

/*对话框*/
#include "waitmessagebox.h"
#define DefaultWindowSize QSize(400,300)
//https://cloud.tencent.com/developer/ask/sof/427509
static WaitMessageBox* getWaitMessageBox(const QString& text,bool deleteOnClose = true,QWidget*parent = nullptr)
{ // 等待对话框
    //QMessageBox * dlg = new QMessageBox(parent);
    WaitMessageBox * dlg = new WaitMessageBox(parent);
    //dlg->setMinimumSize(DefaultWindowSize);
    dlg->setWaitText(text);
    if (deleteOnClose)
        dlg->setAttribute(Qt::WA_DeleteOnClose);
    return dlg;
}

#include "label.h"
static QDialog* getWaitDialog(bool deleteOnClose = true,QWidget*parent = nullptr)
{ // 等待对话框
    QDialog * dlg = new QDialog(parent);
//    auto label = new Label;
//    label->setText(WaitDialogMsg);
//    dlg->setWindowModality(Qt::WindowModal);
//    if (deleteOnClose)
//        dlg->setAttribute(Qt::WA_DeleteOnClose);
//    dlg->setStandardButtons(QMessageBox::NoButton);
//    dlg->setFixedSize(DefaultWindowSize);
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
    if (pathExisted(path)) return true;

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
