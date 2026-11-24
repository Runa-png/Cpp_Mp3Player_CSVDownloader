#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#pragma once

class downloadWidget;

class changeUrlWindow : public QWidget {
  public:
    changeUrlWindow(downloadWidget*);
    void update();
    void dataUpdated();
  private:
    downloadWidget* parent;

    // Widgets
    QLineEdit* nameLabel;
    QLabel* artistLabel;
    QLabel* albumLabel;
    QLineEdit* urlLabel;
};