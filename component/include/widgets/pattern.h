#ifndef EOSI_PATTERN_H
#define EOSI_PATTERN_H

#include "widgets.h"
#include "alias.h"
#include "groupinfo.h"

class COMPONENT_IMEXPORT Pattern : public QWidget
{
    Q_OBJECT
public:
    enum PatternState{UnInitialState,TickState};
    explicit Pattern(int rows, int cols,QWidget*parent= nullptr);
    void setPatternSize(int rows,int cols);
    void toggleState(PatternState state);

    void selectPoint(int row,int col, bool isSelected = true); // 选中某个点
    void selectPoints(QCPointVector points, bool isSelected = true); // 选中点列表的所有点
    void clearPoints();
private:
    void init();
    QSize getChildSize() const; // 每个圆都在划分的小正方形区域内,计算正方形的长度
    QRectF2DVector getChildRects() const; // 拿到所有小正方形的区域
    QPointF2DVector getBorderPoints() const;// 按行从左到右拿到所有分隔线上的点,可用于画线
    QPointFVector getRowHeaderPoints(); // 获取行表头文字的点,用于绘制文字
    QPointFVector getColHeaderPoints();// 获取列表头文字的点,用于绘制文字
    QPointF2DVector getCenterPoints() const; // 获取小正方形区域的中心用于绘制圆用
    void drawLine(QPainter&painter); // 绘制线
    void drawText(QPainter&painter); // 行列表头的文字
    void drawHighlight(QPainter&painter); // 绘制鼠标选中的高亮和已选择区域的高亮
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void initDrapPoints();
    void initSelectPoints();
    void clearMousePoint();//清除鼠标选中的圆
private:
    PatternState mState; // 当前状态,默认UnInitialState
    int mrows = 0;
    int mcols = 0;
    const int mGap = 30; // 圆之间的间隔
    const int mSpace = 10; // 到窗口边框留出的距离
    QColor mInnerCircleColor =  Qt::darkGray; // 默认填充内圆的颜色
    QColor mMouseClickColor = Qt::green; // 鼠标点击填充内圆的颜色
    bool mMouseEvent; // 是否启用鼠标事件
    QPoint mMousePos; // 鼠标点击的坐标,不是真实物理坐标
    QPoint mLastPos; // 鼠标左键点击的真实物理坐标
    QRectF mDrapRect; // 鼠标拖动生成的矩形
    QBool2DVector mDrapPoints; // 拖拽矩形内选中的点赋值true
    QHoleInfo2DVector mHoleInfo; // 每个孔的所有信息用结构体封装
    //QBoolColorPair2DVector mSelectPoints; // 被选中的所有点赋值true,color 2个信息
private:
    void select(QCColor color);
signals:
    void mouseClicked(const QPoint& point);
};

#endif //EOSI_PATTERN_H
