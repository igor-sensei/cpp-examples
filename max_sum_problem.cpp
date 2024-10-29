// Given a directed acyclic graph with integer node values, find the path that yields the maximum sum of values.
// This implementation uses depth-first search combined with memoization to optimize performance.
// By caching the maximum path sum for each node, we avoid redundant calculations for overlapping sub-paths.
//
// This approach ensures efficient computation of the maximum path sum in O(V + E) time complexity,
// where V is the number of nodes and E is the number of edges in the graph.

#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <climits>
#include <cassert> // For a simple unit test

// Structure to hold the graph
struct Node
{
    int value;
    std::vector<Node *> children;
};

int maxPathSumCached(Node *node, std::unordered_map<Node *, int> &cache)
{
    if (!node)
        return 0; // Null node has a sum of 0

    // Check if the result for this node is already stored in the cache
    if (cache.find(node) != cache.end())
        return cache[node];

    int maxSum = 0;
    for (Node *child : node->children)
        maxSum = std::max(maxSum, maxPathSumCached(child, cache));
    maxSum += node->value; // Add the current node's value
    cache[node] = maxSum;  // Add to the cache
    return maxSum;
}

int maxPathSum(Node *root)
{
    std::unordered_map<Node *, int> cache; // Cache to store max path sums for each node
    return maxPathSumCached(root, cache);
}

int main()
{
    // Construct a graph
    Node root = Node{10};
    Node child1 = Node{2};
    Node child2 = Node{10};
    Node child3 = Node{-25};
    Node child4 = Node{3};
    Node child5 = Node{4};

    // Build its structure
    root.children = {&child1, &child2};
    child1.children = {&child3};
    child3.children = {&child4, &child5};

    std::cout << "Maximum path sum: " << maxPathSum(&root) << std::endl;
    assert(maxPathSum(&root) == 20);
    return 0;
}