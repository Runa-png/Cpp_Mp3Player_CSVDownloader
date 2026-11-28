#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QJsonObject>
#include <QPushButton>

#include <iostream>

#include "windows/changeUrl/changeUrl.h"
#include "widgets/stackedWidgets/downloadWidget/downloadWidget.h"
#include "configurableWidgets/searchedSongs/searchedSongWidget.h"
#include "colors/colors.h"

changeUrlWindow::changeUrlWindow(downloadWidget* parentPointer) : QWidget() {
  parent = parentPointer;
  
  this->setStyleSheet(QString::fromStdString(std::format(R"(
    background-color: {};
    )", colors::color2)
  ));
  
  QVBoxLayout* topLayout = new QVBoxLayout(this);

  QWidget* mainWidget = new QWidget();
  mainWidget->setStyleSheet(QString::fromStdString(std::format(R"(
    background-color: {};
    border-radius: 20px;
    )", colors::color3)
  ));
  
  QVBoxLayout* mainLayout = new QVBoxLayout(mainWidget);

  QWidget* informationContainerWidget = new QWidget();
  QVBoxLayout* informationContainerLayout = new QVBoxLayout(informationContainerWidget);
  informationContainerLayout->setContentsMargins(25,50,25,50);
  
  QWidget* nameContainerWidget = new QWidget();
  QVBoxLayout* nameContainerLayout = new QVBoxLayout(nameContainerWidget);
  nameLabel = new QLineEdit();
  nameLabel->setAlignment(Qt::AlignCenter);
  nameLabel->setStyleSheet(QString::fromStdString(std::format(R"(
    color: {};
    font-size:20px;
    font-weight: 800;
    border:1px solid {};
    border-radius:5px;
    padding:7px;
    text-decoration: underline;
    )", colors::color6, colors::color5)
  ));
  nameContainerLayout->addWidget(nameLabel);
  
  artistLabel = new QLabel();
  artistLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
  artistLabel->setAlignment(Qt::AlignCenter);
  artistLabel->setStyleSheet(QString::fromStdString(std::format(R"(
    color: {};
    font-size:17px;
    font-weight: 800
    )", colors::text1)
  ));
  
  albumLabel = new QLabel();
  albumLabel->setAlignment(Qt::AlignCenter);
  albumLabel->setStyleSheet(QString::fromStdString(std::format(R"(
    color: {};
    font-size:15px;
    font-weight: 800
    )", colors::text1)
  ));
  
  QWidget* urlContainerWidget = new QWidget();
  QVBoxLayout* urlContainerLayout = new QVBoxLayout(urlContainerWidget);
  urlLabel = new QLineEdit();
  urlLabel->setAlignment(Qt::AlignCenter);
  urlLabel->setStyleSheet(QString::fromStdString(std::format(R"(
    color: {};
    font-size:12px;
    font-weight: 800;
    border:1px solid {};
    border-radius:5px;
    padding:7px;
    text-decoration: underline;
    )", colors::color6, colors::color5)
  ));
  urlContainerLayout->addWidget(urlLabel);

  QWidget* buttonContainerWidget = new QWidget();
  QVBoxLayout* buttonContainerLayout = new QVBoxLayout(buttonContainerWidget);
  buttonContainerLayout->setAlignment(Qt::AlignCenter);
  QPushButton* updateButton = new QPushButton("Update!");
  updateButton->setCursor(Qt::PointingHandCursor);
  updateButton->setStyleSheet(QString::fromStdString(std::format(R"(
    border:none;
    border-radius:10px;
    background-color: {};
    color: {};
    font-size:18px;
    font-weight:800;
    )", colors::color4, colors::text1)
  ));
  updateButton->setFixedSize(200,50);
  buttonContainerLayout->addWidget(updateButton);

  connect(updateButton, &QPushButton::clicked, this, [this](){dataUpdated();});

  informationContainerLayout->addWidget(nameContainerWidget, 1);
  informationContainerLayout->addWidget(artistLabel, 1);
  informationContainerLayout->addWidget(albumLabel, 1);
  informationContainerLayout->addWidget(urlContainerWidget, 1);
  informationContainerLayout->addWidget(buttonContainerWidget, 1);

  mainLayout->addWidget(informationContainerWidget, 1);
  topLayout->addWidget(mainWidget, 1);

  this->setFixedHeight(this->height());
  this->setFixedWidth(this->width() + 100);
}

void changeUrlWindow::update() { // When the window opens
  QJsonObject& data = parent->selectedWidget->songDataJson["data"];
  
  nameLabel->setText(data["selectedName"].toString());
  artistLabel->setText(data["Artist"].toString());
  albumLabel->setText(data["Album"].toString());
  urlLabel->setText(data["url"].toString());
}

void changeUrlWindow::dataUpdated() { // When the update buttons is pressed
  QJsonObject& data = this->parent->selectedWidget->songDataJson["data"];
  
  data["selectedName"] = this->nameLabel->text(); // The text that is in the Name text input
  data["url"] = this->urlLabel->text(); // The text that is in the URL text input

  this->hide();
}