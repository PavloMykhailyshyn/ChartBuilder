#include "chartbuilder.h"
#include "ui_chartbuilder.h"

ChartBuilder::ChartBuilder(QWidget *parent) : QMainWindow(parent), ui(new Ui::ChartBuilder)

{
    ui->setupUi(this);
    known_formulas_ =
    {
        "sin(x)",
        "cos(x)",
        "ln(x)",
        "x*x*x",
        "|x|",
        "sqrt(x)"
    };
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
        if (ui->formulaInput->text() == "")
        {
            const int points = 2;
            QVector<double> x(points), y(points);
            x[0] = ui->X1->text().toDouble();
            x[1] = ui->X2->text().toDouble();
            y[0] = ui->Y1->text().toDouble();
            y[1] = ui->Y2->text().toDouble();
            QString inputed_points = QString::number(x[0]) + QString::number(x[1]) + QString::number(y[0]) + QString::number(y[1]);
            QVector<QString>::iterator iter = qFind(inputed_formulas_.begin(), inputed_formulas_.end(), inputed_points);
            if(iter == inputed_formulas_.end())
            {
                inputed_formulas_.push_back(inputed_points);
            }
            else
            {
                ui->customPlot->removeGraph(iter - inputed_formulas_.begin());
            }
            ui->customPlot->addGraph();
            ui->customPlot->graph()->setData(x, y);
            pen_.setColor(Qt::blue);
            ui->customPlot->graph()->setPen(pen_);
            pens_.push_back(pen_);
            ui->customPlot->replot();
            saved_ = false;
        }
        else
        {
           QString inputed_formula = ui->formulaInput->text();
           bool quadratic_function = false;
           QVector<QString>::iterator it = qFind(known_formulas_.begin(), known_formulas_.end(), inputed_formula);
           if(it == known_formulas_.end())
           {
               const int amount_of_str = 9;
               QVector <QString> str(amount_of_str);
               for(int i(0); i < amount_of_str; ++i)
               {
                   str[i] = inputed_formula;
               }
               if((str[0].remove(QRegExp("[^0-9]")) != "") &&
                       (str[1].remove(QRegExp("[0-9.]")) == "*x*x+*x+" || str[2].remove(QRegExp("[0-9.]")) == "*x*x-*x-" ||
                        str[3].remove(QRegExp("[0-9.]")) == "*x*x-*x+" || str[4].remove(QRegExp("[0-9.]")) == "*x*x+*x-" ||
                        str[5].remove(QRegExp("[0-9.]")) == "-*x*x+*x+" || str[6].remove(QRegExp("[0-9.]")) == "-*x*x-*x-" ||
                        str[7].remove(QRegExp("[0-9.]")) == "-*x*x+*x-" || str[8].remove(QRegExp("[0-9.]")) == "-*x*x-*x+"))
                   quadratic_function = true;
           }
           if(quadratic_function || it != known_formulas_.end())
           {
               QVector<QString>::iterator iter = qFind(inputed_formulas_.begin(), inputed_formulas_.end(), inputed_formula);
               if(iter == inputed_formulas_.end())
               {
                   inputed_formulas_.push_back(inputed_formula);
               }
               else
               {
                   ui->customPlot->removeGraph(iter - inputed_formulas_.begin());
               }
               QPair <QVector<double>, QVector<double> > pair = graph_.BuildGraphByFormula(inputed_formula);
               ui->customPlot->addGraph();
               ui->customPlot->graph()->setData(pair.first, pair.second);
               QTime midnight(0,0,0);
               qsrand(midnight.secsTo(QTime::currentTime()));
               pen_.setColor(QColor(qrand() % 256, qrand() % 256, qrand() % 256));
               ui->customPlot->graph()->setPen(pen_);
               pens_.push_back(pen_);
               ui->customPlot->replot();
               saved_ = false;
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
        selected_ = false;
        for (int i(0); i < ui->customPlot->graphCount(); ++i)
        {
            QCPGraph *graph = ui->customPlot->graph(i);
            if (graph->selected())
            {
                temp = i;
                selected_ = true;
                break;
            }
        }
        if (selected_)
        {
            ui->customPlot->removeGraph(temp);
            pens_.erase(pens_.begin() + temp);
            inputed_formulas_.erase(inputed_formulas_.begin() + temp);
            ui->customPlot->replot();
            saved_ = false;
            if (ui->customPlot->graphCount() == 0)
                saved_ = true;
        }
        else if (!selected_)
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
        if (date.day() < 10)
            name += "0";
        name += QString::number(date.day()) + ".";
        if (date.month() < 10)
            name += "0";
        name += QString::number(date.month()) + ".";
        name += QString::number(date.year()) + "_";
        if (time.hour() < 10)
            name += "0";
        name += QString::number(time.hour()) + "-";
        if (time.minute() < 10)
            name += "0";
        name += QString::number(time.minute()) + "-";
        if (time.second() < 10)
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
            saved_ = true;
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
        selected_ = false;
        for (int i(0); i < ui->customPlot->graphCount(); ++i)
        {
            QCPGraph *graph = ui->customPlot->graph(i);
            if (graph->selected())
            {
                temp = i;
                selected_ = true;
                break;
            }
        }
        if (selected_)
        {
            changed_ = false;
            if (ui->LineThickness->currentText() != "None")
            {
                QString line = ui->LineThickness->currentText().left(ui->LineThickness->currentText().indexOf(' '));
                int width = line.toInt();
                pens_[temp].setWidth(width);
                ui->customPlot->graph(temp)->setPen(pens_[temp]);
                saved_ = false;
                changed_ = true;
            }
            if (ui->Color->currentText() != "None")
            {
                pens_[temp].setColor(ui->Color->currentText());
                ui->customPlot->graph(temp)->setPen(pens_[temp]);
                saved_ = false;
                changed_ = true;
            }
            if (ui->LineType->currentText() != "None")
            {
                int index = ui->LineType->currentIndex();
                switch (index)
                {
                case 1:
                    pens_[temp].setStyle(Qt::SolidLine);
                    break;
                case 2:
                    pens_[temp].setStyle(Qt::DashLine);
                    break;
                case 3:
                    pens_[temp].setStyle(Qt::DotLine);
                    break;
                case 4:
                    pens_[temp].setStyle(Qt::DashDotLine);
                    break;
                case 5:
                    pens_[temp].setStyle(Qt::DashDotDotLine);
                    break;
                }
                ui->customPlot->graph(temp)->setPen(pens_[temp]);
                saved_ = false;
                changed_ = true;
            }
            if(changed_)
                ui->customPlot->replot();
            else
            {
                QMessageBox msgBox;
                msgBox.setIcon(QMessageBox::Information);
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setText("Select appropriate options to change graph style.");
                msgBox.setWindowTitle("Info");
                msgBox.exec();
            }
        }
        else if (!selected_)
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
    if (!saved_)
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
    pens_.clear();
    inputed_formulas_.clear();
    ui->customPlot->replot();
    saved_ = true;
}

void ChartBuilder::on_actionExit_triggered()
{
    ExitApp();
}

void ChartBuilder::on_actionInfo_triggered()
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setText("To plot the graph you can enter a formula like:\
    ""sin(x)"",\
    ""cos(x)"",\
    ""ln(x)"",\
    ""x*x*x"",\
    "" |x|"",\
    ""sqrt(x)"",\
    ""a*x*x+b*x+c"".");
    msgBox.setWindowTitle("Information");
    msgBox.exec();
}
