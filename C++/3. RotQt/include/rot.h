#ifndef ROT_H_
#define ROT_H_

#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

QString encrypt(const QString &target, int key);
QString decrypt(const QString &target, int key);
int findKey(const QString &target);
QString readFile(const QString &fileName);
void helpMsg();
bool writeFile(const QString &outputText, const QString &outputFile);

#endif
