#include "template_utility.h"
#include "gtn_utility.h"

int main()
{
    int cmmnd = 1; // Переменная, хранящая команду пользователя 
    GTN_data gtn_struct; // Метод развертки на месте
    string delimeter_screen = "*******************************************************************************";

    do { // Цикл для считывания команд пользователя
        cout << endl << delimeter_screen << endl;
        switch (cmmnd) { // Выбор опций в главном меню
        case 0: {
            return(0);
        }
        case 1: {
            help();
            break;
        }
        case 2: { // Показать все объекты
            show_object_pipe(gtn_struct.pipe_struct.mp_pipe);
            show_object_cs(gtn_struct.cs_struct.mp_cs);
            break;
        }
        case 3: { // Добавить трубу
            add_object_pipe(gtn_struct.pipe_struct);
            break;
        }
        case 4: { // Редактировать  трубу
            interact(gtn_struct.pipe_struct.mp_pipe, edit_pipe);
            break;
        }
        case 5: { // Удалить трубу
            interact(gtn_struct.pipe_struct.mp_pipe, delete_pipe);
            break;
        }
        case 6: { // Добавить кс
            add_object_cs(gtn_struct.cs_struct);
            break;
        }
        case 7: { // Редактировать КС
            interact(gtn_struct.cs_struct.mp_cs, edit_cs);
            break;
        }
        case 8: { // Удалить КС
            interact(gtn_struct.cs_struct.mp_cs, delete_cs);
            break;
        }
        case 9: { // Фильтрация и редактирование труб по ремонту
            filtering_pipe_repair(gtn_struct.pipe_struct.mp_pipe);
            break;
        }
        case 10: { // Фильтрация труб по названию
            filtering_pipe_name(gtn_struct.pipe_struct.mp_pipe);
            break;
        }
        case 11: { // Фильтрация КС по проценту задействованных цехов
            filtering_cs_workshops_on(gtn_struct.cs_struct.mp_cs);
            break;
        }
        case 12: { // Фильтрация КС по названию
            filtering_cs_name(gtn_struct.cs_struct.mp_cs);
            break;
        }
        case 13: {
            create_new_connection(gtn_struct.connect_struct, gtn_struct.pipe_struct, gtn_struct.cs_struct.mp_cs);
            break;
        }
        case 14: {
            show_all_connection(gtn_struct);
            break;
        }
        case 15: {
            component_of_graph(gtn_struct);
            break;
        }
        case 16: {
            topology_sort_by_dfs(gtn_struct);
            break;
        }
        case 17: {
            find_all_shortest_path(gtn_struct);
            break;
        }
        case 18: {
            find_max_flow(gtn_struct);
            break;
        }
        case 19: {
            save_data(gtn_struct);
            break;
        }
        case 20: {
            load_data(gtn_struct);
            break;
        }
        default: {
            break;
        }
        }
        cout << endl << delimeter_screen << endl;
        cout << "\n Main Menu >> ";
        cmmnd = checker(0, 20);
    } while (1);
    cout << " Good luck!\n\n\n";
}