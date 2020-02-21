#ifndef REMOTE_H_
#define REMOTE_H_

#include <QObject>

#include <unordered_map>

#include <QLocalServer>
#include <QLocalSocket>

#include <nlohmann/json.hpp>

#include "state.h"

using json = nlohmann::json;

class Remote : public QObject {
    Q_OBJECT

private:
    State& state_;

    std::unordered_map<std::string, std::function<void(Remote*, QLocalSocket*, json&)>> operations_;

    QLocalServer* server_;

public:
    Remote(QObject* parent, State& state);
    ~Remote() override;

private:
    void set_current(QLocalSocket* socket, json& parameters);
    void set_autoplay(QLocalSocket* socket, json& parameters);
    void get_state(QLocalSocket* socket, json& parameters);

private slots:
    void onConnect();
    void onData();
};

#endif