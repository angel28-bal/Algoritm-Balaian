#pragma once
#include "utility.h"

// interacts to object
template<typename Val>
using interaction_func = void(*)(unordered_map <int, Val>(&mp), int id);

void edit_pipe(unordered_map <int, pipeline>& mp, int id)
{
    cout << "\n Old repair status: " << mp[id].get_repair() << endl;;
    cout << " Enter the new repair status: ";
    mp[id].set_repair(checker_bool());
    cout << "\n Accepted" << endl;
}

void delete_pipe(unordered_map <int, pipeline>& mp, int id)
{
    mp.erase(id);
}

void edit_cs(unordered_map <int, cs>& mp, int id)
{
    while (1) {
        cout << " Enter the new number of the WORKING workshops: ";
        mp[id].set_workshops_on(checker());
        if (mp[id].get_load() > 1)  cout << "\n The number of WORKING workshops can't be more than ALL workshop. Try again \n" << endl;
        else break;
    }
    cout << "\n Accepted" << endl;
}

void delete_cs(unordered_map <int, cs>& mp, int id)
{
    mp.erase(id);
}

template<typename val>
void interact(unordered_map<int, val>& obj, interaction_func<val> some_fun)
{
    cout << " Enter the number of object" << endl;
    auto iter = obj.find(checker());
    if (iter == obj.end())
    {
        cout << "\n We cannot find this id \n" << endl;
        return;
    }
    int find_id = iter->first;
    some_fun(obj, find_id);
    cout << " [" << find_id << "] have been interacted" << endl;
};

// filters
template<typename V, typename T>
using filter_pipe = bool(*)(const pair <int, V> p, T param);
bool filter_pipe_repair(const pair <int, pipeline> p, bool param) {
    return p.second.get_repair() == param;
}
bool filter_pipe_name(const pair <int, pipeline> p, string param) {
    size_t search = p.second.get_name_pipe().find(param);
    return search != std::string::npos;
}

bool filter_per_cs_workshops_on(const pair <int, cs> p, int param) {
    return p.second.get_load() == param;
}
bool filter_cs_name(const pair <int, cs> p, string param) {
    size_t search = p.second.get_name_cs().find(param);
    return search != std::string::npos;
}

template<typename V, typename T>
vector<int> find_using_filter(const unordered_map <int, V> mp, filter_pipe<V, T> f, T param) {
    vector<int> res;
    if (mp.size())
        for (auto iter : mp)
            if (f(iter, param))
            {
                res.push_back(iter.first);
                cout << iter.second;
            }
    return res;
}
/////////////////////////////////////////////////////////
void filtering_pipe_repair(unordered_map <int, pipeline>& mp) {
    cout << " Enter status of repair: ";
    vector<int> found_vector = find_using_filter(mp, filter_pipe_repair, checker_bool());
    offer_interacte(mp, found_vector);
}

void filtering_pipe_name(unordered_map <int, pipeline>& mp) {
    cout << " Enter name or part of name: ";

    string param;
    cin >> param;
    vector<int> found_vector = find_using_filter(mp, filter_pipe_name, param);
    offer_interacte(mp, found_vector);
}

void filtering_cs_workshops_on(const unordered_map <int, cs>& mp) {
    cout << " Enter percent of working workshops: ";
    find_using_filter(mp, filter_per_cs_workshops_on, checker(0, 100));
}

void filtering_cs_name(const unordered_map <int, cs>& mp) {
    cout << " Enter name or part of name: ";
    string param;
    cin >> param;
    find_using_filter(mp, filter_cs_name, param);
}