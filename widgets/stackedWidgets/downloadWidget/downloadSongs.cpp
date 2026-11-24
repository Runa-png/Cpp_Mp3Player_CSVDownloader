#include "widgets/stackedWidgets/downloadWidget/downloadSongs.h"
#include "widgets/stackedWidgets/downloadWidget.h"
#include "configurableWidgets/searchedSongs/searchedSongWidget.h"
#include "cppFunctions/convertDictToJson/convertDictToJson.h"

#include <string>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QString>
#include <QStringList>
#include <iostream>
#include <QProcess>

void downloadWidget::downloadSongs() {
  if (presentWidgets.empty()) {
    emit SEARCHFINISHED();
    return;
  }

  std::string songQuery;
  QJsonArray songArray;
  for (searchedSong* widget : presentWidgets) {
    QJsonObject data = widget->songDataJson["data"];
    songArray.append(data);
  }
  songQuery = QJsonDocument(songArray).toJson(QJsonDocument::Compact).toStdString();

  std::cout << songQuery << std::endl;

  QString path = "pythonFunctions/";
  QStringList params;
  params << "downloadSongs.py" << QString::fromStdString(songQuery);

  auto* process = new QProcess();
  process->setWorkingDirectory(path);

  QObject::connect( // Text printed to the terminal
    process, // Link to process object
    &QProcess::readyReadStandardOutput, // Text printed
    process,
    [process, this]() {
      while (process->canReadLine()) { // Iterate over each song that the python script found
        QJsonObject json = convertDictToJson(process->readLine());
        downloadResponseRecieved(json);
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
      emit SEARCHFINISHED();
      process->deleteLater();
    }
  );

  process->start("python3", params);

  return;
}

void downloadWidget::downloadResponseRecieved(QJsonObject data) {  
  std::cout << "Download recieved: " << data["response"].toBool() << std::endl;
  return;
}