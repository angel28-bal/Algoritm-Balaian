#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

using namespace std;

class pipeline
{
public:
    friend ostream& operator<<(ostream& os, const pipeline& p);
    friend istream& operator>>(istream& os, pipeline& p);
    friend ofstream& operator<<(ofstream& ofs, const pipeline& p);
    friend ifstream& operator>>(ifstream& ifs, pipeline& p);

    int get_id_pipeline() const;
    void set_id_pipeline(int max_pipeline_id);

    string get_name_pipe() const;
    int get_lenght() const;
    int get_diameter() const;

    bool get_repair() const;
    void set_repair(bool new_status);

    bool get_used() const;
    void set_used(bool new_used);

private:
    bool used = false;
    int id_pipeline = 0; //ID трубы
    string name_pipe; // Название трубы
    int lenght = 0; // Длина в метрах
    int diameter = 0; // Диаметр трубы в мм
    bool repair = 0; // 0 Функционирует, 1 Находится в ремонте
};

struct pipelines_data
{
    unordered_map <int, pipeline> mp_pipe;
    int max_id_pipeline = 0;
};