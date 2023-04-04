#include "world.h"
#include <iostream>

world::world(int t, int c)
{
    num_takts = t;
    num_countries = c;
    toler_matrix = new double*[num_countries];
    desire_matrix = new double*[num_countries];
    throughput_matrix = new double*[num_countries];
    flow_matrix = new double*[num_countries];
    for (int i = 0; i < num_countries; i++){
        toler_matrix[i] = new double[num_countries];
        desire_matrix[i] = new double[num_countries];
        throughput_matrix[i] = new double[num_countries];
        flow_matrix[i] = new double[num_countries];
        countries.push_back(std::move(country(i)));
        //countries.emplace_back(7);
        //std::cout << "fine" << std::endl;
       // std::cout << countries[i].culture_matrix[0][0] << std::endl;
    }
    for (int i = 0; i < num_countries; i++){
        //учесть коеффициенты потом
        for (int j = 0; j < num_countries; j++){
            if (i == j){
                desire_matrix[i][j] = countries[i].GetDesire() + countries[i].GetMoral();
                throughput_matrix[i][i] = 0;
                flow_matrix[i][i] = 0;
            }
            else{
                desire_matrix[i][j] = countries[j].GetDesire() - countries[i].GetDesire();
                throughput_matrix[i][j] = countries[j].GetPopulation() * 0.1;
                if (desire_matrix[i][j] <= 0)
                    flow_matrix[i][j] = 0;
                else
                    flow_matrix[i][j] = std::min(countries[i].GetPopulation() * desire_matrix[i][j], throughput_matrix[i][j]);
                    //подумать про фильтр надо будет
                    //влияние потоков п.9
            }
        }
    }
}

world::~world()
{
     for (int i = 0; i < num_countries; i++){
        delete [] toler_matrix[i];
        delete [] desire_matrix[i];
        delete [] throughput_matrix[i];
        delete [] flow_matrix[i];
    }
    delete [] toler_matrix;
}

void world::Migration(){
    for (auto country : countries){
        country.StepForward();
    }
    for (int i = 0; i < num_countries; i++){
        for (int j = 0; j < num_countries; j++)
            toler_matrix[i][j] = countries[i].GetTolerVec()[j];
    }
}

