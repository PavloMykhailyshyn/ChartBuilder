#include "graph.h"

Graph::Graph() { }

Graph::~Graph() { }

QPair <QVector<double>, QVector<double> > Graph::BuildGraphByFormula(const QString& formula)
{
    if (formula == "sin(x)")
    {
        const int amount_of_points = 250001;
        QVector<double> x(amount_of_points), y(amount_of_points);
        int temp = -amount_of_points / 2;
        for (int i(0); i < amount_of_points; ++i)
        {
            x[i] = temp / 50.0;
            y[i] = qSin(x[i]);
            ++temp;
        }
        QPair <QVector<double>, QVector<double> > pair_of_points (x, y);
        return pair_of_points;
    }
    if (formula == "cos(x)")
    {
        const int amount_of_points = 250001;
        QVector<double> x(amount_of_points), y(amount_of_points);
        int temp = -amount_of_points / 2;
        for (int i(0); i < amount_of_points; ++i)
        {
            x[i] = temp / 50.0;
            y[i] = qCos(x[i]);
            ++temp;
        }
        QPair <QVector<double>, QVector<double> > pair_of_points (x, y);
        return pair_of_points;
    }
    if (formula == "ln(x)")
    {
        const int amount_of_points = 250001;
        QVector<double> x(amount_of_points), y(amount_of_points);
        int temp = -amount_of_points / 2;
        for (int i(0); i < amount_of_points; ++i)
        {
            x[i] = temp / 50.0;
            y[i] = qLn(x[i]);
            ++temp;
        }
        QPair <QVector<double>, QVector<double> > pair_of_points (x, y);
        return pair_of_points;
    }
    if (formula == "x*x*x")
    {
        const int amount_of_points = 50001;
        QVector<double> x(amount_of_points), y(amount_of_points);
        int temp = -amount_of_points / 2;
        for (int i(0); i < amount_of_points; ++i)
        {
            x[i] = temp / 50.0;
            y[i] = x[i] * x[i] * x[i];
            ++temp;
        }
        QPair <QVector<double>, QVector<double> > pair_of_points (x, y);
        return pair_of_points;
    }
    if (formula == "|x|")
    {
        const int amount_of_points = 50001;
        QVector<double> x(amount_of_points), y(amount_of_points);
        int temp = -amount_of_points / 2;
        for (int i(0); i < amount_of_points; ++i)
        {
            x[i] = temp / 50.0;
            y[i] = qFabs(x[i]);
            ++temp;
        }
        QPair <QVector<double>, QVector<double> > pair_of_points (x, y);
        return pair_of_points;
    }
    if (formula == "sqrt(x)")
    {
        const int amount_of_points = 50001;
        QVector<double> x(amount_of_points), y(amount_of_points);
        int temp = -amount_of_points / 2;
        for (int i(0); i < amount_of_points; ++i)
        {
            x[i] = temp / 50.0;
            y[i] = qSqrt(x[i]);
            ++temp;
        }
        QPair <QVector<double>, QVector<double> > pair_of_points (x, y);
        return pair_of_points;
    }
    const int amount_of_points = 50001;
    QVector<double> x(amount_of_points), y(amount_of_points);
    double a = formula.left(formula.indexOf('*')).toDouble();
    QString b_str;
    for(int i = formula.indexOf('x') + 3, j(0); i < formula.lastIndexOf('*'); ++i, ++j)
    {
        b_str[j] = formula[i];
    }
    double b = b_str.toDouble();
    QString c_str;
    for(int i = formula.lastIndexOf('x') + 1, j(0); i < formula.length(); ++i, ++j)
    {
        c_str[j] = formula[i];
    }
    double c = c_str.toDouble();
    int temp = -amount_of_points / 2;
    for (int i(0); i < amount_of_points; ++i)
    {
        x[i] = temp / 50.0;
        y[i] = a * x[i] * x[i] + b * x[i] + c;
        ++temp;
    }
    QPair <QVector<double>, QVector<double> > pair_of_points (x, y);
    return pair_of_points;
}

