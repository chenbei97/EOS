
#include "imagedelegate.h"
#include "qpainter.h"
#include "qevent.h"
#include "picture.h"

void ImageDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
{
    auto pix_var = index.model()->data(index, ImageDataRole);
    if (pix_var.isValid()){
        auto pix = pix_var.value<QImage>();
        painter->drawImage(option.rect,pix);
        auto pen = painter->pen();
        pen.setWidth(1);
        pen.setColor(Qt::black);
        painter->setPen(pen);
        painter->drawRect(option.rect);
    }
}

void ImageDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                         const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}

bool ImageDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
                                const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonDblClick)
    {
        auto  v = index.model()->data(index, ImagePathRole);
        if (v.isValid())
        {
            auto path = v.value<QString>();
            Picture::showOriginal(path);
        }
    }
    return QStyledItemDelegate::editorEvent(event,model,option,index);
}
