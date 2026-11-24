#pragma once

#include <QWidget>
#include <QJsonObject>
#include <map>
#include <string>
#include <QJsonObject>
#include <QPushButton>

class downloadWidget;
class changeUrlWindow;

class searchedSong : public QWidget {
  public:
    searchedSong(int height, QJsonObject songData, downloadWidget* parent);
    std::map<std::string, QJsonObject> songDataJson;
    QWidget* topWidget;
    QPushButton* updateUrlButton;
  private:
    changeUrlWindow* settingsWindow;
};