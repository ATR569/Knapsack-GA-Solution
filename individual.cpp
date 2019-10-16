#include <bitset>
#include <vector>
#include <string>

using namespace std;

const int MAXN = 10;

const double INF = 1e18;
typedef vector<bool> Genoma;

class Individual{
    Genoma genoma;
    double fitnessValue;
public:    
    Individual(Genoma genoma);
    Individual(int size);
    
    Individual crossOver(Individual c, int cutPos);
    void mutation(int pos);
    bool isInitialized();

    int getSize();
    Genoma getGenoma();
    string getGenomaAsString();
    double getFitness();

    void setFitness(double fitnessValue);

    //  Comparador para a função sort()
    bool operator<(const Individual & ind) const {
        return fitnessValue > ind.fitnessValue;
    }
};

Individual::Individual(Genoma genoma){
    this->genoma = genoma;
    //  Fitness não inicializado
    this->fitnessValue = INF;
}

Individual::Individual(int size){
    Genoma genoma(size, 0);
    int genes = rand()%size + 1;

    while (genes--){
        int b = rand()%size;
        genoma[b] = 1;
    } 

    this->fitnessValue = INF;
    this->genoma = genoma;
}

Individual Individual::crossOver(Individual c, int cutPos){
    Genoma genoma;

    for (int i = 0; i < cutPos; i++){
        genoma.push_back(this->genoma[i]);
    }

    for (int i = cutPos; i < this->genoma.size(); i++){
        genoma.push_back(c.genoma[i]);
    }
    return Individual(genoma);
}

void Individual::mutation(int pos){
    this->genoma[pos] = !this->genoma[pos];
}

bool Individual::isInitialized(){
    return this->fitnessValue < INF;
}

int Individual::getSize(){
    return this->genoma.size();
}

Genoma Individual::getGenoma(){
    return this->genoma;
}

//  Função para debug
string Individual::getGenomaAsString(){
    string str;
    for (int i = 0; i < genoma.size(); i++) {
        str.push_back(genoma.at(i)?'1':'0');
    }
    return str;    
}

void Individual::setFitness(double fitnessValue){
    this->fitnessValue = fitnessValue;
}

double Individual::getFitness(){
    return this->fitnessValue;
}