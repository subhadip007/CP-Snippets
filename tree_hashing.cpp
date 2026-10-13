#include <vector>
#include <map>
#include <algorithm>

/**
 * Counts structurally unique rooted subtrees using AHU canonical hashing.
 * 
 * @param root        0-indexed or 1-indexed root node
 * @param adj_matrix  n x n (or (n+1) x (n+1)) adjacency matrix: adj_matrix[u][v] != 0 if edge exists
 * @return            Total count of unique rooted subtree shapes
 */
int countUniqueSubtrees(int root, const std::vector<std::vector<int>>& adj_matrix) {
    int n = adj_matrix.size();
    std::map<std::vector<int>, int> memo;
    int next_id = 1;

    auto dfs = [&](auto& self, int u, int p) -> int {
        std::vector<int> child_hashes;
        
        for (int v = 0; v < n; ++v) {
            if (adj_matrix[u][v] && v != p) {
                child_hashes.push_back(self(self, v, u));
            }
        }
        
        std::sort(child_hashes.begin(), child_hashes.end());
        
        auto it = memo.find(child_hashes);
        if (it == memo.end()) {
            memo[child_hashes] = next_id++;
            return next_id - 1;
        }
        return it->second;
    };

    dfs(dfs, root, -1);
    return memo.size();
}


//Driver code example
int main() {
    int n;
    cin >> n;
    
    vector<vector<int>> adj(n + 1);
    
    for(int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v; 
        adj[u].push_back(v); 
        adj[v].push_back(u); 
    }
    
    // Unpack the pair
    int unique_subtrees = countUniqueSubtrees(1, adj);
    
    cout << "Total unique subtrees: " << unique_subtrees << "\n";
    
    return 0; 
}
