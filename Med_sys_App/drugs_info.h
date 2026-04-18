#pragma once
#include <string>
using namespace std;

class drugs_info
{
public:
    string m_name;
    int m_id;
    string m_kind;
    int m_num;
    int m_rmb;

    drugs_info() {}

    drugs_info(int id, string name, string kind, int num, int rmb)
    {
        m_name = name;
        m_id = id;
        m_kind = kind;
        m_num = num;
        m_rmb = rmb;
    }
};