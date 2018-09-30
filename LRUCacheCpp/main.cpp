#include "LRUCache.h"
#include <iostream>

using namespace std;

int main()
{
    LRUCache lrucache(10);
    lrucache.put(1, 2);
    std::cout << "cached: 1 --> 2" << endl;
    int *value = lrucache.get(1);
    if (value == nullptr)
        cout << "value is nullptr" << endl;
    else
        std::cout << "key: 1, value: " << *value << endl;
    return 0;
}
