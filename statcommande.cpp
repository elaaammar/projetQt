#include "statcommande.h"
#include "ui_statcommande.h"
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QPainter>

statcommande::statcommande(QWidget *parent)
    : QDialog(parent),
    ui(new Ui::statcommande),
    chartView(nullptr),
    series(nullptr)
{
    ui->setupUi(this);
}

void statcommande::paintEvent(QPaintEvent *)
{
    // Statistics based on commande status
    int enCours = c.statistique1();  // count of commands with status "En cours"
    int termine = c.statistique2();   // count of commands with status "Terminé"

    float total = enCours + termine;
    if (total == 0) total = 1; // avoid division by zero

    float pEnCours = (enCours * 100.0f) / total;
    float pTermine = (termine * 100.0f) / total;

    // Create pie series
    QPieSeries *series = new QPieSeries();
    series->append("Avant 2020", enCours);
    series->append("Apres 2020", termine);

    // Highlight slices with percentages
    auto sliceEnCours = series->slices().at(0);
    sliceEnCours->setLabel(QString("Avant 2020: %1%").arg(pEnCours, 0, 'f', 1));
    sliceEnCours->setExploded(true);
    sliceEnCours->setLabelVisible(true);

    auto sliceTermine = series->slices().at(1);
    sliceTermine->setLabel(QString("Apres 2020: %1%").arg(pTermine, 0, 'f', 1));
    sliceTermine->setLabelVisible(true);

    // Create chart
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des commandes Avant et apres 2020");
    chart->legend()->setAlignment(Qt::AlignBottom);

    // Attach chart to chartView (inside ui->label or a layout)
    chartView = new QChartView(chart, ui->label);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(850, 600);
    chartView->show();
}

statcommande::~statcommande()
{
    delete ui;
}

void statcommande::on_pushButton_clicked()
{
    close();
}
