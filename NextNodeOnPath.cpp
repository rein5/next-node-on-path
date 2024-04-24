#include "NextNodeOnPath.hpp"
#include <iostream>

NextNodeOnPath::NextNodeOnPath(const std::vector<int> &nodeVals,
                               const std::vector<int> &parent,
                               const std::vector<std::deque<int>> &children,
                               int root) : nodeVals(nodeVals),
                                           parent(parent),
                                           children(children),
                                           root(root)
{
    nodeToPreOrderPosition.resize(nodeVals.size());
    nodeToPostOrderPosition.resize(nodeVals.size());
    preOrderLabelsInPostOrder.resize(nodeVals.size());

    // compute pre-order labels
    preOrder(root);

    // sort pre-order labels by post-order
    sortLabelsByPostOrder(root);

    // preprocess labels for range min queries
    labelsRMQ = RMQ(preOrderLabelsInPostOrder);

    // preprocess tree for LCA
    treeLCA = LCA(nodeVals, parent, children, root);
}

int NextNodeOnPath::query(int i, int j)
{
    if (treeLCA.lca(i, j) != i) // j not in i's subtree: go up
    {
        return parent[i];
    }

    // j is in i's subtree: descent into the correct child, found by RMQ on preOrderLabelsInPostOrder
    return preOrderTraversal[labelsRMQ.rangeMin(nodeToPostOrderPosition[j], nodeToPostOrderPosition[i] - 1)];
}

void NextNodeOnPath::preOrder(int root)
{
    nodeToPreOrderPosition[root] = preOrderTraversal.size();
    preOrderTraversal.push_back(root);

    for (int node : children[root]) // left to right
    {
        preOrder(node);
    }
}

void NextNodeOnPath::sortLabelsByPostOrder(int root)
{
    for (int node : children[root]) // left to right
    {
        sortLabelsByPostOrder(node);
    }

    nodeToPostOrderPosition[root] = postOrderTraversal.size();
    preOrderLabelsInPostOrder[postOrderTraversal.size()] = nodeToPreOrderPosition[root];
    postOrderTraversal.push_back(root);
}
