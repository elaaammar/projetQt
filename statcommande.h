#ifndef STATCOMMANDE_H
#define STATCOMMANDE_H

#include <QDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include "commande.h"

namespace Ui {
class statcommande;
}

class statcommande : public QDialog
{
    Q_OBJECT

protected:
    void paintEvent(QPaintEvent *);

public:
    explicit statcommande(QWidget *parent = nullptr);
    ~statcommande();

private slots:
    void on_pushButton_clicked();

private:
    Ui::statcommande *ui;
    QVector<QPointF> dataPoints;
    QChartView *chartView;
    QPieSeries *series;
    commande c;
};

#endif // STATCOMMANDE_H
