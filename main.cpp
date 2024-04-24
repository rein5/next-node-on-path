#include <iostream>
#include <vector>
#include "RMQ.hpp"
#include "LCA.hpp"
#include "NextNodeOnPath.hpp"
#include "TestUtils.hpp"

int main()
{
    /*** RMQ data structure usage example ***/

    // Sequence
    std::vector<int> seq = {7, 4, 13, 10, -1, 21, 11, 14, 6, 7};

    // Preprocess sequence for RMQ queries
    RMQ rmq(seq);

    // Execute query
    int i = 2, j = 5;
    int m = rmq.rangeMin(i, j);

    std::cout << "Minimum in range [" << i << ", " << j << "]: " << m << std::endl;



    /*** NextNodeOnPath data structure usage example ***/

    // Tree
    std::vector<int> nodeVals = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    std::vector<int> parent = {-1, 0, 1, 1, 0, 4, 4, 4, 5, 5, 6, 7, 7, 0, 13};
    std::vector<std::vector<int>> children = {{1, 4, 13}, {2, 3}, {}, {}, {5, 6, 7}, {8, 9}, {10}, {11, 12}, {}, {}, {}, {}, {}, {14}, {}};
    int root = 0;

    // Preprocess tree for next-node-on-path queries
    NextNodeOnPath nextNodeOnPath(nodeVals, parent, children, root);

    // Execute query
    int x = 4, y = 10;
    int nextNode = nextNodeOnPath.query(x, y);

    std::cout << "Next node on the path from " << nodeVals[x] << " to " << nodeVals[y] << ": "
              << nodeVals[nextNode] << "\n\n";



    /*** Execute stress tests ***/
    testRMQ();
    testNextNodeOnPath();
}