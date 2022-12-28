#ifndef __MEMORIZATION_H__
#define __MEMORIZATION_H__

#include <tuple>
#include <vector>
#include <optional>
#include <string>
#include <functional>

#include <iostream>

namespace FunAndGames
{

namespace details
{

template <typename Callable>
struct CallableTypeDeductor;

template <typename Res, typename ... Args>
struct CallableTypeDeductor<Res(*)(Args ...)>
{
    using ResultType = Res;
    using ArgsType = std::tuple<Args ...>;
};

template <typename Res, typename ... Args>
struct CallableTypeDeductor<Res(Args ...)>
{
    using ResultType = Res;
    using ArgsType = std::tuple<Args ...>;
};

template <typename Res, typename Class, typename ... Args>
struct CallableTypeDeductor<Res(Class::*)(Args ...) const>
{
    using ResultType = Res;
    using ArgsType = std::tuple<Args ...>;
};

template <typename Res, typename ... Args>
struct CallableTypeDeductor<std::function<Res(Args ...)>>
{
    using ResultType = Res;
    using ArgsType = std::tuple<Args ...>;
};

template <std::size_t N, typename Key, typename Value>
struct LRU
{
    LRU() { if (N != 0) cache.reserve(N); }

    Value
    addData(const Key &key, const Value &value)
    {
        if (N == 0) return value;
        return cache.size() == N ? addDataFullLRU(key, value) : addDataNotFullLRU(key, value);
    }

    std::optional<Value>
    getValue(const Key &key)
    {
        if (cache.empty()) return std::nullopt;
        for (auto iter = head; iter != N; iter = cache[iter].next) {
            if (cache[iter].key != key) continue;
            if (iter != head) {
                cache[cache[iter].prev].next = cache[iter].next;
                if (cache[iter].next != N) cache[cache[iter].next].prev = cache[iter].prev;
                else tail = cache[iter].prev;
                cache[head].prev = iter;
                cache[iter].next = head;
                cache[iter].prev = N;
                head = iter;
            }
            return cache[iter].value;
        }
        return std::nullopt;
    }

private:
   struct Entry
    {
        Entry(const Key &k, const Value &v, std::size_t n, std::size_t p) : key(k), value(v), next(n), prev(p) {}

        Key key;
        Value value;
        std::size_t next;
        std::size_t prev;
    };

    Value
    addDataNotFullLRU(const Key &key, const Value &value)
    {
        if (cache.empty()) {
            cache.emplace_back(key, value, N, N);
            return value;
        }
        cache[head].prev = cache.size();
        cache.emplace_back(key, value, head, N);
        head = cache[head].prev;
        return value;
    }

    Value
    addDataFullLRU(const Key &key, const Value &value)
    {
        cache[tail].key = key;
        cache[tail].value = value;
        if (N < 2) return value;
        cache[tail].next = head;
        cache[cache[tail].prev].next = N;
        auto new_tail = cache[tail].prev;
        cache[tail].prev = N;
        cache[head].prev = tail;
        head = tail;
        tail = new_tail;
        return value;
    }

    std::vector<Entry> cache;
    std::size_t head = 0;
    std::size_t tail = 0;
};

} // namespace details

template <std::size_t CacheSize, typename Callable>
class Memorization
{
    using TypeDeductor = details::CallableTypeDeductor<Callable>;

public:
    Memorization(const Callable &_callable) : callable(_callable) {}

    template <typename ... Args>
    typename TypeDeductor::ResultType
    operator()(const Args & ... args)
    {
        auto args_tuple = std::tuple(args ...);
        auto cached_value = cache.getValue(args_tuple);
        if (cached_value) return cached_value.value();
        return cache.addData(args_tuple, callable(args ...));
    }

private:
    Callable callable;
    details::LRU<CacheSize, typename TypeDeductor::ArgsType, typename TypeDeductor::ResultType> cache;
};

template <std::size_t CacheSize, typename Callable>
Memorization<CacheSize, Callable> genMemo(Callable callable) { return Memorization<CacheSize, Callable>(callable); }

} // namespace FunAndGames

#endif // __MEMORIZATION_H__
