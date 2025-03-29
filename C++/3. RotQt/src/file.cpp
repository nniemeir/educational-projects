#include "../include/rot.h"

QString readFile(const QString &fileName) {
  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
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
    return false;
  }
  QTextStream out(&file);
  out << outputText;
  file.close();
  return true;
}
