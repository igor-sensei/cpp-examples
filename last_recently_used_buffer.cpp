// Least Recently Used cache with FIFO-like eviction policy.
// Accessing an element with `get` moves it to the front, marking it as recently used.
// When capacity is exceeded, the least recently used item (at the back) is evicted.

#include <unordered_map>
#include <list>
#include <stdexcept>
#include <iostream>
#include <cassert> // For a simple unit test

class LRUCache
{
public:
    LRUCache(size_t capacity) : capacity_(capacity) {}

    // Put an element into the cache with the given key
    void put(int key, const std::string &value)
    {
        // If the key is already in the cache, update the value and move to front
        if (cache_map_.find(key) != cache_map_.end())
        {
            auto it = cache_map_[key];
            it->second = value;
            buffer_.splice(buffer_.begin(), buffer_, it); // Move to front
            return;
        }

        if (buffer_.size() >= capacity_)
        {
            // Remove the least recently used (LRU), which is at the back
            auto last = buffer_.back();
            cache_map_.erase(last.first);
            buffer_.pop_back();
        }

        // Add the new key-value pair to the front
        buffer_.emplace_front(key, value);
        cache_map_[key] = buffer_.begin();
    }

    std::string get(int key)
    {
        if (cache_map_.find(key) == cache_map_.end())
            throw std::out_of_range("Key not found");

        // Access the element and move it to the front
        auto it = cache_map_[key];
        buffer_.splice(buffer_.begin(), buffer_, it);

        return it->second;
    }

private:
    typedef std::pair<int, std::string> PairType;
    size_t capacity_;
    std::list<PairType> buffer_;                                                // Doubly-linked list to store cache data in order of use
    std::unordered_map<int, typename std::list<PairType>::iterator> cache_map_; // Maps keys to list iterators for O(1) access
};

void runTests()
{
    LRUCache buffer(3);

    // Add elements and verify retrieval
    buffer.put(1, "one");
    buffer.put(2, "two");
    buffer.put(3, "three");
    assert(buffer.get(1) == "one");
    assert(buffer.get(2) == "two");
    assert(buffer.get(3) == "three");

    // Exceeding capacity - oldest element (1) should be removed
    buffer.put(4, "four");
    try
    {
        buffer.get(1);
        assert(false); // Should not reach this line
    }
    catch (const std::out_of_range &)
    {
        // Expected, as key 1 should already be removed
    }
    assert(buffer.get(2) == "two");
    assert(buffer.get(3) == "three");
    assert(buffer.get(4) == "four");

    // Update an existing element
    buffer.put(2, "two-updated");
    assert(buffer.get(2) == "two-updated");

    // Ensure retrieving an existing element does not evict it
    buffer.get(3);         // Access element 3
    buffer.put(5, "five"); // Should evict element 4, not 3
    try
    {
        buffer.get(4);
        assert(false); // Should not reach here
    }
    catch (const std::out_of_range &)
    {
        // Expected, as key 4 should be evicted
    }
    assert(buffer.get(2) == "two-updated");
    assert(buffer.get(5) == "five");
    assert(buffer.get(3) == "three");

    std::cout << "All tests passed" << std::endl;
}

int main()
{
    runTests();
    return 0;
}
