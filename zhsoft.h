#ifndef ZHSOFT_H
#define ZHSOFT_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QStandardItemModel>
#include "comboboxitemdelegate.h"


namespace Ui {
class ZhSoft;
}

class ZhSoft : public QMainWindow
{
    Q_OBJECT

public:
    explicit ZhSoft(QWidget *parent = 0);
    ~ZhSoft();

private slots:
    void on_actionOuvrir_triggered();

    void on_pushButtonMoveUp_clicked();

    void on_actionActualiser_triggered();

    void on_pushButtonMoveDown_clicked();

private:
    Ui::ZhSoft *ui;

    // arguments
    QSqlDatabase msaViapix_db;
    QString filePath;
    QStandardItemModel _rubriqueAndTypeModel;
    QStandardItemModel _crossModel;
    ComboBoxItemDelegate *comboDelegate;
    ComboBoxItemDelegate *comboDelegate1;

    // méthodes
    void findRubriqueAndTypeTables(const QString &strMSAfilename, QList<QPair<QString, int>> &valueThemeAndTypeList);
    void updateRubriqueAndTypeNames();
};

#endif // ZHSOFT_H
