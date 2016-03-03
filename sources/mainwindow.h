#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    enum class ButtonVisible {
        STOP,
        RELOAD
    };

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    void updateButtons(ButtonVisible button);

public slots:
    void loadStarted();
    void loadFinished(bool);
    void openHomePage();
    void openPage();
    void savePage();
    void printPage();
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
