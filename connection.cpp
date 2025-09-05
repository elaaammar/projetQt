#include "connection.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
connection::connection()
{

}
bool connection::createconnect()
{bool test=false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("2a30source");//inserer le nom de la source de données ODBC
    db.setUserName("projetcpp2a30");//inserer nom de l'utilisateur
    db.setPassword("ela123");//inserer mot de passe de cet utilisateur

    if (db.open())
        test=true;

    return  test;
}
