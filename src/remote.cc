#include "remote.h"

#include <iostream>

#include <QLocalSocket>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

Remote::Remote(QObject* parent, State& state)
    : QObject(parent)
    , state_(state)
{
    server_ = new QLocalServer(this);
    if (!server_->listen("/tmp/kiosk")) {
        if (QLocalServer::removeServer("/tmp/kiosk")) {
            server_->listen("/tmp/kiosk");
        }
    }

    connect(server_, &QLocalServer::newConnection, this, &Remote::onConnect);

    operations_["get_state"] = &Remote::get_state;
    operations_["set_current"] = &Remote::set_current;
    operations_["set_autoplay"] = &Remote::set_autoplay;
}

Remote::~Remote()
{
}

void Remote::onConnect()
{
    QLocalSocket *connection = server_->nextPendingConnection();
    connect(connection, &QLocalSocket::readyRead, this, &Remote::onData);
    connect(connection, &QLocalSocket::disconnected, connection, &QLocalSocket::deleteLater);
}

void Remote::onData()
{
    QLocalSocket* connection = (QLocalSocket*) sender();
    QByteArray data = connection->readAll();
    json request;

    try {
        request = json::parse(data.data());
    } catch (json::exception& e) {
        fail(connection, "invalid JSON");
        return;
    }

    if (!(request.count("operation") && request["operation"].is_string())) {
        fail(connection, "missing 'operation'");
        return;
    }

    if (!(request.count("parameters") && request["parameters"].is_array())) {
        fail(connection, "missing 'parameters'");
        return;
    }

    std::string operation = request["operation"].get<std::string>();
    if (operations_.count(operation) == 0) {
        fail(connection, std::string("unknown operation '") + operation + "'");
        return;
    }

    operations_[operation](this, connection, request["parameters"]);
}

void Remote::success(QLocalSocket* socket)
{
    json response;
    success(socket, response);
}

void Remote::success(QLocalSocket* socket, json& payload)
{
    payload["status"] = "success";
    socket->write(payload.dump().c_str());
}

void Remote::fail(QLocalSocket* socket, std::string message)
{
    json response;
    response["status"] = "error";
    response["message"] = message;
    socket->write(response.dump().c_str());
}

void Remote::set_current(QLocalSocket* socket, json& parameters)
{
    if (!(parameters.size() == 1 && parameters[0].is_number_integer())) {
        fail(socket, "set_current takes exactly one integer argument");
        return;
    }

    state_.setCurrentIndex(parameters[0].get<int>());
    success(socket);
}

void Remote::set_autoplay(QLocalSocket* socket, json& parameters)
{
    if (!(parameters.size() == 1 && parameters[0].is_boolean())) {
        fail(socket, "set_autoplay takes exactly one boolean argument");
        return;
    }

    state_.setAutoplay(parameters[0].get<bool>());
    success(socket);
}

void Remote::get_state(QLocalSocket* socket, json& parameters)
{
    json response;
    response["autoplay"] = state_.autoplay();
    response["current"] = state_.currentIndex();
    response["items"] = {};
    
    for (const State::Item& item : state_.items()) {
        json obj;
        obj["name"] = item.name();
        obj["url"] = item.url();
        response["items"].push_back(obj);
    }

    success(socket, response);
}