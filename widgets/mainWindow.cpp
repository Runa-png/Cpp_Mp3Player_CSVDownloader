#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedWidget>

// Toolbar
#include <QToolBar>
#include <QToolButton>
#include <QMenu>

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

  QVBoxLayout* mainLayout = new QVBoxLayout(mainWidget);
  mainLayout->setContentsMargins(0,0,0,0);

  // Main toolbar
  QToolBar* toolbar = new QToolBar("main toolbar");

  // Music Options
  QToolButton* musicToolButton = new QToolButton();
  musicToolButton->setStyleSheet(QString::fromStdString(std::format(R"(font-size:17px; font-weight: 550;)")));
  musicToolButton->setText("Music");
  musicToolButton->setPopupMode(QToolButton::InstantPopup);
  
  QMenu* musicMenu = new QMenu(musicToolButton);
  musicMenu->setStyleSheet(QString::fromStdString(std::format(R"(QMenu {{background-color:{};}} QMenu::item {{color:{};}} QMenu::item:selected {{color: {}}})", colors::color5, colors::text1, colors::color1)));
  QAction* downloadMusicSelected = musicMenu->addAction("Download Music");
  QAction* manageMusicSelected = musicMenu->addAction("Manage Music");

  musicToolButton->setMenu(musicMenu);
  toolbar->addWidget(musicToolButton);

  mainLayout->addWidget(toolbar);
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
