#include <QWidget>
#include <QVBoxLayout>

#include <map>
#include <string>

#include "widgets/stackedWidgets/musicManagementWidget/musicManagementWidgets/songItem.h"

songItem::songItem(std::map<std::string, std::string>* songData) : QWidget() {
  QVBoxLayout* topLayout = new QVBoxLayout();

  qDebug() << songData;
}