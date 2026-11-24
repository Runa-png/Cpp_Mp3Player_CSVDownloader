#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>

#include <sqlite3.h>
#include <iostream>
#include <filesystem>

#include "widgets/mainWindow.h"
#include <QLoggingCategory> // Stop those annoying warnings

#include "main.h"

int main(int argc, char *argv[]) {
  bool successfulInit = initialise();
  if (!successfulInit) {
    return 1;
  }
  
  QApplication app(argc, argv);
  QMainWindow window;

  QLoggingCategory::setFilterRules(
    "qt.qpa.wayland.textinput=false"
  );

  window.setCentralWidget(new mainWindow());

  window.show();

  return app.exec();
}

bool initialise() {
  sqlite3* db;

  if (!std::filesystem::exists("appData")) {
    std::cout << "Creating folder appData" << std::endl;
    std::filesystem::create_directory("appData");
  }

  if (!std::filesystem::exists("appData/music")) {
    std::cout << "Creating folder music" << std::endl;
    std::filesystem::create_directory("appData/music");
  }

  if (sqlite3_open("appData/database.db", &db) != SQLITE_OK) {
    qWarning() << "Database error:" << sqlite3_errmsg(db);
    sqlite3_close(db);
    return false;
  }

  const char* query = "CREATE TABLE IF NOT EXISTS songs (id INTEGER PRIMARY KEY AUTOINCREMENT, songName TEXT, artistName TEXT, albumName TEXT, fileLocation TEXT UNIQUE)";
  sqlite3_exec(db, query, nullptr, nullptr, nullptr);

  std::cout << "Database opened" << std::endl;
  sqlite3_close(db);
  return true;
}