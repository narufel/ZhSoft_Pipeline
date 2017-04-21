#ifndef CUSTOMMODEL_H
#define CUSTOMMODEL_H

#include <QAbstractTableModel>
#include <QModelIndex>
#include <QObject>


class customModel : public QAbstractTableModel
{
public:
    customModel(const QStringList &RubriqueList, QStringList &attributList, QStringList &operateurList, QObject *parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    QStringList RubriqueList;
    QStringList attributList;
    QStringList operateurList;
    enum Colonnes { Rubrique = 0, Attribut = 1, Operateur = 2, NbColonnes = Operateur + 1};

};
#endif // CUSTOMMODEL_H
