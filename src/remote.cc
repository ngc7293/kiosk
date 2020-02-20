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
        std::cerr << "Could not parse JSON: " << e.what() << std::endl;
        return;
    }

    if (!(request.count("operation") && request["operation"].is_string())) {
        connection->write(R"({"status": "error", "message": "missing operation parameter"})");
        return;
    }

    std::string operation = request["operation"].get<std::string>();

    if (operation == "set_autoplay") {
        if (!(request.count("parameters") && request["parameters"].is_array() && request["parameters"].size() == 1 && request["parameters"][0].is_boolean())) {
            connection->write(R"({"status": "error", "message": "set_autoplay takes exactly one boolean argument")");
        }

        state_.setAutoplay(request["parameters"][0].get<bool>());
        connection->write(R"({"status": "success"})");
    }

    if (operation == "set_current") {
        if (!(request.count("parameters") && request["parameters"].is_array() && request["parameters"].size() == 1 && request["parameters"][0].is_number_integer())) {
            connection->write(R"({"error": "set_current takes exactly one integer argument")");
        }

        state_.setCurrentIndex(request["parameters"][0].get<int>());
        connection->write(R"({"status": "success"})");
    }

    if (operation == "get_state") {
        json response;
        response["autoplay"] = state_.autoplay();
        response["items"] = {};
        
        for (const State::Item& item : state_.items()) {
            json obj;
            obj["name"] = item.name();
            obj["url"] = item.url();
            response["items"].push_back(obj);
        }

        connection->write(response.dump().c_str());
    }
}