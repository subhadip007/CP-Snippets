#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

// 1. Custom hasher for std::vector (still required for unordered_map)
struct VectorHasher {
    size_t operator()(const vector<int>& V) const {
        size_t hash = V.size();
        for(int i : V) {
            hash ^= i + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        }
        return hash;
    }
};

// 2. The standalone functional tree hasher
// Returns the unique ID (hash) of the root. 
// Populates unique_count with the total number of distinct subtrees.
int computeTreeHash(const vector<vector<int>>& adj, int root, int& unique_count) {
    unordered_map<vector<int>, int, VectorHasher> shape_to_id;
    int next_id = 1;

    // Recursive DFS lambda. Captures shape_to_id and next_id by reference.
    auto dfs = [&](auto& self, int node) -> int {
        vector<int> child_ids;
        
        // Get IDs of all children
        for (int child : adj[node]) {
            child_ids.push_back(self(self, child));
        }
        
        // Canonicalize the shape by sorting
        sort(child_ids.begin(), child_ids.end());
        
        // Assign new ID if we haven't seen this shape
        if (shape_to_id.find(child_ids) == shape_to_id.end()) {
            shape_to_id[child_ids] = next_id++;
        }
        
        return shape_to_id[child_ids];
    };

    // Run DFS from root
    int root_hash = dfs(dfs, root);
    
    // Save the total number of unique shapes found
    unique_count = shape_to_id.size();
    
    return root_hash;
}

//usage example
 //     Root
 //      /    \
 //   NodeA  NodeB
 //   /   \     \
 // Leaf1 Leaf2 Leaf3

int main() {
    int n = 6;
    vector<vector<int>> adj(n);

    // Root (0) -> NodeA (1) and NodeB (2)
    adj[0].push_back(1);
    adj[0].push_back(2);

    // NodeA (1) -> Leaf1 (3) and Leaf2 (4)
    adj[1].push_back(3);
    adj[1].push_back(4);

    // NodeB (2) -> Leaf3 (5)
    adj[2].push_back(5);

    int unique_subtrees = 0;
    int root_id = computeTreeHash(adj, 0, unique_subtrees);

    cout << "Hash ID for the entire tree (Root ID): " << root_id << "\n";
    cout << "Total unique subtree shapes: " << unique_subtrees << "\n";

    return 0;
}
