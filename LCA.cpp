#include "LCA.hpp"

#include <iostream>
#include <cmath>
#include <stack>
#include <cstdlib>

LCA::LCA(const std::vector<int> &nodeVals,
         const std::vector<int> &parent,
         const std::vector<std::deque<int>> &children,
         int root) : nodeVals(nodeVals),
                     parent(parent),
                     children(children),
                     root(root)
{
    preprocessForLCA();
}

int LCA::lca(int i, int j)
{
    if (i < 0 || j < 0 || i >= nodeVals.size() || j >= nodeVals.size())
    {
        throw std::out_of_range("Index out of bounds.");
    }

    // convert i, j to their first occurrence within euler tour
    i = firstOccurrence[i];
    j = firstOccurrence[j];

    // enforce i < j
    if (j < i)
        std::swap(i, j);

    int iBlock = i / blockSize, jBlock = j / blockSize;

    int resIndex;
    if (iBlock != jBlock) // first case: i and j not in the same block
    {
        resIndex = minByDepth(suffixMinIndex[i], prefixMinIndex[j]);
        if (jBlock > iBlock + 1) // there are whole blocks in between
        {
            resIndex = minByDepth(resIndex, blockRangeRMQ(iBlock + 1, jBlock - 1));
        }
    }
    else // second case: i and j within single block
    {
        resIndex = singleBlockRMQ(iBlock, i % blockSize, j % blockSize);
    }

    return etSeq[resIndex];
}

int LCA::singleBlockRMQ(int block, int i, int j)
{
    int minWithinBlockIndex = MIN[blockBinaryString[block]][i][j];
    return block * blockSize + minWithinBlockIndex;
}

/*
Computes the min across a range of whole blocks. Returns the index within the ET
*/
int LCA::blockRangeRMQ(int k, int l)
{
    // trivial case: the range is a single block
    if (k == l)
    {
        return blockMinIndex[k];
    }

    // exponent for next-smallest power of two less than the range size
    int e = floor(log2(l - k + 1));
    int windowSize = 1 << e; // 2^e

    return minByDepth(pow2Windows[e - 1][k], pow2Windows[e - 1][l + 1 - windowSize]);
}

void LCA::preprocessForLCA()
{    
    // Perform Euler Tour of the input tree
    eulerTour();

    blockSize = floor(log2(depthEtSeq.size()));

    // Build vectors prefixMinIndex, suffixMinIndex and blockMinIndex
    prefixMinIndex.resize(depthEtSeq.size());
    suffixMinIndex.resize(depthEtSeq.size());
    blockMinIndex.resize(depthEtSeq.size() / blockSize); // uneven tail elems are discarded

    int pMinIndex = 0, sMinIndex = depthEtSeq.size() - 1;
    prefixMinIndex[0] = pMinIndex;
    suffixMinIndex[depthEtSeq.size() - 1] = sMinIndex;

    for (int i = 1; i < depthEtSeq.size(); i++)
    {
        pMinIndex = minByDepth(pMinIndex, i);
        prefixMinIndex[i] = pMinIndex;

        if ((i + 1) % blockSize == 0)
        {
            blockMinIndex[i / blockSize] = pMinIndex;
            pMinIndex = i + 1;
        }

        if ((depthEtSeq.size() - i) % blockSize == 0)
        {
            sMinIndex = depthEtSeq.size() - i - 1;
        }

        sMinIndex = minByDepth(sMinIndex, depthEtSeq.size() - 1 - i);
        suffixMinIndex[depthEtSeq.size() - 1 - i] = sMinIndex;
    }

    // Build Sparse Table (power-of-two sized windows) on top of the blockMinIndex array
    int levels = floor(log2(blockMinIndex.size()));
    pow2Windows.resize(levels);

    // Compute first size-2 window array directly from blockMinIndex
    pow2Windows[0].resize(blockMinIndex.size() - 1);
    for (int i = 0; i < pow2Windows[0].size(); i++)
    {
        pow2Windows[0][i] = minByDepth(blockMinIndex[i], blockMinIndex[i + 1]);
    }

    // Compute subsequent arrays one level at a time
    for (int j = 2; j <= levels; j++)
    {
        int windowSize = 1 << j; // 2^j
        pow2Windows[j - 1].resize(blockMinIndex.size() - windowSize + 1);
        for (int i = 0; i < pow2Windows[j - 1].size(); i++)
        {
            pow2Windows[j - 1][i] = minByDepth(pow2Windows[j - 2][i], pow2Windows[j - 2][i + windowSize / 2]);
        }
    }

    // for each block, compute its binary string from the +/-1 depth changes;
    // this binary string is encoded as an int;
    // blockBinaryString[b] contains the binary string for block b
    blockBinaryString.assign(blockMinIndex.size() + 1, 0);
    int maxBinaryString = 0;
    int b = 0, i = 0, j = 0;
    while (i < etSeq.size())
    {
        if (j == blockSize)
        {
            j = 1;
            b++;
            i++;
            continue;
        }
        if (j > 0 && depthEtSeq[i] == depthEtSeq[i - 1] + 1) // depth increases by 1
        {
            blockBinaryString[b] += 1 << (j - 1); // add '1' corresponding to 2^(j-1)
            maxBinaryString = std::max(maxBinaryString, blockBinaryString[b]);
        }
        i++;
        j++;
    }

    // precompute MIN table (only for block binary strings that are actually present)
    MIN.resize(maxBinaryString + 1);
    for (int b = 0; b < blockBinaryString.size(); b++)
    {
        if (!MIN[blockBinaryString[b]].empty()) // sub-table already built
        {
            continue;
        }

        MIN[blockBinaryString[b]].assign(blockSize, std::vector<int>(blockSize));
        for (int i = 0; i < blockSize; i++)
        {
            MIN[blockBinaryString[b]][i][i] = i;
            for (int j = i + 1; j < blockSize; j++)
            {
                MIN[blockBinaryString[b]][i][j] = MIN[blockBinaryString[b]][i][j - 1];
                if (b * blockSize + j < etSeq.size())
                {
                    MIN[blockBinaryString[b]][i][j] = minByDepth(b * blockSize + MIN[blockBinaryString[b]][i][j],
                                                                 b * blockSize + j) -
                                                      b * blockSize;
                }
            }
        }
    }
}

// finds which index (i or j) corresponds to the minimum depth within the euler tour
int LCA::minByDepth(int i, int j)
{
    return depthEtSeq[i] < depthEtSeq[j] ? i : j;
}

void LCA::dfs(int root, int parent, int d)
{
    firstOccurrence[root] = etSeq.size();
    etSeq.push_back(root);
    depth[root] = d;

    for (int child : children[root]) // left to right
    {
        dfs(child, root, d + 1);
        etSeq.push_back(root);
    }
}

void LCA::eulerTour()
{
    depth.assign(nodeVals.size(), 0);
    firstOccurrence.assign(nodeVals.size(), -1);
    etSeq.clear();

    dfs(root, -1, 0);

    depthEtSeq.resize(etSeq.size());

    for (int i = 0; i < etSeq.size(); i++)
    {
        depthEtSeq[i] = depth[etSeq[i]];
    }
}

