#include "widgets/stackedWidgets/downloadWidget/downloadWidget.h"
#include <iostream>
#include <QString>

#include "colors/colors.h"
#include "configurableWidgets/searchedSongs/searchedSongWidget.h"

void downloadWidget::searchFound(QJsonObject songData) {
  int index = songData["#"].toString().toInt() - 1; // 1 should point to index 0
  
  presentWidgets[index]->topWidget->setStyleSheet(QString::fromStdString(std::format("background-color: {}; border-radius:15px", colors::searchedSong))); // Color the widget yellow, to show it was searched
  presentWidgets[index]->updateUrlButton->setStyleSheet(QString::fromStdString(std::format("color: {}; font-size: 15px; font-weight: 700; background-color: {};", colors::searchedSongText, colors::searchedSongButton)));
  presentWidgets[index]->updateUrlButton->show(); // Show the edit URL Button
  presentWidgets[index]->songDataJson["data"] = songData;
}