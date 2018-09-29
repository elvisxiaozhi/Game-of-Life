#ifndef CELLLABEL_H
#define CELLLABEL_H

#include <QWidget>
#include <QLabel>
#include <QDebug>

class CellLabel : public QLabel
{
    Q_OBJECT
public:
    explicit CellLabel(QWidget *parent = nullptr);
    bool isAlive;

private:

protected:
    void mousePressEvent(QMouseEvent *);

signals:
    void dead();
};

#endif // CELLLABEL_H
