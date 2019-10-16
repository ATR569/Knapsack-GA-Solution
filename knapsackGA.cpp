
#include "population.cpp"

using namespace std;

class KnapsackGA{
    Population * population;
    vector<int> weights;
    vector<int> costs;
    int knapsackCap;

    Population * getPopulation();
    double getFitness(int i);
    int getIndividualValue(Individual individual);
    int getIndividualWeight(Individual individual);
    
    int roullete(vector<double> & probSelection);
    bool roullete(double prob);

    void calcFitness();
    void fitPopulation();
public:
    KnapsackGA(vector<int> & weights, vector<int> & costs, int populationSize, int knapsackCap);
    
    int solveGA(int generations, double probCross, double probMutation);
};

//  Construtor
KnapsackGA::KnapsackGA(vector<int> & weights, vector<int> & costs, int populationSize, int knapsackCap){
    this->weights = weights;
    this->costs = costs;
    this->population = new Population(populationSize, costs.size());
    this->knapsackCap = knapsackCap;
}

Population * KnapsackGA::getPopulation(){
    return this->population;
}

//  Retorna o valor fitness do k-ésimo indivíduo
double KnapsackGA::getFitness(int k){
    Individual individual = population->get(k);
    
    int weight = getIndividualWeight(individual);
    int cost = getIndividualValue(individual);

    double a = (double)weight/knapsackCap;

    //  Penalização proporcional ao quadrado peso (w > 4x C => fitness < 0)
    return (16 - a*a)*cost; 
}

//  Atualiza o valor fitness dos indivíduos ainda não calculados
void KnapsackGA::calcFitness(){
    vector<Individual> * individuals = population->getIndividuals();
    
    for (int i = 0; i < individuals->size(); i++) {
        if (!individuals->at(i).isInitialized()){
            individuals->at(i).setFitness(getFitness(i));
        }
    }
    
}

//  Roleta
int KnapsackGA::roullete(vector<double> & probSelection){
    double x = (double)rand() / RAND_MAX;
    int id = upper_bound(probSelection.begin(), probSelection.end(), x) - probSelection.begin();
    return min(id, (int)probSelection.size()-1);
}

//  Roleta binária com probabilidade prob
bool KnapsackGA::roullete(double prob){
    return ((double)rand() / RAND_MAX) < prob;    
}

//  Calcula o valor do indivíduo
int KnapsackGA::getIndividualValue(Individual individual){
    Genoma genoma = individual.getGenoma();
    int individualSize = individual.getSize();
    int cost = 0;

    for (int i = 0; i < individualSize; i++)  {
        if (genoma[i]){
            cost += costs[i];
        }
    }

    return cost;
}

//  Calcula o peso do indivíduo
int KnapsackGA::getIndividualWeight(Individual individual){
    Genoma genoma = individual.getGenoma();
    int individualSize = individual.getSize();
    int weight = 0;

    for (int i = 0; i < individualSize; i++)  {
        if (genoma[i]){
            weight += weights[i];
        }
    }

    return weight;
}

//  Realiza a seleção dos melhores indivíduos usando a roleta e elitismo
void KnapsackGA::fitPopulation(){
    vector<Individual> * popIndividuals = this->population->getIndividuals();
    
    //  Elitismo
    int idBest = -1;
    int bestValue = 0;

    for (int i = 0; i < popIndividuals->size(); i++) {
        Individual candidate =  popIndividuals->at(i);
        int value = getIndividualValue(candidate);
        if (getIndividualWeight(candidate) <= knapsackCap && value > bestValue){
            idBest = i;
            bestValue = value;
        }
    }
    
    vector<Individual> individuals;

    if (idBest >= 0){
        individuals.push_back(popIndividuals->at(idBest));
        popIndividuals->erase(popIndividuals->begin() + idBest);
    }

    //  Seleção dos melhores indivíduos através da roleta
    int popSize = this->population->getSize();
    while (individuals.size() < popSize){
        vector<double> probSelection = this->population->getProbSelection();
        int id = roullete(probSelection);
        
        individuals.push_back(popIndividuals->at(id));
        popIndividuals->erase(popIndividuals->begin() + id);
    }
    
    this->population->setIndividuals(individuals);
}

//  Resolve o problema da mochila
int KnapsackGA::solveGA(int generations, double probCross, double probMutation){
    calcFitness();
    while (generations--){
        //  Vetor de probabilidade proporcional ao fitness
        vector<double> probSelection = population->getProbSelection();

        //  Evolução da população
        for (int i = 0; i < population->getSize()/2; i++) {
            if (roullete(probCross)){
                //  Seleção dos pais pela roleta
                Individual parent1 = this->population->get(roullete(probSelection));
                Individual parent2 = this->population->get(roullete(probSelection));

                //  Crossover em posição aleatória
                int crossPos = rand()%parent1.getSize();

                Individual newInd1 = parent1.crossOver(parent2, crossPos);
                Individual newInd2 = parent2.crossOver(parent1, crossPos);

                //  Mutação em posição aleatória
                if (roullete(probMutation)){
                    int mutPos = rand()%newInd1.getSize();
                    newInd1.mutation(mutPos);
                }
                if (roullete(probMutation)){
                    int mutPos = rand()%newInd2.getSize();
                    newInd2.mutation(mutPos);
                }

                //  Calcula o fitness dos novos indivíduos
                int tam = this->population->getIndividuals()->size();

                this->population->addIndividual(newInd1);
                population->getIndividuals()->at(tam).setFitness(getFitness(tam));
                
                this->population->addIndividual(newInd2);
                population->getIndividuals()->at(tam+1).setFitness(getFitness(tam+1));
            }
        }
        
        //  Seleciona os melhores indivíduos pra manter o tamanho da população
        fitPopulation();
    }

    //  Procura entre os indivíduos o que representa a melhor solução
    int solution = 0;
    for (int i = 0; i < this->population->getSize(); i++) {
        if (getIndividualWeight(population->get(i)) <= knapsackCap)
            solution = max(solution, getIndividualValue(population->get(i)));
    }

    return solution;
}