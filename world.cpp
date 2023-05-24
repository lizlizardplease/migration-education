#include "world.h"
#include <iostream>
#include <fstream>

world::world(int t, int c, std::string filename, double* rts)
{
    num_takts = t;
    num_countries = c;
    std::ofstream output(filename);
    toler_matrix = new double*[num_countries];
    desire_matrix = new double*[num_countries];
    throughput_matrix = new double*[num_countries];
    flow_matrix = new double*[num_countries];
    for (int i = 0; i < num_countries; i++){
        toler_matrix[i] = new double[num_countries];
        desire_matrix[i] = new double[num_countries];
        throughput_matrix[i] = new double[num_countries];
        flow_matrix[i] = new double[num_countries];
        countries.emplace_back(i, c, rts[i]);
    }
    UpdateEveryMatrix();
    SaveInformation(output, 0);
    for (int i = 0; i < num_takts; i++){
        if (i == 2)
            std::cout << " ";
        std::cout << "TAKT" << i << std::endl;
        Migration();
        SaveInformation(output, i + 1);
    }
    output.close();
}

void world::UpdateEveryMatrix(){
    for (int i = 0; i < num_countries; i++){
        //учесть коеффициенты потом
        for (int j = 0; j < num_countries; j++){
            if (i == j){
                desire_matrix[i][j] = countries[i].GetDesire() + countries[i].GetMoral(); //коеффициенты
               // std::cout << i << " " <<  j << " " <<  countries[i].GetDesire() << " " << countries[i].GetMoral() << std::endl;
                throughput_matrix[i][i] = 0;
                flow_matrix[i][i] = 0;
            }
            else{
                desire_matrix[i][j] = countries[j].GetDesire() - countries[i].GetDesire(); //желание переехать из i в другие
                    //std::cout << i << " " <<  j <<  " " << countries[i].GetDesire() << " " << countries[j].GetDesire() << std::endl;
                throughput_matrix[i][j] = countries[j].GetPopulation() * 0.1;
                if (desire_matrix[i][j] <= 0)
                    flow_matrix[i][j] = 0;
                else
                    flow_matrix[i][j] = std::min(countries[i].GetPopulation() * desire_matrix[i][j], throughput_matrix[i][j]); //поток из i в другие
                    //подумать про фильтр надо будет
                    //влияние потоков п.9
            }
            std::cout << desire_matrix[i][j] << " ";
        }
        std::cout << std::endl;
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
    for (int i = 0; i < num_countries; i++){
        try{
            if (countries[i].GetPopulation() < 0)
                throw std::logic_error("Population is less than zero....");
            std:: cout << "live " << countries[i].GetPopulation() << std::endl;
            for (int j = 0; j < num_countries; j++){
                std:: cout << "leave " << flow_matrix[i][j] << std::endl;
                auto m = countries[i].Departure(flow_matrix[i][j]);
                for (auto el : m)
                    std::cout << el << " ";
                std::cout << std::endl;
                countries[j].Entry(flow_matrix[i][j], i, m);
            }
        }
        catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }
    }
    for (int i = 0; i < num_countries; i++){
        countries[i].StepForward();
    }
    for (int i = 0; i < num_countries; i++){
        for (int j = 0; j < num_countries; j++)
            toler_matrix[i][j] = countries[i].GetTolerVec()[j];
    }
    UpdateEveryMatrix();
}

void world::SaveInformation(std::ofstream &file, int takt){
    for(int i = 0; i < num_countries; i++){
        for(int j = 0; j < 4; j++)
            file << countries[i].GetDemography()[j] << "," << countries[i].GetRes_tech() << "," << j << "," << takt << std::endl;
    }
}