struct HoleCoordinate
{
    int x = 0;
    int y = 0;
    QString group;
    QString medicine;
    QString dose;
    QString unit;

    QPoint getCoordinate() {
        return QPoint(x,y);
    }
};

struct ViewCoordinate
{
    double x = 0.0;
    double y = 0.0;
    HoleCoordinate hole;
};

struct ImageInfo
{
    QString path;
    QDateTime stamp;
    QString channel;
    ViewCoordinate view;

    void setHoleCoordinate(const QPoint& hole) {
        view.hole.x = hole.x();
        view.hole.y = hole.y();
    }

    QPoint getHoleCoordinate() const {
        return QPoint(view.hole.x,view.hole.y);
    }

    void setHoleCoordinate(int x,int y) {
        view.hole.x = x;
        view.hole.y = y;
    }

    void setHoleGroup(const QString& group) {
        view.hole.group = group;
    }

    QString getHoleGroup() const {
        return view.hole.group;
    }

    void setViewCoordinate(double x,double y) {
        view.x = x;
        view.y = y;
    }

    QPointF getViewCoordinate() const {
        return QPointF(view.x,view.y);
    }
};
typedef QVector<ImageInfo> ImageInfoVector;

struct PlateImageInfo
{
    ImageInfoVector images;
    bool isRunning = false;
    QDateTime experTime;
    QString objective; // 4xBR
    QSize plateSize;
    QString experName;
    // 其它实验信息参数等

    void clear() {
        images.clear();
        isRunning = false;
        experName.clear();
        objective.clear();
        plateSize = QSize();
        experTime = QDateTime();
    }

    // 获取使用的所有孔坐标
    QPointVector holeCoordinates() const {
        QPointVector vec;
        for(auto image: images) {
            vec.append(image.view.hole.getCoordinate());
        }
        return vec;
    }

    // 获取指定孔的所有不重复视野坐标,因为可能坐标相同只是时间戳不同
    QPointFVector viewCoordinates(int x, int y) const {
        QPointFVector vec;
        for(auto image: images) {
            if (image.view.hole.getCoordinate() == QPoint(x,y)) {
                if (!vec.contains(QPointF(image.view.x,image.view.y)))
                    vec.append(QPointF(image.view.x,image.view.y));
            }
        }
        return vec;
    }

    // 获取指定孔所有信息
    ImageInfoVector holeImages(int hole_x,int hole_y) const
    {
        ImageInfoVector vec;
        for(auto image: images) {
            if (image.view.hole.x == hole_x && image.view.hole.y == hole_y) {
                vec.append(image);
            }
        }
        return vec;
    }

    // 获取指定孔内指定点的信息
    ImageInfoVector viewImages(int hole_x,int hole_y,double view_x,double view_y) const {
        ImageInfoVector vec;
        for(auto image: images) {
            if (image.view.hole.x == hole_x && image.view.hole.y == hole_y) { // 指定孔
                //qDebug()<<(QString::number(0.000000000011) == QString::number(0.0000000000111));
                if (qAbs(view_x - image.view.x) < 1e-6
                    && qAbs(view_y - image.view.y) < 1e-6) { // 指定视野
                    vec.append(image);
                }
            }
        }
        return vec;
    }

    // 获取指定孔内指定点且是某个通道的图像,注意不区分时间戳,所以视野坐标可能是相同的
    ImageInfoVector viewChannelImages(int hole_x,int hole_y,double view_x,double view_y, const QString& channel) {
        ImageInfoVector vec;
        for(auto image: images) {
            if (image.view.hole.x == hole_x && image.view.hole.y == hole_y && channel == image.channel) { // 指定孔和通道
//                LOG<< image.getViewCoordinate() << QPointF(view_x,view_y)
//                    <<(qAbs(view_x - image.view.x) < 1e-6)<<(qAbs(view_y - image.view.y) < 1e-6);
                if (qAbs(view_x - image.view.x) < 1e-6
                    && qAbs(view_y - image.view.y) < 1e-6 ) { // 指定视野和通道
                    vec.append(image);
                }
            }
        }
        return vec;
    }

    ImageInfoVector viewChannelImages(const QPoint& hole,const QPointF& view, const QString& channel) {
        return viewChannelImages(hole.x(),hole.y(),view.x(),view.y(),channel);
    }

    // 获取指定孔的指定视野的照片都涉及哪些通道
    QStringList getViewChannels(int hole_x,int hole_y,double view_x,double view_y) const {
        QStringList vec;
        for(auto image: images) {
            if (image.view.hole.x == hole_x && image.view.hole.y == hole_y
                && qAbs(view_x - image.view.x) < 1e-6
                && qAbs(view_y - image.view.y) < 1e-6) { // 指定孔和视野
                if (!vec.contains(image.channel))
                    vec.append(image.channel);
            }
        }
        return vec;
    }

    // 获取指定孔的视野属于哪个组
    QString getViewGroup(int hole_x,int hole_y,double view_x,double view_y) const {
        QString tmpGroup;
        for(auto image: images) {
            if (image.view.hole.x == hole_x && image.view.hole.y == hole_y
                && qAbs(view_x - image.view.x) < 1e-6
                && qAbs(view_y - image.view.y) < 1e-6) { // 指定孔和视野
                tmpGroup = image.getHoleGroup(); // 同一个孔的视野都是一样的组
                break;
            }
        }
        return tmpGroup;
    }

};

struct DataPatternHoleInfo
{
    bool isSelected = false;
    QPoint hole;
    PlateImageInfo info;
};
typedef QVector<DataPatternHoleInfo> DataPatternHoleInfoVector;
typedef QVector<DataPatternHoleInfoVector> DataPatternHoleInfo2DVector;
