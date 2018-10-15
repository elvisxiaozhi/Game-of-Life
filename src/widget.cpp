#include "widget.h"
#include "ui_widget.h"
#include <QKeyEvent>

const int Widget::border = 20;
const int Widget::hiddenPlaces = 10;
const int Widget::rows = 30 + Widget::border * 2 + Widget::hiddenPlaces * 2;
const int Widget::cols = 30 + Widget::border * 2 + Widget::hiddenPlaces * 2;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("Game of Life");
    setFixedSize(800, 700);

    hasStarted = false;

    setGameLayout();
    setSettingsLayout();

    setExploder();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::setSettingsLayout()
{
    //install event filter to block signals from QComobox, so when the key is pressed, the index of QComobox will not change
    ui->comboBox->installEventFilter(this);

    ui->comboBox->addItem("Clear");
    ui->comboBox->addItem("Glider");
    ui->comboBox->addItem("Small Exploder");
    ui->comboBox->addItem("Exploder");
    ui->comboBox->addItem("10 Cell Row");
    ui->comboBox->addItem("Lightweight Spaceship");
    ui->comboBox->addItem("Tumbler");
    ui->comboBox->addItem("Gosper Glider Gun");
    ui->comboBox->setCurrentIndex(1);

//    ui->speedLbl->setPixmap(QPixmap(":/icons/speed.png").scaled(40, 40, Qt::KeepAspectRatio));
//    ui->enlargeLbl->setPixmap(QPixmap(":/icons/enlarge.png").scaled(40, 40, Qt::KeepAspectRatio));
    ui->speedLbl->setText("Speed: ");
    ui->enlargeLbl->setText("Enlarge: ");

    ui->speedSlider->setMinimum(1);
    ui->speedSlider->setMaximum(10);

    ui->enlargeSlider->setMinimum(0);
    ui->enlargeSlider->setMaximum(20);

    connect(ui->enlargeSlider, &QSlider::valueChanged, [this](int){ changeBoardSize(); });
}

void Widget::setGameLayout()
{
    for(int i = 0; i < rows; ++i) {
        QVector<CellLabel *> colVec;
        for(int j = 0; j < cols; ++j) {
            CellLabel *cell = new CellLabel(this, i, j);
            cell->hide();

            ui->gameLayout->addWidget(cell, i, j);

            colVec.push_back(cell);
        }
        cellLblVec.push_back(colVec);
    }

    changeBoardSize();

    timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, &Widget::timerTimeOut);
    connect(ui->nextButton, &QPushButton::clicked, this, &Widget::timerTimeOut);
    connect(ui->startAndStopBtn, &QPushButton::clicked, this, &Widget::startOrStopGame);
    connect(ui->clearButton, &QPushButton::clicked, this, &Widget::clearCells);
    connect(ui->speedSlider, &QSlider::valueChanged, [this](int value) {
        if(hasStarted) {
            timer->start(1000 / value);
        }
    });
}

void Widget::changeBoardSize()
{
    qDebug() << ui->enlargeSlider->value();
    for(int i = 0; i < rows; ++i) {
        for(int j = 0; j < cols; ++j) {
            if(i >= border + hiddenPlaces - ui->enlargeSlider->value() &&
                    i < rows - border - hiddenPlaces + ui->enlargeSlider->value() &&
                    j >= border + hiddenPlaces - ui->enlargeSlider->value() &&
                    j < cols - border - hiddenPlaces + ui->enlargeSlider->value()) {
                cellLblVec[i][j]->show();
            }
            else {
                cellLblVec[i][j]->hide();
            }
        }
    }
}

int Widget::returnNeighborNums(int row, int col)
{
    int nums = 0;
    for(int i = -1; i < 2; ++i) {
        for(int j = -1; j < 2; ++j) {
            if(row + i >= 0 && row + i < rows && col + j >= 0 && col + j < cols) {
                if(row + i == row && col + j == col) {
                    continue;
                }
                else if(cellLblVec[row + i][col + j]->isAlive) {
                    ++nums;
                }
            }
        }
    }

    return nums;
}

void Widget::addReproductCellPos(int row, int col)
{
    for(int i = -1; i < 2; ++i) {
        for(int j = -1; j < 2; ++j) {
            if(row + i >= 0 && row + i < rows && col + j >= 0 && col + j < cols) {
                if(row + i == row && col + j == col) {
                    continue;
                }
                else if(returnNeighborNums(row + i, col + j) == 3) {
                    reproductCellPos.push_back(std::make_pair(row + i, col + j));
                }
            }
        }
    }
}

void Widget::addAboutToDieCellPos(int row, int col)
{
    if(returnNeighborNums(row, col) < 2 || returnNeighborNums(row, col) > 3) {
        aboutToDieCellPos.push_back(std::make_pair(row, col));
    }
}

void Widget::changeCellStatus()
{
    for(int i = 0; i < reproductCellPos.size(); ++i) {
        cellLblVec[reproductCellPos[i].first][reproductCellPos[i].second]->alive();
    }

    for(int i = 0; i < aboutToDieCellPos.size(); ++i) {
        cellLblVec[aboutToDieCellPos[i].first][aboutToDieCellPos[i].second]->dead();
    }
}

void Widget::setGlider()
{
    cellLblVec[rows / 2 - 1][cols / 2]->alive();
    cellLblVec[rows / 2][cols / 2 + 1]->alive();
    for(int i = 0; i < 3; ++i) {
        cellLblVec[rows / 2 + 1][cols / 2 - 1 + i]->alive();
    }
}

void Widget::setSmallExploder()
{
    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < 3; ++j) {
            cellLblVec[rows / 2 - 2 + i][cols / 2 - 1 + j]->alive();
        }
    }

    cellLblVec[rows / 2][cols / 2]->dead();
    cellLblVec[rows / 2 - 2][cols / 2 - 1]->dead();
    cellLblVec[rows / 2 - 2][cols / 2 + 1]->dead();
    cellLblVec[rows / 2 + 1][cols / 2 - 1]->dead();
    cellLblVec[rows / 2 + 1][cols / 2 + 1]->dead();
}

void Widget::setExploder()
{
    for(int i = - 5; i <= 5; ++i) {
        if(i == 0) {
            continue;
        }
        else {
            cellLblVec[rows / 2][cols / 2 + i]->alive();
        }
    }

    for(int i = -1; i < 2; ++i) {
        if(i == 0) {
            cellLblVec[rows / 2 + i][cols / 2 - 3]->dead();
            cellLblVec[rows / 2 + i][cols / 2 + 3]->dead();
        }
        else {
            cellLblVec[rows / 2 + i][cols / 2 - 3]->alive();
            cellLblVec[rows / 2 + i][cols / 2 + 3]->alive();
        }
    }
}

bool Widget::eventFilter(QObject *watched, QEvent *event)
{
    if(ui->comboBox) {
        if(ui->comboBox == watched && event->type() == QEvent::KeyPress){
           QKeyEvent *key = static_cast<QKeyEvent *>(event);
           if(!key->text().isEmpty())
                return true;
        }
    }

    return QObject::eventFilter(watched, event);
}

void Widget::timerTimeOut()
{
    reproductCellPos.clear();
    aboutToDieCellPos.clear();

    for(int i = 0; i < rows; ++i) {
        for(int j = 0; j < cols; ++j) {
            if(cellLblVec[i][j]->isAlive) {
                addReproductCellPos(i, j);
                addAboutToDieCellPos(i, j);
            }
        }
    }

    changeCellStatus();
}

void Widget::clearCells()
{
    timer->stop();
    hasStarted = false;

    for(int i = 0; i < rows; ++i) {
        for(int j = 0; j < cols; ++j) {
            if(cellLblVec[i][j]->isAlive) {
                cellLblVec[i][j]->dead();
            }
        }
    }
}

void Widget::startOrStopGame()
{
    if(hasStarted) {
        timer->stop();
        hasStarted = false;
        ui->startAndStopBtn->setText("Start");
    }
    else {
        timer->start(1000 / ui->speedSlider->value());
        hasStarted = true;
        ui->startAndStopBtn->setText("Stop");
    }
}
