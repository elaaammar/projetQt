#ifndef STATSWINDOW_H
#define STATSWINDOW_H

#include <QDialog>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QListWidget>

namespace Ui {
class statswindow;
}

class statswindow : public QDialog
{
    Q_OBJECT

public:
    explicit statswindow(QWidget *parent = nullptr);
    ~statswindow();

private:
    Ui::statswindow *ui;
    QChartView *chartView = nullptr;
    void loadStats();
    void buildChart();
};

#endif // STATSWINDOW_H
