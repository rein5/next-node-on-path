#include "RMQ.hpp"
#include <stack>

RMQ::RMQ(const std::vector<int> &seq) : LCA()
{
    if (seq.empty())
    {
        throw std::invalid_argument("Input sequence cannot be empty.");
    }

    nodeVals = seq;

    if (seq.size() > 2)
    {
        buildCartesianTree();
        preprocessForLCA();
    } // else, don't bother. RMQs of size <= 2 are an edge case we handle directly
}

int RMQ::rangeMin(int i, int j)
{
    if (i < 0 || j < 0 || i >= nodeVals.size() || j >= nodeVals.size())
    {
        throw std::out_of_range("Index out of bounds.");
    }

    // edge case
    if (abs(i - j) < 2)
        return nodeVals[i] < nodeVals[j] ? nodeVals[i] : nodeVals[j];

    // compute and return the LCA between i and j within the Cartesian Tree
    return nodeVals[lca(i, j)];
}

void RMQ::buildCartesianTree()
{
    // Build Cartesian Tree from input sequence
    parent.assign(nodeVals.size(), -1);
    children.assign(nodeVals.size(), {});
    std::stack<int> s;
    for (int i = 0; i < nodeVals.size(); i++)
    {
        int last = -1;
        while (!s.empty() && nodeVals[s.top()] >= nodeVals[i])
        {
            last = s.top();
            s.pop();
        }
        if (!s.empty())
            parent[i] = s.top();
        if (last >= 0)
            parent[last] = i;
        s.push(i);
    }

    for (int i = 0; i < parent.size(); i++)
    {
        if (parent[i] == -1) // root
        {
            root = i;
        }
        else if (parent[i] < i) // node i must be right child of its parent
        {
            children[parent[i]].push_back(i); // insert as last child
        }
        else // parent[i] > i   -> node i must be left child of its parent
        {
            children[parent[i]].insert(children[parent[i]].begin(), i); // insert as first child
        }
    }
}
