/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-16 16:15:37
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-16 16:22:44
 * @FilePath: \EOS\component\src\datastructure\textdelegate.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "textdelegate.h"

QWidget *TextDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                                 const QModelIndex &index) const
{
    Q_UNUSED(index);
    Q_UNUSED(option);
    TextEdit * textEdit = new TextEdit(parent);
    return textEdit;
}

void TextDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString text = index.model()->data(index,Qt::EditRole | Qt::DisplayRole).toString();
    TextEdit * textEdit = static_cast<TextEdit*>(editor);
    textEdit->setText(text);
}

void TextDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                             const QModelIndex &index) const
{ // 双击的时候才触发这个函数
    TextEdit *textEdit = static_cast<TextEdit*>(editor);
    model->setData(index, textEdit->text(), Qt::EditRole);
}

void TextDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                                     const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}

bool TextDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
                                            const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonDblClick)
    {
        auto model = index.model();
        auto  v = model->data(index, Qt::DisplayRole);
        if (v.isValid())
        {
            TextEdit * dlg = new TextEdit;
            dlg->setAttribute(Qt::WA_DeleteOnClose);
            dlg->resize(DefaultWindowSize);
            dlg->setText(v.toString());
            dlg->show();
        }
        event->accept();
    }
    return QStyledItemDelegate::editorEvent(event,model,option,index);
}
