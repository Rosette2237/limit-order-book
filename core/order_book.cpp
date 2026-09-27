#include "core/order_book.hpp"

#include <algorithm>
#include <stdexcept>



namespace core {

result order_book::add(side s, std::int64_t price, std::int64_t qty)
{
    if (price <= 0 || qty <= 0) {
        throw std::invalid_argument{"price and quantity must be positive"};
    }
    order o{next_id++, qty};
    result r{o.id, {}};

    auto match = [&](auto& levels){
        while (o.qty > 0 && !levels.empty()) {
            auto best = levels.begin();
            const std::int64_t p = best-> first;
            // if price not crossed, stop matching
            if (s == side::buy) {
                if (p > price) {
                    break;
                }
            } else {
                if (p < price) {
                    break;
                }
            }

            queue& q = best->second;
            while (o.qty > 0 && !q.empty()) {
                order& resting = q.front();
                const std::int64_t n = std::min(o.qty, resting.qty);
                o.qty -= n;
                resting.qty -= n;
                // record the trade
                if (s == side::buy) {
                    r.trades.push_back(trade{o.id, resting.id, p, n});
                } else {
                    r.trades.push_back(trade{resting.id, o.id, p, n});
                }
                if (resting.qty == 0) {
                    q.pop_front();
                }
            }
            if (q.empty()) {
                levels.erase(best);
            }
        }
    };
    // left qty become resting order in the book
    if (s == side::buy) {
        match(asks);
        if (o.qty > 0) {
            bids[price].push_back(o);
        }
    }
    else {
        match(bids);
        if (o.qty > 0) {
            asks[price].push_back(o);
        }
    }
    return r;
}

bool order_book::cancel(std::int64_t id)
{
    auto remove = [id](auto& levels){
        auto has_id = [id](const order& o) {
            return o.id == id;
        };

        for (auto level = levels.begin(); level != levels.end(); ++ level) {
            auto& orders = level->second;
            if (orders.remove_if(has_id) > 0) {
                if (orders.empty()) {
                    levels.erase(level);
                }
                return true;
            }

        }
        return false;
    };
    if (remove(bids) || remove(asks)) {
        return true;
    }
    return false;
}

snapshot order_book::top() const
{
    auto collect = [](const auto& levels) {
        std::vector<level> out;
        for (const auto& [price, q] : levels) {
            if (out.size() == depth) {
                break;
            }
            std::int64_t qty = 0;
            for (const order& o : q) {
                qty+= o.qty;
            }
            out.push_back(level{price, qty});
        }
        return out;
    };
    return snapshot{collect(bids), collect(asks)};
}

}
