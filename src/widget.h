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
    static const int border;
    static const int hiddenPlaces;
    static const int rows;
    static const int cols;
    QVector<QVector<CellLabel *> > cellLblVec;
    QTimer *timer;
    QVector<std::pair<int, int> > reproductCellPos;
    QVector<std::pair<int, int> > aboutToDieCellPos;
    bool hasStarted;

    void setSettingsLayout();
    void setGameLayout();
    void addLblsToBoard();
    int returnNeighborNums(int, int);
    void addReproductCellPos(int, int);
    void addAboutToDieCellPos(int, int);
    void changeCellStatus();

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    void timerTimeOut();
    void clearCells();
    void enlargeBoard(int);
};

#endif // WIDGET_H
