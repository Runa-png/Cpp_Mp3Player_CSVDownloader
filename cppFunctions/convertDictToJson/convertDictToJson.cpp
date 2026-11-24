#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <string>
#include <QByteArray>

#include "cppFunctions/convertDictToJson/convertDictToJson.h"

QJsonObject convertDictToJson(QByteArray dict) {
  QJsonDocument document = QJsonDocument::fromJson(dict);

  if (!document.isObject()) {
    return {};
  }

  QJsonObject songdata = document.object();
  return songdata;
}