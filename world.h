#include "country.cpp"

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
    void Migration();
public:
    world(int num_countries, int num_takts);
    ~world();
};

