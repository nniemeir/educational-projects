#include "../include/mainwindow.h"
#include "../include/rot.h"
#include "./ui_mainwindow.h"

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

  QObject::connect(ui->saveButton, &QPushButton::clicked, this,
                   &MainWindow::saveFile);

  QObject::connect(ui->decryptButton, &QPushButton::clicked, this,
                   &MainWindow::decryptText);

  QObject::connect(ui->encryptButton, &QPushButton::clicked, this,
                   &MainWindow::encryptText);

  QObject::connect(ui->fileButton, &QPushButton::clicked, this,
                   &MainWindow::loadFile);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::loadFile() {
  inputFileName =
      QFileDialog::getOpenFileName(this, "Open a file", QDir::homePath());
  if (inputFileName != "") {
    QString originalFileContents = readFile(inputFileName);
    if (originalFileContents.isEmpty()) {
      QMessageBox::warning(nullptr, "Warning",
                           QString("%1 is empty").arg(inputFileName));
      return;
    }
    ui->sourceTextEdit->setText(originalFileContents);
    ui->decryptButton->setEnabled(true);
    ui->encryptButton->setEnabled(true);
  }
}

int MainWindow::setKey(QString target) {
  QString key = ui->keyLineEdit->text();
  if (key == "") {
    currentKey = findKey(target);
    QMessageBox::information(
        nullptr, "Information",
        QString("The key appears to be %1").arg(currentKey));
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

void MainWindow::decryptText() { processFile(false); }

void MainWindow::encryptText() { processFile(true); }

void MainWindow::saveFile() {
  QString path = QFileDialog::getSaveFileName(
      this, tr("Save Output Text"), QDir::homePath(),
      tr("Text File (*.txt);;All Files (*)"));
  if (path != "") {
    QString strPath = path;
    if (!writeFile(processedFileContents, strPath)) {
      QMessageBox::critical(
          nullptr, "Error",
          QString("Unable to open %1 for writing").arg(strPath));
    }
  }
}
