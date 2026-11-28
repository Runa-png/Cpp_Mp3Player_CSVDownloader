#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QShowEvent>

#include "widgets/stackedWidgets/musicManagementWidget/musicManagement.h"
#include "widgets/stackedWidgets/musicManagementWidget/musicManagementWidgets/songItem.h"
#include "widgets/stackedWidgets/musicManagementWidget/musicManagementFunctions/pullMusicFiles.h"

musicManagement::musicManagement() : QWidget() {
  QVBoxLayout* topLayout = new QVBoxLayout(this);
  topLayout->setContentsMargins(0,0,0,0);

  QLabel* debug = new QLabel("Music Management");
  topLayout->addWidget(debug);
}

void musicManagement::findSongs() {
  qDebug() << "Finding Songs";
  pullMusicFiles();
}

void musicManagement::showEvent(QShowEvent* event) {
  findSongs();
}