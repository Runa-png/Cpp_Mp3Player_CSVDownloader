#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>

#include "widgets/mainWindow.h"
#include <QLoggingCategory> // Stop those annoying warnings

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  QMainWindow window;

  QLoggingCategory::setFilterRules(
    "qt.qpa.wayland.textinput=false"
  );

  window.setCentralWidget(new mainWindow());

  window.show();

  return app.exec();
}