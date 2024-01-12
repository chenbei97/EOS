

#ifndef EOS_IMAGEDELEGATE_H
#define EOS_IMAGEDELEGATE_H

#include "datastructure.h"

#define ImageDataRole Qt::DecorationRole
#define ImagePathRole (Qt::UserRole+1)

class COMPONENT_IMEXPORT ImageDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model,
                     const QStyleOptionViewItem &option, const QModelIndex &index) override;
};

#endif //EOS_IMAGEDELEGATE_H
