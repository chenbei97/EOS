
#include "combodelegate.h"

void ComboDelegate::addItems(const QStringList &texts)
{
    mComboItems = texts;
}

bool ComboDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
                               const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonDblClick)
    {
        auto model = index.model();
        auto  v = model->data(index, Qt::DisplayRole).toString();
        event->accept();
    }
    return QStyledItemDelegate::editorEvent(event,model,option,index);
}


QWidget *ComboDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    ComboBox *editor = new ComboBox(parent);
    editor->addItems(mComboItems);
    editor->setCurrentIndex(0);
    return editor;
}

void ComboDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    ComboBox *cb = static_cast<ComboBox*>(editor);
    QString value = index.model()->data(index, Qt::DisplayRole).toString();
    cb->setCurrentText(value);
}

void ComboDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    ComboBox *cb = static_cast<ComboBox*>(editor);
    QString displayData = cb->currentText();
    auto lastValue = model->data(index,Qt::DisplayRole).toString();
    model->setData(index, displayData, Qt::DisplayRole);
    if (lastValue != displayData) // 放在这里发射,setEditor会总是慢一拍
        emit currentTextChanged(displayData,index);
}

void ComboDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}