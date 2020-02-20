#ifndef REMOTE_H_
#define REMOTE_H_

#include <QObject>

#include <QLocalServer>

#include "state.h"

class Remote : public QObject {
    Q_OBJECT

private:
    State& state_;

    QLocalServer* server_;

public:
    Remote(QObject* parent, State& state);
    ~Remote() override;

private slots:
    void onConnect();
    void onData();
};

#endif