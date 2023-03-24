#include "world.h"
#include <iostream>

world::world(int t, int c)
{
    num_takts = t;
    num_countries = c;
    toler_matrix = new double*[num_countries];
    desire_matrix = new double*[num_countries];
    for (int i = 0; i < num_countries; i++){
        toler_matrix[i] = new double[num_countries];
        desire_matrix[i] = new double[num_countries];
        country tmp_c = country(rand() * 100);
        countries.push_back(tmp_c);
    }
    for (int i = 0; i < num_countries; i++){
        //учесть коеффициенты потом
        for (int j = 0; j < num_countries; j++){
            if (i == j){
                desire_matrix[i][j] = countries[i].GetDesire() + countries[i].GetMoral();
            }
            else{
                desire_matrix[i][j] = countries[i].GetDesire() - countries[j].GetDesire();
            }
        }
    }
}

world::~world()
{
     for (int i = 0; i < num_countries; i++){
        delete [] toler_matrix[i];
        delete [] desire_matrix[i];
    }
}


