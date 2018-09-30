#ifndef LRUCACHE_H
#define LRUCACHE_H

#include <list>
#include <stddef.h>
#include <unordered_map>

class LRUCache
{
protected:
    using Key = int;
    using Value = int;
    using Node = std::pair<Key, Value>;
    using List = std::list<LRUCache::Node>;
    using List_iterator = List::iterator;

    const size_t maxSize;
    List list;
    std::unordered_map<Key, List_iterator> map;

public:
    LRUCache(size_t size) : maxSize(size) {}
    virtual ~LRUCache() {}
    bool put(Key key, Value); // Returns true if some element was deleted and false otherwize
    size_t size() const;
    struct iterator;
    Value *get(Key);

private:
    void erase_oldest();
    void cache_new_element(LRUCache::Key key, LRUCache::Value value);
    List_iterator touch(List_iterator);
};

#endif // LRUCACHE_H
