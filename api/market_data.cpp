#include "api/market_data.hpp"
#include "api/convert.hpp"

#include <memory>

namespace api {

void market_data::publish(const core::update& update)
{
    Json::Value json = to_json(update);
    std::string message = json.toStyledString();
    subs.publish(message);
}

void market_data::handleNewMessage(const drogon::WebSocketConnectionPtr&, std::string &&, const drogon::WebSocketMessageType&) {}

void market_data::handleNewConnection(const drogon::HttpRequestPtr&, const drogon::WebSocketConnectionPtr& conn)
{
    auto send_message = [conn](const std::string& message) {
        conn->send(message);
    };
    auto subscriber_id = subs.subscribe(send_message);
    auto context= std::make_shared<drogon::SubscriberID>(subscriber_id);
    conn-> setContext(context);
}

void market_data::handleConnectionClosed(const drogon::WebSocketConnectionPtr& conn)
{
    auto subscriber_id = conn->getContextRef<drogon::SubscriberID>();
    subs.unsubscribe(subscriber_id);
}

}
