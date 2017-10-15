#include "stdafx.h"
#include "DifficultyEvaluation.h"
#include <vector>
#include <utility>
#include <fstream>
#include "CaaeSudoku.h"
using namespace std;
DifficultyEvaluation::DifficultyEvaluation()
{
    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            puzzle[i][j] = 0;
        }
    }
}
DifficultyEvaluation::~DifficultyEvaluation(){}

/*
��ʼ����Ҫ�����Ѷȵ�������Ŀ��
����λ�õĺ�ѡ��������0��
*/
void DifficultyEvaluation::PuzzleInit(int p[][9])
{
    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            puzzle[i][j] = p[i][j];
			for (int k = 1; k < 10; ++k)
			{
				candidate[i][j][k] = 1;
			}
			candidate[i][j][0] = 9;
            if (puzzle[i][j] != 0)
            {
                candidate[i][j][0] = 0;
            }
        }
    }
}

/*
ɾ��(row, col)λ�õĺ�ѡ��num��
����λ�����������û�к�ѡ��num�������κβ�����
*/
void DifficultyEvaluation::CandidateDelete(int row, int col, int num)
{
    if (puzzle[row][col]==0 && candidate[row][col][num] == 1)
    {
        candidate[row][col][num] = 0;
        candidate[row][col][0]--;
    }
}

/*
Ψ�෨�ų��������������ų���ѡ����
*/
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

/*
�����ų�����
*/
void DifficultyEvaluation::UpdateCell()
{
    // ����9����
    for (int cellnum = 0; cellnum < 9; ++cellnum)
    {
        // ����˹�����ʼλ������
        int rs = 3 * (cellnum / 3);
        int cs = 3 * (cellnum % 3);
        // 1��9����������num����Ϊ��ѡ���ĳ��ִ���
        for (int num = 1; num <= 9; ++num)
        {
            // appear����num��Ϊ��ѡ�����ֵ�λ������
            vector<pair<int, int>> appear;
            for (int row = rs; row < rs + 3; ++row)
            {
                for (int col = cs; col < cs + 3; col++)
                {
                    if (puzzle[row][col]==0 && candidate[row][col][num]==1)
                    {
                        // �ҵ�num���ֵ�λ�ã�����֮
                        pair<int, int> tmp(row, col);
                        appear.push_back(tmp);
                    }
                }
            }
            if (appear.size() == 3)
            {
                // ��num����3�β�ͬ�л�ͬ�У���ɾ��������ͬ��/�е�num��ѡ��
                if (appear[0].first == appear[1].first && appear[1].first == appear[2].first)// �ж�ͬ��
                {
                    for (int col = 0; col < 9; ++col)
                    {
                        if (col < cs || col > cs + 2)
                        {
                            CandidateDelete(appear[0].first, col, num);
                        }
                    }
                }
                else if (appear[0].second == appear[1].second && appear[1].second == appear[2].second)// �ж�ͬ��
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
                // ��num����2�β�ͬ�л�ͬ�У���ɾ��������ͬ��/�е�num��ѡ��
                if (appear[0].first == appear[1].first)// �ж�ͬ��
                {
                    for (int col = 0; col < 9; ++col)
                    {
                        if (col < cs || col > cs + 2)
                        {
                            CandidateDelete(appear[0].first, col, num);
                        }
                    }
                }
                else if (appear[0].second == appear[1].second)// �ж�ͬ��
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

/*
��ʽ�����ų�����
*/
void DifficultyEvaluation::UpdateNakedPair()
{
    for (int k = 0; k < 9; ++k)
    {
        // kth row
        // �жϵ�k���Ƿ������ʽ����
        for (int col = 0; col < 8; ++col)
        {
            if (candidate[k][col][0] != 2)
                continue;
            // �ҵ���ѡ������Ϊ2��һ��λ��
            // �Ӹ�λ�����Ѱ��һ���͸�λ�ú�ѡ������ͬ��λ��
            for (int ncol = col + 1; ncol < 9; ++ncol)
            {
                if (candidate[k][ncol][0] != 2)
                    continue;
                // �ҵ���һ����ѡ������Ϊ2��λ��
                bool r = true;
                int n1 = 0;
                int n2 = 0;
                for (int i = 1; i < 10; ++i)
                {
                    // �ж�����λ�õĺ�ѡ�����Ƿ���ͬ
                    r &= candidate[k][col][i] == candidate[k][ncol][i];
                    // n1,n2��¼2����ѡ��
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
                    // ����λ�þ�����ͬ�ĺ�ѡ����
                    // update row k: delete n1, n2 except (k,col), (k,ncol)
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
        // �жϵ�k���Ƿ������ʽ���ԣ��ṹͬ��k�е����Σ�
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
        // �жϵ�k���Ƿ������ʽ����
        for (int cln = 0; cln < 8; ++cln)
        {
            // clnΪ��������λ��0-7
            // ����cln��Ӧ�ĵ�k��������λ��
            int row = 3 * (k / 3) + cln / 3;
            int col = 3 * (k % 3) + cln % 3;
            if (candidate[row][col][0] != 2)
                continue;
            // �ҵ���ѡ������Ϊ2��һ��λ��
            // �Ӹ�λ�����Ѱ��һ���͸�λ�ú�ѡ������ͬ��λ��
            for (int ncln = cln + 1; ncln < 9; ++ncln)
            {
                // ����ncln��Ӧ�ĵ�k����λ��
                int nrow = 3 * (k / 3) + ncln / 3;
                int ncol = 3 * (k % 3) + ncln % 3;
                if (candidate[nrow][ncol][0] != 2)
                    continue;
                // �ҵ���һ����ѡ������Ϊ2��λ��
                bool r = true;
                int n1 = 0;
                int n2 = 0;
                for (int i = 1; i < 10; ++i)
                {
                    // �ж�������λ�õĺ�ѡ���Ƿ���ͬ
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
                    // �����ͬ��ɾȥ��������λ�õ�������ѡ
                    // ������λ�����ͬ�л�ͬ�У�֮ǰ����ѭ���Ѷ�ͬ������������������
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

/*
��ʽ�����ų�����
*/
void DifficultyEvaluation::UpdateHiddenPair()
{
    for (int k = 0; k < 9; ++k)
    {
        // kth row
        // ��k�еĴ���
        // record[i]��¼i+1��Ϊ��ѡ�����ֵ�λ��
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
        // Ѱ�����������������ҳ���λ����ͬ�ĺ�ѡ������ʽ���ԣ�
        for (int i = 0; i < 8; i++)
        {
            for (int j = i + 1; j < 9; j++)
            {
                if (record[i].size() == 2 && record[j].size() == 2 && record[i][0] == record[j][0] && record[i][1] == record[j][1])
                {
                    // ����ڵ�k���У���ѡ��i+1��j+1����2����λ����ͬ
                    // ɾ��������λ�õ������ѡ��
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
        // ��k�еĴ���˼·ͬ��k��
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
        // ��k���Ĵ���
        record.clear();
        // rs,csΪ��k������ʼλ��
        int rs = 3 * (k / 3);
        int cs = 3 * (k % 3);
        // ����Ĳ���ͬ��
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

/*
XWing�ų�����
*/
void DifficultyEvaluation::UpdateXWing()
{
    for (int num = 1; num < 10; num++)
    {
        // search by row�����в���
        for (int row = 0; row < 8; row++)
        {
            // appear��¼num����row���ֵ�λ��
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
            // num����2�Σ�Ǳ�ڵ�xwing���������漸��Ѱ��numͬ������2�ε���
            for (int nrow = row + 1; nrow < 9; nrow++)
            {
                // nAppear��¼num����nrow���ֵ�λ��
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
                // �ҵ�num�������ε��У����ж��ĸ�λ���Ƿ񹹳ɾ���
                if (appear[0].second == nAppear[0].second&&appear[1].second == nAppear[1].second)
                {
                    // ɾ���ĸ��ߵĳ����ĸ���λ�õĺ�ѡ��num
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
        // search by col�� ���в��ң��ṹ���ƣ�
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

/*
�ж������Ƿ��ѽ����
����true��ʾ�ѽ��������false��ʾδ�����
*/
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

/*
��puzzle�ģ�row��col��λ������num��
*/
void DifficultyEvaluation::Fill(int row, int col, int num)
{
    puzzle[row][col] = num;
    candidate[row][col][0] = 0;
}

/*
Ψ�෨������Ѱ��ֻ��һ����ѡ���Ŀո���֮��
����true�������ɹ���
����false��δ�ҵ����������Ŀո�
ע�⣺����ֻ��һ���ո񼴷��أ�
*/
bool DifficultyEvaluation::FillNakedSingle()
{
    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            if (puzzle[i][j] == 0 && candidate[i][j][0] == 1)
            {
                // Ѱ�ҵ�һ����ѡ������Ϊ1�Ŀո���֮
                for (int num = 1; num < 10; num++)
                {
                    if (candidate[i][j][num] == 1)
                    {
                        Fill(i, j, num);
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

/*
��ʽΨ�ࣺѰ�������ӽṹ�����й�����ֻ������һ��λ�õĺ�ѡ�������ڸ�λ����ú�ѡ����
����true�������ɹ���
����false��δ�ҵ����������Ŀո�
ע�⣺����ֻ��һ���ո񼴷��أ�
*/
bool DifficultyEvaluation::FillHiddenSingle()
{
    for (int num = 1; num < 10; num++)
    {
        // num������ѡ��
        for (int k = 0; k < 9; k++)
        {
            vector<pair<int, int>> appear;
            // kth row
            // appear��¼num��k�г��ֵ�λ��
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
                // ֻ����1�Σ����λ��ֻ����num
                Fill(appear[0].first, appear[0].second, num);
                return true;
            }
            appear.clear();
            // kth column
            // appear��¼num��k�г��ֵ�λ��
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
                // ֻ����1�Σ����λ��ֻ����num
                Fill(appear[0].first, appear[0].second, num);
                return true;
            }
            appear.clear();
            // kth cell
            // appear��¼num��k�����ֵ�λ��
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
                // ֻ����1�Σ����λ��ֻ����num
                Fill(appear[0].first, appear[0].second, num);
                return true;
            }
        }
    }
    return false;
}

/*
ʹ�������������Խ��г��ԣ�
����֮һ�ɹ�����true�����ֲ��Ծ�ʧ�ܷ���false��
updateCount�������˴�������Ӧ���ų�������Ч�����������ɹ���ò�����Ч������1��
fillCount1������NakedSingle�������Ե���Ч�����������ɹ���ò�����Ч������1��
fillCount2������HiddenSingle����������Ч�����������ɹ���ò�����Ч������1��
*/
bool DifficultyEvaluation::TryToFill(int &updateCount, int &fillCount1, int &fillCount2)
{
    if (FillNakedSingle())
    {
        updateCount++;
        fillCount1++;
        return true;
    }
    else if (FillHiddenSingle())
    {
        updateCount++;
        fillCount2++;
        return true;
    }
    return false;
}

/*
�������������������Ѷ�������
*/
Difficulty DifficultyEvaluation::Evaluate(int p[][9])
{
    PuzzleInit(p);
    int c1 = 0, c2 = 0, c3 = 0, c4 = 0, c5 = 0, f1 = 0, f2 = 0;
    int count = 0;
    while (!IsFinished())
    {
        UpdateSingle();
        if (TryToFill(c1, f1, f2))
            continue;
        UpdateCell();
        if (TryToFill(c2, f1, f2))
            continue;
        UpdateNakedPair();
        if (TryToFill(c3, f1, f2))
            continue;
        UpdateHiddenPair();
        if (TryToFill(c4, f1, f2))
            continue;
        UpdateXWing();
        if (TryToFill(c5, f1, f2))
            continue;
        break;
    }
    cout << c1 << ' ' << c2 << ' ' << c3 << ' ' << c4 << ' ' << c5 << endl;
    cout << f1 << ' ' << f2 << endl;
    double pr = double(f1) / double(f2);
    bool usedUpperStra = c2 != 0 || c3 != 0 || c4 != 0 || c5 != 0;
    bool isHard = usedUpperStra || !IsFinished() || pr < 1.8;
    bool isMedium = !usedUpperStra && IsFinished() && pr >= 1.8 && pr <= 10;
    bool isEasy = !usedUpperStra && IsFinished() && pr>10;
    if (isEasy)
        return Difficulty::EASY;
    else if (isMedium)
        return Difficulty::NORMAL;
    else
        return Difficulty::HARD;
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