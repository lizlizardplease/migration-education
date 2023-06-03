#include <iostream>
#include <fstream>
#include "world.cpp"

int main(int argc, char *argv[])
{
    // double rts[10] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
    // auto x = new world(10, 50, "experiment_2_li.txt", "experiment_2_d.txt", rts);
    double rts[5] = {0.0, 0.1, 0.2, 0.3,  0.5};
    auto x = new world(5, 50, "experiment_1_li.txt", "experiment_1_d.txt", rts);
    // double rts[3] = { 0.2,  0.5, 0.7};
    // auto x = new world(3, 50, "experiment_0_li.txt", "experiment_0_d.txt", rts);
    return 0;
}
