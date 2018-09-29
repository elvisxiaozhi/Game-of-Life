#include "widget.h"
#include "ui_widget.h"
#include "celllabel.h"

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
    for(int i = 0; i < 30; ++i) {
        for(int j = 0; j < 30; ++j) {
            CellLabel *cell = new CellLabel(this);
            ui->gameLayout->addWidget(cell, i, j);
        }
    }
}
