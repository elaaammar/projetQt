#ifndef COMMANDE_H
#define COMMANDE_H
#include <qstring>
#include <qsqlquery>
#include <qsqlquerymodel>
#include <qdate>
class commande
{
private:
    int idc,idlivreur;
    QDate date_commande;
    QString adresse,status,client;
public:
    commande();
    commande(int,QString,QString,QString,QDate,int);
    // Getters
    int getIdc() const;
    int getIdlivreur() const;
    QDate getDateCommande() const;
    QString getAdresse() const;
    QString getStatus() const;
    QString getClient() const;
    // Setters
    void setIdc(int);
    void setIdlivreur(int);
    void setDateCommande(QDate);
    void setAdresse(QString);
    void setStatus(QString);
    void setClient(QString);
    //Crud
    bool Ajouter();
    QSqlQueryModel* afficher();
    bool Supprime(int);
    bool modifier();
    //Statistique/
    int statistique1();
    int statistique2();
    //Recherche/
    QSqlQueryModel *afficher_adresse(QString ch);
    QSqlQueryModel *afficher_status(QString ch);
    QSqlQueryModel *afficher_client(QString ch);
    //Tri/
    QSqlQueryModel *afficher_choix(QString choix);
    //PDF/
    commande* readcommande(QString val);

    QSqlQueryModel* getExpiringCommands();
};
#endif // COMMANDE_H
