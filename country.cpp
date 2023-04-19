#include "country.h"

country::country(int s)
{
    step = 0;
    name = s;
    res_tech =  ((double) rand() / (RAND_MAX));
    res_cult = 1 - res_tech;
    tol = res_cult;
    num_countries = 3;
    education_levels[1] = n[1] * res_tech;
    education_levels[2] = (n[2] + n[3]) * res_tech;
    education_levels[0] = n[0] + (n[1] + n[2] + n[3]) * (1 - res_tech);
    unassimilated = 0;
    instability_ind = 0;
    living_standard = res_tech;
    natural_disaster = false;
    b = 0.4;
    d2 = 0.1;
    d3 = 0.1;
    d4 = 0.1;
   // culture_matrix = new double*[num_countries];
    toler_vec = new double[num_countries];
    for (int i = 0; i < num_countries; i++){
        toler_vec[i] = 0; //вообще вроде не 0
        /*culture_matrix[i] = new double[num_countries];
        for(int j = 0; j < num_countries; j++){
            if (i == j)
                culture_matrix[i][i] = 1;
            else
                culture_matrix[i][j] = 0;
        }*/
    }
    toler_vec[name] = 1;
}
void country::DemographicChanges(double a, double g, double m){
    int grew_up_babies = round(g * n[0]);
    int grew_up_teens = round(n[1] * m);
    int got_old = round(n[2] * a);
    int new_babies = round(n[2] * b);
    education_levels[0] += new_babies; 
    education_levels[1] *= (1 - d2);
    education_levels[2] *= (1 - (n[3] * d4 + n[2] * d3) / (n[2] + n[3]));
    n[0] += new_babies - grew_up_babies;
    n[1] += grew_up_babies - round(n[1] * d2) - grew_up_teens;
    n[2] += grew_up_teens - round(n[2]  * d3) - got_old;
    n[3] += got_old - round(n[3] * d4);
}

void country::Learning(double a){
    //обработать исключения по-хорошему
    int to_uni = round(n[1] * a * res_tech);
    int graduated = round(education_levels[1] * a * res_tech);
    education_levels[0] -= to_uni;
    education_levels[1] += to_uni - graduated;
    education_levels[2] += graduated;
}

void country::UpdateTolerVec(double a, double b){
    for (int i = 0; i < num_countries; i++){
        toler_vec[i] += a * (toler_vec[i] * (1 - res_tech)); //+ culture_matrix[name][i] * b + tol);
    }
}
void country::UpdateInstability(double a1, double a2, double toleranceCtr){
    double omega = 0;
    for (int i = 0; i < num_countries; i++){
        if(migrants[i] > 0 && toler_vec[i] <= toleranceCtr)
            omega += toler_vec[i] * migrants[i];  //не нравится мне
    }
    if (omega == 0)
        instability_ind = a1 * unassimilated + a2;
    else   
        instability_ind = a1 * unassimilated + a2 + (1 - omega / std::reduce(migrants.begin(), migrants.end()));
}

void country::Assimilation(double k){
    //запускать только носле второго такта миграции
    for (int i = 0; i < num_countries * (step - 1); i++){
        double k = toler_vec[i % num_countries];
        if (k == 1)
            assim[i] = 1;
        else
            assim[i] += k * 0.5;
        if (assim[i] > 1)
            assim[i] = 1;
    }

}

void country::UpdateLivingStandard(double a1, double a2){
    living_standard = (education_levels[2]/ (n[0] + n[1] + n[2] + n[3])) * a1 - a2 * instability_ind;
}

void country::Entry(int amount, int motherland){
    migrants.push_back(amount);
    if (toler_vec[motherland] == 1)
        assim.push_back(1);
    else
        assim.push_back(0);
}

void country::Departure(int amount){
    if (n[1] > amount){  //сначала уезжает молодежь
        n[1] -= amount;
        return;
    }
    amount -= n[1];
    n[1] = 0;
    if (n[2] + n[0] > amount){ //потом дети и взрослые
        n[0] -= amount / 2; 
        amount -= amount / 2;
        n[2] -= amount;
        return;
    }
    amount -= n[2] + n[0];
    n[2] = 0;
    n[0] = 0;
    if (n[3] > amount)
        n[3] -= amount;
    else
        n[3] = 0;
}
void country::StepForward(double* flow){
    DemographicChanges(0.1, 0.2, 0.3); //пока не знаю как считать то
    Learning(0.3);
    UpdateTolerVec(0.2, 0.3);
    Assimilation(0.4);
    UpdateInstability(0.3, 0.6, 0.8);
    UpdateLivingStandard(0.4, 0.6);
}


country::~country() {
    delete[] toler_vec;
    //for (int i = 0; i < num_countries; i++) {
      //  delete [] culture_matrix[i];
   // }
    //delete [] culture_matrix;
}