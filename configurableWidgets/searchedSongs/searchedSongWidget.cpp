#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QJsonObject>
#include <QLabel>
#include <QPushButton>

#include <iostream>

#include "colors/colors.h"
#include "configurableWidgets/searchedSongs/searchedSongWidget.h"
#include "cppFunctions/convertDictToJson/convertDictToJson.h"
#include "windows/changeUrl/changeUrl.h"
#include "widgets/stackedWidgets/downloadWidget.h"

searchedSong::searchedSong(int height, QJsonObject songData, downloadWidget* parent) : QWidget() {
  songDataJson = {{"data", songData}};
  
  QVBoxLayout* mainLayout = new QVBoxLayout(this);

  topWidget = new QWidget();
  topWidget->setFixedHeight(height);
  topWidget->setStyleSheet(QString::fromStdString(std::format(R"(
    background-color: {};
    border-radius:15px;
  )", colors::color4)));
  QHBoxLayout* topLayout = new QHBoxLayout(topWidget);

  // Song counter
  QWidget* counterWidget = new QWidget();
  counterWidget->setFixedWidth(125);
  QVBoxLayout* counterLayout = new QVBoxLayout(counterWidget);

  QLabel* counterLabel = new QLabel("Song: " + songData["#"].toString());
  counterLabel->setStyleSheet(QString::fromStdString(std::format(R"(
      color: {};
      font-size: 17px;
      font-weight: 700;
    )", colors::text1
  )));
  counterLayout->addWidget(counterLabel);

  topLayout->addWidget(counterWidget);

  // Song Information
  QWidget* songInfoWidget = new QWidget();
  songInfoWidget->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
  QVBoxLayout* songInfoLayout = new QVBoxLayout(songInfoWidget);

  QLabel* songNameLabel = new QLabel(songData["Song"].toString());
  songNameLabel->setStyleSheet(QString::fromStdString(std::format(R"(
      color: {};
      font-size: 14px;
      font-weight: 700;
    )", colors::text1
  )));
  songInfoLayout->addWidget(songNameLabel);

  QLabel* songArtistLabel = new QLabel(songData["Artist"].toString());
  songArtistLabel->setStyleSheet(QString::fromStdString(std::format(R"(
      color: {};
      font-size: 12px;
      font-weight: 700;  
    )", colors::text2
  )));
  songInfoLayout->addWidget(songArtistLabel);

  QLabel* songAlbumLabel = new QLabel(songData["Album"].toString());
  songAlbumLabel->setStyleSheet(QString::fromStdString(std::format(R"(
      color: {};
      font-size: 10px;
      font-weight: 700;
    )", colors::text3
  )));
  songInfoLayout->addWidget(songAlbumLabel);

  topLayout->addWidget(songInfoWidget);

  // Update URL Button
  updateUrlButton = new QPushButton("Update \n URL");
  updateUrlButton->hide(); // Show when we have the URL
  updateUrlButton->setCursor(Qt::PointingHandCursor);
  updateUrlButton->setFixedSize(height-15, height-15);
  updateUrlButton->setStyleSheet(QString::fromStdString(std::format(R"(
    color: {};
    font-size: 15px;
    font-weight: 700;
    background-color: {};
  )", colors::text1, colors::color5
  )));
  topLayout->addWidget(updateUrlButton);

  connect(updateUrlButton, &QPushButton::clicked, this, [this, parent]() {parent->selectedWidget = this; parent->settingsWindow->update(); parent->settingsWindow->show();});

  mainLayout->addWidget(topWidget);
}