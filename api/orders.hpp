#pragma once

#include "core/matching_engine.hpp"

#include <drogon/HttpController.h>
#include <cstdint>
#include <functional>
#include <memory>





namespace api {

class orders : public drogon::HttpController<orders, false>
{
  public:
    using callback = std::function<void(const drogon::HttpResponsePtr&)>;
    explicit orders(std::shared_ptr<core::matching_engine> engine);

    METHOD_LIST_BEGIN
    ADD_METHOD_TO(orders::submit, "/orders", drogon::Post);
    ADD_METHOD_TO(orders::cancel, "/orders/{id}", drogon::Delete);
    ADD_METHOD_TO(orders::get_book, "/book", drogon::Get);
    METHOD_LIST_END

    void submit(const drogon::HttpRequestPtr& req, callback&& reply);
    void cancel(const drogon::HttpRequestPtr& req, callback&& reply, std::int64_t id);
    void get_book(const drogon::HttpRequestPtr& req, callback&& reply);

  private:
    std::shared_ptr<core::matching_engine> engine;
};


}
