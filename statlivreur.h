#ifndef STATLIVREUR_H
#define STATLIVREUR_H

#include <QDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include "livreur.h"

namespace Ui {
class statlivreur;
}

class statlivreur : public QDialog
{
    Q_OBJECT

public:
    explicit statlivreur(QWidget *parent = nullptr);
    ~statlivreur();

private slots:
    void on_pushButton_clicked();

private:
    Ui::statlivreur *ui;
    QChartView *chartView;
    QPieSeries *series;
    livreur l;
};

#endif // STATLIVREUR_H
