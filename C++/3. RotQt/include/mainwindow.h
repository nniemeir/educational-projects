#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>

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
    void decryptText();
    void encryptText();
    void loadFile();
    void saveFile();

private:
    Ui::MainWindow *ui;
    QString inputFileName;
    QString processedFileContents;
    int currentKey;
    int setKey();
    int setKey(QString target);
    void processFile(bool isEncrypting);
};
#endif // MAINWINDOW_H
