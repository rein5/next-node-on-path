#ifndef LCA_HPP
#define LCA_HPP

#include <vector>
#include <deque>

/**
 * Class to perform Lower Common Ancestor (LCA) queries on a tree in O(1) time, after
 * O(n) space and time preprocessing.
 * The LCA problem is solved by reduction to a simplified "+/-1 RMQ" problem 
 * over the depth Euler Tour of the tree.
 */
class LCA
{
public:
    /**
     * Constructor. It takes arrays for node values, parent and child links, 
     * as well as the index of the root within nodeVals.
     */
    LCA(const std::vector<int>& nodeVals, 
        const std::vector<int>& parent, 
        const std::vector<std::deque<int>>& children, 
        int root); 

    /**
     * Default empty constructor.
     */
    LCA(){} 

    /**
     * Finds the LCA between nodes i and j.
     * @param i index of first node within nodeVals
     * @param j index of second node within nodeVals
     * @return LCA(i, j)
     */
    int lca(int i, int j);

protected: 
    // Tree representation
    std::vector<int> nodeVals; 
    std::vector<int> parent;
    std::vector<std::deque<int>> children;
    int root;

    void preprocessForLCA();

private:
    // Euler Tour
    std::vector<int> etSeq; // sequence of indices over nodeVals
    std::vector<int> depthEtSeq;
    std::vector<int> firstOccurrence;
    std::vector<int> depth;

    // Block-level data
    int blockSize;
    std::vector<int> prefixMinIndex;
    std::vector<int> suffixMinIndex;
    std::vector<int> blockMinIndex;
    std::vector<std::vector<int>> pow2Windows;      // Sparse Table: pow2Windows[i] contains mins for windows of size 2^(i+1)
    std::vector<int> blockBinaryString;             // maps from block index to the int-encoded block binary string
    std::vector<std::vector<std::vector<int>>> MIN; // MIN[b][i][j]: index of min depth over the range i...j within block b

    void eulerTour();
    void dfs(int root, int parent, int d);
    int minByDepth(int i, int j);
    int blockRangeRMQ(int k, int l);
    int singleBlockRMQ(int block, int i, int j);
};

#endif // LCA_HPP