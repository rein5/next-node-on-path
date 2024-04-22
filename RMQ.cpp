#include "RMQ.hpp"

#include <iostream>
#include <cmath>
#include <stack>
#include <cstdlib>

RMQ::RMQ(const std::vector<int> &seq) : seq(seq)
{
    if (seq.size() > 2)
    {
        buildCartesianTree();
        preprocessForLCA();
    }
}

int RMQ::range_min(int i, int j)
{
    // edge case
    if (abs(i - j) < 2)
        return seq[i] < seq[j] ? seq[i] : seq[j];

    // convert i, j to their first occurrence within euler tour
    i = first_occurrence[i];
    j = first_occurrence[j];

    // enforce i < j
    if (j < i)
        std::swap(i, j);

    int i_block = i / block_size, j_block = j / block_size;

    int res_idx = 0;
    if (i_block != j_block) // first case: i and j not in the same block
    {
        res_idx = min_by_depth(suffix_min_idx[i], prefix_min_idx[j]);
        if (j_block > i_block + 1) // there are whole blocks in between
        {
            res_idx = min_by_depth(res_idx, pow_2_RMQ(i_block + 1, j_block - 1));
        }
    }
    else // second case: i and j within single block
    {
        res_idx = RMQ_within_block(i_block, i % block_size, j % block_size);
    }

    return seq[ET_seq[res_idx]];
}

int RMQ::RMQ_within_block(int block, int i, int j)
{
    int min_within_block_idx = MIN[block_binary_string[block]][i][j];
    return block * block_size + min_within_block_idx;
}

/*
Computes the min across a range of whole blocks. Returns the index within the ET
*/
int RMQ::pow_2_RMQ(int k, int l)
{
    // trivial case: the range is a single block
    if (k == l)
    {
        return block_min_idx[k];
    }

    // exponent for next-smallest power of two less than the range size
    int e = floor(log2(l - k + 1));
    int win_size = 1 << e; // 2^e

    return min_by_depth(pow_2_windows[e - 1][k], pow_2_windows[e - 1][l + 1 - win_size]);
}

void RMQ::preprocessForLCA()
{
    // Perform Euler Tour of the Cartesian Tree built from the input sequence
    eulerTour();

    block_size = floor(log2(depth_ET_seq.size()));

    // Build vectors prefix_min_idx, suffix_min_idx and block_min_idx
    prefix_min_idx.resize(depth_ET_seq.size());
    suffix_min_idx.resize(depth_ET_seq.size());
    block_min_idx.resize(depth_ET_seq.size() / block_size); // uneven tail elems are discarded

    int pmin_idx = 0, smin_idx = depth_ET_seq.size() - 1;
    prefix_min_idx[0] = pmin_idx;
    suffix_min_idx[depth_ET_seq.size() - 1] = smin_idx;

    for (int i = 1; i < depth_ET_seq.size(); i++)
    {
        pmin_idx = min_by_depth(pmin_idx, i);
        prefix_min_idx[i] = pmin_idx;

        if ((i + 1) % block_size == 0)
        {
            block_min_idx[i / block_size] = pmin_idx;
            pmin_idx = i + 1;
        }

        if ((depth_ET_seq.size() - i) % block_size == 0)
        {
            smin_idx = depth_ET_seq.size() - i - 1;
        }

        smin_idx = min_by_depth(smin_idx, depth_ET_seq.size() - 1 - i);
        suffix_min_idx[depth_ET_seq.size() - 1 - i] = smin_idx;
    }

    // Build Sparse Table (power-of-two sized windows) on top of the block_min_idx array
    int levels = floor(log2(block_min_idx.size()));
    pow_2_windows.resize(levels);

    // Compute first size-2 window array directly from block_min_idx
    pow_2_windows[0].resize(block_min_idx.size() - 1);
    for (int i = 0; i < pow_2_windows[0].size(); i++)
    {
        pow_2_windows[0][i] = min_by_depth(block_min_idx[i], block_min_idx[i + 1]);
    }

    // Compute subsequent arrays one level at a time
    for (int j = 2; j <= levels; j++)
    {
        int win_size = 1 << j; // 2^j
        pow_2_windows[j - 1].resize(block_min_idx.size() - win_size + 1);
        for (int i = 0; i < pow_2_windows[j - 1].size(); i++)
        {
            pow_2_windows[j - 1][i] = min_by_depth(pow_2_windows[j - 2][i], pow_2_windows[j - 2][i + win_size / 2]);
        }
    }

    // for each block, compute its binary string from the +/-1 depth changes;
    // this binary string is encoded as an int;
    // block_binary_string[b] contains the binary string for block b
    block_binary_string.assign(block_min_idx.size() + 1, 0);
    int max_binary_string = 0;
    int b = 0, i = 0, j = 0;
    while (i < ET_seq.size())
    {
        if (j == block_size)
        {
            j = 1;
            b++;
            i++;
            continue;
        }
        if (j > 0 && depth_ET_seq[i] == depth_ET_seq[i - 1] + 1) // depth increases by 1
        {
            block_binary_string[b] += 1 << (j - 1); // add '1' corresponding to 2^(j-1)
            max_binary_string = std::max(max_binary_string, block_binary_string[b]);
        }
        i++;
        j++;
    }

    // precompute MIN table (only for block binary strings that are actually present)
    MIN.resize(max_binary_string + 1);
    for (int b = 0; b < block_binary_string.size(); b++)
    {
        if (!MIN[block_binary_string[b]].empty()) // sub-table already built
        {
            continue;
        }

        MIN[block_binary_string[b]].assign(block_size, std::vector<int>(block_size));
        for (int i = 0; i < block_size; i++)
        {
            MIN[block_binary_string[b]][i][i] = i;
            for (int j = i + 1; j < block_size; j++)
            {
                MIN[block_binary_string[b]][i][j] = MIN[block_binary_string[b]][i][j - 1];
                if (b * block_size + j < ET_seq.size())
                {
                    MIN[block_binary_string[b]][i][j] = min_by_depth(b * block_size + MIN[block_binary_string[b]][i][j],
                                                                     b * block_size + j) -
                                                        b * block_size;
                }
            }
        }
    }
}

// finds which index (i or j) corresponds to the minimum depth within the euler tour
int RMQ::min_by_depth(int i, int j)
{
    return depth_ET_seq[i] < depth_ET_seq[j] ? i : j;
}

void RMQ::dfs(int root, int parent, int d)
{
    first_occurrence[root] = ET_seq.size();
    ET_seq.push_back(root);
    depth[root] = d;

    if (left_child[root] != -1)
    {
        dfs(left_child[root], root, d + 1);
        ET_seq.push_back(root);
    }

    if (right_child[root] != -1)
    {
        dfs(right_child[root], root, d + 1);
        ET_seq.push_back(root);
    }
}

void RMQ::eulerTour()
{
    depth.assign(seq.size(), 0);
    first_occurrence.assign(seq.size(), -1);
    ET_seq.clear();

    dfs(root, -1, 0);

    depth_ET_seq.resize(ET_seq.size());

    for (int i = 0; i < ET_seq.size(); i++)
    {
        depth_ET_seq[i] = depth[ET_seq[i]];
    }
}

void RMQ::buildCartesianTree()
{
    // Build Cartesian Tree from input sequence
    parent.assign(seq.size(), -1);
    left_child.assign(seq.size(), -1);
    right_child.assign(seq.size(), -1);
    std::stack<int> s;
    for (int i = 0; i < seq.size(); i++)
    {
        int last = -1;
        while (!s.empty() && seq[s.top()] >= seq[i])
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
            right_child[parent[i]] = i;
        }
        else // parent[i] > i   -> node i must be left child of its parent
        {
            left_child[parent[i]] = i;
        }
    }
}
