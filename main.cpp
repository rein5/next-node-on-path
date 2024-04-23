#include <iostream>
#include <vector>
#include <cstdlib>
#include "RMQ.hpp"
#include "LCA.hpp"

#define MAX_TEST_SEQ_LENGTH 500

void rmq_stress_test()
{
    srand(time(0));
    std::vector<int> seq;
    int totalCorrect = 0, total = 0;
    for (int l = 1; l <= MAX_TEST_SEQ_LENGTH; l++)
    {
        if (l % 10 == 0)
            std::cout << "l: " << l << "..." << std::endl;

        seq.resize(l);
        for (int i = 0; i < l; i++)
        {
            seq[i] = std::rand() % 201 - 100; // random int in [-100, 100] range
        }

        RMQ rmq(seq);

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
        if (wrong > 0)
        {
            // Output the generated sequence
            std::cout << "Sequence of length " << l << ": {";
            for (int i = 0; i < l; i++)
            {
                std::cout << seq[i] << (i < l - 1 ? ", " : "");
            }
            std::cout << "}\n";
            std::cout << "correct test cases: " << correct << "/" << correct + wrong << std::endl;
        }

        total += correct + wrong;
        totalCorrect += correct;
    }

    std::cout << "\n\t******* TOTAL CORRECT: " << totalCorrect << "/" << total << std::endl;
}





void preOrder(int root, std::vector<std::deque<int>>& children, 
    std::vector<int>& preOrderTraversal, 
    std::vector<int>& nodeToPreOrderPosition)
{
    nodeToPreOrderPosition[root] = preOrderTraversal.size();
    preOrderTraversal.push_back(root);

    for (int node : children[root]) // left to right
    {
        preOrder(node, children, preOrderTraversal, nodeToPreOrderPosition);
    }
}

void sortLabelsByPostOrder(int root, 
                std::vector<std::deque<int>>& children, 
                std::vector<int>& preOrderTraversal, 
                std::vector<int>& postOrderTraversal, 
                std::vector<int>& nodeToPreOrderPosition,
                std::vector<int>& nodeToPostOrderPosition,
                std::vector<int>& preOrderLabelsInPostOrder)
{
    for (int node : children[root]) // left to right
    {
        sortLabelsByPostOrder(node, children, preOrderTraversal, postOrderTraversal, nodeToPreOrderPosition, nodeToPostOrderPosition, preOrderLabelsInPostOrder);
    }

    nodeToPostOrderPosition[root] = postOrderTraversal.size();
    preOrderLabelsInPostOrder[postOrderTraversal.size()] = nodeToPreOrderPosition[root];
    postOrderTraversal.push_back(root);
}


int main()
{
    std::vector<int> nodeVals = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    std::vector<int> parent =  {-1, 0, 1, 1, 0, 4, 4, 4, 5, 5, 6, 7, 7, 0, 13};
    std::vector<std::deque<int>> children = {{1, 4, 13}, {2, 3}, {}, {}, {5, 6, 7}, {8, 9}, {10}, {11, 12}, {}, {}, {}, {}, {}, {14}, {}};
    int root = 0;

    LCA lca(nodeVals, parent, children, root);

    int m = lca.lca(2, 3); // NB, these are indices

    std::cout << "lca: " << m << std::endl; 

    std::vector<int> preOrderTraversal, postOrderTraversal;
    std::vector<int> nodeToPreOrderPosition(nodeVals.size()), nodeToPostOrderPosition(nodeVals.size()), preOrderLabelsInPostOrder(nodeVals.size());
    
    preOrder(root, children, preOrderTraversal, nodeToPreOrderPosition);
    sortLabelsByPostOrder(root, children, preOrderTraversal, postOrderTraversal, nodeToPreOrderPosition, nodeToPostOrderPosition, preOrderLabelsInPostOrder);


    RMQ rmqLabels(preOrderLabelsInPostOrder);

    int x = 4, y = 10; // x is ancestor of y 

    std::cout << "rangeMin between indices: " << nodeToPostOrderPosition[y]<<", "<< nodeToPostOrderPosition[x] - 1<< std::endl; 
    int nextEdgeOnPath = nodeVals[preOrderTraversal[rmqLabels.rangeMin(nodeToPostOrderPosition[y], nodeToPostOrderPosition[x] - 1)]];

    std::cout << "next-edge-on-path(5, 11): " << nextEdgeOnPath << std::endl; 
    



    std::cout << " +++ preOrderTraversal: "  << ": {";
    for (int i = 0; i < preOrderTraversal.size(); i++)
    {
        std::cout << preOrderTraversal[i] << (i < preOrderTraversal.size() - 1 ? ", " : "");
    }
    std::cout << "}\n";

    std::cout << " +++ postOrderTraversal: "  << ": {";
    for (int i = 0; i < postOrderTraversal.size(); i++)
    {
        std::cout << postOrderTraversal[i] << (i < postOrderTraversal.size() - 1 ? ", " : "");
    }
    std::cout << "}\n";

    std::cout << " +++ nodeToPostOrderPosition: "  << ": {";
    for (int i = 0; i < nodeToPostOrderPosition.size(); i++)
    {
        std::cout << nodeToPostOrderPosition[i] << (i < nodeToPostOrderPosition.size() - 1 ? ", " : "");
    }
    std::cout << "}\n";

    std::cout << " +++ preOrderLabelsInPostOrder: "  << ": {";
    for (int i = 0; i < preOrderLabelsInPostOrder.size(); i++)
    {
        std::cout << preOrderLabelsInPostOrder[i] << (i < preOrderLabelsInPostOrder.size() - 1 ? ", " : "");
    }
    std::cout << "}\n";

    /*
    nodeVals: : {7, 4, 13, 10, -1, 21, 11, 14, 6, 7}
    root: 4
    parent: : {1, 4, 3, 1, -1, 6, 8, 6, 4, 8}
    children: : {[] [0 3 ] [] [2 ] [1 8 ] [] [5 7 ] [] [6 9 ] [] }
    */

    //std::vector<int> nodeVals = {7, 4, 13, 10, -1, 21, 11, 14, 6, 7};
    //std::vector<int> parent = {1, 4, 3, 1, -1, 6, 8, 6, 4, 8};
    //std::vector<std::deque<int>> children = {{}, {0, 3}, {}, {2}, {1, 8}, {}, {5, 7}, {}, {6, 9}, {}};
    //int root = 4;




    //RMQ rmq({7, 4, 13, 10, -1, 21, 11, 14, 6, 7});
    //int m = rmq.rangeMin(1, 3);

    //std::cout << "rmq: " << m << std::endl;

    

/*
    std::cout << "\n\n*********** euler tour: "  << ": {";
    for (int i = 0; i < etSeq.size(); i++)
    {
        std::cout << etSeq[i] << (i < etSeq.size() - 1 ? ", " : "");
    }
    std::cout << "}\n";
*/

    // rmq_stress_test();
}