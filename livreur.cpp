#include "livreur.h"
#include <qsqlquery>
#include <qsqlquerymodel>
#include <qmessagebox>
#include <qobject>
#include <qdebug>
// Helper for parsing dates
QDate parseDatelivreur(const QString &dateStr) {
    QDateTime dt = QDateTime::fromString(dateStr, Qt::ISODate);
    if (dt.isValid())
        return dt.date();
    QDate d = QDate::fromString(dateStr, "dd/MM/yy");
    if (!d.isValid())
        d = QDate::fromString(dateStr, "dd/MM/yyyy");
    return d;
}
// ---------------- Constructors ----------------
livreur::livreur() {
    idl = 0;
    nom = prenom = email = telephone = "";
    date_embauche = QDate::currentDate();
}
livreur::livreur(int idl, QString nom, QString prenom, QString email, QString telephone, QDate date_embauche) {
    this->idl = idl;
    this->nom = nom;
    this->prenom = prenom;
    this->email = email;
    this->telephone = telephone;
    this->date_embauche = date_embauche;
}
// ---------------- Getters ----------------
int livreur::getIdl() const { return idl; }
QString livreur::getNom() const { return nom; }
QString livreur::getPrenom() const { return prenom; }
QString livreur::getEmail() const { return email; }
QString livreur::getTelephone() const { return telephone; }
QDate livreur::getDateEmbauche() const { return date_embauche; }
// ---------------- Setters ----------------
void livreur::setIdl(int idl) { this->idl = idl; }
void livreur::setNom(QString nom) { this->nom = nom; }
void livreur::setPrenom(QString prenom) { this->prenom = prenom; }
void livreur::setEmail(QString email) { this->email = email; }
void livreur::setTelephone(QString telephone) { this->telephone = telephone; }
void livreur::setDateEmbauche(QDate date) { this->date_embauche = date; }
// ---------------- CRUD ----------------
bool livreur::Ajouter() {
    QSqlQuery query;
    query.prepare("INSERT INTO livreur (idl, nom, prenom, email, telephone, date_embauche) "
                  "VALUES (:idl, :nom, :prenom, :email, :telephone, :date_embauche)");
    query.bindValue(":idl", idl);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":email", email);
    query.bindValue(":telephone", telephone);
    query.bindValue(":date_embauche", date_embauche);
    if(query.exec()) {
        QMessageBox::information(nullptr, QObject::tr("Succès"), "Livreur ajouté avec succès", QMessageBox::Ok);
        return true;
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Erreur"), "Échec de l'ajout du livreur", QMessageBox::Cancel);
        return false;
    }
}
QSqlQueryModel* livreur::afficher() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM livreur");
    model->setHeaderData(0, Qt::Horizontal, "ID Livreur");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Prenom");
    model->setHeaderData(3, Qt::Horizontal, "Email");
    model->setHeaderData(4, Qt::Horizontal, "Telephone");
    model->setHeaderData(5, Qt::Horizontal, "Date Embauche");
    return model;
}
bool livreur::Supprime(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM livreur WHERE idl = :idl");
    query.bindValue(":idl", id);
    return query.exec();
}
bool livreur::modifier() {
    QSqlQuery query;
    query.prepare("UPDATE livreur SET nom=:nom, prenom=:prenom, email=:email, telephone=:telephone, date_embauche=:date_embauche WHERE idl=:idl");
    query.bindValue(":idl", idl);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":email", email);
    query.bindValue(":telephone", telephone);
    query.bindValue(":date_embauche", date_embauche);
    return query.exec();
}
// ---------------- Recherche ----------------
QSqlQueryModel* livreur::afficher_nom(QString ch) {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM livreur WHERE nom = '"+ch+"'");
    model->setHeaderData(0, Qt::Horizontal, "ID Livreur");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Prenom");
    model->setHeaderData(3, Qt::Horizontal, "Email");
    model->setHeaderData(4, Qt::Horizontal, "Telephone");
    model->setHeaderData(5, Qt::Horizontal, "Date Embauche");
    return model;
}
QSqlQueryModel* livreur::afficher_prenom(QString ch) {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM livreur WHERE prenom = '"+ch+"'");
    model->setHeaderData(0, Qt::Horizontal, "ID Livreur");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Prenom");
    model->setHeaderData(3, Qt::Horizontal, "Email");
    model->setHeaderData(4, Qt::Horizontal, "Telephone");
    model->setHeaderData(5, Qt::Horizontal, "Date Embauche");
    return model;
}
QSqlQueryModel *livreur::afficher_livreur_email(QString email)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM livreur WHERE email = '" + email + "'");
    model->setHeaderData(0, Qt::Horizontal, "ID Livreur");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Prenom");
    model->setHeaderData(3, Qt::Horizontal, "Email");
    model->setHeaderData(4, Qt::Horizontal, "Telephone");
    model->setHeaderData(5, Qt::Horizontal, "Date Embauche");
    return model;
}
// ---------------- Tri ----------------
QSqlQueryModel* livreur::afficher_choix(QString choix) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QString queryStr;
    if(choix == "date embauche croissant")
        queryStr = "SELECT * FROM livreur ORDER BY date_embauche ASC";
    else if(choix == "date embauche decroissant")
        queryStr = "SELECT * FROM livreur ORDER BY date_embauche DESC";
    else if(choix == "nom croissant")
        queryStr = "SELECT * FROM livreur ORDER BY nom ASC";
    else if(choix == "nom decroissant")
        queryStr = "SELECT * FROM livreur ORDER BY nom DESC";
    else if(choix == "email croissant")
        queryStr = "SELECT * FROM livreur ORDER BY email ASC";
    else if(choix == "email decroissant")
        queryStr = "SELECT * FROM livreur ORDER BY email DESC";
    model->setQuery(queryStr);
    return model;
}
// ---------------- Statistique ----------------
int livreur::statistique1() {
    int count = 0;
    QSqlQuery query("SELECT date_embauche FROM livreur");
    while(query.next()) {
        QDate date = parseDatelivreur(query.value(0).toString());
        if(date.isValid() && date.year() < 2020) count++;
    }
    return count;
}
int livreur::statistique2() {
    int count = 0;
    QSqlQuery query("SELECT date_embauche FROM livreur");
    while(query.next()) {
        QDate date = parseDatelivreur(query.value(0).toString());
        if(date.isValid() && date.year() >= 2020) count++;
    }
    return count;
}
// ---------------- Read ----------------
livreur* livreur::readlivreur(QString val) {
    QSqlQuery query;
    query.prepare("SELECT * FROM livreur WHERE idl = :id");
    query.bindValue(":id", val);
    if(query.exec() && query.next()) {
        setIdl(query.value("idl").toInt());
        setNom(query.value("nom").toString());
        setPrenom(query.value("prenom").toString());
        setEmail(query.value("email").toString());
        setTelephone(query.value("telephone").toString());
        setDateEmbauche(parseDatelivreur(query.value("date_embauche").toString()));
    }
    return this;
}
QSqlQueryModel* livreur::getAllLivreurs()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT idl, nom FROM livreur");
    query.exec();
    model->setQuery(std::move(query));
    return model;
}
