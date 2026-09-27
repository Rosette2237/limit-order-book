#pragma once

#include "core/order_book.hpp"
#include "core/matching_engine.hpp"

#include <json/json.h>
#include <optional>

namespace api {

std::optional<core::side> to_side(const Json::Value& v);
Json::Value to_json(const core::result& r);
Json::Value to_json(const core::snapshot& s);
Json::Value to_json(const core::update& u);

}
