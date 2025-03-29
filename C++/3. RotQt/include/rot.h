#ifndef ROT_H_
#define ROT_H_

#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <cstdlib>
#include <unistd.h>

QString shiftText(const QString &target, int key, bool isEncrypting);
int findKey(const QString &target);
QString readFile(const QString &fileName);
bool writeFile(const QString &outputText, const QString &outputFile);

#endif
