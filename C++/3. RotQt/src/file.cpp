#include "../include/rot.h"

QString readFile(const QString &fileName) {
  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QMessageBox::critical(nullptr, "Error",
                          QString("Unable to access %1").arg(fileName));
    return QString();
  }

  QTextStream in(&file);
  QString content = in.readAll();
  file.close();
  return content;
}

bool writeFile(const QString &outputText, const QString &outputFile) {
  QFile file(outputFile);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    QMessageBox::critical(
        nullptr, "Error",
        QString("Unable to open %1 for writing").arg(outputFile));
    return false;
  }

  QTextStream out(&file);
  out << outputText;
  file.close();
  return true;
}
