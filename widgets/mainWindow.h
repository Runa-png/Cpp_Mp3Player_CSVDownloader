#include <QWidget>
#include <QStackedWidget>

// Forward declarations
class middleRightWidget;

class mainWindow : public QWidget {
  public:
  middleRightWidget* middleRightWindowWidget;  
  mainWindow();
};

class topWidget : public QWidget {
  public:
    topWidget(middleRightWidget* middleRightWidgetPointer);
};

class middleLeftWidget : public QWidget {
  public:
    middleLeftWidget();
};

class middleRightWidget : public QWidget {
  public:
    middleRightWidget();
    QStackedWidget* stackedWidget;
};

class bottomWidget : public QWidget {
  public:
    bottomWidget();
};