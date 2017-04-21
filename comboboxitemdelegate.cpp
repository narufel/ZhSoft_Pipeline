#include "comboboxitemdelegate.h"

#include <QApplication>
#include <QComboBox>
#include <QPixmap>
#include <QPainter>



ComboBoxItemDelegate::ComboBoxItemDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{
}

ComboBoxItemDelegate::ComboBoxItemDelegate(QStringList ItemList, QObject* parent) : QStyledItemDelegate(parent)
{
    _itemsList = ItemList;
}


ComboBoxItemDelegate::~ComboBoxItemDelegate()
{
}


QWidget* ComboBoxItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    // ComboBox ony in column 2
//    if ((index.column() == 1) && (index.column() == 0))
//        return QStyledItemDelegate::createEditor(parent, option, index);

    QList<QString> operateurList;
    operateurList << "Presence" << "PremiereValeur" << "Somme" << "Moyenne" << "MoyenneABS" << "Min" << "Max" << "Var" << "stDev";
    // Create the combobox and populate it
    QComboBox *cb = new QComboBox(parent);

    if(index.column() == 2) {

        for(int i = 0; i<operateurList.count(); i++) {

            cb->addItem(operateurList.at(i));
        }
        cb->setEditable(true);
    }

    if(index.column() == 1) {
        QList<QString> tmpList;
        tmpList << " " << _itemsList;
        for(int i = 0; i<tmpList.count(); i++) {
            cb->addItem(tmpList.at(i));
        }

            //cb->setCurrentIndex(0);
        cb->setEditable(true);
    }


    cb->setStyleSheet("QComboBox { color : green;}");




    return cb;
}


void ComboBoxItemDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    if (QComboBox* cb = qobject_cast<QComboBox*>(editor)) {
       // get the index of the text in the combobox that matches the current value of the itenm
       QString currentText = index.data(Qt::EditRole).toString();
       int cbIndex = cb->findText(currentText);
       cb->addItem("");
       // if it is valid, adjust the combobox
       if (cbIndex >= 0)
           cb->setCurrentIndex(cbIndex);
    } else {
        QStyledItemDelegate::setEditorData(editor, index);
    }
}


void ComboBoxItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    if (QComboBox* cb = qobject_cast<QComboBox*>(editor))
        // save the current text of the combo box as the current value of the item
        model->setData(index, cb->currentText(), Qt::EditRole);
    else
        QStyledItemDelegate::setModelData(editor, model, index);

//    QComboBox* comboBox = qobject_cast<QComboBox*>(editor);

//    int selectedItem = comboBox->currentIndex();
//        if(selectedItem == -1)
//        {
//                model->setData(index, -1, Qt::EditRole);
//                model->setData(index, comboBox->currentText(), Qt::DisplayRole);
//        }
//        else
//        {
//                model->setData(index, selectedItem, Qt::EditRole);
//                model->setData(index, comboBox->itemText(selectedItem), Qt::DisplayRole);
//        }


}

//paint
void ComboBoxItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {

    if (index.column() == 6) {
        int value = index.model()->data(index, Qt::DisplayRole).toInt();

        QStyleOptionButton check_box_style_option;
        check_box_style_option.state != QStyle::State_Enabled;
        if (value == 1) {
            check_box_style_option.state != QStyle::State_On;
        } else {
            check_box_style_option.state != QStyle::State_Off;
        }


        QApplication::style()->drawControl(QStyle::CE_CheckBox, &check_box_style_option, painter);
    } else
        QStyledItemDelegate::paint(painter, option, index);
}


