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

typedef QVector<QColor> QColorVector;
typedef const QVector<QColor>& QCColorVector;
typedef QVector<QPoint> QPointVector;
typedef const QVector<QPoint>& QCPointVector;
typedef QVector<QPointF> QPointFVector;
typedef const QVector<QPointF>& QCPointFVector;
typedef QVector<QString> QStringVector;
typedef const QVector<QString>& QCStringVector;
typedef QVector<QString> QFieldQVector;
typedef const QVector<QString>& QCFieldVector;
typedef QVector<QString> QValueVector;
typedef const QVector<QString>& QCValueVector;
typedef QVector<bool> QBoolVector;

typedef QList<QColor> QColorList;
typedef const QList<QColor>& QCColorList;
typedef QList<QPoint> QPointList;
typedef const QList<QPoint>& QCPointList;
typedef QList<QPointF> QPointFList;
typedef const QList<QPointF>& QCPointFList;
typedef QList<QString> QFieldList;
typedef const QList<QString>& QCFieldList;
typedef QList<QString> QValueList;
typedef const QList<QString>& QCValueList;
typedef const QList<QString>& QCStringList;
typedef QList<bool> QBoolList;

typedef QVector<QVector<QString>> QValue2DVector;
typedef  const QVector<QVector<QString>>& QCValue2DVector;

#endif //EOSI_ALIAS_H
