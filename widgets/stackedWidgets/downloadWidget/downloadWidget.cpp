#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QProcess>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

#include <iostream>

#include "widgets/stackedWidgets/downloadWidget/downloadWidget.h"
#include "colors/colors.h"
#include "configurableWidgets/searchedSongs/searchedSongWidget.h"
#include "widgets/stackedWidgets/downloadWidget/downloadWidgetSubs/downloadSongs.h"
#include "windows/changeUrl/changeUrl.h"

downloadWidget::downloadWidget() : QWidget() {
  QVBoxLayout* topLayout = new QVBoxLayout(this);
  topLayout->setContentsMargins(0,0,0,0);
  topLayout->setSpacing(0);

  // User Inputs
  QWidget* userInputsWidget = new QWidget();
  QHBoxLayout* userInputsLayout = new QHBoxLayout(userInputsWidget);

  userLocationInput = new QLineEdit();
  QPushButton* userSearchButton = new QPushButton("Search");
  QPushButton* userDownloadButton = new QPushButton("Download");
  userInputsLayout->addWidget(userLocationInput,80);
  userInputsLayout->addWidget(userSearchButton,10);
  userInputsLayout->addWidget(userDownloadButton,10);

  topLayout->addWidget(userInputsWidget);

  // Change URL settings window
  settingsWindow = new changeUrlWindow(this);
  selectedWidget = nullptr;

  running = false;

  connect(userLocationInput, &QLineEdit::editingFinished, this, [this]() {csvSelected(userLocationInput->text().toStdString());});
  connect(userSearchButton, &QPushButton::clicked, this, [this]() {if (!running) {running = true; searchSongs();};});
  connect(userDownloadButton, &QPushButton::clicked, this, [this]() {if (!running) {running = true; downloadSongs();};}); // Don't run if the searcher is running

  // Signals from searcher and downloader
  connect(this, &downloadWidget::SEARCHFINISHED, this, [this](){this->running = false;});

  // Scrollable Area
  QScrollArea* songScrollArea = new QScrollArea();
  songScrollArea->setWidgetResizable(true);
  QWidget* songScrollWidget = new QWidget();
  songScrollLayout = new QVBoxLayout(songScrollWidget);
  songScrollLayout->setAlignment(Qt::AlignTop);
  songScrollArea->setWidget(songScrollWidget);
  topLayout->addWidget(songScrollArea);
}
