// This is a simple implementation of two sum problem.
//
//
// Notes on the parallelisation:
// The hash map-based implementation of the two-sum problem is inherently sequential.
//
// While the brute force approach can be parallelized to potentially speed up finding pairs,
// it remains inefficient due to its O(n**2) time complexity.
// Moreover, coordinating shared memory across threads adds considerable overhead,
// which diminishes any gains from parallel execution.
//
// The optimized solution that uses a hash map to store previously seen numbers
// relies on sequential checks and updates.
// This dependency between iterations makes it difficult to parallelize effectively.

#include <iostream>
#include <vector>
#include <unordered_map>
#include <cassert> // For a simple unit test

bool hasTwoSum(std::vector<int> &nums, int target)
{
    std::unordered_map<int, int> numMap; // Store the number and its index
    for (int i = 0; i < nums.size(); ++i)
    {
        int complement = target - nums[i];
        if (numMap.find(complement) != numMap.end())
            return true;
        numMap[nums[i]] = i;
    }

    return false; // No solution was found
}

void runTests()
{
    // Basic positive case
    std::vector<int> nums1 = {2, 7, 11, 15};
    assert(hasTwoSum(nums1, 9) == true);

    // No pair found
    std::vector<int> nums2 = {1, 2, 3, 4};
    assert(hasTwoSum(nums2, 8) == false);

    // Empty array should return false
    std::vector<int> nums3 = {};
    assert(hasTwoSum(nums3, 0) == false);

    // Negative numbers with valid pair
    std::vector<int> nums4 = {-3, 4, 3, 90};
    assert(hasTwoSum(nums4, 0) == true);

    // Duplicates and zero sum
    std::vector<int> nums5 = {0, 4, 0, 5};
    assert(hasTwoSum(nums5, 0) == true);

    std::cout << "All tests passed" << std::endl;
}

int main()
{
    runTests();
    return 0;
}
