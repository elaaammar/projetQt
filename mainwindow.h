#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <qmainwindow>
#include <qsqlquerymodel>
#include <qmessagebox>
#include <qdate>
#include <qprinter>
#include <qpainter>
#include <qstandardpaths>
#include <qdir>
#include <qsystemtrayicon>
#include "commande.h"
#include "livreur.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    /* ========== Commande Slots ========== */
    void on_ajouter_clicked();
    void on_modifier_clicked();
    void on_tableView_clicked(const QModelIndex &index);
    void on_tableView_doubleClicked(const QModelIndex &index);
    void on_pushButton_clicked();          // Stats
    void on_pushButton_rech_clicked();     // Recherche
    void on_pushButton_2_clicked();        // Refresh
    void on_comboBox_tri_activated(int index);
    void on_pushButton_pdf_clicked();      // PDF export
    /* ========== Livreur Slots ========== */
    void on_ajouter_2_clicked();
    void on_modifier_2_clicked();
    void on_tableView_2_clicked(const QModelIndex &index);
    void on_tableView_2_doubleClicked(const QModelIndex &index);
    void on_pushButton_rech_2_clicked();
    void on_pushButton_refresh_clicked();
    void on_comboBox_tri_2_activated(int index);
    void on_pushButton_4_clicked();        // Stats Livreur
    void on_pushButton_pdf_2_clicked();

    void on_pushButton_notify_2_clicked();

private:
    Ui::MainWindow *ui;
    commande c;   // Commande CRUD object
    livreur l;   // Livreur CRUD object
    QSystemTrayIcon *trayIcon;
    /* ========== Helper Functions ========== */
    void refreshTable();
    void refreshTableLivreur();
    // Input validation
    bool validateCommandeInputs();
    bool validateLivreurInputs();
    // Clear input fields
    void clearCommandeInputs();
    void clearLivreurInputs();
    // Populate Livreur combo box dynamically
    void populateLivreurComboBox();

    void showExpirationNotifications();
};
#endif // MAINWINDOW_H
