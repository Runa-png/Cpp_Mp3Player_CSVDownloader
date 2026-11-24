#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedWidget>

#include "widgets/mainWindow.h"

#include "colors/colors.h"

#include "widgets/stackedWidgets/downloadWidget.h"

mainWindow::mainWindow() : QWidget() {
  QVBoxLayout* topLayout = new QVBoxLayout(this);
  topLayout->setContentsMargins(0,0,0,0);
  topLayout->setSpacing(0);

  // Top section
  topWidget* topWindowWidget = new topWidget();
  topLayout->addWidget(topWindowWidget,3);

  // Middle Section
  QWidget* middleWidget = new QWidget();
  QHBoxLayout* middleLayout = new QHBoxLayout(middleWidget);
  middleLayout->setSpacing(0);
  middleLayout->setContentsMargins(0,0,0,0);
  middleLeftWidget* middleLeftWindowWidget = new middleLeftWidget();
  middleLayout->addWidget(middleLeftWindowWidget);
  middleRightWidget* middleRightWindowWidget = new middleRightWidget();
  middleLayout->addWidget(middleRightWindowWidget);
  topLayout->addWidget(middleWidget,85);

  // Bottom Section
  bottomWidget* bottomWindowWidget = new bottomWidget();
  topLayout->addWidget(bottomWindowWidget, 12);
}

topWidget::topWidget() : QWidget() {
  QVBoxLayout* topLayout = new QVBoxLayout(this);
  topLayout->setContentsMargins(0,0,0,0);

  QWidget* mainWidget = new QWidget();
  mainWidget->setStyleSheet(QString::fromStdString(
    std::format(R"(
      background-color: {};
    )", colors::color1
  )
  ));

  topLayout->addWidget(mainWidget);
}

middleLeftWidget::middleLeftWidget() : QWidget() {
  QVBoxLayout* topLayout = new QVBoxLayout(this);
  topLayout->setContentsMargins(0,0,0,0);

  QWidget* mainWidget = new QWidget();
  mainWidget->setFixedWidth(80);
  mainWidget->setStyleSheet(QString::fromStdString(
    std::format(R"(
      background-color: {};
    )", colors::color2
  )
  ));

  topLayout->addWidget(mainWidget); 
}

middleRightWidget::middleRightWidget() : QWidget() {
  QVBoxLayout* topLayout = new QVBoxLayout(this);
  topLayout->setContentsMargins(0,0,0,0);

  QWidget* mainWidget = new QWidget();
  mainWidget->setStyleSheet(QString::fromStdString(
    std::format(R"(
      background-color: {};
    )", colors::color3
  )
  ));
  QVBoxLayout* mainLayout = new QVBoxLayout(mainWidget);
  mainLayout->setContentsMargins(0,0,0,0);

  // Stacked Widget
  QStackedWidget* stackedWidget = new QStackedWidget();
  mainLayout->addWidget(stackedWidget, 1);

  // Add widgets to the stacked widget
  downloadWidget* downloadStackedWidget = new downloadWidget();
  stackedWidget->addWidget(downloadStackedWidget);
  stackedWidget->setCurrentIndex(0);

  topLayout->addWidget(mainWidget); 
}

bottomWidget::bottomWidget() : QWidget() {
  QVBoxLayout* topLayout = new QVBoxLayout(this);
  topLayout->setContentsMargins(0,0,0,0);

  QWidget* mainWidget = new QWidget();
  mainWidget->setStyleSheet(QString::fromStdString(
    std::format(R"(
      background-color: {};
    )", colors::color3
  )
  ));

  topLayout->addWidget(mainWidget);
}
