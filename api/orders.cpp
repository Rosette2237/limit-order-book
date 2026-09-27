#include "api/orders.hpp"
#include "api/convert.hpp"

#include <stdexcept>
#include <string>
#include <utility>


namespace api {

namespace {
drogon::HttpResponsePtr error(drogon::HttpStatusCode code, const std::string& msg)
{
    Json::Value out;
    out["error"] = msg;
    auto resp = drogon::HttpResponse::newHttpJsonResponse(out);
    resp->setStatusCode(code);
    return resp;
}
}


orders::orders(std::shared_ptr<core::matching_engine> engine): engine{std::move(engine)} {}
// post
void orders::submit(const drogon::HttpRequestPtr&req, callback&& reply)
{
    auto json = req-> jsonObject();
    if (!json) {
        reply(error(drogon::k400BadRequest, "must be JSON"));
        return;
    }

    const Json::Value& body = *json;
    auto side = to_side(body["side"]);
    bool valid_side = side.has_value();
    bool valid_price = body["price"].isInt64();
    bool valid_qty = body["qty"].isInt64();
    if (!valid_side || !valid_price || !valid_qty) {
        reply(error(drogon::k400BadRequest, "need side (buy or sell), price and qty (integers)"));
        return;
    }

    try {
        core::result r = engine-> add(*side, body["price"].asInt64() , body["qty"].asInt64());
        reply(drogon::HttpResponse::newHttpJsonResponse(to_json(r)));
    }
    catch (const std::invalid_argument&e) {
        reply(error(drogon::k400BadRequest, e.what()));
    }
}

// delete
void orders::cancel(const drogon::HttpRequestPtr&, callback&& reply, std::int64_t id)
{
    if (!engine-> cancel(id)) {
        reply(error(drogon::k404NotFound, "no resting order with this id"));
        return;
    }
    Json::Value out;
    out["id"] = id;
    out["canceled"] = true;
    reply(drogon::HttpResponse::newHttpJsonResponse(out));
}

// get
void orders::get_book(const drogon::HttpRequestPtr&, callback&& reply)
{
    reply(drogon::HttpResponse::newHttpJsonResponse(to_json(engine-> top())));
}

}
