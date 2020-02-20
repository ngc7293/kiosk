#ifndef TICKER_H_
#define TICKER_H_

#include <QObject>

#include <QTimer>

#include "state.h"

class Ticker : public QObject {
    Q_OBJECT

public:
    static constexpr int ROTATION_MS = 10 * 1000;

private:
    State& state_;
    QTimer* timer_;

public:
    Ticker(QObject* parent, State& state)
        : QObject(parent)
        , state_(state)
    {
        timer_ = new QTimer();

        connect(timer_, &QTimer::timeout, this, &Ticker::tick);

        if (state_.autoplay()) {
            timer_->start(ROTATION_MS);
        }
    }
    ~Ticker() {}

public slots:
    void update()
    {
        if (state_.autoplay()) {
            if (!timer_->isActive()) {
                timer_->start(ROTATION_MS);
            }
        }
        else {
            timer_->stop();
        }
    }

private slots:
    void tick()
    {
        state_.next();
    }
};

#endif