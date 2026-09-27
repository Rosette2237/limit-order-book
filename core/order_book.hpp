#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <list>
#include <map>
#include <vector>



namespace core {

constexpr std::size_t depth = 5;
enum class side { buy, sell };
struct order {
    std::int64_t id;
    std::int64_t qty;
};
struct trade {
    std::int64_t buy;
    std::int64_t sell;
    std::int64_t price;
    std::int64_t qty;
};
struct level {
    std::int64_t price;
    std::int64_t qty;
};
struct snapshot {
    std::vector<level> bids;
    std::vector<level> asks;
};
struct result {
    std::int64_t id;
    std::vector<trade> trades;
};


class order_book {
public:
    result add(side s, std::int64_t price, std::int64_t qty);
    bool cancel(std::int64_t id);
    snapshot top() const;

private:
    using queue = std::list<order>;
    std::map<std::int64_t, queue, std::greater<>>bids;
    std::map<std::int64_t, queue> asks;
    std::int64_t next_id = 1;
};


}
