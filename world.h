#include "country.h"

class world
{
private:
    std::vector <country> countries;
    double** toler_matrix;
    double** desire_matrix;
    int num_takts;
    int num_countries;
public:
    world(int num_countries, int num_takts);
    ~world();
};

