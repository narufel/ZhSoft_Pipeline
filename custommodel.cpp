#include "custommodel.h"
#include <QModelIndex>

customModel::customModel(const QStringList &RubriqueList, QStringList &attributList, QStringList &operateurList, QObject *parent):QAbstractTableModel(parent)
{
    if (RubriqueList.count() > 0)
        this->RubriqueList = RubriqueList;
    if (attributList.count() > 0)
        this->attributList = attributList;
    if (operateurList.count() > 0)
        this->operateurList = operateurList;

}

int customModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : this->RubriqueList.count();
}
int customModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : NbColonnes;
}
QVariant customModel::data(const QModelIndex &index, int role) const
{
    if (! index.isValid()) return QVariant() ;
    if(((unsigned int)index.row()>=this->RubriqueList.count())||(index.column()>=NbColonnes))
        return QVariant();
    if ((role == Qt::DisplayRole || role == Qt::EditRole)) {
        if(index.column() == Rubrique)
            return this->RubriqueList.at(index.row());
        else if(index.column() == Attribut)
            return this->attributList.at(index.row());
        else if(index.column() == Operateur)
            return this->operateurList.at(index.row());
    }
    return QVariant();
}
bool customModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        //this->RubriqueList.replace(index.row(), value.toString());
        this->attributList.replace(index.row(), value.toString());
        //this->operateurList.replace(index.row(), value.toString());
        emit(dataChanged(index, index));
        return true;
    }
    return false;
}

Qt::ItemFlags customModel::flags(const QModelIndex &index) const
{
    if (! index.isValid())
        return Qt::ItemIsEnabled;
    if (index.column() == Rubrique)  {
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    }
    else if (index.column() == Attribut)  {
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
    }
    else if (index.column() == Operateur)  {
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
    }
    return QAbstractTableModel::flags(index);

}
QVariant customModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
        {
            if (orientation == Qt::Horizontal) {
                switch (section)
                {
                case 0:
                    return QString("Rubriques");
                case 1:
                    return QString("Attributs");
                case 2:
                    return QString("Opérateurs");

                }
            }
        }
        return QVariant();
}


