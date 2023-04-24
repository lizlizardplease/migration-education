#include "world.h"
#include <iostream>
#include <fstream>

world::world(int t, int c)
{
    num_takts = t;
    num_countries = c;
    std::ofstream output("output.txt");
    toler_matrix = new double*[num_countries];
    desire_matrix = new double*[num_countries];
    throughput_matrix = new double*[num_countries];
    flow_matrix = new double*[num_countries];
    for (int i = 0; i < num_countries; i++){
        toler_matrix[i] = new double[num_countries];
        desire_matrix[i] = new double[num_countries];
        throughput_matrix[i] = new double[num_countries];
        flow_matrix[i] = new double[num_countries];
        countries.emplace_back(i);
        std::cout << countries[i].GetPopulation() << std::endl;
    }
    UpdateEveryMatrix();
    SaveInformation(output);
    for (int i = 0; i < num_takts; i++){
        Migration();
        SaveInformation(output);
    }
    output.close();
}

void world::UpdateEveryMatrix(){
    for (int i = 0; i < num_countries; i++){
        //учесть коеффициенты потом
        for (int j = 0; j < num_countries; j++){
            if (i == j){
                desire_matrix[i][j] = countries[i].GetDesire() + countries[i].GetMoral(); //коеффициенты
                throughput_matrix[i][i] = 0;
                flow_matrix[i][i] = 0;
            }
            else{
                desire_matrix[i][j] = countries[j].GetDesire() - countries[i].GetDesire(); //желание переехать из i в другие
                throughput_matrix[i][j] = countries[j].GetPopulation() * 0.1;
                if (desire_matrix[i][j] <= 0)
                    flow_matrix[i][j] = 0;
                else
                    flow_matrix[i][j] = std::min(countries[i].GetPopulation() * desire_matrix[i][j], throughput_matrix[i][j]); //поток из i в другие
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
    std::cout << "ok!" << std::endl;
    for (int i = 0; i < num_countries; i++){
        std:: cout << "live " << countries[i].GetPopulation() << std::endl;
        for (int j = 0; j < num_countries; j++){
            std:: cout << "leave " << flow_matrix[i][j] << std::endl;
            countries[j].Entry(flow_matrix[i][j], i, countries[i].Departure(flow_matrix[i][j]));
        }
    }
    std::cout << "ok!" << std::endl;
    for (int i = 0; i < num_countries; i++){
        countries[i].StepForward(flow_matrix[i]);
    }
    std::cout << "ok!" << std::endl;
    for (int i = 0; i < num_countries; i++){
        for (int j = 0; j < num_countries; j++)
            toler_matrix[i][j] = countries[i].GetTolerVec()[j];
    }
    std::cout << "ok!" << std::endl;
    UpdateEveryMatrix();
    std::cout << "cool!" << std::endl;
}

void world::SaveInformation(std::ofstream &file){
    for(int i = 0; i < num_countries; i++)
        file << countries[i].GetPopulation() << " ";
    file << std::endl;
}