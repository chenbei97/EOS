/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-16 16:05:27
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-16 16:22:20
 * @FilePath: \EOS\component\src\datastructure\linedelegate.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "linedelegate.h"

LineDelegate::LineDelegate(int min,int max,QObject*parent):QStyledItemDelegate(parent)
{
    minval = min;
    maxval = max;
}

void LineDelegate::setIntRange(int min, int max)
{
    minval = min;
    maxval = max;
}

QWidget *LineDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                            const QModelIndex &index) const
{
    Q_UNUSED(index);
    Q_UNUSED(option);
    LineEdit * edit = new LineEdit(parent);
    edit->setIntRange(minval,maxval);
    return edit;
}

void LineDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString text = index.model()->data(index,Qt::EditRole | Qt::DisplayRole).toString();
    LineEdit * textEdit = static_cast<LineEdit*>(editor);
    textEdit->setText(text);
}

void LineDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                        const QModelIndex &index) const
{ // 双击的时候才触发这个函数
    LineEdit *textEdit = static_cast<LineEdit*>(editor);
    model->setData(index, textEdit->text(), Qt::EditRole);
}

void LineDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                                const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}

