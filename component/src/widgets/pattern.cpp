#include "pattern.h"

void Pattern::drawHighlight(QPainter&painter)
{
    auto cell_size = getChildSize();
    int cell_w = cell_size.width();
    int cell_h = cell_size.height();

    int radius = cell_w>=cell_h? cell_h/2: cell_w/2; // 选较小的确保圆在矩形内

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
        mMousePos = QPoint(-1,-1);
        mLastPos = event->pos();
        auto rects = getChildRects(); // 所有小正方形区域匹配这个坐标
        for(int row = 0; row < mrows; ++row)
            for(int col = 0; col < mcols; ++col)
                if (rects[row][col].contains(mLastPos))
                    mMousePos = {row,col};
        update();
        emit mouseClicked(mMousePos);
    }

    event->accept();
}

void Pattern::mouseDoubleClickEvent(QMouseEvent*event)
{
    if (mMouseEvent) {
        mMousePos = QPoint(-1,-1);
        mLastPos = event->pos();
        auto rects = getChildRects(); // 所有小正方形区域匹配这个坐标
        for(int row = 0; row < mrows; ++row)
            for(int col = 0; col < mcols; ++col)
                if (rects[row][col].contains(mLastPos))
                    mMousePos = {row,col};
        update();
        emit doubleClicked(mMousePos);
    }
    event->accept();
}

void Pattern::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    auto pen = painter.pen();
    pen.setWidth(2);
    painter.setPen(pen);

    painter.drawRect(0,0,width(),height());

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
        auto center = colpts[row];
        painter.drawText(center,QChar(row+65));
    }
}

QSize Pattern::getChildSize() const
{ // 每个圆都在划分的小正方形区域内,计算正方形的长度
    int w = width() ;
    int h = height() ;
    // 总宽度-2个到边界的gap-圆之间留出的mSpace,边界sapce留出2个,共(mCol-1+2)个
    int cell_w = (w-2*mGap-mSpace*(mcols+1))/mcols;
    int cell_h = (h-2*mGap-mSpace*(mrows+1)) / mrows;
    return  QSize(cell_w,cell_h);
}

QRectF2DVector Pattern::getChildRects() const
{ // 拿到所有小正方形的区域
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
{ // 按行从左到右拿到所有分隔线上的点,可用于画线
    auto cell_size = getChildSize();
    int cell_w = cell_size.width();
    int cell_h = cell_size.height();

    QPointF2DVector points;
    for(int row = 0; row < mrows+1; ++row) // 4x6,4行5条边,所以需要mrows+1多加1边
    {
        QPointFVector p; // 拿到每行的分隔点
        for(int col = 0; col < mcols+1; ++col)
            p.append(QPointF(mGap+mSpace/2+(cell_w+mSpace)*col, // 分隔点坐标从起始位置(mgap+maspace/2)开始
                             mGap+mSpace/2+(cell_h+mSpace)*row));
        points.append(p);
    }

    return points;
}

QPointFVector Pattern::getRowHeaderPoints()
{// 获取行表头文字的点,用于绘制文字
    auto cell_size = getChildSize();
    int cell_w = cell_size.width();
    QPointFVector points;
    for(int col = 0; col < mcols; ++col)
        points.append(QPointF(mGap+mSpace/2+cell_w/2+(cell_w+mSpace)*col,
                              mGap-mSpace/2)); // 列y坐标不变就是mgap-space/2
    return points;
}

QPointFVector Pattern::getColHeaderPoints()
{// 获取列表头文字的点,用于绘制文字
    auto cell_size = getChildSize();
    int cell_h = cell_size.height();
    QPointFVector points;
    for(int row = 0; row < mrows; ++row)
        points.append(QPointF(mGap-mSpace, // 行x坐标不变就是mgap-space
                              mGap+mSpace+cell_h/2+(cell_h+mSpace)*row));
    return points;
}

QPointF2DVector Pattern::getCenterPoints() const
{// 获取小正方形区域的中心用于绘制圆用
    auto cell_size = getChildSize();
    int cell_w = cell_size.width();
    int cell_h = cell_size.height();

    QPointF2DVector centerPts;
    for(int row = 0; row < mrows; ++row)
    {
        QVector<QPointF> p;
        for(int col = 0; col < mcols; ++col)
            // 中心位置相对起点左上角多了cell_w和cell_h的一半,还多了mSpace
            p.append(QPointF(mGap+cell_w/2+mSpace+(cell_w+mSpace)*col,
                             mGap+cell_h/2+mSpace+(cell_h+mSpace)*row));
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

void Pattern::clearMousePoint()
{
    mMousePos = QPoint(-1,-1);
    update();
}

QPoint Pattern::currentMousePoint() const
{
    return mMousePos;
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

//QSize Pattern::sizeHint() const
//{
//    return QSize(PatternSizeHintWidth,PatternSizeHintHeight);
//}