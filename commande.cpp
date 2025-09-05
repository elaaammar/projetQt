#include "commande.h"
#include <qsqlquery>
#include <qtdebug>
#include <qsqlquerymodel>
#include <qobject>
#include <qmessagebox>
// Default constructor
commande::commande()
{
    idc = 0;
    idlivreur = 0;
    adresse = "";
    status = "";
    client = "";
    date_commande = QDate::currentDate();
}
// Parameterized constructor
commande::commande(int idc, QString adresse, QString status, QString client, QDate date_commande, int idlivreur)
{
    this->idc = idc;
    this->adresse = adresse;
    this->status = status;
    this->client = client;
    this->date_commande = date_commande;
    this->idlivreur = idlivreur;
}
/* ---------------- Getters ---------------- */
int commande::getIdc() const { return idc; }
int commande::getIdlivreur() const { return idlivreur; }
QDate commande::getDateCommande() const { return date_commande; }
QString commande::getAdresse() const { return adresse; }
QString commande::getStatus() const { return status; }
QString commande::getClient() const { return client; }
/* ---------------- Setters ---------------- */
void commande::setIdc(int idc) { this->idc = idc; }
void commande::setIdlivreur(int idlivreur) { this->idlivreur = idlivreur; }
void commande::setDateCommande(QDate date) { this->date_commande = date; }
void commande::setAdresse(QString adresse) { this->adresse = adresse; }
void commande::setStatus(QString status) { this->status = status; }
void commande::setClient(QString client) { this->client = client; }
/* ---------------- CRUD ---------------- */
bool commande::Ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO commande (idc, adresse, status, client, date_commande, idlivreur) "
                  "VALUES (:idc, :adresse, :status, :client, :date_commande, :idlivreur)");
    query.bindValue(":idc", idc);
    query.bindValue(":adresse", adresse);
    query.bindValue(":status", status);
    query.bindValue(":client", client);
    query.bindValue(":date_commande", date_commande);
    query.bindValue(":idlivreur", idlivreur);
    if (query.exec()) {
        QMessageBox::information(nullptr, QObject::tr("Succès"),
                                 QObject::tr("Commande ajoutée avec succès"), QMessageBox::Ok);
        return true;
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                              QObject::tr("Échec de l'ajout de la commande"), QMessageBox::Cancel);
        return false;
    }
}
QSqlQueryModel* commande::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM commande");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Commande"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Adresse"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Status"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Client"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Date Commande"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("ID Livreur"));
    return model;
}
bool commande::Supprime(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM commande WHERE idc = :idc");
    query.bindValue(":idc", id);
    if (query.exec()) {
        QMessageBox::information(nullptr, QObject::tr("Succès"),
                                 QObject::tr("Commande supprimée avec succès"), QMessageBox::Ok);
        return true;
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                              QObject::tr("Échec de la suppression de la commande"), QMessageBox::Cancel);
        return false;
    }
}
bool commande::modifier()
{
    QSqlQuery query;
    query.prepare("UPDATE commande SET adresse = :adresse, status = :status, client = :client, "
                  "date_commande = :date_commande, idlivreur = :idlivreur "
                  "WHERE idc = :idc");
    query.bindValue(":idc", idc);
    query.bindValue(":adresse", adresse);
    query.bindValue(":status", status);
    query.bindValue(":client", client);
    query.bindValue(":date_commande", date_commande);
    query.bindValue(":idlivreur", idlivreur);
    if (query.exec()) {
        QMessageBox::information(nullptr, QObject::tr("Succès"),
                                 QObject::tr("Commande modifiée avec succès"), QMessageBox::Ok);
        return true;
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                              QObject::tr("Échec de la modification de la commande"), QMessageBox::Cancel);
        return false;
    }
}
// ## Recherche ## //
QSqlQueryModel *commande::afficher_adresse(QString ch)
{
    QSqlQueryModel * model =new QSqlQueryModel();
    model->setQuery("SELECT * FROM commande where adresse= '"+ch+"' ");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Commande"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Adresse"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Status"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Client"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Date Commande"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("ID Livreur"));
    return model;
}
QSqlQueryModel *commande::afficher_status(QString ch)
{
    QSqlQueryModel * model =new QSqlQueryModel();
    model->setQuery("SELECT * FROM commande where status= '"+ch+"' ");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Commande"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Adresse"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Status"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Client"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Date Commande"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("ID Livreur"));
    return model;
}
QSqlQueryModel *commande::afficher_client(QString ch)
{
    QSqlQueryModel * model =new QSqlQueryModel();
    model->setQuery("SELECT * FROM commande where client= '"+ch+"' ");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Commande"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Adresse"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Status"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Client"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Date Commande"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("ID Livreur"));
    return model;
}
// ## Statistique ## //
QDate parseDate(const QString &dateStr) {
    QDateTime dt = QDateTime::fromString(dateStr, Qt::ISODate);
    if (dt.isValid())
        return dt.date();
    QDate d = QDate::fromString(dateStr, "dd/MM/yy");
    if (!d.isValid())
        d = QDate::fromString(dateStr, "dd/MM/yyyy");
    return d;
}
int commande::statistique1() {
    int count = 0;
    QSqlQuery query("SELECT date_commande FROM commande");
    while (query.next()) {
        QString dateStr = query.value(0).toString().trimmed();
        QDate date = parseDate(dateStr);
        qDebug() << "Parsed:" << dateStr << "->" << date;
        if (date.isValid() && date.year() < 2020) {
            count++;
        }
    }
    return count;
}
int commande::statistique2() {
    int count = 0;
    QSqlQuery query("SELECT date_commande FROM commande");
    while (query.next()) {
        QString dateStr = query.value(0).toString().trimmed();
        QDate date = parseDate(dateStr);
        qDebug() << "Parsed:" << dateStr << "->" << date;
        if (date.isValid() && date.year() >= 2020) {
            count++;
        }
    }
    return count;
}
// ## Tri ## //
QSqlQueryModel *commande:: afficher_choix(QString choix)
{
    QSqlQueryModel * model =new QSqlQueryModel();
    if(choix == "date commande croissant")
    {
        model->setQuery("SELECT * FROM commande  ORDER BY date_commande ASC ;");
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Commande"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("Adresse"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("Status"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("Client"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Date Commande"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("ID Livreur"));
    }
    else if(choix == "date commande decroissant")
    {
        model->setQuery("SELECT * FROM commande  ORDER BY date_commande DESC;");
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Commande"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("Adresse"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("Status"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("Client"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Date Commande"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("ID Livreur"));
    }
    else if(choix == "identifiant croissant")
    {
        model->setQuery("SELECT * FROM commande  ORDER BY idc ASC;");
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Commande"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("Adresse"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("Status"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("Client"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Date Commande"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("ID Livreur"));
    }
    else if(choix == "identifiant decroissant")
    {
        model->setQuery("SELECT * FROM commande  ORDER BY idc DESC;");
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Commande"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("Adresse"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("Status"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("Client"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Date Commande"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("ID Livreur"));
    }
    else if(choix == "adresse croissant")
    {
        model->setQuery("SELECT * FROM commande  ORDER BY adresse ASC;");
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Commande"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("Adresse"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("Status"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("Client"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Date Commande"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("ID Livreur"));
    }
    else if(choix == "adresse decroissant")
    {
        model->setQuery("SELECT * FROM commande  ORDER BY adresse DESC;");
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Commande"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("Adresse"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("Status"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("Client"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Date Commande"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("ID Livreur"));
    }
    return model ;
}
commande* commande::readcommande(QString val)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM commande WHERE idc = :id");
    query.bindValue(":id", val);
    if(query.exec() && query.next())
    {
        setIdc(query.value("idc").toInt());
        setAdresse(query.value("adresse").toString());
        setStatus(query.value("status").toString());
        setClient(query.value("client").toString());
        QString dateStr = query.value("date_commande").toString();
        setDateCommande(parseDate(dateStr));
        setIdlivreur(query.value("idlivreur").toInt());
    }
    return this;
}

// Add this function implementation
QSqlQueryModel* commande::getExpiringCommands()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT * FROM commande WHERE date_commande = :date");
    query.bindValue(":date", QDate::currentDate().addDays(1));
    query.exec();
    model->setQuery(std::move(query));
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Commande"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Adresse"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Status"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Client"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Date Commande"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("ID Livreur"));
    return model;
}
