#include "individual.cpp"
#include <algorithm>

class Population{
    int generation;
    int size;
    vector<Individual> individuals;
public:
    Population(int populationSize, int individualSize);

    int getSize();
    Individual get(int i);
    vector<Individual> * getIndividuals();
    vector<double> getProbSelection();
    
    void addIndividual(Individual individual);
    void sortPopulation();
    void setIndividuals(vector<Individual> & individuals);
};

Population::Population(int populationSize, int individualSize){
    this->size = populationSize;
    this->generation = 1;    
    
    for (int i = 0; i < populationSize; i++){
        individuals.push_back(Individual(individualSize));
    }
}

int Population::getSize(){
    return this->size;
}

Individual Population::get(int i){
    return this->individuals.at(i);
}

vector<Individual> * Population::getIndividuals(){
    return &this->individuals;
}

void Population::addIndividual(Individual individual){
    this->individuals.push_back(individual);
}

void Population::sortPopulation(){
    sort(individuals.begin(), individuals.end());
}

//  Retorna um vetor de probabilidades de seleção [0, 1]
vector<double> Population::getProbSelection(){
    sortPopulation();
    
    //  Desloca todos os valores dos fitness para acima de 1
    double offset = 0;
    
    if (this->individuals.back().getFitness() < 0){
        offset = 1 - individuals.back().getFitness();
    }

    double total = this->individuals.size()*offset;
    for (int i = 0; i < this->individuals.size(); i++){
        total += this->individuals[i].getFitness();
    }

    //  Vetor de probabilidade acumulada dos indivíduos
    vector<double> probSelection;

    probSelection.push_back((this->individuals[0].getFitness()+offset)/total);

    for (int i = 1; i < this->individuals.size(); i++){
        probSelection.push_back((this->individuals[i].getFitness()+offset)/total);
        probSelection[i] += probSelection[i-1];
    }

    return probSelection;
}

void Population::setIndividuals(vector<Individual> & individuals){
    this->individuals = individuals;
}
