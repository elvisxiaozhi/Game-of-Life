#ifndef CELLLABEL_H
#define CELLLABEL_H

#include <QWidget>
#include <QLabel>
#include <QDebug>

class CellLabel : public QLabel
{
    Q_OBJECT
public:
    CellLabel(QWidget *parent = nullptr, int row = 0, int col = 0);
    bool isAlive;

private:
    int row, col;

protected:
    void mousePressEvent(QMouseEvent *);

signals:
    void dead();
};

#endif // CELLLABEL_H
