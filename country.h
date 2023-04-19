#include <cmath>
#include <vector>
#include <numeric>

class country {
    private:
        double b, d2, d3, d4, Md23; //вектор демографии
        int step;  //текущий такт миграции
        int n[4] = {400, 100, 1000, 200}; //численность групп населения
        double tol; //коэффициент толерантности в данной стране к мигрантам в целом
        double res_tech;  //доля ресурса на техническое образование
        double res_cult; //доля ресурса на культурное образование
        double unassimilated;  //доля неассимилировавшихся мигрантов
        double instability_ind;  //индекс социальной нестабильности
        int num_countries;    //число стран в мире (не уверена, что пригодится)
        int name; // номер ну или название страны
        double* toler_vec;
        //double** culture_matrix; //матрица культур
        double living_standard;
        bool natural_disaster;
        int education_levels[3]; //число людей с разным уровнем технического образования (пока мигрантов не учитываю)
        std::vector<int> migrants;  //численность мигрантов из разных стран
        std::vector<double> assim;  //коеффициент ассимиляции мигрантов из разных стран 
        void DemographicChanges(double a, double g, double m); //демографические изменения на данном такте
        void Learning(double a);  //образовательне изменения на данном такте
        void UpdateInstability(double a1, double a2, double toleranceCtr);
        void Assimilation(double k); //процесс ассимиляции на данном такте
        void UpdateLivingStandard(double a1, double a2); //(другие коефф-ты или те же?) 
        void UpdateTolerVec(double a);
    public:
        country(int name);
        void Entry(int amount, int motherland); //мигранты из страны motherland приехали
        void Departure(int amount); //уезжают из страны
        void StepForward(double* flow); //такт миграции
        double GetDesire() {return living_standard / instability_ind;};
        double GetMoral() {return 1 - res_tech;}
        int GetPopulation() {return n[0] + n[1] + n[2] + n[3];}
        void DisasterSent() {natural_disaster = true;}   
        double* GetTolerVec(){return toler_vec;};
        ~country();
};



