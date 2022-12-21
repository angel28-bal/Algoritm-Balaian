#include "pipeline.h"
#include "utility.h"

// overloading

// out
ostream& operator<<(ostream& os, const pipeline& p)
{
    return os << "\n Pipeline [" << p.id_pipeline << "]" << endl << " The name of pipeline: " << p.name_pipe << endl << " The length of pipeline: " << p.lenght << endl
        << " The pipeline's diameter: " << p.diameter << endl << " Repair status: " << p.repair << endl << " Working status: " << p.used << "Flow: " << p.diameter * p.lenght / 100 << endl;;
}

// in
istream& operator>>(istream& is, pipeline& p)
{
    cout << "\n Enter the pipeline's name: ";
    getline(cin, p.name_pipe);
    cout << " Enter the length of pipeline in meters: ";
    p.lenght = checker();
    cout << " Enter pipeline's the diameter in millimeters: ";
    p.diameter = checker();
    cout << " Enter pipeline's status (0 working, 1 in the repair): ";
    p.repair = checker_bool();
    cout << "\n The pipeline number has been successfully added" << endl;
    return is;
}

// out file
ofstream& operator<<(ofstream& ofs, const pipeline& p)
{
    ofs << 1 << endl << p.id_pipeline << endl << p.name_pipe << endl << p.lenght << endl << p.diameter << endl << p.repair << endl << endl;
    return ofs;
};

//  in file
ifstream& operator>>(ifstream& ifs, pipeline& p)
{
    //ifs >> p.id;
    getline(ifs, p.name_pipe);
    ifs >> p.lenght;
    ifs >> p.diameter;
    ifs >> p.repair;
    return ifs;
};

// getters
int pipeline::get_id_pipeline() const
{
    return(id_pipeline);
}

void pipeline::set_id_pipeline(int max_pipeline_id)
{
    id_pipeline = max_pipeline_id;
}

string pipeline::get_name_pipe() const
{
    return(name_pipe);
}

int  pipeline::get_lenght() const
{
    return(lenght);
}

int pipeline::get_diameter() const
{
    return(diameter);
}

bool pipeline::get_repair() const
{
    return(repair);
}

void pipeline::set_repair(bool new_status)
{
    repair = new_status;
}

bool pipeline::get_used() const
{
    return(used);
}

// setters
void pipeline::set_used(bool new_used)
{
    used = new_used;
}
