#include <iostream>
#include <vector>
#include "RMQ.hpp"
#include "LCA.hpp"
#include "NextNodeOnPath.hpp"

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

int main()
{
    std::vector<int> nodeVals = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    std::vector<int> parent = {-1, 0, 1, 1, 0, 4, 4, 4, 5, 5, 6, 7, 7, 0, 13};
    std::vector<std::vector<int>> children = {{1, 4, 13}, {2, 3}, {}, {}, {5, 6, 7}, {8, 9}, {10}, {11, 12}, {}, {}, {}, {}, {}, {14}, {}};
    int root = 0;

    NextNodeOnPath nextNodeOnPath(nodeVals, parent, children, root);

    int x = 4, y = 10;
    int nextNode = nextNodeOnPath.query(x, y);

    std::cout << "next node on the path from " << nodeVals[x] << " to " << nodeVals[y] << ": "
              << nodeVals[nextNode] << std::endl;

    rmq_stress_test();
}