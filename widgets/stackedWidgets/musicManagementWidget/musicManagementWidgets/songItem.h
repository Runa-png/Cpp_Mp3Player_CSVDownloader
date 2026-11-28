#include <QWidget>
#include <map>
#include <string>

class songItem : public QWidget {
  public:
    songItem(std::map<std::string, std::string>*);
};