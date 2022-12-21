#include "utility.h"

// checkers
bool checker_bool() // Проверка на булевое значение
{
    bool in;
    while (1) {
        if (cin >> in)
        {
            cin.ignore(1024, '\n'); // Отсекать всё, что дальше пробела
            return(in);
        }
        else
        {
            cout << "\n Incorrect input. Try again. Enter 0 or 1: ";
            cin.clear();
            cin.ignore(1024, '\n');
        }
    }
};

int checker() // Проверка на положительное число 
{
    int in;
    while (1) {
        if ((cin >> in) && (in >= 0))
        {
            cin.ignore(1024, '\n');
            return(in);
        }
        else
        {
            cout << "\n Incorrect input. Try again. Enter positive number: ";
            cin.clear();
            cin.ignore(1024, '\n');
        }
    }
}

int checker(int a, int b) // Проверка на число в интервале от a до b включительно
{
    int in;
    while (1) {
        if ((cin >> in) && (in >= a) && (in <= b))
        {
            cin.clear();
            cin.ignore(1024, '\n');
            return(in);
        }
        else
        {
            cout << "\n Incorrect input. Try again. Enter number from [" << a << "] till [" << b << "]: ";
            cin.clear();
            cin.ignore(1024, '\n');
        }
    }
}

void help()
{
    cout << "\n 1 Help \n 2 Show All Objects \n\n 3 Add Pipeline \n 4 Edit Repair Status of Pipeline \n 5 Delete pipeline \n\n"
        << " 6 Add Compressor Stations \n 7 Edit Number of Working Workshops \n 8 Delete compressor station \n\n"
        << " 9 Filter pipeline [repair status] \n 10 Filter pipeline [name] \n 11 Filter compressor station [percent of not working] \n 12 Filter compressor station [name] \n\n"
        << " 13 Create new connection \n 14 Show all connection \n 15 Show all component \n\n 16 Topology sort \n 17 Find all shortest path \n 18 Find max flow \n\n"
        << " 19 Save data \n 20 Load data \n\n 0 Exit \n";
}

// Функции вывода, добавления, редактирования и удаления объектов труб
void show_object_pipe(const unordered_map <int, pipeline>& mp)
{
    if (mp.size())
    {
        for (auto iter : mp)
            cout << iter.second;
    }
    else { std::cout << "\n Sorry, we found no one pipeline" << endl; }
}

void add_object_pipe(struct pipelines_data& pipe_struct)
{
    cin >> pipe_struct.mp_pipe[++pipe_struct.max_id_pipeline];
    pipe_struct.mp_pipe[pipe_struct.max_id_pipeline].set_id_pipeline(pipe_struct.max_id_pipeline);
}

// Функции вывода, добавления, редактирования и удаления объектов КС
void show_object_cs(const unordered_map <int, cs>& mp)
{
    if (mp.size())
        for (auto iter : mp)
            cout << iter.second;
    else
        cout << "\n Sorry, we found no one compressor station" << endl;
}

void add_object_cs(struct cs_data& cs_struct)
{
    cin >> cs_struct.mp_cs[++cs_struct.max_id_cs];
    cs_struct.mp_cs[cs_struct.max_id_cs].set_cs_id(cs_struct.max_id_cs);
}

bool checker_exist_file(const string& name_file)
{
    ifstream exist(name_file + ".txt"); // Проверка существования файла
    bool file_exist;
    exist.is_open() ? file_exist = 1 : file_exist = 0;
    exist.close();
    return(file_exist);
}

void save_data(const GTN_data& gtn_struct)
{
    string name_file;
    do {
        cout << "\n Enter the name of file, using for saving data: ";
        getline(cin, name_file);
        if (checker_exist_file(name_file))
            cout << " This file is already exist. Rewrite file ? Yes 1 / No 0: ";
        else
            break;
    } while (!checker_bool());

    ofstream outing(name_file + ".txt", ios::out);

    if (outing.is_open())
    {
        if (gtn_struct.pipe_struct.mp_pipe.size())
            for (auto iter : gtn_struct.pipe_struct.mp_pipe)
                outing << iter.second;
        if (gtn_struct.cs_struct.mp_cs.size())
            for (auto iter : gtn_struct.cs_struct.mp_cs)
                outing << iter.second;
        if (gtn_struct.connect_struct.size())
            outing << gtn_struct.connect_struct;
        outing.close();
    }
    cout << "\n Data "; (gtn_struct.pipe_struct.mp_pipe.size()) || (gtn_struct.cs_struct.mp_cs.size()) ? cout << "have" : cout << "haven't"; cout << " been saved";
}

void load_data(GTN_data& gtn_struct)
{
    //string name_file;
    //while (1) {
    //    cout << "\n Enter the name of file, using for loading data: ";
    //    getline(cin, name_file);
    //    if (checker_exist_file(name_file))
    //    {
    //        cout << "\n This file has been find ";
    //        break;
    //    }
    //    else
    //    {
    //        cout << " We cannot find this file. Try again ? Yes 1/ No 0: ";
    //        if (checker_bool() == false)
    //            break;
    //    }
    //}

    //ifstream ining(name_file + ".txt");
    ifstream ining("myfile.txt");
    if (ining.is_open())
    {
        int type = 0;
        int id = 0;
        while (1) // Считывание данных из файла до конца файла
        {
            if (ining.eof()) break;
            ining >> type;
            if (type == 1) {
                ining >> id;
                ining.ignore();
                ining >> gtn_struct.pipe_struct.mp_pipe[id];
                gtn_struct.pipe_struct.mp_pipe.at(id).set_id_pipeline(id);
                if (id > gtn_struct.pipe_struct.max_id_pipeline)
                    gtn_struct.pipe_struct.max_id_pipeline = id;
            }
            if (type == 2) {
                ining >> id;
                ining.ignore();
                ining >> gtn_struct.cs_struct.mp_cs[id];
                gtn_struct.cs_struct.mp_cs.at(id).set_cs_id(id);
                if (id > gtn_struct.cs_struct.max_id_cs)
                    gtn_struct.cs_struct.max_id_cs = id;
            }
            if (type == 3) {
                ining.ignore();
                while (1) {
                    ining >> gtn_struct;
                    if (ining.eof()) break;
                }
            }
        }
        ining.close();
        cout << "\n Data "; (gtn_struct.pipe_struct.mp_pipe.size()) || (gtn_struct.cs_struct.mp_cs.size()) ? cout << "have" : cout << "haven't"; cout << " been download" << endl;
    }
}

void offer_interacte(unordered_map <int, pipeline>& mp, vector<int>& found_objects) {
    cout << "\n Would you like edit this pipelines ? 1 Yes/ 0 No: ";
    if (checker_bool() == false)
        return;
    cout << "\n Choice what you want to edit, enter 0 for All or through a space ID of object: ";
    string string_id, delimeter_id = " ", temp;
    getline(cin, string_id);
    size_t position = 0;
    if (string_id != "0") {
        found_objects.clear();
        while (position < string_id.size()) {
            temp = temp.assign(string_id, position, string_id.find(delimeter_id, position) - position);
            found_objects.push_back(stoi(temp));
            position += temp.size() + delimeter_id.size();
        }
    }

    cout << "\n Edit or Delete ? 1/2: ";
    switch (checker(1, 2)) {
    case 1: {
        cout << "\n Set new repair status: "; bool new_status = checker_bool();
        for (int i : found_objects)
            mp[i].set_repair(new_status);
        break;
    }
    case 2: {
        for (int i : found_objects)
            mp.erase(i);
        break;
    }
    default: {
        break;
    }
    }

}