#ifndef RMQ_HPP
#define RMQ_HPP

#include <vector>
#include "LCA.hpp"

/**
 * Class to perform Range Minimum Queries (RMQ) over a static sequence in O(1) time, after
 * O(n) space and time preprocessing.
 * The RMQ problem is solved by reduction to an LCA problem over a Cartesian Tree built from the sequence,
 * which is again reduced to a simplified "+/-1 RMQ" problem over the depth Euler Tour of the tree.
 */
class RMQ : private LCA
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

    void buildCartesianTree();
};

#endif // RMQ_HPP