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
    ui->label_outputArea->setWordWrap(true);
    ui->pushButton_decrypt->setEnabled(false);
    ui->actionSave->setEnabled(false);
    ui->pushButton_encrypt->setEnabled(false);
    ui->label_outputArea->setText("...");
    ui->label_outputArea->setAlignment(Qt::AlignLeft);
    setWindowTitle("ROT");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_File_triggered()
{
    file_name = QFileDialog::getOpenFileName(this, "Open a file");
    if (file_name != "") {
    QString preText = readFile( file_name);
    ui->label_outputArea->setWordWrap(true);
    ui->label_outputArea->setText(preText);
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
        ui->label_outputArea->setText(postText);
        this->postText = postText;
}

void MainWindow::on_pushButton_encrypt_clicked()
{
        QString target = readFile( file_name);
        numKey = setKey(target);
        QString postText = encrypt(target, numKey);
        ui->label_outputArea->setText(postText);
        this->postText = postText;

}

void MainWindow::on_actionSave_triggered()
{
    QString path = QFileDialog::getSaveFileName(this,
                                                    tr("Save Output Text"), "",
                                                    tr("Text File (*.txt);;All Files (*)"));
    QString strPath= path;
    writeFile(postText, strPath);
}

