#include "widgets/stackedWidgets/musicManagementWidget/musicManagementFunctions/pullMusicFiles.h"

#include <sqlite3.h>
#include <iostream>

void pullMusicFiles() {
  std::cout << "Searching for music files" << std::endl;

  sqlite3* db;

  // Open the database
  if (sqlite3_open("appData/database.db", &db) != SQLITE_OK) {
    sqlite3_close(db);
  } else { 

  // Pull all data out of the songs table
  const char* query = "SELECT * FROM songs";
  sqlite3_exec(db, query, callBack, nullptr, nullptr);

  }
}

int callBack(void* data, int argc, char** argv, char** columnNames) {
  for (int i = 0; i < argc; i++) {
    std::cout << columnNames[i] << ": "<< (argv[i] ? argv[i] : "NULL") << std::endl;
  }

  return 0;
}