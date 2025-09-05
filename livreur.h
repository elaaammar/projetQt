#ifndef LIVREUR_H
#define LIVREUR_H
#include <qstring>
#include <qsqlquery>
#include <qsqlquerymodel>
#include <qdate></qdate></qsqlquerymodel></qsqlquery></qstring>
class livreur
{
private:
    int idl;
    QString nom, prenom, email, telephone;
    QDate date_embauche;
public:
    livreur();
    livreur(int, QString, QString, QString, QString, QDate);
    // Getters
    int getIdl() const;
    QString getNom() const;
    QString getPrenom() const;
    QString getEmail() const;
    QString getTelephone() const;
    QDate getDateEmbauche() const;
    // Setters
    void setIdl(int);
    void setNom(QString);
    void setPrenom(QString);
    void setEmail(QString);
    void setTelephone(QString);
    void setDateEmbauche(QDate);
    // CRUD
    bool Ajouter();
    QSqlQueryModel* afficher();
    bool Supprime(int);
    bool modifier();
    // Recherche
    QSqlQueryModel* afficher_nom(QString);
    QSqlQueryModel* afficher_prenom(QString);
    QSqlQueryModel *afficher_livreur_email(QString email);
    // Tri
    QSqlQueryModel* afficher_choix(QString);
    // Statistique
    int statistique1(); // exemple: count before 2020
    int statistique2(); // example: count after 2020
    // PDF / read
    livreur* readlivreur(QString val);
    QSqlQueryModel* getAllLivreurs();
};
#endif // LIVREUR_H
