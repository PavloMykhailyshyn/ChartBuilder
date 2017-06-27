#ifndef CHARTBUILDER_H
#define CHARTBUILDER_H

#include <QMainWindow>
#include "graph.h"


namespace Ui {
class ChartBuilder;
}

class ChartBuilder : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChartBuilder(QWidget *parent = 0);
    ~ChartBuilder();

private slots:

    void on_DrawGraph_clicked();

    void lineEditFormulaTextChanged();

    void lineEditPointsTextChanged();

    void on_Delete_clicked();

    void on_Save_clicked();

    void on_Change_clicked();

    void on_Exit_clicked();

    void on_actionSave_triggered();

    void on_actionDelete_triggered();

    void on_actionExit_triggered();

    void on_actionInfo_triggered();

private:
    Ui::ChartBuilder *ui;
    Graph graph_;
    QVector <QString> known_formulas_;
    QPen pen_;
    QVector <QPen> pens_;
    QVector <QString> inputed_formulas_;
    bool saved_ = true;
    bool selected_ = false;
    bool changed_ = false;
    void SaveGraph();
    void DeleteGraph();
    void ExitApp();
};

#endif // CHARTBUILDER_H
