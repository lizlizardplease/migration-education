#include <cmath>
#include <vector>
#include <numeric>

class country {
    private:
        double b, d2, d3, d4; //вектор демографии
        int n[4];  //численность групп населения
        double res_tech;  //доля ресурса на техническое образование
        double unassimilated;  //доля неассимилировавшихся мигрантов
        double instability_ind;  //индекс социальной нестабильности
        int num_countries;    //число стран в мире (не уверена, что пригодится)
        std::vector<double> toler_vec;
        //double** toler_matrix;  //знаменитая матрица толерантности (пока не поняла зачем тут)
        int education_levels[3]; //число людей с разным уровнем технического образования (пока мигрантов не учитываю)
        std::vector<int> migrants;  //численность мигрантов из разных стран
        std::vector<double> assim;  //коеффициент ассимиляции мигрантов из разных стран (с учетом проведенного в стране времени?)
        void DemographicChanges(double a, double g, double m); //демографические изменения на данном такте
        void Learning(double a);  //образовательне изменения на данном такте
        void UpdateInstability(double a1, double a2, double toleranceCtr);
        void Assimilation(double k); //процесс ассимиляции на данном такте
     public:
        void StepForward(); //такт миграции


};
 
