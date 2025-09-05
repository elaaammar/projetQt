#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qmessagebox>
#include <qsqlquery>
#include <qdebug>
#include <qprinter>
#include <qpainter>
#include <qdir>
#include <qstandardpaths>
#include <qstring>
#include <qregularexpression>
#include <qsystemtrayicon>

void MainWindow::populateLivreurComboBox()
{
    ui->idlivreur->clear();
    QSqlQueryModel* model = l.getAllLivreurs();
    for (int i = 0; i < model->rowCount(); ++i) {
        int id = model->data(model->index(i, 0)).toInt();
        QString nom = model->data(model->index(i, 1)).toString();
        ui->idlivreur->addItem(nom, id);
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    populateLivreurComboBox();
    refreshTable();
    refreshTableLivreur();
    // Initialize system tray icon with a proper Windows-compatible .ico file
    trayIcon = new QSystemTrayIcon(this);
    QIcon appIcon(":/icons/app.ico");  // <-- Add an .ico file to your Qt resources (qrc)
    trayIcon->setIcon(appIcon);
    trayIcon->setVisible(true);
    trayIcon->show();  // <-- Required on Windows for notifications to appear

    qDebug() << "System tray icon initialized, visible:" << trayIcon->isVisible();
    qDebug() << "System tray supported:" << QSystemTrayIcon::isSystemTrayAvailable();
    qDebug() << "Icon valid:" << !appIcon.isNull();

    // Connect notification button
    connect(ui->pushButton_notify, &QPushButton::clicked, this, &MainWindow::showExpirationNotifications);

    // Show notifications on startup
    showExpirationNotifications();
    connect(ui->listWidget, &QListWidget::currentRowChanged, ui->stackedWidget, &QStackedWidget::setCurrentIndex);
    for(int i = 0; i < ui->listWidget->count(); ++i) {
        QListWidgetItem* item = ui->listWidget->item(i);
        item->setTextAlignment(Qt::AlignCenter); // center text
    }
}
MainWindow::~MainWindow()
{
    delete trayIcon;
    delete ui;
}

void MainWindow::showExpirationNotifications()
{
    QSqlQueryModel* expModel = c.getExpiringCommands();
    qDebug() << "Expiring commands count:" << expModel->rowCount();
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        qDebug() << "System tray not available, falling back to QMessageBox";
        QString message = expModel->rowCount() > 0 ? "Commandes trouvées:\n" : "Aucune commande expire demain.";
        for (int i = 0; i < expModel->rowCount(); ++i) {
            QString id = expModel->data(expModel->index(i, 0)).toString();
            QString client = expModel->data(expModel->index(i, 5)).toString();
            QString adresse = expModel->data(expModel->index(i, 3)).toString();
            QString status = expModel->data(expModel->index(i, 4)).toString();
            message += QString("ID: %1, Client: %2, Adresse: %3, Status: %4\n")
                           .arg(id, client, adresse, status);
        }
        QMessageBox::information(this, "Commande Expirant", message);
    } else if (expModel->rowCount() > 0) {
        for (int i = 0; i < expModel->rowCount(); ++i) {
            QString id = expModel->data(expModel->index(i, 0)).toString();
            QString client = expModel->data(expModel->index(i, 5)).toString();
            QString adresse = expModel->data(expModel->index(i, 3)).toString();
            QString status = expModel->data(expModel->index(i, 4)).toString();
            QString message = QString("Commande %1 expire demain !\nClient: %2\nAdresse: %3\nStatus: %4")
                                  .arg(id, client, adresse, status);
            qDebug() << "Showing notification for command ID:" << id;
            trayIcon->showMessage("Commande Expirant", message, QSystemTrayIcon::Warning, 5000);
        }
    } else {
        qDebug() << "No expiring commands found.";
        trayIcon->showMessage("Commande Expirant", "Aucune commande expire demain.", QSystemTrayIcon::Warning, 5000);
    }
    delete expModel; // Clean up
}

/* =================== Commande Input Validation =================== */
bool MainWindow::validateCommandeInputs()
{
    int idc = ui->idc->text().toInt();
    QString adresse = ui->adresse->text().trimmed();
    QString status = ui->status->text().trimmed();
    QString client = ui->client->text().trimmed();
    QDate date = ui->dateEdit->date();
    if(ui->idc->text().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "ID est requis !");
        return false;
    }
    if(adresse.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Adresse est requis !");
        return false;
    }
    if(status.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Status est requis !");
        return false;
    }
    if(client.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Client est requis !");
        return false;
    }
    if(!date.isValid()) {
        QMessageBox::warning(this, "Erreur", "Date invalide !");
        return false;
    }
    return true;
}
/* =================== Clear Inputs =================== */
void MainWindow::clearCommandeInputs()
{
    ui->idc->clear();
    ui->adresse->clear();
    ui->status->clear();
    ui->client->clear();
    ui->dateEdit->setDate(QDate::currentDate());
    ui->idlivreur->setCurrentIndex(0);
}
void MainWindow::clearLivreurInputs()
{
    ui->idl->clear();
    ui->nom->clear();
    ui->prenom->clear();
    ui->email->clear();
    ui->num->clear();
    ui->dateEdit_2->setDate(QDate::currentDate());
}
void MainWindow::refreshTable()
{
    ui->tableView->setModel(c.afficher());
}
/* =================== CRUD =================== */
void MainWindow::on_ajouter_clicked()
{
    int idc = ui->idc->text().toInt();
    QString adresse = ui->adresse->text();
    QString status = ui->status->text();
    QString client = ui->client->text();
    QDate date = ui->dateEdit->date();
    int idlivreur = ui->idlivreur->currentData().toInt();
    commande com(idc, adresse, status, client, date, idlivreur);
    if (com.Ajouter()) {
        refreshTable();
        clearCommandeInputs();
        populateLivreurComboBox();
    }
}
void MainWindow::on_modifier_clicked()
{
    int idc = ui->idc->text().toInt();
    QString adresse = ui->adresse->text();
    QString status = ui->status->text();
    QString client = ui->client->text();
    QDate date = ui->dateEdit->date();
    int idlivreur = ui->idlivreur->currentData().toInt();
    commande com(idc, adresse, status, client, date, idlivreur);
    if (com.modifier()) {
        refreshTable();
        clearCommandeInputs();
        populateLivreurComboBox();
    }
}
/* =================== TableView interactions =================== */
// Single click: fill inputs with row data
void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    if (!index.isValid()) return;
    QAbstractItemModel *model = ui->tableView->model();
    int row = index.row();
    ui->idc->setText(model->data(model->index(row, 0)).toString());
    ui->adresse->setText(model->data(model->index(row, 3)).toString());
    ui->status->setText(model->data(model->index(row, 4)).toString());
    ui->client->setText(model->data(model->index(row, 5)).toString());
    ui->dateEdit->setDate(model->data(model->index(row, 2)).toDate());
    int idlivreur = model->data(model->index(row, 1)).toInt();
    int idx = ui->idlivreur->findData(idlivreur);
    if (idx >= 0) ui->idlivreur->setCurrentIndex(idx);
}
// Double click: delete row
void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    if (!index.isValid()) return;
    int row = index.row();
    int id = ui->tableView->model()->data(ui->tableView->model()->index(row, 0)).toInt();
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Supprimer",
                                  "Voulez-vous supprimer cette commande ?",
                                  QMessageBox::Yes | QMessageBox::No    );
    if (reply == QMessageBox::Yes) {
        c.Supprime(id);
        refreshTable();
        populateLivreurComboBox();
    }
}
#include"statcommande.h"
void MainWindow::on_pushButton_clicked()
{
    statcommande stc;
    stc.exec();
}
void MainWindow::on_pushButton_rech_clicked()
{
    QString val=ui->lineEdit->text();
    QString option=ui->comboBox_rech->currentText();
    if((val!="")&&(option=="Client"))
    {        ui->tableView->setModel(c.afficher_client(val));}
    else if((val!="")&&(option=="Adresse"))
    {
        ui->tableView->setModel(c.afficher_adresse(val));
    }
    else if((val!="")&&(option=="Status"))
    {
        ui->tableView->setModel(c.afficher_status(val));
    }
    ui->lineEdit->setText("");
}
void MainWindow::on_pushButton_2_clicked()
{
    refreshTable();
}
void MainWindow::on_comboBox_tri_activated(int index)
{
    QString choix=ui->comboBox_tri->currentText();
    ui->tableView->setModel(c.afficher_choix(choix));
}
void MainWindow::on_pushButton_pdf_clicked()
{
    QString idpdf = ui->lineEdit_pdf->text();
    commande* cmd;
    commande c;
    cmd = c.readcommande(idpdf);
    if (!cmd) {
        qWarning() << "Commande not found!";
        return;
    }
    QString id = QString::number(cmd->getIdc());
    QString adresse_string = cmd->getAdresse();
    QString status_string = cmd->getStatus();
    QString client_string = cmd->getClient();
    QString date_string = cmd->getDateCommande().toString("dd/MM/yyyy");
    QString idlivreur_string = QString::number(cmd->getIdlivreur());
    QColor titleColor(209, 148, 87);
    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString pdfPath = desktopPath + "/gestion_commande_pdf/" + client_string + "_" + id + ".pdf";
    QDir dir(desktopPath + "/gestion_commande_pdf/");
    if (!dir.exists()) dir.mkpath(".");
    printer.setOutputFileName(pdfPath);
    QPainter painter;
    if (!painter.begin(&printer)) {
        qWarning("Failed to open PDF file for writing");
        return;
    }
    painter.setFont(QFont("Bahnschrift Light", 25));
    painter.setPen(titleColor);
    painter.drawText(130, 230, "Information relative a la commande");
    painter.setPen(Qt::black);
    QPen borderPen(Qt::black);
    borderPen.setWidth(1);
    painter.setPen(borderPen);
    painter.drawText(260, 150, "Gestion Commande");
    painter.setPen(Qt::black);
    painter.drawText(250, 300, "ID : " + id);
    painter.drawText(250, 400, "Adresse : " + adresse_string);
    painter.drawText(250, 500, "Status : " + status_string);
    painter.drawText(250, 600, "Client : " + client_string);
    painter.drawText(250, 700, "Date commande : " + date_string);
    painter.drawText(250, 800, "ID Livreur : " + idlivreur_string);
    painter.end();
    qDebug() << "PDF saved to:" << pdfPath;
}
/* =================== Livreur Table Refresh =================== */
void MainWindow::refreshTableLivreur()
{
    ui->tableView_2->setModel(l.afficher());
}
/* =================== Input Validation =================== */
bool MainWindow::validateLivreurInputs()
{
    QString nom = ui->nom->text().trimmed();
    QString prenom = ui->prenom->text().trimmed();
    QString email = ui->email->text().trimmed();
    QString num = ui->num->text().trimmed();
    if(ui->idl->text().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "ID est requis !");
        return false;
    }
    if(nom.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Nom est requis !");
        return false;
    }
    if(prenom.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Prenom est requis !");
        return false;
    }
    if(email.isEmpty() || !email.contains("@")) {
        QMessageBox::warning(this, "Erreur", "Email invalide !");
        return false;
    }
    QRegularExpression reNum("^\d{8,}$"); // at least 8 digits
    if(num.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Numero invalide !");
        return false;
    }
    return true;
}
/* =================== CRUD =================== */
void MainWindow::on_ajouter_2_clicked()
{
    if(!validateLivreurInputs()) return;
    int id = ui->idl->text().toInt();
    QString nom = ui->nom->text();
    QString prenom = ui->prenom->text();
    QString email = ui->email->text();
    QString telephone = ui->num->text();
    QDate date_embauche = ui->dateEdit_2->date();
    livreur liv(id, nom, prenom, email, telephone, date_embauche);
    if(liv.Ajouter()) {
        refreshTableLivreur();
        clearLivreurInputs();
        populateLivreurComboBox();
    }
}
void MainWindow::on_modifier_2_clicked()
{
    int id = ui->idl->text().toInt();
    QString nom = ui->nom->text();
    QString prenom = ui->prenom->text();
    QString email = ui->email->text();
    QString telephone = ui->num->text();
    QDate date_embauche = ui->dateEdit_2->date();
    livreur liv(id, nom, prenom, email, telephone, date_embauche);
    if(liv.modifier()) {
        refreshTableLivreur();
        clearLivreurInputs();
        populateLivreurComboBox();
    }
}
/* =============== Single Click to Fill =============== */
void MainWindow::on_tableView_2_clicked(const QModelIndex &index)
{
    if(!index.isValid()) return;
    QAbstractItemModel *model = ui->tableView_2->model();
    int row = index.row();
    ui->idl->setText(model->data(model->index(row, 0)).toString());
    ui->nom->setText(model->data(model->index(row, 1)).toString());
    ui->prenom->setText(model->data(model->index(row, 2)).toString());
    ui->email->setText(model->data(model->index(row, 3)).toString());
    ui->num->setText(model->data(model->index(row, 4)).toString());
    ui->dateEdit_2->setDate(model->data(model->index(row, 5)).toDate());
}
/* =============== Double Click to Delete =============== */
void MainWindow::on_tableView_2_doubleClicked(const QModelIndex &index)
{
    if(!index.isValid()) return;
    QAbstractItemModel *model = ui->tableView_2->model();
    int row = index.row();
    int id = model->data(model->index(row, 0)).toInt(); // assuming column 0 = ID
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Supprimer",
                                  "Voulez-vous supprimer ce livreur ?",
                                  QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes) {
        l.Supprime(id);
        refreshTableLivreur();
        populateLivreurComboBox();
    }
}
/* =================== Search =================== */
void MainWindow::on_pushButton_rech_2_clicked()
{
    QString val = ui->lineEdit_rech_2->text();
    QString option = ui->comboBox_rech_2->currentText();
    if(val.isEmpty()) {
        refreshTableLivreur();
        return;
    }
    if(option == "Nom")
        ui->tableView_2->setModel(l.afficher_nom(val));
    else if(option == "Prenom")
        ui->tableView_2->setModel(l.afficher_prenom(val));
    else if(option == "Email")
        ui->tableView_2->setModel(l.afficher_livreur_email(val));
    ui->lineEdit_rech_2->setText("");
}
/* =================== PDF =================== */
void MainWindow::on_pushButton_pdf_2_clicked()
{
QString val = ui->lineEdit_pdf_2->text();
livreur* lv = l.readlivreur(val);
if(!lv) {
QMessageBox::warning(this, "Erreur", "Livreur introuvable !");
return;
}
QString id = QString::number(lv->getIdl());
QString nom = lv->getNom();
QString prenom = lv->getPrenom();
QString email = lv->getEmail();
QString tel = lv->getTelephone();
QString date_str = lv->getDateEmbauche().toString("dd/MM/yyyy");
QColor titleColor(209,148,87);
QPrinter printer;
printer.setOutputFormat(QPrinter::PdfFormat);
QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
QString pdfPath = desktopPath + "/gestion_livreur_pdf/" + nom + "_" + id + ".pdf";
QDir dir(desktopPath + "/gestion_livreur_pdf/");
if(!dir.exists()) dir.mkpath(".");
printer.setOutputFileName(pdfPath);
QPainter painter;
if(!painter.begin(&printer)) {
qWarning("Impossible de créer le PDF !");
return;
}
painter.setFont(QFont("Bahnschrift Light", 25));
painter.setPen(titleColor);
painter.drawText(130, 230, "Information sur le Livreur");
painter.setPen(Qt::black);
painter.drawText(250, 300, "ID : " + id);
painter.drawText(250, 400, "Nom : " + nom);
painter.drawText(250, 500, "Prenom : " + prenom);
painter.drawText(250, 600, "Email : " + email);
painter.drawText(250, 700, "Telephone : " + tel);
painter.drawText(250, 800, "Date Embauche : " + date_str);
painter.end();
QMessageBox::information(this, "PDF Créé", "PDF généré avec succès !");
}
void MainWindow::on_pushButton_refresh_clicked()
{
refreshTableLivreur();
}
void MainWindow::on_comboBox_tri_2_activated(int index)
{
QString choix = ui->comboBox_tri_2->currentText(); // Get current combo box choice
ui->tableView_2->setModel(l.afficher_choix(choix)); // l = your livreur object
}
#include "statlivreur.h"
void MainWindow::on_pushButton_4_clicked()
{
statlivreur sl;
sl.exec();
}

#include "statswindow.h"
void MainWindow::on_pushButton_notify_2_clicked()
{
    statswindow stw;
    stw.exec();
}

