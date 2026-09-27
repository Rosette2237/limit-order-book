#pragma once

#include "core/order_book.hpp"

#include <cstdint>
#include <functional>
#include <mutex>
#include <vector>



namespace core {

struct update {
    std::vector<trade> trades;
    snapshot top;
};

class matching_engine {
public:
    using listener = std::function<void(const update&)>;
    explicit matching_engine(listener on_update);
    result add(side s, std::int64_t price, std::int64_t qty);
    bool cancel(std::int64_t id);
    snapshot top() const;

private:
    mutable std::mutex m;
    order_book book;
    listener notify;
};

}
