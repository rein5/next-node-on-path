#include "TestUtils.hpp"
#include "RMQ.hpp"
#include "LCA.hpp"
#include "NextNodeOnPath.hpp"
#include <iostream>

#define MAX_RMQ_TEST_SEQ_LENGTH 500
#define MAX_NNOP_TEST_TREE_SIZE 500

void testRMQ()
{
    std::cout << "+++ Testing the RMQ data structure against random sequences of length up to " << MAX_RMQ_TEST_SEQ_LENGTH << " +++\n";
    srand(time(0));
    std::vector<int> seq;
    int totalCorrect = 0, total = 0;
    for (int sequenceLength = 1; sequenceLength <= MAX_RMQ_TEST_SEQ_LENGTH; sequenceLength++)
    {
        if (sequenceLength % 50 == 0)
            std::cout << "Sequence length: " << sequenceLength << "..." << std::endl;

        // generate random sequence
        seq.resize(sequenceLength);
        for (int i = 0; i < sequenceLength; i++)
        {
            seq[i] = std::rand() % 201 - 100; // random int in [-100, 100] range
        }

        // preprocess sequence for RMQ queries
        RMQ rmq(seq);

        // test RMQ queries on current sequence
        int correct = 0, wrong = 0;
        for (int i = 0; i < seq.size(); i++)
        {
            for (int j = 0; j < seq.size(); j++)
            {
                // find min in i...j
                int k, l;
                if (i <= j)
                {
                    k = i;
                    l = j;
                }
                else
                {
                    k = j;
                    l = i;
                }

                int m = seq[k];
                for (int q = k + 1; q <= l; q++)
                    m = std::min(m, seq[q]);

                int res = rmq.rangeMin(i, j);
                if (m == res)
                {
                    correct++;
                }
                else
                {
                    wrong++;
                }
            }
        }

        total += correct + wrong;
        totalCorrect += correct;
    }

    std::cout << "\n\t******* Total correct queries: " << totalCorrect << "/" << total << "\n\n";
}

// Generates next-node-on-path test samples for the given tree using Depth First Search 
// samples correspond to paths from a fixed source node to any other node
void dfsNextNodeOnPathSamples(int root,
                              std::vector<int> &parent,
                              std::vector<std::vector<int>> &children,
                              std::list<int> &path,
                              std::vector<bool> &visited,
                              std::vector<std::vector<int>> &testSamples)
{
    path.push_back(root);
    visited[root] = true;

    // add test sample
    if (root != path.front())
    {
        testSamples.push_back({path.front(), *std::next(path.begin()), path.back()}); // (source, next, destination)
    }

    // dfs into children and parent
    for (int child : children[root])
    {
        if (!visited[child])
        {
            dfsNextNodeOnPathSamples(child, parent, children, path, visited, testSamples);
        }
    }
    if (parent[root] != -1 && !visited[parent[root]])
    {
        dfsNextNodeOnPathSamples(parent[root], parent, children, path, visited, testSamples);
    }

    path.pop_back();
}

void testNextNodeOnPath()
{
    std::cout << "+++ Testing the NextNodeOnPath data structure against random n-ary trees of size up to " << MAX_NNOP_TEST_TREE_SIZE << " +++\n";
    srand(time(0));
    int totalCorrect = 0, total = 0;
    std::vector<int> nodeVals, parent;
    std::vector<std::vector<int>> children;
    int root = 0;

    for (int treeSize = 2; treeSize <= MAX_NNOP_TEST_TREE_SIZE; treeSize++)
    {
        if (treeSize % 50 == 0)
            std::cout << "Tree size: " << treeSize << "..." << std::endl;

        // generate random n-ary tree
        nodeVals.resize(treeSize);
        parent.resize(treeSize);
        children.assign(treeSize, {});

        nodeVals[root] = std::rand() % 201 - 100;
        parent[root] = -1;
        for (int i = 1; i < treeSize; i++)
        {
            nodeVals[i] = std::rand() % 201 - 100; // random int in [-100, 100] range
            int randomParent = std::rand() % i;    // pick random parent in [0, i - 1] range
            parent[i] = randomParent;
            children[randomParent].push_back(i); // add i to randomParent's children
        }

        // generate test samples for current tree with Depth First Search
        std::vector<std::vector<int>> testSamples; // contains tuples (source, next, destination)
        for (int node = 0; node < nodeVals.size(); node++)
        {
            std::list<int> path;
            std::vector<bool> visited(nodeVals.size(), false);
            dfsNextNodeOnPathSamples(node, parent, children, path, visited, testSamples);
        }

        // preprocess tree for next-node-on-path queries
        NextNodeOnPath nextNodeOnPath(nodeVals, parent, children, root);

        // test queries
        int correct = 0, wrong = 0;
        for (std::vector<int> &sample : testSamples)
        {
            int x = sample[0], next = sample[1], y = sample[2];

            int computedNext = nextNodeOnPath.query(x, y);

            if (computedNext == next)
            {
                correct++;
            }
            else
            {
                wrong++;
            }
        }

        total += correct + wrong;
        totalCorrect += correct;
    }

    std::cout << "\n\t******* Total correct queries: " << totalCorrect << "/" << total << "\n\n";
}
