#include "core/matching_engine.hpp"

#include <utility>

namespace core {

matching_engine::matching_engine(listener on_update ) : notify{std::move(on_update)} {}

result matching_engine::add(side s, std::int64_t price, std::int64_t qty)
{
    std::lock_guard<std::mutex> lock{m };
    result r= book.add(s, price, qty);
    notify(update{r.trades, book.top()});
    return r;
}

bool matching_engine::cancel(std::int64_t id)
{
    std::lock_guard<std::mutex> lock{m};
    if (!book.cancel(id)) {
        return false;
    }
    notify(update{{}, book.top()});
    return true;
}

snapshot matching_engine::top() const
{
    std::lock_guard<std::mutex> lock{m};
    return book.top();
}
}
