/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 13:27:27
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-16 14:06:18
 * @FilePath: \EOS\component\include\alias.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_ALIAS_H
#define EOSI_ALIAS_H

#include "qvariant.h"
#include "qstring.h"
#include "qcolor.h"
#include "qpoint.h"
#include "qrect.h"
#include "qdatetime.h"
#include "qlist.h"
#include "qmap.h"
#include "qvector.h"
#include "qpair.h"
#include "qqueue.h"
#include "qset.h"
#include "qdebug.h"

#include <random>
using std::sort;
using std::random_device;
using std::mt19937;
using std::uniform_int_distribution;

typedef const QVariant& QCVariant;
typedef const QString& QCString;
typedef const QPoint& QCPoint;
typedef const QPointF& QCPointF;
typedef const QColor& QCColor;
typedef const QRect& QCRect;
typedef const QRectF& QCRectF;

typedef QVector<QColor> QColorVector;
typedef const QVector<QColor>& QCColorVector;
typedef QVector<QPoint> QPointVector;
typedef const QVector<QPoint>& QCPointVector;
typedef QVector<QPointF> QPointFVector;
typedef const QVector<QPointF>& QCPointFVector;
typedef QVector<QRect> QRectVector;
typedef const QVector<QRect>& QCRectVector;
typedef QVector<QRectF> QRectFVector;
typedef const QVector<QRectF>& QCRectFVector;
typedef QVector<QVector<QPointF>> QPointF2DVector;
typedef const QVector<QVector<QPointF>>& QCPointF2DVector;
typedef QVector<QVector<QRectF>> QRectF2DVector;
typedef const QVector<QVector<QRectF>>& QCRectF2DVector;
typedef QVector<QString> QStringVector;
typedef const QVector<QString>& QCStringVector;
typedef QVector<QString> QFieldQVector;
typedef const QVector<QString>& QCFieldVector;
typedef QVector<QString> QValueVector;
typedef const QVector<QString>& QCValueVector;
typedef QVector<bool> QBoolVector;
typedef const QVector<bool>& QCBoolVector;
typedef QVector<QVector<bool>> QBool2DVector;
typedef const QVector<QVector<bool>>& QCBool2DVector;

typedef QList<QColor> QColorList;
typedef const QList<QColor>& QCColorList;
typedef QList<QPoint> QPointList;
typedef const QList<QPoint>& QCPointList;
typedef QList<QPointF> QPointFList;
typedef QList<QRect> QRectList;
typedef const QList<QRect>& QCRectList;
typedef QList<QRectF> QRectFList;
typedef const QList<QRectF>& QCRectFList;
typedef const QList<QPointF>& QCPointFList;
typedef QList<QList<QPointF>> QPointF2DList;
typedef const QList<QList<QPointF>>& QCPointF2DList;
typedef QList<QList<QRectF>> QRectF2DList;
typedef const QList<QList<QRectF>>& QCRectF2DList;
typedef QList<QString> QFieldList;
typedef const QList<QString>& QCFieldList;
typedef QList<QString> QValueList;
typedef const QList<QString>& QCValueList;
typedef const QList<QString>& QCStringList;
typedef QList<bool> QBoolList;
typedef const QList<bool>& QCBoolList;
typedef QList<QList<bool>> QBool2DList;
typedef const QList<QList<bool>>& QCBool2DList;
typedef QStringList QFieldsList;
typedef const QFieldsList& QCFieldsList;

typedef QVector<QVector<QString>> QValue2DVector;
typedef  const QVector<QVector<QString>>& QCValue2DVector;

typedef QPair<bool,QColor> QBoolColorPair;
typedef const QPair<bool,QColor>& QCBoolColorPair;
typedef QVector<QPair<bool,QColor>> QBoolColorPairVector;
typedef const QVector<QPair<bool,QColor>>& QCBoolColorPairVector;
typedef QVector<QVector<QBoolColorPair>> QBoolColorPair2DVector;
typedef const QVector<QVector<QBoolColorPair>>& QCBoolColorPair2DVector;

typedef QQueue<QByteArray> QByteQueue;
typedef const QVariantMap& QCVariantMap;
typedef QVariant(*TcpParseFuncPointer)(QCVariantMap m);
typedef QByteArray(*TcpAssembleFuncPointer)(QCVariantMap m);

struct HoleInfo {
    // 分组窗口去更新的信息
    QString type; // 本孔分配的实验类型
    QString group; // 本孔分配的组名
    QString medicine; // 本孔分配的药物
    QString dose; // 本孔分配的剂量
    QString doseunit; // 本孔分配的剂量单位
    QColor color; // 本孔分配的组颜色

    // 拖拽区域控制的
    bool isselected; // 本孔是否被分配分组

    // 初始化就赋值
    QPoint coordinate; // 本孔坐标

    // 视野窗口传递来的信息
    QPointVector viewpoints; // 本孔分配的视野坐标
    QSize viewsize; // 本孔分配的视野尺寸

    // 每次打开分组窗口就会重新计算已有的分组数
    QSet<QString> allgroup; // 当前所有分过的组别信息

    friend QDebug operator<<(QDebug debug, const HoleInfo& s) {
        debug << "(type,group,medicine,dose,unit,allgroup,color,coordinate,viewpoints,viewsize,isselect)=[";
        debug <<s.type<<",";
        debug <<s.group<<",";
        debug <<s.medicine<<",";
        debug <<s.dose<<",";
        debug <<s.doseunit<<",";
        debug <<s.allgroup<<",";
        debug <<s.color<<",";
        debug <<s.coordinate<<",";
        debug <<s.viewpoints<<",";
        debug <<s.viewsize<<",";
        debug <<s.isselected<<"]";
        return debug;
    }
};
typedef QVector<HoleInfo> QHoleInfoVector;
typedef const QVector<HoleInfo>& QCHoleInfoVector;
typedef QVector<QVector<HoleInfo>> QHoleInfo2DVector;
typedef const QVector<QVector<HoleInfo>>& QCHoleInfo2DVector;

typedef QMap<QString,QString> TimeInfo;
typedef QMap<QString,QString> LensInfo;
typedef QMap<QString,QString> ZStackInfo;
typedef QMap<QString,QString> WellInfo;
typedef QMap<QString,QString> ObjectiveInfo;
typedef QMap<QString,QString> ChannelInfo;
typedef QMap<QString,QString> CameraInfo;
typedef QMap<QString,CameraInfo> MultiCameraInfo;
typedef QVariantMap PreviewToolInfo;
typedef QVariantMap PreviewPatternInfo;
typedef QVariantMap PreviewInfo;
typedef QMap<QString,QString> LocationObjectiveInfo;
//struct ChannelInfo {
//    QString channel;
//    bool isChecked;
//    QString expousre;
//    QString gain;
//    QString bright;
//

//};


#endif //EOSI_ALIAS_H
