#include "widgets/stackedWidgets/downloadWidget/downloadWidget.h"
#include <QProcess>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

#include <iostream>

#include "cppFunctions/convertDictToJson/convertDictToJson.h"
#include "widgets/stackedWidgets/downloadWidget/downloadWidget.h"
#include "configurableWidgets/searchedSongs/searchedSongWidget.h"

void downloadWidget::searchSongs() {
  if (presentWidgets.empty()) {
    std::cout << "Search is empty" << std::endl;
    return;
  }

  std::cout << "Searching!" << std::endl;

  std::string songQuery;
  QJsonArray songArray;
  for (searchedSong* widget : presentWidgets) {
    QJsonObject data = widget->songDataJson["data"];
    std::cout << "Index: " << data["#"].toInt() << std::endl;
    songArray.append(data);
  }
  songQuery = QJsonDocument(songArray).toJson(QJsonDocument::Compact).toStdString();

  QString path = "pythonFunctions/";
  QStringList params;
  params << "getUrl.py" << QString::fromStdString(songQuery);

  auto* process = new QProcess();
  process->setWorkingDirectory(path);

  QObject::connect( // Text printed to the terminal
    process, // Link to process object
    &QProcess::readyReadStandardOutput, // Text printed
    process,
    [process, this]() {
      while (process->canReadLine()) { // Iterate over each song that the python script found
        QJsonObject json = convertDictToJson(process->readLine());
        std::cout << "Recieved: " << json["url"].toString().toStdString() << std::endl;
        searchFound(json);
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