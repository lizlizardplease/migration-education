#include <cmath>
#include <vector>
#include <numeric>
#include <stdexcept>
#include <iostream>
#include <array>


class country {
    private:
        int population;
        double b, d2, d3, d4, Md23; //вектор демографии
        int step;  //текущий такт миграции
        std::array<int, 4> n; //численность групп населения
        double tol; //коэффициент толерантности в данной стране к мигрантам в целом
        double res_tech;  //доля ресурса на техническое образование
        double res_cult; //доля ресурса на культурное образование
        double instability_ind;  //индекс социальной нестабильности
        int num_countries;  
        int num_takts;
        bool leaved;
        int name; // номер ну или название страны
        double* toler_vec;
        int* culture_vec; //вектор культур
        double living_standard;
        bool natural_disaster;
        int education_levels[3]; //число людей с разным уровнем технического образования (пока мигрантов не учитываю) 
        std::vector<int> migrants; //численность мигрантов из разных стран
        std::vector<double> assim;  //коеффициент ассимиляции мигрантов из разных стран 
        void DemographicChanges(double a, double g, double m); //демографические изменения на данном такте
        void Learning(double a);  //образовательне изменения на данном такте
        void UpdateInstability(double assimilationCtr, double toleranceCtr);
        void Disaster();
        void Assimilation(); //процесс ассимиляции на данном такте
        void UpdateLivingStandard(double a1); //(другие коефф-ты или те же?) 
        void UpdateTolerVec(double a);
        void Death();
        void assrt();
    public:
        country (int name, int nc, int num_takts, double rt);
        void Entry(int motherland, const std::pair<std::vector<int>, std::vector<int>> &demography); //мигранты из страны motherland приехали
        std::pair<std::vector<int>, std::vector<int>> Departure(int amount); //уезжают из страны
        void StepForward(); //такт миграции
        void ComeHome(int x);
        double GetDesire() {return living_standard - instability_ind;}
        double GetMoral() {return 1.0 - res_tech;}
        double GetLivingStandard() {return living_standard;};
        double GetInst() {return instability_ind;}
        int GetPopulation() {return population;}
        void DisasterSent() {natural_disaster = true;}  
        void DisasterStoped() {natural_disaster = false;} 
        double* GetTolerVec(){return toler_vec;};
        std::array<int, 4> GetDemography(){return n;}
        double GetRes_tech(){return res_tech;}
        int mg_debug;
        ~country();
};



