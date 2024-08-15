#include "../include/mainwindow.h"
#include "../include/rot.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QFile file(":/stylesheet.qss");
    if (!file.open(QFile::ReadOnly)) {
        qWarning() << "Failed to open stylesheet file:" << file.errorString();
        return;
    }
    QString styleSheet = QLatin1String(file.readAll());
    qApp->setStyleSheet(styleSheet);
    file.close();
    ui->pushButton_decrypt->setEnabled(false);
    ui->actionSave->setEnabled(false);
    ui->pushButton_encrypt->setEnabled(false);
    ui->outputArea->setReadOnly(true);
    ui->outputArea->setText("...");
    ui->outputArea->setAlignment(Qt::AlignLeft);
    setWindowTitle("ROTQt");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_File_triggered()
{
    file_name = QFileDialog::getOpenFileName(this, "Open a file",  QDir::homePath());
    if (file_name != "") {
    QString preText = readFile( file_name);
    ui->outputArea->setText(preText);
    ui->pushButton_decrypt->setEnabled(true);
    ui->pushButton_encrypt->setEnabled(true);
    ui->actionSave->setEnabled(true);
    }
}

int MainWindow::setKey(QString target) {
    QString key = ui->lineEdit_key->text();
    if (key == "") {
        numKey = findKey(target);
    }
    else {
    numKey = key.toInt();
    }
    return numKey;
}

void MainWindow::on_pushButton_decrypt_clicked()
{
        QString target = readFile(file_name);
        numKey = setKey(target);
        QString postText = decrypt(target, numKey);
        ui->outputArea->setText(postText);
        this->postText = postText;
}

void MainWindow::on_pushButton_encrypt_clicked()
{
        QString target = readFile( file_name);
        numKey = setKey(target);
        QString postText = encrypt(target, numKey);
        ui->outputArea->setText(postText);
        this->postText = postText;

}

void MainWindow::on_actionSave_triggered()
{
    QString path = QFileDialog::getSaveFileName(this,
                                                    tr("Save Output Text"), QDir::homePath(),
                                                    tr("Text File (*.txt);;All Files (*)"));
     if (path != "") {
    QString strPath= path;
    writeFile(postText, strPath);
     }
     }

