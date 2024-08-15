#include "../include/mainwindow.h"
#include "../include/rot.h"


QString readFile(const QString &fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(nullptr, "Error", QString("Unable to access %1").arg(fileName));
        return QString();  // Return an empty QString to indicate failure
    }

    QTextStream in(&file);
    QString content = in.readAll();  // Read the entire file content into QString
    file.close();  // Always close the file when done
    return content;
}

bool writeFile(const QString &outputText, const QString &outputFile) {
    QFile file(outputFile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(nullptr, "Error", QString("Unable to open %1 for writing").arg(outputFile));
        return false;  // Return false to indicate failure
    }

    QTextStream out(&file);
    out << outputText;  // Write the content to the file
    file.close();  // Always close the file when done
    return true;  // Return true to indicate success
}

