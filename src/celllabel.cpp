#include "celllabel.h"
#include <QMouseEvent>

CellLabel::CellLabel(QWidget *parent) : QLabel(parent), isAlive(false)
{
    setStyleSheet("QLabel { border: 1px solid grey; }");
    connect(this, &CellLabel::dead, [this](){ this->deleteLater(); });
}

void CellLabel::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) {
        isAlive = true;
        setStyleSheet("QLabel { background-color: black; }");
    }
}
