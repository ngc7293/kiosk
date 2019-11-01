#ifndef APP_H_
#define APP_H_

#include <vector>

#include <QMainWindow>
#include <QAction>
#include <QMenu>
#include <QCommandLineParser>
#include <QStackedWidget>
#include <QTimer>
#include <QtWebEngineWidgets/QWebEngineView>

class App : public QMainWindow {
    Q_OBJECT

private:
    QStackedWidget* stack_;
    QMenu* menu_;

    QAction* fullscreen_act_;
    QAction* add_board_act_;
    QAction* toggle_cycle_act_;
    QTimer* timer_;

public:
    explicit App(QCommandLineParser& parser, QWidget* parent = 0);
    ~App();

private:
    void addView(QUrl url);
    bool readUrlFile(QString path);

public slots:
    void promptAddBoard();
    void toggleFullScreen();
    
    void toggleCycle();
    void showNext();
};

#endif