#include "widgets/stackedWidgets/downloadWidget/downloadSongs.h"
#include "widgets/stackedWidgets/downloadWidget.h"
#include "configurableWidgets/searchedSongs/searchedSongWidget.h"
#include "cppFunctions/convertDictToJson/convertDictToJson.h"
#include "colors/colors.h"

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
      std::cout << "Download Finished" << std::endl;
      process->deleteLater();
    }
  );

  process->start("python3", params);

  return;
}

void downloadWidget::downloadResponseRecieved(QJsonObject data) {  
  qDebug() << data;
  qDebug() << data["index"];
  
  if (!data["successful"].toBool()) { // Failed
    presentWidgets[data["index"].toString().toInt()-1]->topWidget->setStyleSheet(QString::fromStdString(std::format("background-color: {}; border-radius:15px", colors::failSong))); // Color the widget yellow, to show it was searched
    presentWidgets[data["index"].toString().toInt()-1]->updateUrlButton->show(); // Hide the edit URL Button
    presentWidgets[data["index"].toString().toInt()-1]->updateUrlButton->setStyleSheet(QString::fromStdString(std::format(R"(color: {}; font-size: 15px; font-weight: 700; background-color: {};)", colors::failSongText, colors::failSongButton)));
    presentWidgets[data["index"].toString().toInt()-1]->statusLabel->show();
    presentWidgets[data["index"].toString().toInt()-1]->statusLabel->setText(data["code"].toString());

    presentWidgets[data["index"].toString().toInt()-1]->statusLabel->setStyleSheet(QString::fromStdString(std::format(R"(
      color: {};
      font-size: 15px;
      text-decoration: underline;
      font-weight: 700;
    )", colors::failSongText
    )));
  } else { // Succeeded
    if (data.contains("code")) { // Succeeded but there was a code alongside
      presentWidgets[data["index"].toString().toInt()-1]->statusLabel->show();
      presentWidgets[data["index"].toString().toInt()-1]->statusLabel->setText(data["code"].toString());

      presentWidgets[data["index"].toString().toInt()-1]->statusLabel->setStyleSheet(QString::fromStdString(std::format(R"(
        color: {};
        font-size: 15px;
        text-decoration: underline;
        font-weight: 700;
      )", colors::successSongText
      )));
    } else { // Prevent the status label from showing up from past downloads
      presentWidgets[data["index"].toString().toInt()-1]->statusLabel->hide();
    }

    presentWidgets[data["index"].toString().toInt()-1]->topWidget->setStyleSheet(QString::fromStdString(std::format("background-color: {}; border-radius:15px", colors::successSong))); // Color the widget yellow, to show it was searched
    presentWidgets[data["index"].toString().toInt()-1]->updateUrlButton->hide(); // Hide the edit URL Button
  };

  return;
}