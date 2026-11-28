#include "configurableWidgets/searchedSongs/searchedSongWidget.h"
#include "widgets/stackedWidgets/downloadWidget.h"

#include <iostream>

void downloadWidget::addSong(QJsonObject songData) {
  int height = 150;

  searchedSong* searchedSongWidget = new searchedSong(height, songData, this);
  songScrollLayout->addWidget(searchedSongWidget);
  presentWidgets.push_back(searchedSongWidget);
}