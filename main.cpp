#include "api/market_data.hpp"
#include "api/orders.hpp"
#include "core/matching_engine.hpp"

#include <drogon/HttpAppFramework.h>
#include <memory>


int main()
{
    auto md = std::make_shared<api::market_data>();
    auto engine = std::make_shared<core::matching_engine>([md](const core::update& u) {md->publish(u);});

    drogon::app().registerController(md);
    drogon::app().registerController(std::make_shared<api::orders>(engine));
    drogon::app().addListener("0.0.0.0", 8080).setThreadNum(4).run();
}
