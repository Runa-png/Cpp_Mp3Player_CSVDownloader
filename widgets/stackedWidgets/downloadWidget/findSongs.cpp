#include <string>
#include <QObject>
#include <QProcess>

#include "widgets/stackedWidgets/downloadWidget.h"
#include "cppFunctions/convertDictToJson/convertDictToJson.h"
#include "iostream"

void downloadWidget::findSongs(std::string location) {
  std::cout << location << std::endl;

  QString path = "pythonFunctions/";
  QStringList params;
  params << "readCSV.py" << QString::fromStdString(location);

  auto* process = new QProcess();
  process->setWorkingDirectory(path);

  QObject::connect( // Text printed to the terminal
    process, // Link to process object
    &QProcess::readyReadStandardOutput, // Text printed
    process,
    [process, this]() {
      while (process->canReadLine()) { // Iterate over each song that the python script found
        QJsonObject json = convertDictToJson(process->readLine());
        addSong(json);
      }
    }
  );

  QObject::connect( // Error happened
    process,
    &QProcess::readyReadStandardError, // Error occurred
    process,
    [process]() {
      qWarning().noquote() << process->readAllStandardError();
    }
  );

  QObject::connect( // Search completed
    process,
    &QProcess::finished,
    process,
    [process, this]() {
      process->deleteLater();
    }
  );

  process->start("python3", params);
}