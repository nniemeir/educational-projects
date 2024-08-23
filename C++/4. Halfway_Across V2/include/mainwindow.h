#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QEvent> // Include QEvent header if necessary
#include <QFile>
#include <QKeyEvent> // Include QKeyEvent header
#include <QLineEdit>
#include <QPixmap>
#include <QtCore>
#include "locations.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setLocation(const Location& object);
    void setDescription(QString text);
    void closeProgram();

private:
    Ui::MainWindow *ui;
    void importStylesheet();
    void setUiProperties();

private slots:
void handleReturnPressed();
};


#endif // MAINWINDOW_H
