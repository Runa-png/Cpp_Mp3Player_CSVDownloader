#include <QWidget>
#include <QShowEvent>

class musicManagement : public QWidget {
  public:
    musicManagement();
  
  private:
    void findSongs();
    
  protected:
    void showEvent(QShowEvent* event);
};