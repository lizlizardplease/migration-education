#include "world.h"
#include <iostream>
#include <fstream>

world::world(int c, int t, std::string filename1, std::string filename2, double* rts)
{
    num_takts = t;
    num_countries = c;
   // disasters = 0;
    //srand(time(0));
    std::ofstream output1(filename1);
    std::ofstream output2(filename2);
    toler_matrix = new double*[num_countries];
    desire_matrix = new double*[num_countries];
    throughput_matrix = new double*[num_countries];
    flow_matrix = new double*[num_countries];
    countries.reserve(num_countries);
    //int random_takt = rand() % 50;
    for (int i = 0; i < num_countries; i++){
        toler_matrix[i] = new double[num_countries];
        desire_matrix[i] = new double[num_countries];
        throughput_matrix[i] = new double[num_countries];
        flow_matrix[i] = new double[num_countries];
        countries.emplace_back(i, num_countries, c, rts[i]);
    }
    UpdateEveryMatrix();
    //SaveInformation(output1, output2, 0);
    for (int i = 0; i < num_takts; i++){
    //     if (i == random_takt){
    //         std::cout << "DISASTER" << std::endl;
    //         int randomCountry = rand() % num_countries;
    //         countries[randomCountry].DisasterSent();
    //         random_takt = i + (rand() % 50);
    //         disasters = 1;
    //         disasterd_countries[randomCountry] = 5;
    //     }
        std::cout << "TAKT" << i << std::endl;
        Migration();
        if (i ==42)
        {
            std::cout << " ";
        }
        for (int i = 0; i < num_countries; i++){
            countries[i].StepForward();
        }
        UpdateEveryMatrix();
        SaveInformation(output1, output2, i + 1);
        // if (disasters){
        //     for(auto it = disasterd_countries.begin(); it != disasterd_countries.end(); ) {
        //         it->second--;
        //         if (it->second == 0){
        //             countries[i].DisasterStoped();
        //             it = disasterd_countries.erase(it);
        //     } 
        //         else {
        //             ++it;
        //         }
        //     }
        //     if (disasterd_countries.empty()) 
        //         disasters = 0;
        // } 
    }
    output1.close();
    output2.close();
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
                throughput_matrix[i][j] = countries[j].GetPopulation() * 0.01;
                if (desire_matrix[i][j] <= 0)
                    flow_matrix[i][j] = 0;
                else
                    flow_matrix[i][j]= std::min(std::max(countries[i].GetPopulation() * desire_matrix[i][j], 0.0), throughput_matrix[i][j]);
                    //подумать про фильтр надо будет
                    //влияние потоков п.9
            }
        }
        std::cout << std::endl;
    }
    for (int i = 0; i < num_countries; i++){
        for (int j = 0; j < num_countries; j++)
            toler_matrix[i][j] = countries[i].GetTolerVec()[j];
        }
}

world::~world()
{
    for (int i = 0; i < num_countries; i++){
        if (toler_matrix[i] != nullptr)
            delete [] toler_matrix[i];
        if (desire_matrix[i] != nullptr)
            delete [] desire_matrix[i];
        if (throughput_matrix[i] != nullptr)
            delete [] throughput_matrix[i];
        if (flow_matrix[i] != nullptr)
            delete [] flow_matrix[i];
    }
    if (toler_matrix != nullptr)
        delete [] toler_matrix;
    if (desire_matrix != nullptr)
        delete [] desire_matrix;
    if (throughput_matrix != nullptr)
        delete [] throughput_matrix;
    if (flow_matrix != nullptr)
        delete [] flow_matrix;
}

void world::Migration(){
    for (int i = 0; i < num_countries; i++){
        try{
            if (countries[i].GetPopulation() < 0)
                throw std::logic_error("Population is less than zero....");
            //std:: cout << "live " << countries[i].GetPopulation() << std::endl;
            for (int j = 0; j < num_countries; j++){
               // std:: cout << "leave " << flow_matrix[i][j] << std::endl;
                // if (disasters && (disasterd_countries.count(i) > 0 || disasterd_countries.count(j) > 0))
                //     continue;
                auto m = countries[i].Departure(flow_matrix[i][j]);
                countries[j].Entry(i, m);
            }
        }
        catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }
    }
}

void world::SaveInformation(std::ofstream &file1, std::ofstream &file2, int takt){
    for(int i = 0; i < num_countries; i++){
        file1 << countries[i].GetLivingStandard() << "," << countries[i].GetRes_tech() <<
         ","  << takt << ", " << countries[i].GetInst()  << std::endl;
    }
    for(int i = 0; i < num_countries; i++){
        for(int j = 0; j < 4; j++){
        file2 << countries[i].GetDemography()[j] << "," << countries[i].GetRes_tech() <<
         ","  << j << ", " << takt  << std::endl;
        }
    }
}