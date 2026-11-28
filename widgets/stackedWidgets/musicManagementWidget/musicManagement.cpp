#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>

#include "widgets/stackedWidgets/musicManagementWidget/musicManagement.h"

musicManagement::musicManagement() : QWidget() {
  QVBoxLayout* topLayout = new QVBoxLayout(this);
  topLayout->setContentsMargins(0,0,0,0);

  QLabel* debug = new QLabel("Music Management");
  topLayout->addWidget(debug);
}