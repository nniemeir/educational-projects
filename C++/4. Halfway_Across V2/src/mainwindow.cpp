#include "../include/mainwindow.h"
#include "../include/audio.h"
#include "../include/handling.h"
#include "../include/locations.h"
#include "../include/world.h"
#include "src/ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  importStylesheet();
  setUiProperties();
  setWindowTitle("Halfway Across");
  setLocation("", "", &intro);
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

void MainWindow::showEvent(QShowEvent *event) {
  QMainWindow::showEvent(event);

  if (ui->inputText) {
    QTimer::singleShot(0, this, [this]() { ui->inputText->setFocus(); });
  }
}

void MainWindow::setDescription(QString text) { ui->outputArea->setText(text); }

void MainWindow::closeProgram() { QApplication::quit(); }

void MainWindow::setLocation(QString currentMusic, QString currentAmbience,
                             Location *object) {
  if (object) {
    setDescription(object->getDescription());
    QPixmap pix(object->getImage());
    ui->settingImage->setPixmap(pix);
    QString musicPath = object->getMusicPath();
    QString ambiencePath = object->getAmbiencePath();
    if (currentMusic != musicPath) {
      musicPlayer.play(musicPath, 1);
    }
    if (currentAmbience != ambiencePath) {
      ambiencePlayer.play(ambiencePath, 1);
    }
    world.setCurrentLocation(object);
  }
}

void MainWindow::handleReturnPressed() {
  QString input = ui->inputText->text();
  input = input.toUpper();

  int validated = handle.validateVerb(input);
  if (validated == 0) {
    handle.splitInput(this, input);
  } else if (validated == 1) {
    handle.handleVerb(this, input, "", world.getCurrentLocation());
  } else {
    if (input != "") {
      setDescription(QString("You don't know how to %1.").arg(input.toLower()));
    }
  }
  ui->inputText->clear();
}

MainWindow::~MainWindow() { delete ui; }
