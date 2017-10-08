#include "stdafx.h"
#include "DifficultyEvaluation.h"
#include <vector>
#include <utility>
#include<fstream>
using namespace std;
DifficultyEvaluation::DifficultyEvaluation()
{
    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            puzzle[i][j] = 0;
            for (int k = 1; k < 10; ++k)
            {
                candidate[i][j][k] = 1;
            }
            candidate[i][j][0] = 9;
        }
    }
}
DifficultyEvaluation::~DifficultyEvaluation(){}
void DifficultyEvaluation::PuzzleInit(int p[][9])
{
    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            puzzle[i][j] = p[i][j];
            if (puzzle[i][j] != 0)
            {
                candidate[i][j][0] = 0;
            }
        }
    }
}
void DifficultyEvaluation::CandidateDelete(int row, int col, int num)
{
    if (puzzle[row][col]==0 && candidate[row][col][num] == 1)
    {
        candidate[row][col][num] = 0;
        candidate[row][col][0]--;
    }
}
void DifficultyEvaluation::UpdateSingle()
{
    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            if (puzzle[i][j] != 0)
            {
                // update rows and columns
                for (int k = 0; k < 9; ++k)
                {
                    CandidateDelete(i, k, puzzle[i][j]);
                    CandidateDelete(k, j, puzzle[i][j]);
                }
                // update cells
                for (int ci = 3 * (i / 3); ci < 3 * (i / 3) + 3; ++ci)
                {
                    for (int cj = 3 * (j / 3); cj < 3 * (j / 3) + 3; ++cj)
                    {
                        CandidateDelete(ci, cj, puzzle[i][j]);
                    }
                }
            }
        }
    }
}
void DifficultyEvaluation::UpdateCell()
{
    for (int cellnum = 0; cellnum < 9; ++cellnum)
    {
        int rs = 3 * (cellnum / 3);
        int cs = 3 * (cellnum % 3);
        for (int num = 1; num <= 9; ++num)
        {
            vector<pair<int, int>> appear;
            for (int row = rs; row < rs + 3; ++row)
            {
                for (int col = cs; col < cs + 3; col++)
                {
                    if (puzzle[row][col]==0 && candidate[row][col][num]==1)
                    {
                        pair<int, int> tmp(row, col);
                        appear.push_back(tmp);
                    }
                }
            }
            if (appear.size() == 3)
            {
                if (appear[0].first == appear[1].first && appear[1].first == appear[2].first)
                {
                    for (int col = 0; col < 9; ++col)
                    {
                        if (col < cs || col > cs + 2)
                        {
                            CandidateDelete(appear[0].first, col, num);
                        }
                    }
                }
                else if (appear[0].second == appear[1].second && appear[1].second == appear[2].second)
                {
                    for (int row = 0; row < 9; ++row)
                    {
                        if (row < rs || row > rs + 2)
                        {
                            CandidateDelete(row, appear[0].second, num);
                        }
                    }
                }
            }
            else if (appear.size() == 2)
            {
                if (appear[0].first == appear[1].first)
                {
                    for (int col = 0; col < 9; ++col)
                    {
                        if (col < cs || col > cs + 2)
                        {
                            CandidateDelete(appear[0].first, col, num);
                        }
                    }
                }
                else if (appear[0].second == appear[1].second)
                {
                    for (int row = 0; row < 9; ++row)
                    {
                        if (row < rs || row > rs + 2)
                        {
                            CandidateDelete(row, appear[0].second, num);
                        }
                    }
                }
            }
        }
    }
}
void DifficultyEvaluation::UpdateExplicitPair()
{
    for (int k = 0; k < 9; ++k)
    {
        // kth row
        for (int col = 0; col < 8; ++col)
        {
            if (candidate[k][col][0] != 2)
                continue;
            for (int ncol = col + 1; ncol < 9; ++ncol)
            {
                if (candidate[k][ncol][0] != 2)
                    continue;
                bool r = true;
                int n1 = 0;
                int n2 = 0;
                for (int i = 1; i < 10; ++i)
                {
                    r &= candidate[k][col][i] == candidate[k][ncol][i];
                    if (candidate[k][col][i] == 1)
                    {
                        if (n1 == 0)
                        {
                            n1 = i;
                        }
                        n2 = i;
                    }
                }
                if (r)
                {
                    // update row k: delete n1, n2 except col, ncol
                    for (int i = 0; i < 9; ++i)
                    {
                        if (i != col && i != ncol)
                        {
                            CandidateDelete(k, i, n1);
                            CandidateDelete(k, i, n2);
                        }
                    }
                }
            }
        }
        // kth column
        for (int row = 0; row < 8; ++row)
        {
            if (candidate[row][k][0] != 2)
                continue;
            for (int nrow = row + 1; nrow < 9; ++nrow)
            {
                if (candidate[nrow][k][0] != 2)
                    continue;
                bool r = true;
                int n1 = 0;
                int n2 = 0;
                for (int i = 1; i < 10; ++i)
                {
                    r &= candidate[row][k][i] == candidate[nrow][k][i];
                    if (candidate[row][k][i] == 1)
                    {
                        if (n1 == 0)
                        {
                            n1 = i;
                        }
                        n2 = i;
                    }
                }
                if (r)
                {
                    // update column k: delete n1, n2 except row, nrow
                    for (int i = 0; i < 9; ++i)
                    {
                        if (i != row && i != nrow)
                        {
                            CandidateDelete(i, k, n1);
                            CandidateDelete(i, k, n2);
                        }
                    }
                }
            }
        }
        // kth cell
        for (int cln = 0; cln < 8; ++cln)
        {
            int row = 3 * (k / 3) + cln / 3;
            int col = 3 * (k % 3) + cln % 3;
            if (candidate[row][col][0] != 2)
                continue;
            for (int ncln = cln + 1; ncln < 9; ++ncln)
            {
                int nrow = 3 * (k / 3) + ncln / 3;
                int ncol = 3 * (k % 3) + ncln % 3;
                if (candidate[nrow][ncol][0] != 2)
                    continue;
                bool r = true;
                int n1 = 0;
                int n2 = 0;
                for (int i = 1; i < 10; ++i)
                {
                    r &= candidate[row][col][i] == candidate[nrow][ncol][i];
                    if (candidate[row][col][i] == 1)
                    {
                        if (n1 == 0)
                        {
                            n1 = i;
                        }
                        n2 = i;
                    }
                }
                if (r)
                {
                    int rs = 3 * (k / 3);
                    int cs = 3 * (k % 3);
                    for (int i = rs; i < rs + 3; ++i)
                    {
                        for (int j = cs; j < cs + 3; ++j)
                        {
                            if ((i != row||j != col) && (i != nrow||j != ncol))
                            {
                                CandidateDelete(i, j, n1);
                                CandidateDelete(i, j, n2);
                            }
                        }
                    }
                }
            }
        }
    }
}
void DifficultyEvaluation::UpdateHiddenPair()
{
    for (int k = 0; k < 9; ++k)
    {
        // kth row
        vector<vector<pair<int, int>>> record;
        for (int num = 1; num < 10; num++)
        {
            vector<pair<int, int>> appear;
            for (int col = 0; col < 9; col++)
            {
                if (puzzle[k][col] == 0 && candidate[k][col][num] == 1)
                {
                    pair<int, int> tmp(k, col);
                    appear.push_back(tmp);
                }
            }
            record.push_back(appear);
        }
        for (int i = 0; i < 8; i++)
        {
            for (int j = i + 1; j < 9; j++)
            {
                if (record[i].size() == 2 && record[j].size() == 2 && record[i][0] == record[j][0] && record[i][1] == record[j][1])
                {
                    for (int n = 1; n < 10; n++)
                    {
                        if (n != i + 1 && n != j + 1)
                        {
                            CandidateDelete(record[i][0].first, record[i][0].second, n);
                            CandidateDelete(record[i][1].first, record[i][1].second, n);
                        }
                    }
                }
            }
        }
        // kth column
        record.clear();
        for (int num = 1; num < 10; num++)
        {
            vector<pair<int, int>> appear;
            for (int row = 0; row < 9; row++)
            {
                if (puzzle[row][k] == 0 && candidate[row][k][num] == 1)
                {
                    pair<int, int> tmp(row, k);
                    appear.push_back(tmp);
                }
            }
            record.push_back(appear);
        }
        for (int i = 0; i < 8; i++)
        {
            for (int j = i + 1; j < 9; j++)
            {
                if (record[i].size() == 2 && record[j].size() == 2 && record[i][0] == record[j][0] && record[i][1] == record[j][1])
                {
                    for (int n = 1; n < 10; n++)
                    {
                        if (n != i + 1 && n != j + 1)
                        {
                            CandidateDelete(record[i][0].first, record[i][0].second, n);
                            CandidateDelete(record[i][1].first, record[i][1].second, n);
                        }
                    }
                }
            }
        }
        // kth cell
        record.clear();
        int rs = 3 * (k / 3);
        int cs = 3 * (k % 3);
        for (int num = 1; num < 10; num++)
        {
            vector<pair<int, int>> appear;
            for (int row = rs; row < rs + 3; row++)
            {
                for (int col = cs; col < cs + 3; col++)
                {
                    if (puzzle[row][col] == 0 && candidate[row][col][num] == 1)
                    {
                        pair<int, int> tmp(row, col);
                        appear.push_back(tmp);
                    }
                }
            }
            record.push_back(appear);
        }
        for (int i = 0; i < 8; i++)
        {
            for (int j = i + 1; j < 9; j++)
            {
                if (record[i].size() == 2 && record[j].size() == 2 && record[i][0] == record[j][0] && record[i][1] == record[j][1])
                {
                    for (int n = 1; n < 10; n++)
                    {
                        if (n != i + 1 && n != j + 1)
                        {
                            CandidateDelete(record[i][0].first, record[i][0].second, n);
                            CandidateDelete(record[i][1].first, record[i][1].second, n);
                        }
                    }
                }
            }
        }
    }
}
void DifficultyEvaluation::UpdateXWing()
{
    for (int num = 1; num < 10; num++)
    {
        // search by row
        for (int row = 0; row < 8; row++)
        {
            vector<pair<int, int>> appear;
            for (int col = 0; col < 9; col++)
            {
                if (puzzle[row][col] == 0 && candidate[row][col][num] == 1)
                {
                    pair<int, int> tmp(row, col);
                    appear.push_back(tmp);
                }
            }
            if (appear.size() != 2)
                continue;
            for (int nrow = row + 1; nrow < 9; nrow++)
            {
                vector<pair<int, int>> nAppear;
                for (int col = 0; col < 9; col++)
                {
                    if (puzzle[nrow][col] == 0 && candidate[nrow][col][num] == 1)
                    {
                        pair<int, int> tmp(nrow, col);
                        nAppear.push_back(tmp);
                    }
                }
                if (nAppear.size() != 2)
                    continue;
                if (appear[0].second == nAppear[0].second&&appear[1].second == nAppear[1].second)
                {
                    for (int k = appear[0].second + 1; k < appear[1].second; k++)
                    {
                        CandidateDelete(row, k, num);
                        CandidateDelete(nrow, k, num);
                    }
                    for (int k = row + 1; k < nrow; k++)
                    {
                        CandidateDelete(k, appear[0].second, num);
                        CandidateDelete(k, appear[0].second, num);
                    }
                }
            }
        }
        // search by col
        for (int col = 0; col < 8; col++)
        {
            vector<pair<int, int>> appear;
            for (int row = 0; row < 9; row++)
            {
                if (puzzle[row][col] == 0 && candidate[row][col][num] == 1)
                {
                    pair<int, int> tmp(row, col);
                    appear.push_back(tmp);
                }
            }
            if (appear.size() != 2)
                continue;
            for (int ncol = col + 1; ncol < 9; ncol++)
            {
                vector<pair<int, int>> nAppear;
                for (int row = 0; row < 9; row++)
                {
                    if (puzzle[row][ncol] == 0 && candidate[row][ncol][num] == 1)
                    {
                        pair<int, int> tmp(row, ncol);
                        nAppear.push_back(tmp);
                    }
                }
                if (nAppear.size() != 2)
                    continue;
                if (appear[0].first == nAppear[0].first&&appear[1].first == nAppear[1].first)
                {
                    for (int k = appear[0].first + 1; k < appear[1].first; k++)
                    {
                        CandidateDelete(k, col, num);
                        CandidateDelete(k, ncol, num);
                    }
                    for (int k = col + 1; k < ncol; k++)
                    {
                        CandidateDelete(appear[0].first, k, num);
                        CandidateDelete(appear[0].first, k, num);
                    }
                }
            }
        }
    }
}
bool DifficultyEvaluation::IsFinished()
{
    bool r = true;
    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            r &= puzzle[i][j] != 0;
        }
    }
    return r;
}
bool DifficultyEvaluation::Fill()
{
    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            if (puzzle[i][j]==0&&candidate[i][j][0] == 1)
            {
                for (int num = 1; num < 10; num++)
                {
                    if (candidate[i][j][num] == 1)
                    {
                        puzzle[i][j] = num;
                        candidate[i][j][0] = 0;
                        return true;
                    }
                }
            }
        }
    }
    for (int num = 1; num < 10; num++)
    {
        for (int k = 0; k < 9; k++)
        {
            vector<pair<int, int>> appear;
            // kth row
            for (int col = 0; col < 9; col++)
            {
                if (puzzle[k][col]==0 && candidate[k][col][num] == 1)
                {
                    pair<int, int> tmp(k, col);
                    appear.push_back(tmp);
                }
            }
            if (appear.size() == 1)
            {
                puzzle[appear[0].first][appear[0].second] = num;
                candidate[appear[0].first][appear[0].second][0] = 0;
                return true;
            }
            appear.clear();
            // kth column
            for (int row = 0; row < 9; ++row)
            {
                if (puzzle[row][k]==0 && candidate[row][k][num] == 1)
                {
                    pair<int, int> tmp(row, k);
                    appear.push_back(tmp);
                }
            }
            if (appear.size() == 1)
            {
                puzzle[appear[0].first][appear[0].second] = num;
                candidate[appear[0].first][appear[0].second][0] = 0;
                return true;
            }
            appear.clear();
            // kth cell
            int rs = 3 * (k / 3);
            int cs = 3 * (k % 3);
            for (int row = rs; row < rs + 3; row++)
            {
                for (int col = cs; col < cs + 3; col++)
                {
                    if (puzzle[row][col] == 0 && candidate[row][col][num] == 1)
                    {
                        pair<int, int> tmp(row, col);
                        appear.push_back(tmp);
                    }
                }
            }
            if (appear.size() == 1)
            {
                puzzle[appear[0].first][appear[0].second] = num;
                candidate[appear[0].first][appear[0].second][0] = 0;
                return true;
            }
        }
    }
    return false;
}
double DifficultyEvaluation::Evaluate(int p[][9])
{
    PuzzleInit(p);
    int c1 = 0;
    int c2 = 0;
    int c3 = 0;
    int c4 = 0;
    int c5 = 0;
    while (!IsFinished())
    {
        UpdateSingle();
        if (Fill())
        {
            c1++;
            continue;
        }
        UpdateCell();
        if (Fill())
        {
            c2++;
            continue;
        }
        UpdateExplicitPair();
        if (Fill())
        {
            c3++;
            continue;
        }
        UpdateHiddenPair();
        if (Fill())
        {
            c4++;
            continue;
        }
        UpdateXWing();
        if (Fill())
        {
            c5++;
            continue;
        }
        break;
    }
    cout << c1 << ' ' << c2 << ' ' << c3 << ' ' << c4 << ' ' << c5 << endl;
//for debug
   /* for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            if (j == 8)
                cout << candidate[i][j][0] << endl;
            else
                cout << candidate[i][j][0] << ' ';
        }
    }*/
    return c4;
}
void DifficultyEvaluation::GetPuzzle(int p[][9])
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            p[i][j] = puzzle[i][j];
        }
    }
}