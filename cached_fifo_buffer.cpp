// Implement a FIFO buffer with a fixed capacity.

#include <unordered_map>
#include <list>
#include <stdexcept>
#include <iostream>
#include <cassert> // For a simple unit test

class FIFOBuffer
{
public:
    FIFOBuffer(size_t capacity) : capacity_(capacity) {}

    // Put an element into the buffer with the given key
    void put(int key, const std::string &value)
    {
        // If the key is already in the buffer, update the value and move to front
        if (cache_map_.find(key) != cache_map_.end())
        {
            auto it = cache_map_[key];
            it->second = value;
            buffer_.splice(buffer_.begin(), buffer_, it); // Move to front
            return;
        }

        if (buffer_.size() >= capacity_)
        {
            // Remove the least recently added (FIFO), which is at the back
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
        return cache_map_[key]->second;
    }

private:
    typedef std::pair<int, std::string> PairType;
    size_t capacity_;
    std::list<PairType> buffer_;                                                // Double-linked list to store the cache data in FIFO order
    std::unordered_map<int, typename std::list<PairType>::iterator> cache_map_; // Maps keys to list iterators for O(1) access
};

void runTests()
{
    FIFOBuffer buffer(3);

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

    std::cout << "All tests passed" << std::endl;
}

int main()
{
    runTests();
    return 0;
}
