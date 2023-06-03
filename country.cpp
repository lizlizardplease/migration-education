#include "country.h"

country::country(int s, int nc, int steps, double rt)
{
    step = 0;
    name = s;
    num_countries = nc;
    num_takts = steps;
    population = 1000000;
    leaved = 0;
    n[0] = static_cast<int>(round(0.2 * population));
    n[1] = static_cast<int>(round(0.2 * population));
    n[3] = static_cast<int>(round(0.2 * population));
    n[2] = population - (n[0] + n[1] + n[3]);
    res_tech =  rt;
    res_cult = 1.0 - res_tech;
    tol = res_cult;
    education_levels[1] =  static_cast<int>(n[1] * res_tech);
    education_levels[2] = static_cast<int>((n[2] + n[3]) * res_tech);
    education_levels[0] = static_cast<int>(n[0] + (n[1] + n[2] + n[3]) * (1.0 - res_tech));
    instability_ind = 0;
    living_standard = res_tech;
    b = 0.4;
    d2 = 0.1;
    d3 = 0.1;
    d4 = 0.2;
    culture_vec = new int[num_countries];
    toler_vec = new double[num_countries];
    for (int i = 0; i < num_countries; i++){
        toler_vec[i] = 0.0; 
        culture_vec[i] = 0;
    }
    toler_vec[name] = 1.0;
    culture_vec[name] = population;
    assrt();
}
void country::DemographicChanges(double a, double g, double m) {
    assrt();

    int grew_up_babies = static_cast<int>(round(g * n[0]));
    int grew_up_teens = static_cast<int>(round(n[1] * m));
    int got_old = static_cast<int>(round(n[2] * a));
    int new_babies = static_cast<int>(round(n[2] * b));
    int dead_young = static_cast<int>(round(n[1] * d2));
    int dead_adult = static_cast<int>(round(n[2] * d3));
    int dead_old = static_cast<int>(round(n[3] * d4));
    int dead_tmp = 0;

    education_levels[0] += new_babies;
    if (education_levels[2] < dead_adult + dead_old){
        dead_tmp = dead_adult + dead_old - education_levels[2];
        education_levels[2] = 0;
    }
    else
        education_levels[2] -= (dead_adult + dead_old);
    if (education_levels[1] < dead_young){
        dead_tmp += dead_young - education_levels[1];
        education_levels[1] = 0;
    }
    else
        education_levels[1] -= dead_young;
    education_levels[0] -= dead_tmp;
    n[0] += new_babies - grew_up_babies;
    n[1] += grew_up_babies - dead_young - grew_up_teens;
    n[2] += grew_up_teens - dead_adult - got_old;
    n[3] += got_old - dead_old;
    culture_vec[name] += new_babies - dead_adult - dead_old - dead_young;
    population += new_babies - dead_adult - dead_old - dead_young;
    if (population <= 0) 
        Death(); 
    assrt();
}

void country::Learning(double a){
    int to_uni = std::min(static_cast<int>(round(education_levels[0] * a * res_tech)), education_levels[0]);
    int graduated = std::min(static_cast<int>(round(education_levels[1] * a * res_tech)), education_levels[1]);

    // int to_uni = static_cast<int>(round(n[1] * a * res_tech));
    // int graduated = static_cast<int>(
    //                     round(education_levels[1] * a * res_tech));
    education_levels[0] -= to_uni;
    education_levels[1] += to_uni - graduated;
    education_levels[2] += graduated;
    assrt();
}

void country::UpdateTolerVec(double a){
    for (int i = 0; i < num_countries; i++){
        toler_vec[i] += std::min(a * (toler_vec[i] *
             res_cult + res_cult + 0.4 * culture_vec[i] / population), 1.0);
    }
}
void country::UpdateInstability(double assimilationCtr, 
                                    double toleranceCtr){
    assrt();
    double unassimilated = 0.0;
    double disliked = 0.0;
    for (int i = num_countries * step; i < num_countries * (step + 1); i++){
        if (assim[i] < assimilationCtr){
            unassimilated += (double) migrants[i];
        }
        if (toler_vec[i % num_countries] <= toleranceCtr)
            disliked +=  (1 - toler_vec[i % num_countries]) * (double) migrants[i]; 
    }
    std::cout << unassimilated << " " << disliked <<" " << population << std::endl;
    instability_ind = (unassimilated + disliked) / population; 
}

void country::Assimilation(){
    //запускать только носле второго такта миграции
    for (int i = 0; i < num_countries * (step - 1); i++){
        double k = toler_vec[i % num_countries];
        if (k == 1.0 || assim[i] == 1.0)
            assim[i] = 1.0;
        else{
            assim[i] += k * res_cult;
            if (assim[i] > 1.0)
                assim[i] = 1.0;
        }
    }
}


void country::assrt(){
    int dif = education_levels[0] + education_levels[1] + education_levels[2] - population;
    if (dif != 0)
        std::cout << "BAd " << education_levels[0] + education_levels[1] + education_levels[2]  << " " << population << std::endl;
    if (abs(dif) < 2){
        population += dif;
        n[0] += dif;
    }
    else
        std::cout << "very BAd " << std::endl;

}
void country::UpdateLivingStandard(double a1, double a2){
    assrt();
    if (population == 0) 
        living_standard = 0;
    else
        living_standard = ((double) education_levels[2] / (double) population) * a1;
    assrt();
}

void country::Entry(int motherland, const std::pair<std::vector<int>, std::vector<int>> &demography) {
    try{
        assrt();
        if (demography.first[0] < 0 || demography.first[1] < 0 || demography.first[3] < 0 || demography.first[4] <0)
                throw std::logic_error("Amount of coming migrants is less than zero....");
        int amount = 0;
        for (int i = 0; i < 4; i++) 
            n[i] += demography.first[i];
        for (int i = 0; i < 3; i++) {
            education_levels[i] += demography.second[i];
            amount += demography.second[i];
        }
    migrants.push_back(amount);
    assim.push_back(0);
    culture_vec[motherland] += amount;
    population += amount;
    }
    catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }
    assrt();
}

std::pair<std::vector<int>, std::vector<int>>country::Departure(int amount) {
    std::cout << "lol " << education_levels[2] << " " << amount << std::endl;
    std::vector<int> stud(3, 0);
    std::vector<int> leaving(4, 0); 
    if (population <= amount){
        if (leaved)
            return std::make_pair(leaving, stud);
        Death();
        for(int i = 0; i < 3; i++){
            stud[i] = education_levels[i];
            education_levels[i] = 0;
        }
        for(int i = 0; i < 4; i++){
            leaving[i] = n[i];
            n[i] = 0;
        }
        assrt();
        return std::make_pair(leaving, stud);
    }
    //ComeHome(amount);
    culture_vec[name] -= amount;
    population -= amount;

    if (education_levels[2] + education_levels[0] > amount){
        if (education_levels[2] > round(amount / 2) && education_levels[0] > round(amount / 2)){
            education_levels[2] -= round(amount / 2);
            stud[2] = round(amount / 2);
            education_levels[0] -= round(amount/2);
            stud[0] = round(amount / 2);
        }
        else{
            if(education_levels[2] > education_levels[0]){
                stud[0] = education_levels[0];
                stud[2] = amount - education_levels[0];
                education_levels[0] = 0;
                education_levels[2] -= stud[2];
            }
            else{
                stud[2] = education_levels[2];
                stud[0] = amount - education_levels[2];
                education_levels[2] = 0;
                education_levels[0] -= stud[0];
            }
        }
    }
    else{
        stud[0] = education_levels[0];
        stud[2] = education_levels[2];
        stud[1] = std::min(amount - education_levels[2] 
            - education_levels[0], education_levels[1]);
        education_levels[1] -= stud[1];
        education_levels[0] = 0;
        education_levels[2] = 0;
    }
    if (n[1] > amount) {  // сначала уезжает молодежь
        n[1] -= amount;
        leaving[1] = amount;
        assrt();
        return std::make_pair(leaving, stud);
    }
    leaving[1] = n[1];
    amount -= n[1];
    n[1] = 0;

    if (n[2] + n[0] > amount) { // потом дети и взрослые
        if (n[2] >= round(amount / 2) && n[0] >=  round(amount / 2)){
            n[0] -= round(amount / 2);
            leaving[0] += round(amount / 2);
            amount -= round(amount / 2);
            n[2] -= amount;
            leaving[2] += amount;
        } else{
            if (n[2] < round(amount / 2)){
                amount -= n[2];
                leaving[2] = n[2];
                n[2] = 0;
                n[0] -= amount;
                leaving[0] = amount; 
            }
            else{
                amount -= n[0];
                leaving[0] = n[0];
                n[0] = 0;
                n[2] -= amount;
                leaving[2] = amount; 
            }
        }
            assrt();
            return std::make_pair(leaving, stud);
    }
    
    amount -= n[2] + n[0];
    leaving[0] = n[0];
    leaving[2] = n[2];
    n[2] = 0;
    n[0] = 0;
    
    if (n[3] > amount) {
        n[3] -= amount;
        leaving[3] = amount;
        assrt();
        return std::make_pair(leaving, stud);
    } else {
        leaving[3] = n[3];
        n[3] = 0;
        assrt();
        return std::make_pair(leaving, stud);
    }
}

void country::ComeHome(int amnt){
    int i = 0;
    while (i < migrants.size() && amnt > 0) 
    {
        if (migrants[i] < amnt){
            amnt -= migrants[i];
            migrants[i] = 0;
        }
        else{
            migrants[i] -= amnt;
            amnt = 0;
        }
        i++;
    }
}

// void country::Disaster(){
//     living_standard = living_standard * 0.95;
//     for (int i = 0; i < 4; i++){
//         double deduction = 0.03 * n[i];
//         int roundedDeduction = static_cast<int>(deduction + 0.5); // Округление к ближайшему целому
//         population -= roundedDeduction;
//         n[i] -= roundedDeduction;
//     }
//     for (auto& i : migrants){
//         double deduction = 0.03 * i;
//         int roundedDeduction = static_cast<int>(deduction + 0.5); // Округление к ближайшему целому
//         population -= roundedDeduction;
//         i -= roundedDeduction;
//     }0.000000
// }
void country::Death(){
    population = 0;
    culture_vec[name] = 0;
    std::cout << "DEAD " << name << std::endl;
    living_standard = 0;
    instability_ind = 0;
    leaved = true;
}

void country::StepForward(){
    std::cout << population << std::endl;
    assrt();
    if (leaved)
        return;
    else{
    DemographicChanges(0.1, 0.2, 0.3); 
    if (leaved)
        return;
    UpdateTolerVec(0.2);
    if (name == 2){}
    if (step > 0)
        Assimilation();
    UpdateInstability(0.7, 0.7);
    UpdateLivingStandard(1.0, 0.6);
    Learning(0.5);
    }
    step++;

}


country::~country() {
    delete[] culture_vec;
    delete[] toler_vec;
}