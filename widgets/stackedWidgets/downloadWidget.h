#pragma once

#include <QWidget>
#include <string>
#include <QLineEdit>
#include <QJsonObject>
#include <QVBoxLayout>
#include <vector>

#include "windows/changeUrl/changeUrl.h"


class searchedSong;
class changeUrlWindow;
class searchedSong;

class downloadWidget : public QWidget {
  Q_OBJECT

  signals:
    void SEARCHFINISHED();
  
  public:
    downloadWidget();
    std::vector<searchedSong*> presentWidgets;
    changeUrlWindow* settingsWindow;
    searchedSong* selectedWidget;
    bool running;
  private:
    QLineEdit* userLocationInput;
    QVBoxLayout* songScrollLayout;
  
    void csvSelected(std::string location);
    void findSongs(std::string location);
    void addSong(QJsonObject songData);
    void searchSongs();
    void searchFound(QJsonObject songData);
    void downloadSongs();
    void downloadResponseRecieved(QJsonObject data);
};