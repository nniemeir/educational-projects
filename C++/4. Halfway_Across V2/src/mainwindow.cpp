#include "../include/mainwindow.h"
#include "src/ui_mainwindow.h"
#include "../include/handling.h"
#include "../include/world.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  importStylesheet();
  setUiProperties();
  setWindowTitle("Halfway Across");
  setLocation(intro);
}

void MainWindow::importStylesheet() {
    QFile file(":/stylesheets/stylesheet.qss");
    if (!file.open(QFile::ReadOnly)) {
        qWarning() << "Failed to open stylesheet file:" << file.errorString();
        return;
    }
    QString styleSheet = QLatin1String(file.readAll());
    qApp->setStyleSheet(styleSheet);
}

void MainWindow::setUiProperties() {
    ui->outputArea->setReadOnly(true);
    ui->outputArea->setAlignment(Qt::AlignLeft);
    ui->outputArea->setTextInteractionFlags(Qt::NoTextInteraction);
    connect(ui->inputText, &QLineEdit::returnPressed, this,
            &MainWindow::handleReturnPressed);
}

void MainWindow::setDescription(QString text) { ui->outputArea->setText(text); }

void MainWindow::closeProgram() {
    QApplication::quit();
}

void MainWindow::setLocation(const Location &object) {
  setDescription(object.description);
  QPixmap pix(object.image);
  ui->settingImage->setPixmap(pix);
  world.currentLocation = object.name;
}

void MainWindow::handleReturnPressed() {
  QString input = ui->inputText->text();

  if (handle.validateVerb(input)) {
    handle.splitInput(this, input);
  } else {
      if (input != "") {
          setDescription(QString("You don't know how to %1.").arg(input.toLower()));
      }
  }
  ui->inputText->clear();
}

MainWindow::~MainWindow() { delete ui; }