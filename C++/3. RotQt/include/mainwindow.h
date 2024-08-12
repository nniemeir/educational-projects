#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private slots:
    void on_pushButton_decrypt_clicked();

    void on_actionOpen_File_triggered();

    void on_actionSave_triggered();

    void on_pushButton_encrypt_clicked();

private:
    Ui::MainWindow *ui;
    QString file_name;
    std::string postText;
    int numKey;
    int setKey();
    void noFileMsg();
    int setKey(std::string target);
};
#endif // MAINWINDOW_H
