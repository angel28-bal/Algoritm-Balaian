#include "cs.h"
#include "utility.h"

ostream& operator<<(ostream& os, const cs& c)
{
    return os << "\n The compressor station's [" << c.id_cs << "] name: " << c.name_cs << endl
        << " Compressor station consist of [" << c.workshops_on << "/" << c.workshops_all << "] working workshops" << endl
        << " Compressor station's efficiency: [ " << c.efficiency << " %]" << endl;
}

istream& operator>>(istream& is, cs& p)
{
    cout << "\n Enter the compressor station's name: ";
    getline(cin, p.name_cs);
    while (1) {
        cout << " Enter the number of the ALL workshops: ";
        p.workshops_all = checker();
        cout << " Enter the number of WORKING workshops: ";
        p.workshops_on = checker();
        if (p.workshops_on > p.workshops_all)
            cout << "\n WORKING workshops can't be more than ALL workshops. Try again \n" << endl;
        else
            break;
    }
    cout << " Enter the efficiency of the compressor station: ";    p.efficiency = checker(0, 100);
    return is;
}

ofstream& operator<<(ofstream& ofs, const cs& p)
{
    ofs << 2 << endl << p.id_cs << endl << p.name_cs << endl << p.workshops_on << endl << p.workshops_all
        << endl << p.efficiency << endl << endl;
    return ofs;
};

ifstream& operator>>(ifstream& ifs, cs& p)
{
    //ifs >> p.id;
    getline(ifs, p.name_cs);
    ifs >> p.workshops_on;
    ifs >> p.workshops_all;
    ifs >> p.efficiency;
    return ifs;
}

string cs::get_name_cs() const
{
    return(name_cs);
}

double cs::get_load() const
{
    return (((double)workshops_on / workshops_all) * 100);
}


void cs::set_workshops_on(int new_working) // Редактирование КС
{
    workshops_on = new_working;
}


int cs::get_cs_id() const
{
    return(id_cs);
}
void cs::set_cs_id(int max_cs_id)
{
    id_cs = max_cs_id;
}