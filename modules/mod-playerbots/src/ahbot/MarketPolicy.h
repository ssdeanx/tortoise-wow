// CMaNGOS-derived auction valuation/stock policy. GPL-2.0-or-later.
#pragma once
#include <algorithm>
#include <cstdint>
namespace ahbot
{
namespace policy
{
constexpr uint32_t MoneyLimit = 0x7fffffff;
inline uint32_t Money(uint64_t v)
{
    return uint32_t(std::min<uint64_t>(v, MoneyLimit));
}
inline uint32_t Price(uint32_t buy, uint32_t sell, uint32_t quality, uint32_t percent)
{
    uint64_t base = buy;
    if (!buy || (sell && buy / sell > 5))
        base = uint64_t(sell) * (quality <= 1 ? 4 : 5);
    return Money(base * percent / 100);
}
inline uint32_t RebuildCycles(uint32_t min, uint32_t max)
{
    return ((max - min) / 2 + min) * 90;
}
inline uint32_t ItemLevelCap(uint32_t required)
{
    return required >= 60 ? 255 : required + 5;
}
inline uint32_t Stack(uint64_t count, uint32_t max, uint32_t price)
{
    return price && max ? uint32_t(std::min<uint64_t>(count, std::min(max, MoneyLimit / price))) : 0;
}
inline uint32_t Bid(uint32_t start, uint32_t current, uint32_t increment, uint32_t buyout, uint64_t value)
{
    if (buyout && value > buyout)
        return buyout;
    uint64_t next = std::max<uint64_t>(start, uint64_t(current) + increment);
    return next > current && next <= MoneyLimit && value > next ? uint32_t(next) : 0;
}
} // namespace policy
} // namespace ahbot
