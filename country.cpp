#include "country.h"

country::country(int s, int steps, double rt)
{
    step = 0;
    name = s;
    num_takts = steps;
    population = 1000000;
    n[0] = round(0.2 * population);
    n[1] = round(0.2 * population);
    n[3] = round(0.2 * population);
    n[2] = population - (n[0] + n[1] + n[3]);
    //std::cout << "popul" << std::endl;                                       
    //std::cout << n[0] << " " << n[1] << " " << n[2] << " " << n[3] << std::endl;;
    res_tech =  rt;
    res_cult = 1 - res_tech;
    tol = res_cult;
    num_countries = 3;
    education_levels[1] = n[1] * res_tech;
    education_levels[2] = (n[2] + n[3]) * res_tech;
    education_levels[0] = n[0] + (n[1] + n[2] + n[3]) * (1 - res_tech);
    instability_ind = 0;
    living_standard = res_tech;
    natural_disaster = false;
    b = 0.4;
    d2 = 0.1;
    d3 = 0.1;
    d4 = 0.2;
    culture_vec = new double[num_countries];
    toler_vec = new double[num_countries];
    for (int i = 0; i < num_countries; i++){
        toler_vec[i] = 0; 
        culture_vec[i] = 0;
    }
    toler_vec[name] = 1;
    culture_vec[name] = population;
    std::cout << population << std::endl;
}
void country::DemographicChanges(double a, double g, double m){
    std::cout << "demogr 1 " << population << " " << n[1] << " " << n[2] << " " << n[0] << " " << n[3] <<  std::endl;
    int grew_up_babies = round(g * n[0]);
    int grew_up_teens = round(n[1] * m);
    int got_old = round(n[2] * a);
    int new_babies = round(n[2] * b);
    int dead_young = round(n[1] * d2);
    int dead_adult = round(n[2] * d3);
    int dead_old = round(n[3] * d4);
    std:: cout << "ddd " << n[3] << " " << d4 << " " << dead_old  << std::endl;
    education_levels[0] += new_babies; 
    education_levels[1] *= (1 - d2);
    education_levels[2] *= (1 - (n[3] * d4 + n[2] * d3) / (n[2] + n[3]));
    n[0] += new_babies - grew_up_babies;
    n[1] += grew_up_babies - dead_young - grew_up_teens;
    n[2] += grew_up_teens - dead_adult - got_old;
    n[3] += got_old - dead_old;
    culture_vec[name] += new_babies - dead_adult - dead_old - dead_young;
    population += new_babies - dead_adult - dead_old - dead_young;
    //std:: cout << "pop " << population << " " << new_babies << " " << dead_adult << " " << dead_old << " " << dead_young <<  std::endl;
    std::cout << "demogr 2 " << population << " " << n[1] << " " << n[2] << " " << n[0] << " " << n[3] <<  std::endl;
    std::cout << n[1] << "  JJJJJ" << std::endl;
 }

void country::Learning(double a){
    //обработать исключения по-хорошему
    int to_uni = round(n[1] * a * res_tech);
    int graduated = round(education_levels[1] * a * res_tech);
    education_levels[0] -= to_uni;
    education_levels[1] += to_uni - graduated;
    education_levels[2] += graduated;
}

void country::UpdateTolerVec(double a){
    for (int i = 0; i < num_countries; i++){
        toler_vec[i] += std::min(a * (toler_vec[i] *
             res_cult + tol + 0.4 * culture_vec[i] / population), 1.0);
    }
}
void country::UpdateInstability(double assimilationCtr, double toleranceCtr){
    double unassimilated = 0;
    double disliked = 0;
    for (int i = num_countries * step; i < num_countries * (step + 1); i++){
        if (assim[i] < assimilationCtr){
            unassimilated += (double) migrants[i];
        }
        if (toler_vec[i] <= toleranceCtr)
            disliked +=  (1 - toler_vec[i]) * (double) migrants[i]; 
    }
    instability_ind = (unassimilated + disliked) / population; 
}

void country::Assimilation(){
    //запускать только носле второго такта миграции
    for (int i = 0; i < num_countries * (step - 1); i++){
        double k = toler_vec[i % num_countries];
        if (k == 1 || assim[i] == 1)
            assim[i] = 1;
        else{
            assim[i] += k * 0.5;
            if (assim[i] > 1)
                assim[i] = 1;
        }
    }

}

void country::UpdateLivingStandard(double a1, double a2){
    try{
        if (population == 0) 
            throw std::runtime_error("Division by zero. Population = 0");
        living_standard = (double)(education_levels[2] / population) * a1;
    }
    catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }
    //living_standard = (education_levels[2] / population) * a1 - a2 * instability_ind;
}

void country::Entry(int amount, int motherland, const std::vector<int>& demography) {
    for (int i = 0; i < 4; i++) {
        std::cout << i << " " << demography[i] << " " << name << " " << n[i] << std::endl;
        n[i] += demography[i];
    }
    std::cout << std::endl;
    migrants.push_back(amount);
    assim.push_back(0);
    culture_vec[motherland] += amount;
    population += amount;
    std::cout << "entry " << amount << " " << population << " " << name << std::endl;
}

std::vector<int> country::Departure(int amount) {
    culture_vec[name] -= amount;
    population -= amount;
    std::cout << "leaving pr " << amount << " " << population << " " << name << std::endl;
    std::vector<int> leaving(4, 0); 
    std::cout << amount << std::endl;
    if (n[1] > amount) {  // сначала уезжает молодежь
        n[1] -= amount;
        leaving[1] = amount;
        std::cout << "leaving vec " << leaving[0] << " " << leaving[1] << " " << leaving[2] << " " << leaving[3] << std::endl;
        return leaving;
    }
    
    leaving[1] = n[1];
    amount -= n[1];
    n[1] = 0;
    
    if (n[2] + n[0] > amount) { // потом дети и взрослые
        n[0] -= round(amount / 2);
        leaving[0] += round(amount / 2);
        amount -= round(amount / 2);
        n[2] -= amount;
        leaving[2] += amount;
        std::cout << "leaving vec " << leaving[0] << " " << leaving[1] << " " << leaving[2] << " " << leaving[3] << std::endl;
        return leaving;
    }
    
    amount -= n[2] + n[0];
    leaving[0] = n[0];
    leaving[2] = n[2];
    n[2] = 0;
    n[0] = 0;
    
    if (n[3] > amount) {
        n[3] -= amount;
        leaving[3] = amount;
        std::cout << "leaving vec " << leaving[0] << " " << leaving[1] << " " << leaving[2] << " " << leaving[3] << std::endl;
        return leaving;
    } else {
        leaving[3] = n[3];
        n[3] = 0;
        std::cout << "leaving vec " << leaving[0] << " " << leaving[1] << " " << leaving[2] << " " << leaving[3] << std::endl;
        return leaving;
    }
}
void country::StepForward(){
    DemographicChanges(0.1, 0.2, 0.3); 
    Learning(0.3);
    UpdateTolerVec(0.2);
    if (step > 0)
        Assimilation();
    UpdateInstability(0.3, 0.8);
    UpdateLivingStandard(1, 0.6);
    step++;
}


country::~country() {
    delete[] toler_vec;
    delete[] culture_vec;
}