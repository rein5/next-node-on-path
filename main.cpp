#include <iostream>
#include <vector>
#include <cstdlib>
#include "RMQ.hpp"

#define MAX_TEST_SEQ_LENGTH 500

void stress_test()
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
    stress_test();
}