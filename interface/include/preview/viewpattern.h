#ifndef EOSI_VIEWPATTERN_H
#define EOSI_VIEWPATTERN_H

#include "window.h"

namespace V2 { // 2024//11/27 需求变更需要重新设计
    class INTERFACE_IMEXPORT ViewPattern: public QWidget
    {
        Q_OBJECT
    public:
        explicit ViewPattern(QWidget*parent= nullptr);
        void setViewInfo(const ViewInfo& info);
        ViewInfo viewInfo() const;
        void mousePressEvent(QMouseEvent *event) override;
        void mouseReleaseEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void paintEvent(QPaintEvent *event) override;
    private:
        int mSize = 0;
        QBool2DVector mViewMask;
        void initViewMask();
    private:
        ViewInfo mViewInfo;
        QPointF mMousePos;
        QRectF mMouseRect;
        QRectF mDrapRectF;
        QMap<int,ViewRectFVector> mViewRects;
        QMap<int,ViewRectFVector> mTmpRects;
    private:
        QAction * saveviewact;
        QAction * removeviewact;
        QAction * applyholeact;
        QAction * applygroupact;
        QAction * applyallact;
        void onRemoveViewAct();
        void onSaveViewAct();
        void onApplyHoleAct();
        void onApplyGroupAct();
        void onApplyAllAct();
        int holeID() const;
    signals:
        void previewEvent(const QPoint& point);
        void applyHoleEvent(const QVariantMap&m);
        void applyGroupEvent(const QVariantMap&m);
        void applyAllEvent(const QVariantMap&m);
    };
}

inline namespace V1 {
    class INTERFACE_IMEXPORT ViewPattern: public QWidget
    {
        Q_OBJECT
    public:
        enum DrawStrategy {NoStrategy,InnerCircleRect};
        explicit ViewPattern(QWidget*parent= nullptr);
        void setStrategy(DrawStrategy s,const QVariantMap& m = QVariantMap());// 设置策略+传递孔的信息+视野尺寸信息+应用到组使能
        ViewInfo currentViewInfo() const;// 切换brand,objective时外部拿到当前的视野孔信息,然后更新其中尺寸字段重新调用setStrategy
        void updatePatternUi();// objective更新后视野窗口更新了,但是孔图案的ui绘制点还在,要刷新一下(过时,preview.initHoleInfo直接做了这部分工作)
        void clearViewWindowCache(const QPoint& holepoint);
        void clearAllViewWindowCache(int viewsize);
        void updateViewWindowCache(QCPoint holepoint,QCPointVector viewpoints,int viewsize);
        void setDisablePoints(bool enable = true);
        void setDisablePoint(QCPoint point,bool enable = true);
        void setDisablePoints(QCPointVector points, bool enable = true);
    private: // 动作菜单
        DrawStrategy strategy = NoStrategy;
        QVariantMap mCurrentViewInfo;
        int mrows = 0; // viewsize字段去更新
        int mcols = 0;
        QAction * saveviewact;
        QAction * removeviewact;
        QAction * applyholeact;
        QAction * applygroupact;
        QAction * applyallact;
        void onRemoveViewAct();
        void onSaveViewAct();// 保存选择的视野到当前孔id对应的视野数据区并保存到临时信息用于initSelectPoints重新初始化
        void onApplyHoleAct(); // 选点和删点时都会触发,应用到当前孔
        void onApplyGroupAct();//传递视野窗口的组名+组颜色+视野尺寸+当前孔坐标+所有视野坐标信息+更新同组其它孔的视野信息和临时信息
        void onApplyAllAct();
        /*鼠标右击,鼠标释放,构造函数,setStrategy,onSaveViewAct,onRemoveViewAct控制使能*/
    private:
        QMap<int,QBool2DVector> mViewSelectPoints; // 视野已选择的信息
        QMap<int,QBool2DVector> mTmpViewSelectPoints; // 临时变量
        void initSelectPoints(); // setStrategy内调用,构造函数无需调用,更新当前孔的视野数量(分配空间,如果有上次设置过的视野数量信息就更新)
        int viewPointCount() const;// 计算当前孔已经选择的视野数(控制应用到本组的使能,未分过组或者分过组但是没选过视野)

        QRectF mDrapRect; // 鼠标拖动生成的矩形
        QBool2DVector mDrapPoints; // 拖拽矩形内选中的点赋值true
        QBool2DVector mDisablePoints; // 置灰区域,不可选的区域
        void initDrapPoints(); // 拖拽结束后清除这些点
        void initDisablePoints(); // 初始化置灰区域
        int drapPointCount() const;// 计算拖拽区域包含的点个数

    public: // 鼠标事件,viewpattern_mouse.cpp
        QPoint mMousePoint = QPoint(-1,-1);
        QPoint mLastPos = QPoint(-1,-1); // 鼠标左键点击的真实物理坐标
        QColor mMouseClickColor = Qt::green;
        QColor mGrayColor = Qt::gray;
        void mousePressEvent(QMouseEvent *event) override;// 左键点击清除框选,计算鼠标点击的小矩形区域坐标
        void mouseMoveEvent(QMouseEvent *event) override;// 绘制拖拽框
        void mouseReleaseEvent(QMouseEvent *event) override;// 拖拽区域点个数为0才是预览事件

    public:// 绘图的函数,定义在viewpattern_paint.cpp
        void paintEvent(QPaintEvent *event) override;
    private:
        void drawDrapRect(QPainter&painter);// 绘制推拽矩形框包含的小矩形区域
        void drawSelectRect(QPainter&painter); // 根据当前孔的组颜色去绘制视野已经被选择的小矩形区域
        void drawDisbaleRect(QPainter&painter); // 绘制置灰区域
        void drawCircle(QPainter&painter); // 画窗口的内接圆
        void drawInnerLine(QPainter&painter); // 画窗口的线区分出小矩形区域

    private: // 绘制视野正方形区域所有需要计算的东西,定义在viewpattern_geometry.cpp
        double getCircleRadius() const; // 视野圆半径
        double getInnerRectWidth() const; // 小矩形区域的宽度
        double getInnerRectHeight() const; // 小矩形区域的高度
        QPointF getInnerRectTopLeftPoint() const; // 外接正方形左上角顶点
        QPointF getInnerRectTopRightPoint() const;// 外接正方形右上角顶点
        QPointF getInnerRectBottomLeftPoint() const;// 外接正方形左下角顶点
        QPointF getInnerRectBottomRightPoint() const;// 外接正方形右下角顶点
        QPointFVector getInnerVerticalLineTopPoints() const ; // 正方形顶侧的等分点
        QPointFVector getInnerVerticalLineBottomPoints() const ;// 正方形底侧的等分点
        QPointFVector getInnerHorizonalLineLeftPoints() const ; // 正方形左侧的等分点
        QPointFVector getInnerHorizonalLineRightPoints() const ;// 正方形右侧的等分点
        QRectF2DVector getInnerRects() const; // 获取线分割出来的所有小矩形区域

    signals:
        void mouseClicked(const QPoint& point);
        void previewEvent(const QPoint& point); // 单击非框选时是预览
        void applyHoleEvent(const QVariantMap&m);
        void applyGroupEvent(const QVariantMap&m);
        void applyAllEvent(const QVariantMap&m);
    };
}
#endif //EOSI_VIEWPATTERN_H
