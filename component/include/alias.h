/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 13:27:27
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-08 13:08:41
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
typedef QVector<QVector<QPoint>> QPoint2DVector;
typedef const QVector<QVector<QPoint>>& QCPoint2DVector;
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

typedef QPair<QRectF,QImage> QPairImage;
typedef QVector<QPair<QRectF,QImage>> QPairImageVector;

struct ViewPoint {
    QString x;
    QString y;

    ViewPoint(){}
    ViewPoint(const ViewPoint& s) {
        x = s.x;
        y = s.y;
    }
    ViewPoint(double x, double y) {
        this->x = QString::number(x);
        this->y = QString::number(y);
    }
    ViewPoint(int x, int y) {
        this->x = QString::number(x);
        this->y = QString::number(y);
    }
    friend QDebug operator<<(QDebug debug, const ViewPoint& s) {
        debug<<QString("(%1,%2)").arg(s.x).arg(s.y);
        return debug;
    }
};
Q_DECLARE_METATYPE(ViewPoint);
typedef QVector<ViewPoint> ViewPointVector;
struct ViewRectF {
    QRectF rect;
    bool flag;
    ViewRectF(){}
    ViewRectF(const QRectF& rect,bool flag) {
        this->rect = rect;
        this->flag = flag;
    }
    friend QDebug operator<<(QDebug debug, const ViewRectF& s) {
        debug<<"rect = "<<s.rect<<" flag = "<<s.flag;
        return debug;
    }
};
Q_DECLARE_METATYPE(ViewRectF);
typedef QVector<ViewRectF> ViewRectFVector;

typedef QQueue<QByteArray> QByteQueue;
typedef QPair<QString,QByteArray> QRequestMsg;
typedef QQueue<QRequestMsg> QRequestQueue;
typedef const QVariantMap& QCVariantMap;
typedef QVariant(*TcpParseFuncPointer)(QCVariantMap m);
typedef QByteArray(*TcpAssembleFuncPointer)(QCVariantMap m);

struct ValueRangeInt{
    int min;
    int max;
    int def;
    friend QDebug operator<<(QDebug debug, const ValueRangeInt& s) {
        debug <<"min = "<<s.min<<" ";
        debug <<"max = "<<s.max<<" ";
        debug <<"def = "<<s.def;
        return debug;
    }
};

struct ValueRangeUShort{
    ushort min;
    ushort max;
    ushort def;
    friend QDebug operator<<(QDebug debug, const ValueRangeUShort& s) {
        debug <<"min = "<<s.min<<" ";
        debug <<"max = "<<s.max<<" ";
        debug <<"def = "<<s.def;
        return debug;
    }
};

struct ValueRangeUnSigned{
    unsigned min;
    unsigned max;
    unsigned def;
    friend QDebug operator<<(QDebug debug, const ValueRangeUnSigned& s) {
        debug <<"min = "<<s.min<<" ";
        debug <<"max = "<<s.max<<" ";
        debug <<"def = "<<s.def;
        return debug;
    }
};

struct HoleInfo {
    // 分组窗口去更新的信息
    QString type; // 本孔分配的实验类型
    QString medicine; // 本孔分配的药物
    QString dose; // 本孔分配的剂量
    QString doseunit; // 本孔分配的剂量单位
    QString group; // 本孔分配的组名
    QColor color; // 本孔分配的组颜色

    // 拖拽区域控制的
    bool isselected; // 本孔是否被被选中

    // 初始化就赋值
    QPoint coordinate; // 本孔坐标

    // 视野窗口传递来的信息
    QRectFVector viewrects; // 本孔分配的视野区域信息

    ViewPointVector uipoints_v1; // 老接口
    ViewPointVector viewpoints_v1;
    QPointFVector uipoints; // 绘图使用(够用就行)
    QPointFVector viewpoints; // 本孔要扫描的视野坐标(电机坐标更精细)

    int viewsize; // 本孔分配的视野尺寸

    // 每次打开分组窗口就会重新计算分过的组名和所有选中的孔坐标
    QSet<QString> allgroup; // 孔板所有分过的组别信息
    QPoint2DVector allcoordinate; // 孔板所有选过的孔坐标

    friend QDebug operator<<(QDebug debug, const HoleInfo& s) {
        debug << "(group,coordinate,color,viewsize,rects,viewpoints,uipoints,"
                 "isselect,allgroup,allcoordinate,"
                 "type,medicine,dose,unit)=[";
        debug <<s.group<<",";
        debug <<s.coordinate<<",";
        debug <<s.color<<",";
        debug <<s.viewsize<<",";
        debug <<s.viewrects<<",";
        debug <<s.viewpoints<<",";
        debug <<s.uipoints<<",";

        debug <<s.isselected<<",";
        debug <<s.allgroup<<",";
        debug <<s.allcoordinate<<",";

        debug <<s.type<<",";
        debug <<s.medicine<<",";
        debug <<s.dose<<",";
        debug <<s.doseunit<<"]";
        return debug;
    }
};

Q_DECLARE_METATYPE(HoleInfo);
typedef QVector<HoleInfo> QHoleInfoVector;
typedef const QVector<HoleInfo>& QCHoleInfoVector;
typedef QVector<QVector<HoleInfo>> QHoleInfo2DVector;
typedef const QVector<QVector<HoleInfo>>& QCHoleInfo2DVector;

typedef QMap<QString,QString> LocationObjectiveInfo;
typedef QMap<QString,QString> TimeInfo;
typedef QMap<QString,QString> LensInfo;
typedef QMap<QString,QString> ZStackInfo;
typedef QMap<QString,QString> WellInfo;
typedef QMap<QString,QString> FocusInfo;
typedef QMap<QString,QString> ObjectiveInfo;
typedef QMap<QString,QString> ChannelInfo;
typedef QMap<QString,QString> CameraInfo;
typedef QMap<QString,QString> RecordInfo;
typedef QMap<QString,QString> ViewModeInfo;
typedef QMap<QString,CameraInfo> MultiCameraInfo;
typedef QMap<QString,QImage> ChannelImageInfo;
typedef QVariantMap PreviewToolInfo;
typedef QVariantMap ExperToolInfo;
typedef QVariantMap WellPatternInfo;
typedef QVariantMap WellGroupInfo;
typedef QVariantMap WellHoleInfo;
typedef QVariantMap SlidePatternInfo;
typedef QVariantMap PreviewInfo;
typedef QVariantMap ViewInfo;
typedef QVariantMap IniVariable;
#endif //EOSI_ALIAS_H
