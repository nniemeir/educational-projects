#include "../include/mainwindow.h"
#include "../include/rot.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  setWindowTitle("ROTQt");

  QFile file(":/stylesheet.qss");
  if (!file.open(QFile::ReadOnly)) {
    qWarning() << "Failed to open stylesheet file:" << file.errorString();
    return;
  }
  QString styleSheet = QLatin1String(file.readAll());
  qApp->setStyleSheet(styleSheet);
  file.close();

  ui->decryptButton->setEnabled(false);
  ui->encryptButton->setEnabled(false);

  ui->sourceTextEdit->setReadOnly(true);
  ui->sourceTextEdit->setText("...");
  ui->sourceTextEdit->setAlignment(Qt::AlignLeft);

  ui->outputTextEdit->setReadOnly(true);
  ui->outputTextEdit->setText("...");
  ui->outputTextEdit->setAlignment(Qt::AlignLeft);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_fileButton_clicked() {
  inputFileName =
      QFileDialog::getOpenFileName(this, "Open a file", QDir::homePath());
  if (inputFileName != "") {
    QString originalFileContents = readFile(inputFileName);
    ui->sourceTextEdit->setText(originalFileContents);
    ui->decryptButton->setEnabled(true);
    ui->encryptButton->setEnabled(true);
  }
}

int MainWindow::setKey(QString target) {
  QString key = ui->keyLineEdit->text();
  if (key == "") {
    currentKey = findKey(target);
  } else {
    currentKey = key.toInt();
  }
  return currentKey;
}

void MainWindow::processFile(bool isEncrypting) {
  QString target = readFile(inputFileName);
  currentKey = setKey(target);
  QString processedFileContents;
  processedFileContents = shiftText(target, currentKey, isEncrypting);
  ui->outputTextEdit->setText(processedFileContents);
  this->processedFileContents = processedFileContents;
}

void MainWindow::on_decryptButton_clicked() { processFile(false); }

void MainWindow::on_encryptButton_clicked() { processFile(true); }

void MainWindow::on_saveButton_clicked() {
  QString path = QFileDialog::getSaveFileName(
      this, tr("Save Output Text"), QDir::homePath(),
      tr("Text File (*.txt);;All Files (*)"));
  if (path != "") {
    QString strPath = path;
    writeFile(processedFileContents, strPath);
  }
}
