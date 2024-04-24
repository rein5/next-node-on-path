#ifndef NEXTNODEONPATH_HPP
#define NEXTNODEONPATH_HPP

#include <vector>
#include <deque>
#include "RMQ.hpp"
#include "LCA.hpp"


class NextNodeOnPath
{
public:
    /**
     * Constructor. It takes arrays for node values, parent and child links, 
     * as well as the index of the root in nodeVals.
     */
    NextNodeOnPath(const std::vector<int>& nodeVals, 
        const std::vector<int>& parent, 
        const std::vector<std::deque<int>>& children, 
        int root); 

    /**
     * Finds the next node on the unique path between nodes i and j.
     * @param i index of first node in nodeVals
     * @param j index of second node in nodeVals
     * @return next-node-on-path(i, j) (index in nodeVals)
     */
    int query(int i, int j);

private: 
    // Tree representation
    std::vector<int> nodeVals; 
    std::vector<int> parent;
    std::vector<std::deque<int>> children;
    int root;

    // Traversals data
    std::vector<int> preOrderTraversal;
    std::vector<int> postOrderTraversal;
    std::vector<int> nodeToPreOrderPosition;
    std::vector<int> nodeToPostOrderPosition;
    std::vector<int> preOrderLabelsInPostOrder;

    // RMQ and LCA objects
    RMQ labelsRMQ;
    LCA treeLCA;

    void preOrder(int root);
    void sortLabelsByPostOrder(int root);
};

#endif // NEXTNODEONPATH_HPP