#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "celllabel.h"
#include <QTimer>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;
    static const int rows;
    static const int cols;
    QVector<QVector<CellLabel *> > cellLblVec;
    QTimer *timer;

    void setGameLayout();
    int returnNeighborNums(int, int);

private slots:
    void timerTimeOut();
};

#endif // WIDGET_H
