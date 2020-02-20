#include "app.h"

#include <iostream>

#include <QMenuBar>
#include <QFile>
#include <QInputDialog>
#include <QLineEdit>
#include <QtWebEngineWidgets/QWebEngineScript>
#include <QtWebEngineWidgets/QWebEngineScriptCollection>

#include "state.h"
#include "view.h"

App::App(QCommandLineParser& parser, QWidget* parent)
    : QMainWindow(parent)
    , fullscreen_act_(new QAction("Fullscreen", this))
    , add_board_act_(new QAction("Add board", this))
    , toggle_cycle_act_(new QAction("Start", this))
{
    view_ = new View(this, state_);
    ticker_ = new Ticker(this, state_);

    connect(&state_, &State::changed, view_, &View::refresh);
    connect(&state_, &State::changed, ticker_, &Ticker::update);

    addAction(toggle_cycle_act_);
    toggle_cycle_act_->setShortcut(Qt::Key_Space);
    add_board_act_->setShortcut(QKeySequence::AddTab);

    menu_ = menuBar()->addMenu("Actions");
    menu_->addAction(add_board_act_);
    menu_->addAction(fullscreen_act_);
    menu_->addAction(toggle_cycle_act_);
    connect(add_board_act_, &QAction::triggered, this, &App::promptAddBoard);
    connect(fullscreen_act_, &QAction::triggered, this, &App::toggleFullscreen);
    connect(toggle_cycle_act_, &QAction::triggered, this, &App::toggleAutoplay);

    if (parser.isSet("css") && QFile(parser.value("css")).exists()) {
        QFile css(parser.value("css"));
        css.open(QIODevice::ReadOnly);
        view_->setCSS(css.readAll().toStdString());
    }

    if (!(parser.isSet("urls") && readUrlFile(parser.value("urls")))) {
        state_.add("https://trello.com");
    }

    if (parser.isSet("fullscreen")) {
        toggleFullscreen();
    }

    setCentralWidget(view_);
}

App::~App()
{
}

bool App::readUrlFile(QString path)
{
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    if (file.atEnd()) { 
        return false;
    }

    while(!file.atEnd()) {
        QByteArray line = file.readLine();
        state_.add(std::string(line.data()));
    }

    return true;
}

void App::promptAddBoard()
{
    bool ok;
    QString text = QInputDialog::getText(this, "Trello", "Board URL", QLineEdit::Normal, "", &ok);
    state_.add(text.toStdString());
}

void App::toggleFullscreen()
{
    if (isFullScreen()) {
        menuBar()->show();
        showNormal();
    } else {
        menuBar()->hide();
        showFullScreen();
    }
}

void App::toggleAutoplay()
{
    state_.setAutoplay(!state_.autoplay());
}