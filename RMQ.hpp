#ifndef RMQ_HPP
#define RMQ_HPP

#include <vector>

/**
 * Class to perform Range Minimum Queries (RMQ) over a static sequence in O(1) time, after
 * O(n) space and time preprocessing.
 * The RMQ problem is solved by reduction to an LCA problem over a Cartesian Tree built from the sequence,
 * which is again reduced to a simplified "+/-1 RMQ" problem over the depth Euler Tour of the tree.
 */
class RMQ
{
public:
    /**
     * Constructor. It preprocesses the input sequence to allow for fast RMQ queries.
     * @param seq The input sequence over which RMQs will be performed.
     */
    RMQ(const std::vector<int> &seq);

    /**
     * Finds the minimum value in the sequence over the range [i, j].
     * @param i Range start index (inclusive).
     * @param j Range end index (inclusive).
     * @return Minimum value in the range [i, j].
     */
    int rangeMin(int i, int j);

private:
    std::vector<int> seq; // Input sequence over which RMQs are performed

    // Tree representation
    std::vector<int> parent;
    std::vector<int> leftChild;
    std::vector<int> rightChild;
    int root;

    // Euler Tour
    std::vector<int> etSeq; // sequence of indices over seq
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

    void buildCartesianTree();
    void eulerTour();
    void dfs(int root, int parent, int d);
    int minByDepth(int i, int j);
    void preprocessForLCA();
    int blockRangeRMQ(int k, int l);
    int singleBlockRMQ(int block, int i, int j);
};

#endif // RMQ_HPP