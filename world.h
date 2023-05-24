#include "country.cpp"
#include <fstream>
#include <cstdlib> 
#include <unordered_map>
#include <ctime>
#include <algorithm>

class world
{
private:
    std::vector <country> countries;
    double** toler_matrix; //матрица толерантности
    double** desire_matrix; //матрица желания жить в стране
    double** throughput_matrix; //матрица пропускных способностей
    double** flow_matrix; //матрица потоков
    int num_takts;  //число тактов
    int num_countries; //число стран
    std::unordered_map<int, int> disasterd_countries; //страны, в которых происходят природные катаклизмы
    bool disasters;
    void Migration();
    void UpdateEveryMatrix();
    void SaveInformation(std::ofstream & file, int x);
public:
    world(int num_countries, int num_takts, std::string filename, double* rts);
    ~world();
};

