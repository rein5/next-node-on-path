#ifndef TESTUTILS_HPP
#define TESTUTILS_HPP

#include <vector>
#include <list>

// RMQ Test
void testRMQ();

// NextNodeOnPath Test
void testNextNodeOnPath();
void dfsNextNodeOnPathSamples(int root,
                              std::vector<int> &parent,
                              std::vector<std::vector<int>> &children,
                              std::list<int> &path,
                              std::vector<bool> &visited,
                              std::vector<std::vector<int>> &testSamples);

#endif // TESTUTILS_HPP