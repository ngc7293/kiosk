#ifndef APP_H_
#define APP_H_

#include <QMainWindow>

#include <QCommandLineParser>

#include "state.h"
#include "view.h"
#include "ticker.h"

class App : public QMainWindow {
    Q_OBJECT

private:
    State state_;
    View* view_;
    Ticker* ticker_;

    QMenu* menu_;
    QAction* fullscreen_act_;
    QAction* add_board_act_;
    QAction* toggle_cycle_act_;

public:
    explicit App(QCommandLineParser& parser, QWidget* parent = nullptr);
    ~App();

private:
    bool readUrlFile(QString path);

private slots:
    void promptAddBoard();
    void toggleFullscreen();
    void toggleAutoplay();
};

#endif