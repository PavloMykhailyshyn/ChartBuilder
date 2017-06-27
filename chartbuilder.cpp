#include "chartbuilder.h"
#include "ui_chartbuilder.h"

ChartBuilder::ChartBuilder(QWidget *parent) : QMainWindow(parent), ui(new Ui::ChartBuilder)

{
    ui->setupUi(this);
    QSize fixedSize(935, 500);
    ui->centralWidget->setFixedSize(fixedSize);
    setMinimumSize(fixedSize);
    setMaximumSize(fixedSize);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    ui->customPlot->setInteractions(QCP::iRangeZoom | QCP::iRangeDrag | QCP::iSelectLegend | QCP::iSelectPlottables);
    ui->customPlot->xAxis->setLabel("X");
    ui->customPlot->yAxis->setLabel("Y");
    ui->customPlot->xAxis->setRange(-10, 10);
    ui->customPlot->yAxis->setRange(-10, 10);
    connect(ui->formulaInput, SIGNAL(textChanged(QString)), this, SLOT(lineEditFormulaTextChanged()));
    connect(ui->X1, SIGNAL(textChanged(QString)), this, SLOT(lineEditPointsTextChanged()));
    connect(ui->X2, SIGNAL(textChanged(QString)), this, SLOT(lineEditPointsTextChanged()));
    connect(ui->Y1, SIGNAL(textChanged(QString)), this, SLOT(lineEditPointsTextChanged()));
    connect(ui->Y2, SIGNAL(textChanged(QString)), this, SLOT(lineEditPointsTextChanged()));
    ui->X1->setValidator(new QRegExpValidator(QRegExp("^-?([0-9][0-9]*|0)(\\.|,)[0-9]{2}"), this));
    ui->X2->setValidator(new QRegExpValidator(QRegExp("^-?([0-9][0-9]*|0)(\\.|,)[0-9]{2}"), this));
    ui->Y1->setValidator(new QRegExpValidator(QRegExp("^-?([0-9][0-9]*|0)(\\.|,)[0-9]{2}"), this));
    ui->Y2->setValidator(new QRegExpValidator(QRegExp("^-?([0-9][0-9]*|0)(\\.|,)[0-9]{2}"), this));
}

ChartBuilder::~ChartBuilder()
{
    delete ui;
}

void ChartBuilder::Graph_built_on_points()
{
    QVector<double> x(2), y(2);
    x[0] = ui->X1->text().toDouble();
    x[1] = ui->X2->text().toDouble();
    y[0] = ui->Y1->text().toDouble();
    y[1] = ui->Y2->text().toDouble();
    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x, y);
}

void ChartBuilder::Graph_built_on_formula(const QString& formula)
{
    if (formula == "sin(x)")
    {
        int amount = 2501;
        QVector<double> x(amount), y(amount);
        int temp = -amount / 2;
        for (int i = 0; i<amount; ++i)
        {
            x[i] = temp / 50.0;
            y[i] = sin(x[i]);
            ++temp;
        }
        ui->customPlot->addGraph();
        ui->customPlot->graph()->setData(x, y);
        return;
    }
    if (formula == "cos(x)")
    {
        int amount = 2501;
        QVector<double> x(amount), y(amount);
        int temp = -amount / 2;
        for (int i = 0; i<amount; ++i)
        {
            x[i] = temp / 50.0;
            y[i] = cos(x[i]);
            ++temp;
        }
        ui->customPlot->addGraph();
        ui->customPlot->graph()->setData(x, y);
        return;
    }
    if (formula == "x*x")
    {
        int amount = 501;
        QVector<double> x(amount), y(amount);
        int temp = -amount / 2;
        for (int i = 0; i<amount; ++i)
        {
            x[i] = temp / 50.0;
            y[i] = x[i] * x[i];
            ++temp;
        }
        ui->customPlot->addGraph();
        ui->customPlot->graph()->setData(x, y);
        return;
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setText("No such formula in database.");
        msgBox.setWindowTitle("Warning!");
        msgBox.exec();
    }
}

void ChartBuilder::on_DrawGraph_clicked()
{
    if (ui->formulaInput->text() == "" &&
        ui->X1->text() == "" && ui->X2->text() == "" &&
        ui->Y1->text() == "" && ui->Y2->text() == "")
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setText("Please input formula or points.");
        msgBox.setWindowTitle("Info");
        msgBox.exec();
    }
    else
    {
        saved = false;
        if (ui->formulaInput->text() == "")
        {
            Graph_built_on_points();
        }
        else
        {
            Graph_built_on_formula(ui->formulaInput->text());
        }
        ui->customPlot->replot();
    }
}

void ChartBuilder::lineEditFormulaTextChanged()
{
    if (ui->formulaInput->text() == "")
    {
        ui->X1->setEnabled(true);
        ui->X2->setEnabled(true);
        ui->Y1->setEnabled(true);
        ui->Y2->setEnabled(true);
    }
    else
    {
        ui->X1->setDisabled(true);
        ui->X2->setDisabled(true);
        ui->Y1->setDisabled(true);
        ui->Y2->setDisabled(true);
    }
}

void ChartBuilder::lineEditPointsTextChanged()
{
    if (ui->X1->text() == "" && ui->X2->text() == "" &&
        ui->Y1->text() == "" && ui->Y2->text() == "")
        ui->formulaInput->setEnabled(true);
    else
        ui->formulaInput->setDisabled(true);
}

void ChartBuilder::DeleteGraph()
{
    if (ui->customPlot->graphCount() > 0)
    {
        int temp;
        selected = false;
        for (int i(0); i < ui->customPlot->graphCount(); ++i)
        {
            QCPGraph *graph = ui->customPlot->graph(i);
            if (graph->selected())
            {
                temp = i;
                selected = true;
            }
        }
        if (selected)
        {
            ui->customPlot->removeGraph(temp);
            ui->customPlot->replot();
            saved = false;
            if (ui->customPlot->graphCount() == 0)
                saved = true;
        }
        else if (!selected)
        {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setText("Click on graph to delete.");
            msgBox.setWindowTitle("Info");
            msgBox.exec();
        }
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setText("Nothing to delete.");
        msgBox.setWindowTitle("Info");
        msgBox.exec();
    }
}

void ChartBuilder::SaveGraph()
{
    if (ui->customPlot->graphCount() > 0)
    {
        QTime time = QTime::currentTime();
        QDate date = QDate::currentDate();
        QString name;
        if (date.day()<10)
            name += "0";
        name += QString::number(date.day()) + ".";
        if (date.month()<10)
            name += "0";
        name += QString::number(date.month()) + ".";
        name += QString::number(date.year()) + "_";
        if (time.hour()<10)
            name += "0";
        name += QString::number(time.hour()) + "-";
        if (time.minute()<10)
            name += "0";
        name += QString::number(time.minute()) + "-";
        if (time.second()<10)
            name += "0";
        name += QString::number(time.second());
        QDir dir;
        QString outputDir = dir.currentPath();
        QFile file(name + ".png");
        file.open(QIODevice::WriteOnly);
        QMessageBox msgBox;
        msgBox.setStandardButtons(QMessageBox::Ok);
        if (ui->customPlot->savePng(outputDir + "/" + name + ".png", 0, 0, 1.0, -1))
        {
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setText("Direction: " + outputDir + "/" + name + ".png");
            msgBox.setWindowTitle("Saved!");
            saved = true;
        }
        else
        {
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setText("Error: cannot save.");
            msgBox.setWindowTitle("Error!");
        }
        msgBox.exec();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setText("Nothing to save.");
        msgBox.setWindowTitle("Info");
        msgBox.exec();
    }
}

void ChartBuilder::on_Change_clicked()
{
    if (ui->customPlot->graphCount() > 0)
    {
        int temp;
        selected = false;
        for (int i(0); i < ui->customPlot->graphCount(); ++i)
        {
            QCPGraph *graph = ui->customPlot->graph(i);
            if (graph->selected())
            {
                temp = i;
                selected = true;
            }
        }
        if (selected)
        {
            QPen pen;
            pen.setColor(Qt::blue);
            if (ui->LineThickness->currentText() != "None")
            {
                QString line = ui->LineThickness->currentText().left(ui->LineThickness->currentText().indexOf(' '));
                int width = line.toInt();
                pen.setWidth(width);
                ui->customPlot->graph(temp)->setPen(pen);
                saved = false;
            }
            if (ui->Color->currentText() != "None")
            {
                pen.setColor(ui->Color->currentText());
                ui->customPlot->graph(temp)->setPen(pen);
                saved = false;
            }
            if (ui->LineType->currentText() != "None")
            {
                int index = ui->LineType->currentIndex();
                switch (index)
                {
                case 1:
                    pen.setStyle(Qt::SolidLine);
                    break;
                case 2:
                    pen.setStyle(Qt::DashLine);
                    break;
                case 3:
                    pen.setStyle(Qt::DotLine);
                    break;
                case 4:
                    pen.setStyle(Qt::DashDotLine);
                    break;
                case 5:
                    pen.setStyle(Qt::DashDotDotLine);
                    break;
                }
                ui->customPlot->graph(temp)->setPen(pen);
                saved = false;
            }
            ui->customPlot->replot();
        }
        else if (!selected)
        {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setText("Click on graph to change.");
            msgBox.setWindowTitle("Info");
            msgBox.exec();
        }
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setText("Nothing to change.");
        msgBox.setWindowTitle("Info");
        msgBox.exec();
    }
}

void ChartBuilder::ExitApp()
{
    if (!saved)
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setText("Exit without saving?");
        msgBox.setWindowTitle("Warning!");
        int ret = msgBox.exec();
        if (ret == QMessageBox::Ok)
            this->close();
    }
    else
        this->close();
}

void ChartBuilder::on_Save_clicked()
{
    SaveGraph();
}

void ChartBuilder::on_Delete_clicked()
{
    DeleteGraph();
}

void ChartBuilder::on_Exit_clicked()
{
    ExitApp();
}

void ChartBuilder::on_actionSave_triggered()
{
    SaveGraph();
}

void ChartBuilder::on_actionDelete_triggered()
{
    ui->customPlot->clearGraphs();
    ui->customPlot->replot();
    saved = true;
}

void ChartBuilder::on_actionExit_triggered()
{
    ExitApp();
}
