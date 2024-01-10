
#ifndef EOS_COMBODELEGATE_H
#define EOS_COMBODELEGATE_H

#include "datastructure.h"
#include "combobox.h"

class COMPONENT_IMEXPORT ComboDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model,
                     const QStyleOptionViewItem &option, const QModelIndex &index) override;
    void addItems(const QStringList& texts);
private:
    QStringList mComboItems;
};

#endif //EOS_COMBODELEGATE_H
