/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-12-05 16:49:05
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-05 16:51:02
 * @FilePath: \EOS\interface\include\preview\slidepattern.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_SLIDEPATTERN_H
#define EOS_SLIDEPATTERN_H

#include "window.h"

class INTERFACE_IMEXPORT SlidePattern : public QWidget
{
    Q_OBJECT
public:
    explicit SlidePattern(QWidget*parent= nullptr);
    void updateRect(const QRectF& rect);
    void refreshRect();
    void updateSize(int viewsize);
    bool haveSlide() const;
    QRectF slideRect() const;
    QPointFVector viewPoints(bool overlap = true) const;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
private:
    double overlapRate = 0.1;
    double gapRate = 0.1;
    int mSize = 0;
    QPointF mMousePos;
    QRectF mDrapRectF;
    QRectF mSaveRectF;
private:
    bool isValidPoint(const QPointF& point) const;
    bool isValidRect(const QRectF& rect) const;
    QRectF getValidRect() const;
    QPointF getReferencePoint() const;
    double getReferenceWidth() const;
    double getReferenceHeight() const;
    QPointF norm(const QPointF& point) const;
    QRectF norm(const QRectF& rect) const;
    QPointFVector overlap(const QPointFVector &points, double rate) const;
private:
    QAction * saveviewact;
    QAction * removeviewact;
    void onRemoveViewAct();
    void onSaveViewAct();
signals:
    void doubleClicked();
    void rectUpdated(const QRectF& rect);
    void normRectUpdated(const QRectF& rect);
    void previewEvent(const QPointF& point);
};
#endif //EOS_SLIDEPATTERN_H
