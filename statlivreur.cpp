#include "statlivreur.h"
#include "ui_statlivreur.h"
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>

statlivreur::statlivreur(QWidget *parent)
    : QDialog(parent),
    ui(new Ui::statlivreur),
    chartView(nullptr),
    series(nullptr)
{
    ui->setupUi(this);

    // Create pie series
    series = new QPieSeries();
    int active = l.statistique1();   // number of livreurs hired before 2020
    int inactive = l.statistique2(); // number of livreurs hired after 2020

    float total = active + inactive;
    if (total == 0) total = 1; // avoid division by zero

    float pActive = (active * 100.0f) / total;
    float pInactive = (inactive * 100.0f) / total;

    series->append("Avant 2020", active);
    series->append("Après 2020", inactive);

    auto sliceActive = series->slices().at(0);
    sliceActive->setLabel(QString("Avant 2020: %1%").arg(pActive, 0, 'f', 1));
    sliceActive->setExploded(true);
    sliceActive->setLabelVisible(true);

    auto sliceInactive = series->slices().at(1);
    sliceInactive->setLabel(QString("Après 2020: %1%").arg(pInactive, 0, 'f', 1));
    sliceInactive->setLabelVisible(true);

    // Create chart
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des livreurs avant et après 2020");
    chart->legend()->setAlignment(Qt::AlignBottom);

    // Attach chart to chartView
    chartView = new QChartView(chart, ui->label);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(850, 600);
    chartView->show();
}

statlivreur::~statlivreur()
{
    delete ui;
}

void statlivreur::on_pushButton_clicked()
{
    close();
}
