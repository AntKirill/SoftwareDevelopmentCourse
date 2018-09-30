#include "../LRUCache.h"
#include "gtest/gtest.h"
#include <array>
#include <random>

struct LRUCacheExtended : public LRUCache
{
    using LRUCache::LRUCache;
    size_t getMaxSize() { return maxSize; }
    const auto &getMap() const { return map; }
    const std::list<Node> &getList() const { return list; }
};

TEST(LRUCache, constructor)
{
    LRUCacheExtended cache(10);
    EXPECT_EQ(10, cache.getMaxSize());
}

#define CHECK_MAP_LIST(cache, key, value)    \
    {                                        \
        const auto &map = (cache).getMap();  \
        auto it = map.find((key));           \
        EXPECT_TRUE(it != map.end());        \
        auto list_it = it->second;           \
        EXPECT_EQ((key), list_it->first);    \
        EXPECT_EQ((value), list_it->second); \
    }

TEST(LRUCache, simple_put)
{
    LRUCacheExtended cache(10);
    cache.put(1, 0);
    CHECK_MAP_LIST(cache, 1, 0);
    const auto &map = cache.getMap();
    const auto &list = cache.getList();
    EXPECT_EQ(map.size(), list.size());
}

TEST(LRUCache, many_puts)
{
    LRUCacheExtended cache(100);
    for (int i = 0; i != 95; ++i)
        cache.put(i, i + 1);
    for (int i = 0; i != 95; ++i)
        CHECK_MAP_LIST(cache, i, i + 1)
    cache.put(100, 101);
    for (int i = 0; i != 95; ++i)
        CHECK_MAP_LIST(cache, i, i + 1)
    CHECK_MAP_LIST(cache, 100, 101);
}

TEST(LRUCache, edge_put)
{
    LRUCacheExtended cache(100);
    for (int i = 0; i != 100; ++i)
    {
        bool was_something_erased = cache.put(i, i + 1);
        EXPECT_FALSE(was_something_erased);
    }
    bool was_something_erased = cache.put(100, 101);
    EXPECT_TRUE(was_something_erased);
    for (int i = 1; i != 101; ++i)
        CHECK_MAP_LIST(cache, i, i + 1);

    {
        const auto &map = cache.getMap();
        auto it = map.find(0);
        EXPECT_TRUE(it == map.end());
    }
}

TEST(LRUCache, random_puts)
{
    LRUCacheExtended cache(100);
    srand(time(0));
    for (int i = 0; i < 200; ++i)
    {
        cache.put(i, i + 1);
    }
    int max_int = std::numeric_limits<int>::max();
    for (int i = 0; i < 1000; ++i)
    {
        int key = random() % max_int;
        int value = key + 1;
        cache.put(key, value);
        CHECK_MAP_LIST(cache, key, value)
    }
}

TEST(LRUCache, simple_get)
{
    LRUCacheExtended cache(100);
    cache.put(0, 1);
    auto *value_ptr = cache.get(0);
    CHECK_MAP_LIST(cache, 0, *value_ptr);
}

TEST(LRUCache, edge_get)
{
    {
        LRUCacheExtended cache(100);
        for (int i = 0; i != 100; ++i)
            cache.put(i, i + 1000);
        int *value = cache.get(0);
        EXPECT_EQ(*value, 1000);
        cache.put(100, 1100);
        CHECK_MAP_LIST(cache, 100, 1100);
        value = cache.get(1);
        EXPECT_EQ(value, nullptr);
    }
    {
        LRUCacheExtended cache(1);
        EXPECT_EQ(0, cache.size());
        EXPECT_FALSE(cache.put(1, 2));
        EXPECT_EQ(1, cache.size());
        int *value = cache.get(1);
        ASSERT_NE(nullptr, value);
        EXPECT_EQ(2, *value);
        int x = 123456;
        for (int i = 0; i < 1000; ++i)
        {
            bool f = cache.put(i, x);
            EXPECT_TRUE(f);
            int *value = cache.get(i);
            ASSERT_NE(nullptr, value);
            EXPECT_EQ(x, *value);
        }
    }
}

TEST(LRUCache, random_gets)
{
    const int N = 1000;
    LRUCacheExtended cache(N);
    for (int i = 0; i != N; ++i)
        cache.put(i, N + i);
    for (int i = 0; i < 10 * N; ++i)
    {
        int key = random() % 2 * N;
        int *value = cache.get(key);
        if (key < N)
        {
            ASSERT_NE(value, nullptr);
            EXPECT_EQ(*value, key + N);
        }
        else
        {
            EXPECT_EQ(value, nullptr);
        }
    }
}

TEST(LRUCache, empty_cache)
{
    LRUCacheExtended cache(0);
    EXPECT_NO_FATAL_FAILURE(cache.put(1, 1));
    bool was_something_erased = cache.put(1, 2);
    EXPECT_FALSE(was_something_erased);
    EXPECT_EQ(0, cache.size());
}

TEST(LRUCache, random_puts_gets)
{
    constexpr int N = 10000;
    LRUCacheExtended cache(N);
    std::array<int, N> mp;
    for (int i = 0; i < N; ++i)
    {
        int key = i;
        int value = i + N;
        mp[key] = value;
        bool was_something_removed = cache.put(key, value);
        EXPECT_FALSE(was_something_removed);
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 2 * N - 1);

    constexpr int TESTS = 100000;

    for (int i = 0, j = 0; i != TESTS; ++i, ++j)
    {
        int key = dis(gen);
        if (key > N - 1)
        {
            EXPECT_EQ(nullptr, cache.get(key));
        }
        else
        {
            int right_value = mp[key];
            int *cached_value = cache.get(key);
            ASSERT_NE(nullptr, cached_value);
            EXPECT_EQ(*cached_value, right_value);
        }
        if (j >= N)
            j = 0;
        int new_key = j;
        int new_value = key + N;
        mp[new_key] = new_value;
        bool was_something_removed = cache.put(new_key, new_value);
        EXPECT_TRUE(was_something_removed);
    }
}
