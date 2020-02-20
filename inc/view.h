#ifndef VIEW_H_
#define VIEW_H_

#include <QWidget>

#include <unordered_map>

#include <QStackedWidget>
#include <QWebEngineView>

#include "state.h"

class View : public QWidget {
    Q_OBJECT

private:
    State& state_;
    std::string css_;

    QStackedWidget* widget_;
    std::unordered_map<std::string, QWidget*> widgets_;

public:
    View(QWidget* parent, State& state);
    ~View() override;

    void setCSS(std::string css);

private:
    void addView(const std::string_view& url);
    void addCSS(const QWebEngineView& view);

public slots:
    void refresh();
};

#endif