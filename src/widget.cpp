#include "widget.h"
#include "ui_widget.h"
#include "celllabel.h"

const int Widget::rows = 30;
const int Widget::cols = 30;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    setGameLayout();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::setGameLayout()
{
    for(int i = 0; i < rows; ++i) {
        for(int j = 0; j < cols; ++j) {
            CellLabel *cell = new CellLabel(this, i, j);
            ui->gameLayout->addWidget(cell, i, j);
        }
    }
}
