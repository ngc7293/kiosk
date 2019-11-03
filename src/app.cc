#include "app.h"

#include <iostream>

#include <QMenuBar>
#include <QFile>
#include <QInputDialog>
#include <QLineEdit>
#include <QtWebEngineWidgets/QWebEngineScript>
#include <QtWebEngineWidgets/QWebEngineScriptCollection>

App::App(QCommandLineParser& parser, QWidget* parent)
    : QMainWindow(parent)
    , timer_(new QTimer(this))
    , stack_(new QStackedWidget(this))
    , fullscreen_act_(new QAction("Fullscreen", this))
    , add_board_act_(new QAction("Add board", this))
    , toggle_cycle_act_(new QAction("Start", this))
{
    setCentralWidget(stack_);

    addAction(fullscreen_act_);
    fullscreen_act_->setShortcut(QKeySequence::FullScreen);

    addAction(toggle_cycle_act_);
    toggle_cycle_act_->setShortcut(Qt::Key_Space);

    add_board_act_->setShortcut(QKeySequence::AddTab);

    menu_ = menuBar()->addMenu("Actions");
    menu_->addAction(add_board_act_);
    menu_->addAction(fullscreen_act_);
    menu_->addAction(toggle_cycle_act_);
    connect(add_board_act_, &QAction::triggered, this, &App::promptAddBoard);
    connect(fullscreen_act_, &QAction::triggered, this, &App::toggleFullScreen);
    connect(toggle_cycle_act_, &QAction::triggered, this, &App::toggleCycle);

    connect(timer_, &QTimer::timeout, this, &App::showNext);

    if (parser.isSet("css") && QFile(parser.value("css")).exists()) {
        css_ = parser.value("css");
    }

    if (!(parser.isSet("urls") && readUrlFile(parser.value("urls")))) {
        addView(QUrl("https://trello.com"));
    }

    if (parser.isSet("fullscreen")) {
        toggleFullScreen();
    }

    if (parser.isSet("start")) {
        toggleCycle();
    }
}

App::~App()
{
    timer_->stop();

    delete fullscreen_act_;
    delete add_board_act_;
    delete toggle_cycle_act_;
    delete timer_;
    delete stack_;
}

void App::addView(QUrl url)
{
    QWebEngineView* view = new QWebEngineView(stack_);
    view->load(url);
    stack_->addWidget(view);
    if (stack_->count() == 1) {
        stack_->currentWidget()->show();
    }

    if (css_ != "") {
        addCSS(*view);
    }
}

void App::addCSS(QWebEngineView& view)
{
    QFile css(css_);
    if (!css.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QString source = css.readAll();
    QWebEngineScript script;
    QString stylesheet = QString(""\
        "(function() {" \
        "    css = document.createElement('style');"\
        "    css.type = 'text/css';"\
        "    document.head.appendChild(css);"\
        "    css.innerText = '%1';"\
        "})()").arg(source.simplified());

    script.setName("custom_css");
    script.setSourceCode(stylesheet);
    script.setInjectionPoint(QWebEngineScript::DocumentReady);
    script.setRunsOnSubFrames(true);
    script.setWorldId(QWebEngineScript::ApplicationWorld);
    view.page()->scripts().insert(script);
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
        addView(QString(line));
    }

    return true;
}


void App::showNext()
{
    if (stack_->count() == 0) {
        return;
    }
    stack_->setCurrentIndex((stack_->currentIndex() + 1) % stack_->count());
}

void App::promptAddBoard()
{
    bool ok;
    QString text = QInputDialog::getText(this, "Trello", "Board URL", QLineEdit::Normal, "", &ok);
    addView(QUrl(text));
}

void App::toggleFullScreen()
{
    if (isFullScreen()) {
        menuBar()->show();
        showNormal();
    } else {
        menuBar()->hide();
        showFullScreen();
    }
}

void App::toggleCycle()
{
    if (timer_->isActive()) {
        toggle_cycle_act_->setText("Start");
        timer_->stop();
        stack_->setEnabled(true);
    }
    else {
        toggle_cycle_act_->setText("Stop");
        timer_->start(10000);
        stack_->setEnabled(false);
    }
}