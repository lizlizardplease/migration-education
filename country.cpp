#include "country.h"

void country::DemographicChanges(double a, double g, double m){
    int grew_up_babies = round(g * n[0]);
    int grew_up_kids = round(n[1] * m);
    int got_old = round(n[2] * a);
    n[0] += round(n[2] * b) - grew_up_babies;
    n[1] += grew_up_babies - round(n[1] * d2) - grew_up_kids;
    n[2] += grew_up_kids - round(n[2] * d3) - got_old;
    n[3] += got_old - round(n[3] * d4);
}

void country::Learning(double a){
    int to_uni = round(education_levels[0] * a * res_tech);
    int graduated = round(education_levels[1] * a * res_tech);
    education_levels[0] -= to_uni;
    education_levels[1] += to_uni - graduated;
    education_levels[3] += graduated;
}


void country::UpdateInstability(double a1, double a2, double toleranceCtr){
    double omega = 0;
    for (int i = 0; i < num_countries; i++){
        if(migrants[i] > 0 && toler_vec[i] <= toleranceCtr)
            omega += toler_vec[i] * migrants[i];
    }
    if (omega == 0)
        instability_ind = a1 * unassimilated + a2;
    else   
        instability_ind = a1 * unassimilated + a2 + (1 - omega / std::reduce(migrants.begin(), migrants.end()));
}