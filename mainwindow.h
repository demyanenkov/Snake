#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>

namespace Ui {
class MainWindow;
}

class QSettings;
class Scene;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pbStart_clicked();
    void on_pbStop_clicked();
    void on_pbExit_clicked();
    void on_pbClear_clicked();
    void stoped();
    void scoreUp(const int v);

private:
    Ui::MainWindow *ui;
    QSettings *settings;
    Scene *scene;
    quint64 score;

    struct PlayerData { quint64 score; QString name; };
    QList<PlayerData> playerData;

    const int topScoreSize = 10;

    void insertScore(quint64 score, QString name);
};

#endif // MAINWINDOW_H
