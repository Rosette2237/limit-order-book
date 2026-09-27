#pragma once

#include "core/matching_engine.hpp"

#include <drogon/PubSubService.h>
#include <drogon/WebSocketController.h>
#include <string>



namespace api {


class market_data : public drogon::WebSocketController<market_data, false>
{
  public:
    void publish(const core::update& u);
    void handleNewMessage(const drogon::WebSocketConnectionPtr& conn,
                          std::string&& message,
                          const drogon::WebSocketMessageType& type) override;
    void handleNewConnection(const drogon::HttpRequestPtr& req,
                             const drogon::WebSocketConnectionPtr& conn) override;
    void handleConnectionClosed(const drogon::WebSocketConnectionPtr& conn) override;

    WS_PATH_LIST_BEGIN
    WS_PATH_ADD("/marketdata", drogon::Get);
    WS_PATH_LIST_END

  private:
    drogon::Topic<std::string> subs;
};

}
