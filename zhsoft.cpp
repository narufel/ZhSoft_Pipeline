#include "zhsoft.h"
#include "ui_zhsoft.h"
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlDriver>
#include <QSqlField>

ZhSoft::ZhSoft(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ZhSoft)
{
    ui->setupUi(this);

     ui->tableViewMsaDisplay->setModel(&_rubriqueAndTypeModel);

     _crossModel.setColumnCount(3);
     QStringList themeAndTypeLabels;
     themeAndTypeLabels << tr("Rubriques") <<tr("Attributs") << tr("Operateurs");
     _crossModel.setHorizontalHeaderLabels(themeAndTypeLabels);
     ui->tableViewCrossing->setModel(&_crossModel);
     ui->tableViewCrossing->horizontalHeader()->setStretchLastSection(false);

     // comboBox delegate
     comboDelegate = new ComboBoxItemDelegate(this);
     //ui->tableViewCrossing->setItemDelegateForColumn(1, comboDelegate);
     ui->tableViewCrossing->setItemDelegateForColumn(2, comboDelegate);

     // Fill the comboBox with tables of the database
     ui->comboBoxCrossing->setModel(&_rubriqueAndTypeModel);

}

ZhSoft::~ZhSoft()
{
    delete ui;
}

void ZhSoft::on_actionOuvrir_triggered()
{
    filePath = QFileDialog::getOpenFileName(this,
                tr("Import MS access database file"), "",
                tr("MS access File (*.mdb *.accdb);;All Files (*)"));

    if(filePath.isEmpty())
        return;

    updateRubriqueAndTypeNames();

}

void ZhSoft::findRubriqueAndTypeTables(const QString &strMSAfilename, QList<QPair<QString, int>> &valueThemeAndTypeList)
{
    // verifier si le fichier existe
    if(!QFile::exists(strMSAfilename)){
        qDebug()<<Q_FUNC_INFO<<"fichier choisit n'existe pas!";
        return; // file not found
    }

    // connect to the MS access database
    msaViapix_db = QSqlDatabase::addDatabase("QODBC");
    msaViapix_db.setHostName("localhost");
    msaViapix_db.setDatabaseName(QString("Driver={Microsoft Access Driver (*.mdb, *.accdb)};DSN='';DBQ=%1 ;Exclusive=1;").arg(strMSAfilename));

    // verify if the database is ok
    if(!msaViapix_db.open()) {
        QMessageBox::critical(this, "Error", msaViapix_db.lastError().text());
        return;
    }

    // récuperer toutes les tables stockées dans la base de donnée (MS access)
    QStringList strRubriquesList;
    strRubriquesList = msaViapix_db.tables();

    QSqlDriver *sqlDriver = msaViapix_db.driver();

    QStringList defautFieldNames;
    defautFieldNames <<"Id"<<"Section_id"<<"Cumuld"<<"Cumulf"<<"Cumul";

    QString strFldName = QString();
    QVariant::Type fldType;


     QSqlRecord msaRecord;

     QStringList strSortedTable;

     foreach (QString strTableName, strRubriquesList) {

         msaRecord = sqlDriver->record(strTableName);

         // On récupere tous les champs des tables

         if(msaRecord.isEmpty())
             continue;

         // verifions si les tables contiennent les champs suivants : section_id, cumuld, cumulf ou cumul

         int fieldCount = msaRecord.count();


         QStringList fieldsOfTable;

         for(int i = 0;i<fieldCount;i++){
             strFldName = msaRecord.field(i).name();
             fldType = msaRecord.field(i).type();
             fieldsOfTable << strFldName;
             if(defautFieldNames.contains(strFldName,Qt::CaseInsensitive)){
                fieldsOfTable << strFldName;

             } else if(QVariant::Double==fldType || QVariant::Int==fldType||QVariant::UInt ==fldType ){
                fieldsOfTable << strFldName;

             }

         }


         if(!(fieldsOfTable.contains("ID",Qt::CaseInsensitive) && fieldsOfTable.contains("SECTION_ID",Qt::CaseInsensitive)
                 && (fieldsOfTable.contains("CUMUL",Qt::CaseInsensitive) ||fieldsOfTable.contains("CUMULD",Qt::CaseInsensitive)||fieldsOfTable.contains("CUMUF",Qt::CaseInsensitive))))
         {
             continue; // not data table
         }

         strSortedTable.append(strTableName);


         // if data table then check if it has other value

         bool isFoundCumulF = false;

         foreach (QString strFieldName, fieldsOfTable) {

             if(strFieldName.toUpper()=="CUMULF"){
                 isFoundCumulF = true;
             }
         }


            if(isFoundCumulF){ //lianaire
                valueThemeAndTypeList.append(qMakePair(strTableName,1));


            } else{ //ponctuelle
                valueThemeAndTypeList.append(qMakePair(strTableName,0));
            }
   }
}

void ZhSoft::updateRubriqueAndTypeNames()
{
    QList<QPair<QString, int>> rubriqueValTypeList;

    findRubriqueAndTypeTables(this->filePath, rubriqueValTypeList);

    //qDebug() << rubriqueValTypeList;
    if(rubriqueValTypeList.isEmpty())
        return;

    // now refresh theme list model

    _rubriqueAndTypeModel.clear();

    _rubriqueAndTypeModel.setColumnCount(2);
    _rubriqueAndTypeModel.setRowCount(rubriqueValTypeList.count());

    QStringList rubriqueAndTypeLabels;
    rubriqueAndTypeLabels << tr("Rubrique") <<tr("Type");
    _rubriqueAndTypeModel.setHorizontalHeaderLabels(rubriqueAndTypeLabels);

    //-- add theme type
    for(int i = 0; i<rubriqueValTypeList.count(); i++){

        QPair<QString,int> msaPair = rubriqueValTypeList[i];
        QStandardItem *rubriqueNameItem = new QStandardItem(msaPair.first);
        QStandardItem *rubriqueTypeItem = new QStandardItem((msaPair.second==0)?"Ponctuelle":"Linaire");



        rubriqueNameItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
        rubriqueTypeItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);

        _rubriqueAndTypeModel.setItem(i,0,rubriqueNameItem);
        _rubriqueAndTypeModel.setItem(i,1,rubriqueTypeItem);

    }

    ui->tableViewMsaDisplay->resizeColumnsToContents();
    ui->tableViewMsaDisplay->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewMsaDisplay->verticalHeader()->setStretchLastSection(false);
    ui->tableViewMsaDisplay->horizontalHeader()->setStretchLastSection(true);
}

void ZhSoft::on_pushButtonMoveUp_clicked()
{
    QItemSelectionModel* selection = ui->tableViewMsaDisplay->selectionModel();
    QModelIndexList selectionRows = selection->selectedRows();
    QVariant elementSelectionne = ui->tableViewMsaDisplay->model()->data(selectionRows[0], Qt::DisplayRole);

    // on deplace les items choisies dans la TableView
    int count = _crossModel.rowCount();

    _crossModel.insertRows(count,1);
    _crossModel.setData(_crossModel.index(count, 0), elementSelectionne.toString());

    // Recuperer les attributs pour chaque rubrique

    QString selectedRubriqueName = elementSelectionne.toString();

    if(!msaViapix_db.isOpen()){
        qDebug()<<  "msa_db is not openned!";
        return;
    }

    // Récuperer les drivers pour analyser les champs des tables
    QSqlDriver *sqlDriver = msaViapix_db.driver();
    if(!sqlDriver){
        qDebug()<< tr("Base de données msa_db n'a pas été validé");
        return;
    }

    QSqlRecord msaRecord = sqlDriver->record(selectedRubriqueName);

    if(msaRecord.isEmpty()){
        qDebug()<< tr("La table selectionnée ne contient aucun champs!");
        return;
    }

    // Récuperer les attribues des tables
    QStringList defautFieldNames;
    defautFieldNames<<"Id"<<"Section_id"<<"Cumuld"<<"Cumulf"<<"Cumul";

    QStringList fieldsNameList;
    QSqlField msaField;
    int fieldCount = msaRecord.count();
      for(int i = 0;i<fieldCount;i++){
          msaField = msaRecord.field(i);
          if( !defautFieldNames.contains(msaField.name(),Qt::CaseInsensitive) && (msaField.type()==QVariant::Double
                  ||msaField.type()==QVariant::LongLong||msaField.type()==QVariant::Int
                  ||msaField.type()==QVariant::UInt)){
                  fieldsNameList.append(msaField.name());
          }

      }

    // remplir le comboBox par les noms des attribues
    comboDelegate1 = new ComboBoxItemDelegate(fieldsNameList);
    ui->tableViewCrossing->setItemDelegateForColumn(1, comboDelegate1 );


    // make the combo boxes always displayed
    for(int i = 0; i<_crossModel.rowCount(); ++i) {
        ui->tableViewCrossing->openPersistentEditor(_crossModel.index(i,1, QModelIndex()));
        ui->tableViewCrossing->openPersistentEditor(_crossModel.index(i,2, QModelIndex()));

    }



}

void ZhSoft::on_actionActualiser_triggered()
{
    QString currentComboBox = ui->comboBoxCrossing->currentText();
    updateRubriqueAndTypeNames();

    ui->comboBoxCrossing->setCurrentText(currentComboBox);
}

void ZhSoft::on_pushButtonMoveDown_clicked()
{
    _crossModel.removeRow(ui->tableViewCrossing->currentIndex().row());
}
