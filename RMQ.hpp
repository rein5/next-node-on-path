#ifndef RMQ_HPP
#define RMQ_HPP

#include <vector>

class RMQ
{
public:
    RMQ(const std::vector<int> &seq);
    int range_min(int i, int j);

private:
    // Input sequence
    std::vector<int> seq;

    // Tree
    std::vector<int> parent;
    std::vector<int> left_child;
    std::vector<int> right_child;
    int root;

    // Euler Tour
    std::vector<int> ET_seq; // sequence of indices over seq
    std::vector<int> depth_ET_seq;
    std::vector<int> first_occurrence;
    std::vector<int> depth;

    // Block-level data
    std::vector<int> prefix_min_idx;
    std::vector<int> suffix_min_idx;
    int block_size;
    std::vector<int> block_min_idx;
    std::vector<std::vector<int>> pow_2_windows;    // Sparse Table: pow_2_windows[i] contains mins for windows of size 2^(i+1)
    std::vector<int> block_binary_string;           // maps from block index to the int-encoded block binary string
    std::vector<std::vector<std::vector<int>>> MIN; // MIN[b][i][j]: index of min depth over the range i...j within block b

    void buildCartesianTree();
    void eulerTour();
    void dfs(int root, int parent, int d);
    int min_by_depth(int i, int j);
    void preprocessForLCA();
    int pow_2_RMQ(int k, int l);
    int RMQ_within_block(int block, int i, int j);
};

#endif // RMQ_HPP