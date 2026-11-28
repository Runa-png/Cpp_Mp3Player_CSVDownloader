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

#include "widgets/stackedWidgets/downloadWidget/downloadWidget.h"
#include "widgets/stackedWidgets/musicManagementWidget/musicManagement.h"

mainWindow::mainWindow() : QWidget() {
  QVBoxLayout* topLayout = new QVBoxLayout(this);
  topLayout->setContentsMargins(0,0,0,0);
  topLayout->setSpacing(0);

  // Middle Section
  QWidget* middleWidget = new QWidget();
  QHBoxLayout* middleLayout = new QHBoxLayout(middleWidget);
  middleLayout->setSpacing(0);
  middleLayout->setContentsMargins(0,0,0,0);
  middleLeftWidget* middleLeftWindowWidget = new middleLeftWidget();
  middleLayout->addWidget(middleLeftWindowWidget);
  middleRightWindowWidget = new middleRightWidget();
  middleLayout->addWidget(middleRightWindowWidget);
  
  // Bottom Section
  bottomWidget* bottomWindowWidget = new bottomWidget();
  
  // Top section (It relies on the middle section)
  topWidget* topWindowWidget = new topWidget(middleRightWindowWidget);
  
  topLayout->addWidget(topWindowWidget,3);
  topLayout->addWidget(middleWidget,85);
  topLayout->addWidget(bottomWindowWidget, 12);
}

topWidget::topWidget(middleRightWidget* middleRightWidgetPointer) : QWidget() {
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
  musicToolButton->setStyleSheet(QString::fromStdString(std::format(R"(font-size:17px; font-weight: 500;)")));
  musicToolButton->setText("Music");
  musicToolButton->setPopupMode(QToolButton::InstantPopup);
  
  QMenu* musicMenu = new QMenu(musicToolButton);
  musicMenu->setStyleSheet(QString::fromStdString(std::format(R"(QMenu {{background-color:{};}} QMenu::item {{color:{};}} QMenu::item:selected {{color: {}}})", colors::color5, colors::text1, colors::color1)));
  QAction* downloadMusicSelected = musicMenu->addAction("Download Music");
  QAction* manageMusicSelected = musicMenu->addAction("Manage Music");

  connect(downloadMusicSelected, &QAction::triggered, this, [this, middleRightWidgetPointer]() {middleRightWidgetPointer->stackedWidget->setCurrentIndex(0);});
  connect(manageMusicSelected, &QAction::triggered, this, [this, middleRightWidgetPointer]() {middleRightWidgetPointer->stackedWidget->setCurrentIndex(1);});

  musicToolButton->setMenu(musicMenu);

  // Player Options
  QToolButton* playerToolButton = new QToolButton();
  playerToolButton->setStyleSheet(QString::fromStdString(std::format(R"(font-size:17px; font-weight: 500;)")));
  playerToolButton->setText("Player");
  playerToolButton->setPopupMode(QToolButton::InstantPopup);
  
  QMenu* playerMenu = new QMenu(playerToolButton);
  playerMenu->setStyleSheet(QString::fromStdString(std::format(R"(QMenu {{background-color:{};}} QMenu::item {{color:{};}} QMenu::item:selected {{color: {}}})", colors::color5, colors::text1, colors::color1)));
  QAction* clearPlaylist = playerMenu->addAction("Clear");

  playerToolButton->setMenu(playerMenu);
  
  toolbar->addWidget(musicToolButton);
  toolbar->addWidget(playerToolButton);

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
  stackedWidget = new QStackedWidget();
  mainLayout->addWidget(stackedWidget, 1);

  // Add widgets to the stacked widget
  downloadWidget* downloadStackedWidget = new downloadWidget();
  musicManagement* musicManagementStackedWidget = new musicManagement();
  
  stackedWidget->addWidget(downloadStackedWidget); // Index 0
  stackedWidget->addWidget(musicManagementStackedWidget); // Index 1
  
  stackedWidget->setCurrentIndex(1);

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
