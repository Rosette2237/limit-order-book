#include "api/convert.hpp"

#include <vector>

namespace api {

namespace {

Json::Value levels_to_json(const std::vector<core::level>& levels ) {
    Json::Value result{Json::arrayValue};
    for (const core::level& level : levels) {
        Json::Value item;
        item["price"] = level.price;
        item["qty"] = level.qty;
        result.append(item);
    }
    return result;
}

Json::Value trades_to_json(const std::vector<core::trade>& trades){
    Json::Value result{Json::arrayValue};
    for (const core::trade& trade : trades) {
        Json::Value item;
        item["buy"] = trade.buy;
        item["sell"] = trade.sell;
        item["price"] = trade.price;
        item["qty"] = trade.qty;
        result.append(item);
    }
    return result;
}
}


std::optional<core::side> to_side(const Json::Value& value) {
    if (value == "buy") {
        return core::side::buy;
    }
    if (value == "sell") {
        return core::side::sell;
    }
    return std::nullopt;
}

Json::Value to_json(const core::result& result){
    Json::Value json;
    json["id"] = result.id;
    json["trades"] = trades_to_json(result.trades);
    return json;
}

Json::Value to_json(const core::snapshot& snapshot){
    Json::Value json;
    json["bids"] = levels_to_json(snapshot.bids);
    json["asks"] = levels_to_json(snapshot.asks);
    return json;
}

Json::Value to_json(const core::update& update){
    Json::Value json = to_json(update.top);
    json["trades"] = trades_to_json(update.trades);
    return json;
}

}
