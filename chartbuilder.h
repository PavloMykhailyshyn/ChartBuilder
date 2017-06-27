#ifndef CHARTBUILDER_H
#define CHARTBUILDER_H

#include <QMainWindow>
#include "qcustomplot.h"

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

private:
    Ui::ChartBuilder *ui;
    bool saved = true;
    bool selected = false;
    void Graph_built_on_formula(const QString& formula);
    void Graph_built_on_points();
    void SaveGraph();
    void DeleteGraph();
    void ExitApp();
};

#endif // CHARTBUILDER_H
