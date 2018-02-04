#include <QSettings>
#include <QGraphicsView>
#include <QDialog>
#include <QLineEdit>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scene/scene.h"
#include "scene/view.h"
#include "scene/object.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    ui->tableTopScore->setSortingEnabled(false);

    // Восстановление настроек
    settings = new QSettings("snake.ini", QSettings::IniFormat, this);
    if(settings->value("SPEED").isValid())
        ui->spinBoxSpeed->setValue(settings->value("SPEED").toInt());
    for(int i=0;i<topScoreSize;i++)
        if(settings->value(QString("SCORE_%1").arg(i)).isValid()){
            insertScore( settings->value(QString("SCORE_%1").arg(i)).toLongLong(),
                    settings->value(QString("NAME__%1").arg(i)).toString());
        }

    // Настройка основного игрового поля
    scene = new Scene();
    ui->pageGame->layout()->addWidget(new View(scene));
    connect(scene, SIGNAL(endgame()), this, SLOT(stoped()));
    connect(scene, SIGNAL(scoreUp(int)), this, SLOT(scoreUp(int)));
    connect(scene, SIGNAL(infoChanged(QString)), ui->labelInfo, SLOT(setText(QString)));
}

MainWindow::~MainWindow()
{
    settings->setValue("SPEED", ui->spinBoxSpeed->value());
    for(int i=0; i<playerData.size(); i++){
        settings->setValue(QString("SCORE_%1").arg(i), playerData[i].score);
        settings->setValue(QString("NAME__%1").arg(i), playerData[i].name);
    }

    delete ui;
}

void MainWindow::on_pbStart_clicked()
{
    score = 0;
    ui->labelScore->setText("0");
    ui->stackedWidget->setCurrentIndex(1);
    scene->start(ui->spinBoxSpeed->value(), ui->spinBoxWidth->value(), ui->spinBoxHeight->value());
}

void MainWindow::on_pbStop_clicked()
{
    scene->stop();
    stoped();
}

void MainWindow::on_pbExit_clicked()
{
    QApplication::quit();
}

void MainWindow::stoped()
{
    ui->stackedWidget->setCurrentIndex(0);
    if(!score) return;

    // Диалог запроса имени пользователя
    QDialog *dialog = new QDialog(this);
    dialog->setFont(QFont("Comic Sans MS",10));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(new QLabel(QString("Your score: %1").arg(score)));

    if(score && ( playerData.size()<topScoreSize || score>playerData.last().score)){
        layout->addWidget(new QLabel("Congratulation! You win!!!<br>Enter your name:"));

        QLineEdit *lineEdit = new QLineEdit;
        layout->addWidget(lineEdit);
        connect(lineEdit, QLineEdit::editingFinished, [=](){
            QString name = "User";
            if(!lineEdit->text().isEmpty()) name = lineEdit->text();
            dialog->deleteLater();
            insertScore(score,name);
            delete lineEdit; // защита от повтора
        });
    }
    else {
        QPushButton *button = new QPushButton("OK");
        layout->addWidget(button);
        connect(button, SIGNAL(clicked(bool)), dialog, SLOT(deleteLater()));
    }

    dialog->setLayout(layout);
    dialog->show();
}

void MainWindow::scoreUp(const int v)
{
    score += v;
    ui->labelScore->setText(QString("%1").arg(score));

    // Увеличение скорости со временем
    static quint64 old = 0;
    for(quint64 goal=50; old>=(goal/2); goal*=2)
        if(score>=goal && old<goal) scene->increaseSpeed();

    old = score;
}

void MainWindow::insertScore(quint64 score, QString name)
{
    int row=0;
    auto iter = playerData.begin(); // реализация вставки с сохранением индекса на QList
    for(;iter<playerData.cend() && iter->score>score; iter++, row++);

    playerData.insert(iter, { score,name });

    ui->tableTopScore->insertRow(row);
    ui->tableTopScore->setItem(row,0,new QTableWidgetItem(name));
    ui->tableTopScore->setItem(row,1,new QTableWidgetItem(QString("%1").arg(score)));
    ui->tableTopScore->item(row,0)->setTextAlignment(Qt::AlignCenter);
    ui->tableTopScore->item(row,1)->setTextAlignment(Qt::AlignCenter);

    if(playerData.size()>topScoreSize){
        ui->tableTopScore->removeRow(ui->tableTopScore->rowCount()-1);
        playerData.removeLast();
    }
}

void MainWindow::on_pbClear_clicked()
{
    playerData.clear();
    ui->tableTopScore->clear();
    settings->clear();
}
