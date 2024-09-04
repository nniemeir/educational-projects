#include "../include/mainwindow.h"
#include <QApplication>
int main(int argc, char *argv[]) {

  QApplication a(argc, argv);
  MainWindow w;
  srand(time(NULL));
  w.resize(1920, 1080);
  w.show();
  return a.exec();
}
