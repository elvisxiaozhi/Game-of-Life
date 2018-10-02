#include "celllabel.h"
#include <QMouseEvent>

CellLabel::CellLabel(QWidget *parent, int row, int col) : QLabel(parent), isAlive(false)
{
    this->row = row;
    this->col = col;

    setStyleSheet("QLabel { border: 1px solid grey; }");

    connect(this, &CellLabel::alive, [this](){ isAlive = true; setStyleSheet("QLabel { background-color: black; }"); });
    connect(this, &CellLabel::dead, [this](){ isAlive = false; setStyleSheet("QLabel { border: 1px solid grey; }"); });
}

void CellLabel::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) {
        isAlive = true;
        setStyleSheet("QLabel { background-color: black; }");
    }
}
