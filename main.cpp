#include <iostream>
#include <algorithm>
#include <math.h>
#include <time.h>
#include "knapsackGA.cpp"
#include "knapsackDP.cpp"

using namespace std;

int main(){
    srand(time(NULL));
    //  Instância 1
    vector<int> weights = {5, 70, 6, 12, 5, 15, 3, 25, 15, 5};
    vector<int> costs =   {10, 12, 15, 16, 20, 34, 44, 13, 7, 80};
    const int KNAPSACK_CAP = 100;

    //  Instância 2
    //vector<int> weights = {10,  5, 3, 25, 15, 75, 35, 5, 25, 13, 5, 70, 6, 12, 5, 15, 3, 25, 15, 75, 35, 5};
    //vector<int> costs =   {10, 15, 2, 3,  8, 23, 12, 12, 15, 16, 20, 50, 12, 13, 12, 67, 2,34, 44, 13, 7, 80};
    //const int KNAPSACK_CAP = 115;

    //  Instância 3
    //vector<int> weights = {40, 25, 10, 3, 5, 8, 30, 8, 20, 16, 9, 19, 17, 30, 32, 1, 2, 3, 26, 24, 9, 15, 16, 2, 20, 2, 3, 1, 8, 30, 35, 4, 5, 6, 7, 8, 9, 15, 13, 37};
    //vector<int> costs = {2500, 1000, 300, 900, 800, 930, 2000, 1500, 1300, 456, 987, 2400, 2200, 1950, 1750, 300, 100, 130, 1200, 1100, 800, 700, 60, 50, 843, 951, 321, 159, 870, 1753, 2369, 149, 367, 852, 129, 631, 1456, 1789, 1123, 2456};
    //const int KNAPSACK_CAP = 297;
    
    const int POPULATION_SIZE = 10;     
    const int GENERATIONS = 50;

    const double PROB_CROSS = 0.9;
    const double PROB_MUT = 0.4;

    int solDP = solveDP(weights, costs, KNAPSACK_CAP);
    cout << "Solucao DP: "<< solDP << endl;
    cout << "=================================" << endl;
    
    //  Erro médio absoluto
    double erro = 0;
    const int TEST = 100;
    for (int i = 0; i < TEST; i++)    {
        KnapsackGA ga(weights, costs, POPULATION_SIZE, KNAPSACK_CAP);
        int solGA = ga.solveGA(GENERATIONS, PROB_CROSS, PROB_MUT);
        cout << "Solucao "<< i + 1 << " GA: " << solGA <<endl;
        erro += abs(solGA-solDP);
    }
    double MAE = erro/solDP/TEST;
    cout << endl << "=================================" << endl;
    cout << "Mean Absolute Error (MAE): " << MAE << endl;

    return 0;
}