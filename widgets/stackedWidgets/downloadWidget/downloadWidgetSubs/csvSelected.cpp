#include <QWidget>
#include <string>

#include "widgets/stackedWidgets/downloadWidget/downloadWidget.h"
#include "colors/colors.h"
#include "cppFunctions/checkFileExists/checkFileExists.h"
#include "configurableWidgets/searchedSongs/searchedSongWidget.h"

void downloadWidget::csvSelected(std::string location) {
  for (searchedSong* widget : presentWidgets) { // Remove all widgets on reload
    widget->deleteLater();
  };
  presentWidgets.clear();
  
  if (!checkFileIsCSV(location)) {
    userLocationInput->setStyleSheet(QString::fromStdString(std::format(R"(
      color: {}  
    )", colors::badText
    )));
    
    return;
  }
  userLocationInput->setStyleSheet(QString::fromStdString(std::format(R"(
    color: {}  
  )", colors::goodText
  )));

  findSongs(location);
  
  return;
}