#include "view.h"

#include <QLayout>
#include <QUrl>

#include <QWebEngineView>
#include <QtWebEngineWidgets/QWebEngineScript>
#include <QtWebEngineWidgets/QWebEngineScriptCollection>

View::View(QWidget* parent, State& state)
    : QWidget(parent)
    , state_(state)
{
    widget_ = new QStackedWidget(this);
    setLayout(new QHBoxLayout());
    layout()->addWidget(widget_);
    layout()->setContentsMargins(0, 0, 0, 0);
}

View::~View() {}

void View::setCSS(std::string css)
{
    css_ = css;
}

void View::refresh()
{
    for (const State::Item& item : state_.items()) {
        if (widgets_.count(std::string(item.url())) == 0) {
            addView(item.url());
        }
    }

    widget_->setCurrentIndex(state_.currentIndex());
    widget_->setEnabled(!state_.autoplay());
}

void View::addView(const std::string_view& url)
{
    QWebEngineView* view = new QWebEngineView(widget_);
    view->load(QUrl(url.data()));
    widget_->addWidget(view);
    widgets_.emplace(std::string(url), view);

    if (widget_->count() == 1) {
        widget_->currentWidget()->show();
    }

    if (css_ != "") {
        addCSS(*view);
    }
}

void View::addCSS(const QWebEngineView& view)
{
    QWebEngineScript script;
    QString stylesheet = QString(""\
        "(function() {" \
        "    css = document.createElement('style');"\
        "    css.type = 'text/css';"\
        "    document.head.appendChild(css);"\
        "    css.innerText = '%1';"\
        "})()").arg(QString(css_.data()).simplified());

    script.setName("custom_css");
    script.setSourceCode(stylesheet);
    script.setInjectionPoint(QWebEngineScript::DocumentReady);
    script.setRunsOnSubFrames(true);
    script.setWorldId(QWebEngineScript::ApplicationWorld);
    view.page()->scripts().insert(script);
}
