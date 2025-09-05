#include "statswindow.h"
#include "ui_statswindow.h"

statswindow::statswindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::statswindow)
{
    ui->setupUi(this);
    loadStats();
    buildChart();
}

statswindow::~statswindow()
{
    delete ui;
}

void statswindow::loadStats()
{
    QSqlQuery query;
    query.prepare(
        "SELECT L.NOM, COUNT(C.IDC) AS nb_livraisons "
        "FROM LIVREUR L "
        "LEFT JOIN COMMANDE C ON L.IDL = C.IDLIVREUR "
        "GROUP BY L.NOM"
        );
    query.exec();

    while (query.next()) {
        QString livreur = query.value(0).toString();
        int count = query.value(1).toInt();
        ui->listWidget_stats->addItem(livreur + " - " + QString::number(count));
    }
}

void statswindow::buildChart()
{
    QSqlQuery query;
    query.prepare(
        "SELECT L.NOM, COUNT(C.IDC) AS nb_livraisons "
        "FROM LIVREUR L "
        "LEFT JOIN COMMANDE C ON L.IDL = C.IDLIVREUR "
        "GROUP BY L.NOM"
        );
    query.exec();

    QBarSeries *series = new QBarSeries();
    QBarSet *set = new QBarSet("Livraisons");   // ✅ one dataset for all livreurs
    QStringList categories;

    while (query.next()) {
        QString livreur = query.value(0).toString();
        int count = query.value(1).toInt();

        *set << count;          // add the count to the dataset
        categories << livreur;  // add livreur name to X-axis
    }

    series->append(set);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Nombre de livraisons par livreur");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Nb Livraisons");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    if (!chartView) {
        chartView = new QChartView(chart, ui->label_chart); // parent = label
        chartView->setRenderHint(QPainter::Antialiasing);
        chartView->setGeometry(ui->label_chart->rect());
        chartView->show();
    } else {
        chartView->setChart(chart); // replace chart if chartView exists
    }
}
