#include "LRUCache.h"

#ifndef NDEBUG
#include <cassert>
#endif

void LRUCache::erase_oldest()
{
    auto list_last = --list.end();
    Key key = list_last->first;
    list.erase(list_last);
    auto map_it = map.find(key);
    map.erase(map_it);
}

void LRUCache::cache_new_element(LRUCache::Key key, LRUCache::Value value)
{
    list.push_front({key, value});
    map[key] = list.begin();
}

bool LRUCache::put(LRUCache::Key key, LRUCache::Value value)
{
#ifndef NDEBUG
    assert(map.size() == list.size());
#endif

    // check if key already exists in cache
    auto map_it = map.find(key);
    if (map_it != map.end())
    {
        // key exists, so rewrite it
        auto new_it = touch(map_it->second);
#ifndef NDEBUG
        assert(new_it->first == key);
#endif
        *new_it = {key, value};
        map[key] = new_it;
        return true;
    }

    if (map.size() < maxSize)
    {
        cache_new_element(key, value);
        return false;
    }

        // achived maxSize
#ifndef NDEBUG
    assert(map.size() == maxSize);
#endif
    if (maxSize == 0)
        return false;
    erase_oldest();
    cache_new_element(key, value);
    return true;
}

size_t LRUCache::size() const
{
#ifndef NDEBUG
    assert(map.size() == list.size());
#endif
    return map.size();
}

LRUCache::List_iterator LRUCache::touch(LRUCache::List_iterator list_it)
{
    Node x = *list_it;
    list.erase(list_it);
    list.push_front(x);
    return list.begin();
}

LRUCache::Value *LRUCache::get(LRUCache::Key x)
{
#ifndef NDEBUG
    assert(map.size() == list.size());
#endif
    auto it = map.find(x);
    if (it == map.end())
    {
        return nullptr;
    }
    auto new_it = touch(it->second);
    map[x] = new_it;
    return &(new_it->second);
}
