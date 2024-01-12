#include "pattern.h"

void Pattern::drawHighlight(QPainter&painter)
{
    auto cell_size = getInnerRectSize();
    auto cell_w = cell_size.width();
    auto cell_h = cell_size.height();

    auto radius = cell_w>=cell_h? cell_h/2.0: cell_w/2.0; // 选较小的确保圆在矩形内

    auto  centerPts = getCenterPoints();
    QPainterPath path;

    for(int row = 0 ; row < mrows; ++ row)
    {
        for(int col = 0; col < mcols; ++ col)
        {
            auto center = centerPts[row][col];
            painter.drawEllipse(center,radius,radius); // 绘制外圆
            painter.drawEllipse(center,radius*0.75,radius*0.75); // 绘制内圆

            if (mMouseEvent) { // 常规状态只有内外圆,内圆没有填充
                path.clear();
                path.addEllipse(center,radius*0.75,radius*0.75); // 填充内圆
                painter.fillPath(path,mInnerCircleColor);
            }
        }
    }
}

void Pattern::mousePressEvent(QMouseEvent *event)
{
    if (mMouseEvent) {
        if (event->button() == Qt::LeftButton) { // 改为左键才会变化,防止右击也动电机
            //mMousePos = QPoint(-1,-1); // mMousePos是上次的位置不要重置
            mLastPos = event->pos();
//            if (isValidPoint(mLastPos)) {
//                QPoint tmp(-1,-1);
//                auto rects = getAllInnerRects(); // 所有小正方形区域匹配这个坐标
//                for(int row = 0; row < mrows; ++row)
//                    for(int col = 0; col < mcols; ++col)
//                        if (rects[row][col].contains(mLastPos))
//                            tmp = {row,col}; // 找到孔的匹配坐标
//                //LOG<<mLastPos<<mMousePos<<tmp;
//                if (tmp != QPoint(-1,-1)) { // 点击的不是无效位置,确实是某个孔
//                    if(tmp != mMousePos) { // mMousePos是上次点击的有效位置,而且不是重复点相同的孔
//                        mMousePos = tmp;
//                        if (mHoleClickEvent)
//                            emit holeClicked(mMousePos); // 新位置和原来位置不同才动电机
//                    }
//                }
//            }
            update();
        }
    }

    event->accept();
}

void Pattern::mouseDoubleClickEvent(QMouseEvent*event)
{
    if (mMouseEvent) {
        //mMousePos = QPoint(-1,-1);
        mLastPos = event->pos();
        if (isValidPoint(mLastPos)) {
            auto rects = getAllInnerRects(); // 所有小正方形区域匹配这个坐标
            for(int row = 0; row < mrows; ++row)
                for(int col = 0; col < mcols; ++col)
                    if (rects[row][col].contains(mLastPos))
                        mMousePos = {row,col};
            update();
        }
    }
    event->accept();
}

void Pattern::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    auto pen = painter.pen();
    pen.setWidth(DefaultPainterPenWidth);
    painter.setPen(pen);

    painter.drawRect(0,0,width(),height());
    painter.drawRect(getValidRect());

    drawLine(painter);
    drawText(painter);
    drawHighlight(painter);
    event->accept();
}

void Pattern::drawLine(QPainter&painter)
{ // 绘制线
    auto points = getBorderPoints();
    for(int row = 1; row < mrows; ++row) // 首尾2个边的线不再绘制
        painter.drawLine(points[row][0],
                         points[row][mcols]);
    for(int col = 1; col < mcols; ++col)
        painter.drawLine(points[0][col],points[mrows][col]);

}

void Pattern::drawText(QPainter &painter)
{ // 绘制文字
    auto pointSize = mrows>10? 8:DefaultFontSize;

    auto font = painter.font();
    font.setPointSize(pointSize);
    font.setFamily(DefaultFontFamily);
    painter.setFont(font);

    auto rowpts = getRowHeaderPoints();
    for(int col = 0 ; col < mcols; ++ col)
    {
        auto center = rowpts[col];
        painter.drawText(center,QString::number(col+1));
    }

    auto colpts = getColHeaderPoints();
    for(int row = 0 ; row < mrows; ++ row)
    {
        auto center = colpts[row]+QPointF(-5.0,0); // 文字稍微靠左一些
        painter.drawText(center,QChar(row+65));
    }
}

QSizeF Pattern::getInnerRectSize() const
{ // 每个圆都在划分的小正方形区域内,计算正方形的长度
    double w = width() ;
    double h = height() ;
    // 总宽度-2个到边界的gap-圆之间留出的mSpace,边界sapce留出2个,共(mCol-1+2)个
    double cell_w = (w-2.0*mGap-mSpace*(mcols+1))/mcols;
    double cell_h = (h-2.0*mGap-mSpace*(mrows+1)) / mrows;
    return  QSizeF(cell_w,cell_h);
}

QRectF Pattern::getValidRect() const
{
    return QRectF(mGap+mSpace/2.0,mGap+mSpace/2.0,
                 width()-2.0*(mGap+mSpace/2.0),height()-2.0*(mGap+mSpace/2.0));
//    auto rects = getAllInnerRects();
//    auto topleft = rects[0][0].topLeft();
//    auto bottomright = rects[mrows-1][mcols-1].bottomRight();
//    return QRectF(topleft,bottomright);
}

bool Pattern::isValidPoint(const QPointF& point) const
{
    return getValidRect().contains(point);
}

QRectF2DVector Pattern::getAllInnerRects() const
{ // 拿到所有小正方形的区域: 注意并不是圆的外接正方形,而是根据mGap,mSpace等分出来的小正方形区域
    auto points = getBorderPoints();

    QRectF2DVector rects;
    for(int row = 0; row < mrows; ++row)
    {
        QRectFVector r;
        for(int col = 0; col < mcols; ++col)
        {
            auto topleft = points[row][col];
            auto bottomright = points[row+1][col+1];
            r.append(QRectF(topleft,bottomright));
        }
        rects.append(r);
    }
    return rects;
}

QPointF2DVector Pattern::getBorderPoints() const
{ // 划分小正方区域经过的所有点
    auto cell_size = getInnerRectSize();
    auto cell_w = cell_size.width();
    auto cell_h = cell_size.height();

    QPointF2DVector points;
    for(int row = 0; row < mrows+1; ++row) // 4x6,4行5条边,所以需要mrows+1多加1边
    {
        QPointFVector p; // 拿到每行的分隔点
        for(int col = 0; col < mcols+1; ++col)
            p.append(QPointF(mGap+mSpace/2.0+(cell_w+mSpace)*col, // 分隔点坐标从起始位置(mgap+maspace/2)开始
                             mGap+mSpace/2.0+(cell_h+mSpace)*row));
        points.append(p);
    }

    return points;
}

QPointFVector Pattern::getRowHeaderPoints()
{// 获取行表头文字的点,用于绘制文字
    auto cell_size = getInnerRectSize();
    auto cell_w = cell_size.width();
    QPointFVector points;
    for(int col = 0; col < mcols; ++col)
        points.append(QPointF(mGap+mSpace/2.0+cell_w/2.0+(cell_w+mSpace)*col,
                              mGap-mSpace/2.0)); // 列y坐标不变就是mgap-space/2
    return points;
}

QPointFVector Pattern::getColHeaderPoints()
{// 获取列表头文字的点,用于绘制文字
    auto cell_size = getInnerRectSize();
    auto cell_h = cell_size.height();
    QPointFVector points;
    for(int row = 0; row < mrows; ++row)
        points.append(QPointF(mGap-mSpace, // 行x坐标不变就是mgap-space
                              mGap+mSpace+cell_h/2.0+(cell_h+mSpace)*row));
    return points;
}

QPointF2DVector Pattern::getCenterPoints() const
{// 获取小正方形区域的中心用于绘制圆用
    auto cell_size = getInnerRectSize();
    auto cell_w = cell_size.width();
    auto cell_h = cell_size.height();

    QPointF2DVector centerPts;
    for(int row = 0; row < mrows; ++row)
    {
        QVector<QPointF> p;
        for(int col = 0; col < mcols; ++col)
            // 中心位置相对起点左上角多了cell_w和cell_h的一半,还多了mSpace
            p.append(QPointF(mGap+cell_w/2.0+mSpace+(cell_w+mSpace)*col,
                             mGap+cell_h/2.0+mSpace+(cell_h+mSpace)*row));
        centerPts.append(p);
    }

    return centerPts;
}

void Pattern::setPatternSize(int rows,int cols)
{
    mrows = rows;
    mcols = cols;
    mMousePos = {-1,-1};
    mLastPos = {-1,-1};
    update();
}

QSize Pattern::patternSize() const
{
    return QSize(mrows,mcols);
}

Pattern::Pattern(int rows, int cols,QWidget*parent):mrows(rows),mcols(cols),QWidget(parent)
{
    mMouseEvent = false;
    mMousePos = {-1,-1};
    mLastPos = {-1,-1};
    mInnerCircleColor.setAlpha(DefaultColorAlpha);
    mMouseClickColor.setAlpha(DefaultColorAlpha);
    //setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
}

QSize Pattern::minimumSizeHint() const
{
    return DefaultWindowSize;
}