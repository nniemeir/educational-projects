#ifndef ROT_H_
#define ROT_H_

#include <cstdlib>
#include <unistd.h>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

QString shiftText(const QString &target, int key, bool isEncrypting);
int findKey(const QString &target);
QString readFile(const QString &fileName);
bool writeFile(const QString &outputText, const QString &outputFile);

#endif
