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
    static const int Clear;
    static const int Glider;
    static const int SmallExploder;
    static const int Exploder;
    static const int TenCellRow;
    static const int LightWeightSpaceship;
    static const int Tumbler;
    static const int GosperGliderGun;
    QVector<QVector<CellLabel *> > cellLblVec;
    QTimer *timer;
    QVector<std::pair<int, int> > reproductCellPos;
    QVector<std::pair<int, int> > aboutToDieCellPos;
    bool hasStarted;

    void setSettingsLayout();
    void setGameLayout();
    void changeBoardSize();
    int returnNeighborNums(int, int);
    void addReproductCellPos(int, int);
    void addAboutToDieCellPos(int, int);
    void changeCellStatus();
    void setGlider();
    void setSmallExploder();
    void setExploder();
    void set10CellRow();
    void setLightweightSpaceship();
    void setTumbler();
    void setGosperGliderGun();

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    void timerTimeOut();
    void clearCells();
    void startOrStopGame();
    void comboBoxChanged(int);
};

#endif // WIDGET_H
