#include "../Headers.h"
mutex MutexCrossing;
mutex MutexBest;

GA::GA()
{
	this->MaxIteration = 2000;
	this->MaxPopulation = 30;
	this->AllelesSize = 4;
	this->ResultPercent = 50;
	this->MutatePercent = 10;
	this->MutateStep = 10;
	this->MutatePercentNewGenom = 50;
	this->CrossObject = CrossT();
	this->MaxValue = 1000;
	this->TruncationStep = 60;
	this->ElitePercent = 60;
	this->RTTypeSelectionPercent = 50;
}

GA::GA(int MaxIteration, int MaxPopulation, int AllelesSize, int ResultPercent, 
	int MutatePercent, int MutateStep, int MutatePercentNewGenom, CrossT object, 
	int MaxValue, int TruncationStep, int ElitePercent, int RTTypeSelectionPercent)
{
	this->MaxIteration = MaxIteration;
	this->MaxPopulation = MaxPopulation;
	this->AllelesSize = AllelesSize;
	this->ResultPercent = ResultPercent;
	this->MutatePercent = MutatePercent;
	this->MutateStep = MutateStep;
	this->MutatePercentNewGenom = MutatePercentNewGenom;
	this->CrossObject = object;
	this->MaxValue = MaxValue;
	this->TruncationStep = TruncationStep;
	this->ElitePercent = ElitePercent;
	this->RTTypeSelectionPercent = RTTypeSelectionPercent;
}

GA::~GA()
{
}

int GA::GetRand()
{
	random_device random_device;
	mt19937 generator(random_device());
	uniform_int_distribution<> distribution(-GetMaxValue(), GetMaxValue());

	return distribution(generator);
}

int GA::GetRand(int from, int to)
{
	random_device random_device;
	mt19937 generator(random_device());
	uniform_int_distribution<> distribution(from, to);

	return distribution(generator);
}

double GA::DGetRand()
{
	random_device random_device;
	mt19937 generator(random_device());
	uniform_real_distribution<double> distribution(-GetMaxValue(), GetMaxValue());

	return distribution(generator);
}

double GA::GetRand(double from, double to)
{
	random_device random_device;
	mt19937 generator(random_device());
	uniform_real_distribution<double> distribution(from, to);

	return distribution(generator);
}

void GA::GenerateStartPopulation()
{
	srand((unsigned)time(NULL));
	Chromosome obj;
	Population.clear();

	vector<vector<vector<double>>>* pWeightsLayers;
	pWeightsLayers = ReadConfiguration(GetFileNameOutputWeights().c_str());
	if (pWeightsLayers != NULL)
	{
		obj = Chromosome();

		for (int i = 0; i < pWeightsLayers->size(); i++)
		{
			for (int j = 0; j < (*pWeightsLayers)[i].size(); j++)
			{
				for (int z = 0; z < (*pWeightsLayers)[i][j].size(); z++)
				{
					obj.alleles.push_back((*pWeightsLayers)[i][j][z]);
				}
			}
		}

		Population.push_back(obj);
		Population.push_back(obj);
	}

	for (int i = Population.size(); i < GetMaxPopulation(); i++)
	{
		obj = Chromosome(GetAllelesSize());

		for (int j = 0; j < GetAllelesSize(); j++)
			obj.alleles[j] = DGetRand();


		Population.push_back(obj);
	}
}

int GA::Fitness(NeuronNet* pNN)
{
	SYSTEM_INFO siSysInfo;
	GetSystemInfo(&siSysInfo);

	int CPUCount = siSysInfo.dwNumberOfProcessors;
	CPUCount = 4;

	int curstep = Population.size() / CPUCount;

	int Part = 0;
	int CurrentPart = 0;
	int ChromosomePointer = 0;
	vector<thread> ThreadPool;

	CurrentPart = Population.size() - CPUCount;

	while (true)
	{
		ThreadPool.clear();

		if (Part != curstep)
		{
			CurrentPart = CPUCount;
		}
		else
		{
			CurrentPart = Population.size() - (CPUCount*curstep);
		}
		Part++;

		for (int i = 0; i < CurrentPart; i++)
		{
			ThreadPool.push_back(std::thread(&GA::FitnessForOne, this, (*g_pNNs)[i], ref(Population[ChromosomePointer])));
			ChromosomePointer++;
		}

		for (auto &t : ThreadPool)
			t.join();

		if (ChromosomePointer == Population.size())
			break;
	}

	return -1;
}

double GA::FitnessForOne(NeuronNet* pNN, Chromosome &chromosome)
{
	
	//Установка значений весов
	vector<vector<vector<double>>>* pWeightsLayers = new vector<vector<vector<double>>>();

	//Создаем вектор весов синапсов первого слоя нейронов
	vector<vector<double>> Weights = vector<vector<double>>();

	double fitness = 0;
	int FirstNeuronsLayerNumber = pNN->FirstNeuronsLayer.size();
	int SecondNeuronsLayerNumber = pNN->SecondNeuronsLayer.size();
	int ThirdNeuronsLayerNumber = pNN->ThirdNeuronsLayer.size();
	int InputsNumber = pNN->Inputs.size();
	int OutputNeuronsNumber = pNN->OutputNeurons.size();
	int cntAlleles = 0;

	for (int i = 0; i < FirstNeuronsLayerNumber; i++)
	{
		Weights.push_back(vector<double>(InputsNumber));
		for (int j = 0; j < InputsNumber; j++)
		{
			Weights[i][j] = chromosome.alleles[cntAlleles];
			cntAlleles++;
		}
	}
	pWeightsLayers->push_back(Weights);

	//Создаем вектор весов синапсов второго слоя нейронов
	Weights = vector<vector<double>>();
	for (int i = 0; i < SecondNeuronsLayerNumber; i++)
	{
		Weights.push_back(vector<double>(FirstNeuronsLayerNumber));
		for (int j = 0; j < FirstNeuronsLayerNumber; j++)
		{
			Weights[i][j] = chromosome.alleles[cntAlleles];
			cntAlleles++;
		}
	}
	pWeightsLayers->push_back(Weights);

	//Создаем вектор весов синапсов третьего слоя нейронов
	Weights = vector<vector<double>>();
	for (int i = 0; i < ThirdNeuronsLayerNumber; i++)
	{
		Weights.push_back(vector<double>(SecondNeuronsLayerNumber));
		for (int j = 0; j < SecondNeuronsLayerNumber; j++)
		{
			Weights[i][j] = chromosome.alleles[cntAlleles];
			cntAlleles++;
		}
	}
	pWeightsLayers->push_back(Weights);

	//Создаем вектор весов синапсов выходного нейрона
	Weights = vector<vector<double>>();
	for (int i = 0; i < OutputNeuronsNumber; i++)
	{
		Weights.push_back(vector<double>(ThirdNeuronsLayerNumber));
		for (int j = 0; j < ThirdNeuronsLayerNumber; j++)
		{
			Weights[i][j] = chromosome.alleles[cntAlleles];
			cntAlleles++;
		}
	}
	pWeightsLayers->push_back(Weights);

	//Запуск нейронной сети
	vector<vector<double>>* pResults;

	pNN->SetWeights(pWeightsLayers);

	pResults = ProcessNetForAllInputVectors(pNN);

	fitness = GetTrend(pResults)[0];

	//Освобождение обязательно
	delete(pWeightsLayers);
	delete(pResults);


	chromosome.countTrue = fitness;
	chromosome.fitness = abs(fitness - (double) g_pInputs->size());
	chromosome.percent = (double)fitness / (double) g_pInputs->size() * 100;

	MutexBest.lock();
	if (chromosome.fitness < BestPopulation.fitness)
		BestPopulation = chromosome;
	MutexBest.unlock();

	return chromosome.fitness;
}

double GA::MultInv()
{
	double sum = 0;

	for (int i = 0; i < GetMaxPopulation(); i++)
	{
		sum += 1 / Population[i].fitness;
	}

	return sum;
}

void GA::GenerateLikelihoods()
{
	double multinv = MultInv();

	double last = 0;
	for (int i = 0; i < GetMaxPopulation(); i++)
	{
		Population[i].chance = last = last + ((1 / (Population[i].fitness) / multinv) * 100);
	}
}

void GA::Mutate(double &val)
{

	if (GetRand(0, 100)<MutatePercentNewGenom) //New Genom
	{
		val = DGetRand();
	}
	else  //Genom modified by Mutatestep
	{
		double bufval = val;
		bufval = bufval*(double)GetMutateStep() / 100.0;

		if (GetRand(0, 100)<50)
			val += bufval;
		else
			val -= bufval;
	}

}

void GA::ThreadCrossing(int l, int r, int TypeSelection)
{
	for (int i = l; i < r; i++)
	{
		int parent1 = 0, parent2 = 0, iterations = 0;
		double tmp = 0;


		while (parent1 == parent2 || Population[parent1] == Population[parent2])
		{
			if (TypeSelection < GetRTTypeSelectionPercent())
			{
				parent1 = RouletteSelection(GetRand(0.0, 100.0));
				parent2 = RouletteSelection(GetRand(0.0, 100.0));
			}
			else
			{
				parent1 = TournamentSelection(GetRand(0, MaxPopulation - 1), GetRand(0, MaxPopulation - 1));
				parent2 = TournamentSelection(GetRand(0, MaxPopulation - 1), GetRand(0, MaxPopulation - 1));
			}

			if (++iterations > GetMaxPopulation())
				break;
		}

		vector<Chromosome> ret = Crossing(parent1, parent2);
		MutexCrossing.lock();
		CrossingPopulation.push_back(ret[0]);
		CrossingPopulation.push_back(ret[1]);
		MutexCrossing.unlock();
	}
}

vector<Chromosome> GA::Crossing(int p1, int p2)
{

	int mAlleleSize = GetAllelesSize();
	vector<Chromosome> retChromosome;
	Chromosome child = Chromosome(mAlleleSize);
	Chromosome child2 = Chromosome(mAlleleSize);
	int CrossTypePercent = GetRand(1, 100);
	int CrossType;
	int cut1, cut2;
	int from, to;
	double alf;
	double alf2;
	bool linerecomb = false;

	int OnePoint = CrossObject.GetOnePointPercent();
	int TwoPoint = CrossObject.GetTwoPointPercent();
	int Each = CrossObject.GetEachPercent();
	int Line = CrossObject.GetLinePercent();
	int InterM = CrossObject.GetInterMPercent();


	if (CrossTypePercent <= OnePoint)
		CrossType = 0;
	else if (CrossTypePercent > OnePoint && CrossTypePercent <= TwoPoint)
		CrossType = 1;
	else if (CrossTypePercent > TwoPoint && CrossTypePercent <= Each)
		CrossType = 2;
	else if (CrossTypePercent > Each && CrossTypePercent <= Line)
		CrossType = 3;
	else if (CrossTypePercent <= InterM)
		CrossType = 4;
	else
	{
		retChromosome.push_back(Population[p1]);
		retChromosome.push_back(Population[p2]);
		return retChromosome;
	}
		

	switch (CrossType)
	{
	case 0: //One Point

		cut1 = GetRand(1, mAlleleSize - 1);

		for (int i = 0; i < mAlleleSize; i++)
		{
			if (i < cut1)
			{
				child.alleles[i] = Population[p1].alleles[i];
				child2.alleles[i] = Population[p2].alleles[i];
			}
			else
			{
				child.alleles[i] = Population[p2].alleles[i];
				child2.alleles[i] = Population[p1].alleles[i];
			}
		}
		break;

	case 1: //Two Point

		cut1 = GetRand(1, mAlleleSize - 2);
		from = cut1 + 1;
		to = mAlleleSize - 1;
		cut2 = GetRand(from, to);

		for (int i = 0; i < mAlleleSize; i++)
		{
			if (i < cut1)
			{
				child.alleles[i] = Population[p1].alleles[i];
				child2.alleles[i] = Population[p2].alleles[i];
			}
			else if (i >= cut1 && i<cut2)
			{
				child.alleles[i] = Population[p2].alleles[i];
				child2.alleles[i] = Population[p1].alleles[i];
			}
			else
			{
				child.alleles[i] = Population[p1].alleles[i];
				child2.alleles[i] = Population[p2].alleles[i];
			}
		}

		break;

	case 2: //Each

		for (int i = 0; i < mAlleleSize - 1; i += 2)
		{
			child.alleles[i] = Population[p1].alleles[i];
			child.alleles[i + 1] = Population[p2].alleles[i + 1];
			
			child2.alleles[i] = Population[p2].alleles[i];
			child2.alleles[i + 1] = Population[p1].alleles[i + 1];
		}
		child.alleles[mAlleleSize - 1] = Population[p2].alleles[mAlleleSize - 1];
		child2.alleles[mAlleleSize - 1] = Population[p1].alleles[mAlleleSize - 1];
		break;

	case 3: //Line

		linerecomb = true;
		alf = GetRand(-0.25, 1.25);
		alf2 = GetRand(-0.25, 1.25);

	case 4: //Intermediate

		for (int i = 0; i < mAlleleSize; i++)
		{
			if (!linerecomb)
			{

				alf = GetRand(-0.25, 1.25);
				alf2 = GetRand(-0.25, 1.25);

			}

			child.alleles[i] = Population[p1].alleles[i] + alf*(Population[p2].alleles[i] - Population[p1].alleles[i]);
			child2.alleles[i] = Population[p1].alleles[i] + alf2*(Population[p2].alleles[i] - Population[p1].alleles[i]);

		}
		break;
	}
	
	for (int i = 0; i < mAlleleSize / 2; i++)
	{
		if (GetRand(0, 100) < GetMutatePercent())
			Mutate(child.alleles[GetRand(0, mAlleleSize - 1)]);

		if (GetRand(0, 100) < GetMutatePercent())
			Mutate(child2.alleles[GetRand(0, mAlleleSize - 1)]);
	}
	
	retChromosome.push_back(child);
	retChromosome.push_back(child2);

	return retChromosome;
}

int GA::RouletteSelection(double val)
{

	double last = 0;
	for (int i = 0; i < GetMaxPopulation(); i++)
	{
		if (last <= val && val <= Population[i].chance)
			return i;
		else
			last = Population[i].chance;
	}

	return GetRand(0, GetMaxPopulation() - 1);
}

int GA::TournamentSelection(int p1, int p2)
{
	if (Population[p1].fitness < Population[p2].fitness)
		return p1;
	else
		return p2;
}

void GA::CreateNewPopulation()
{

	vector<Chromosome> newpopulation;

	int TypeSelection = GetRand(0, 100);
	
	GenerateLikelihoods();

	SYSTEM_INFO siSysInfo;
	GetSystemInfo(&siSysInfo);

	int cpuCount = siSysInfo.dwNumberOfProcessors;
	int curstep = MaxPopulation / cpuCount;

	int left = 0;
	int right = 0;
	CrossingPopulation.clear();
	vector<thread> pool;

	for (int i = 0; i < cpuCount; i++)
	{
		left = right;
		right = left + curstep;
		if (i + 1 == 4)
			right += MaxPopulation - right;
		pool.push_back(thread(&GA::ThreadCrossing, this, left, right, TypeSelection));
	}

	for (auto &t : pool)
		t.join();

	OldPopulation = Population;
	Population = CrossingPopulation;
}

void GA::SelectNewPopulation()
{
	vector<Chromosome> temp;
	int mTruncationStep = GetMaxPopulation()*GetTruncationStep() / 100;
	int mElitePercent = GetMaxPopulation()*GetElitePercent() / 100;

	Population.insert(Population.end(), OldPopulation.begin(), OldPopulation.end());
	sort(Population.begin(), Population.end());

	int SelectType = GetRand(1, 2);

	switch (SelectType)
	{
	case 0: //All Ellite

		Population.resize(GetMaxPopulation());
		break;

	case 1:	//Truncation

		Population.resize(mTruncationStep);

		for (int i = 0; i < GetMaxPopulation(); i++)
		{
			temp.push_back(Population[GetRand(0, mTruncationStep - 1)]);
		}
		Population = temp;
		break;

	case 2:	//Elite

		temp.insert(temp.begin(), Population.begin(), Population.begin() + mElitePercent);

		for (int i = 0; i < GetMaxPopulation() - mElitePercent; i++)
		{
			temp.push_back(Population[GetRand(0, Population.size() - 1)]);
		}

		Population = temp;
		break;

		Population = temp;
		break;
	}


}

int GA::Run(NeuronNet* pNN)
{
	int ret;
	bool find = false;
	GenerateStartPopulation();

	printf("Find for %d Result Percent\n", GetResultPercent());
	fprintf(GL_STAT,"Find for %d Result Percent\n", GetResultPercent());
	while (GetEpoch() < GetMaxIteration())
	{

		ret = Fitness(pNN);		
		WriteConfiguration(GetFileNameOutputWeights().c_str(), pNN, &BestPopulation);
		if (ret != -1)
			return ret;
		if (BestPopulation.percent >= GetResultPercent())
		{
			find = true;
			break;
		}

		SelectNewPopulation();

		CreateNewPopulation();

		SetEpoch(GetEpoch() + 1);

		printf("Epoch %d\n", GetEpoch());
		printf("Fitness: %f", BestPopulation.fitness);
		printf("\tPercent %f", BestPopulation.percent);
		printf("\tTrue %d from %d\n", BestPopulation.countTrue, g_pInputs->size());

		fprintf(GL_STAT, "Epoch %d\n", GetEpoch());
		fprintf(GL_STAT, "Fitness: %f", BestPopulation.fitness);
		fprintf(GL_STAT, "\tPercent %f", BestPopulation.percent);
		fprintf(GL_STAT, "\tTrue %d from %d\n", BestPopulation.countTrue, g_pInputs->size());

		Population.push_back(BestPopulation);
		Population.push_back(BestPopulation);
		Population.push_back(BestPopulation);
		Population.push_back(BestPopulation);
		Population.push_back(BestPopulation);
		Population.push_back(BestPopulation);

	}
	ret = Fitness(pNN);
	Population.push_back(BestPopulation);
	sort(Population.begin(), Population.end());
	if (find)
		return 0;

	return -1;
}

void GA::SetCrossTPercent(int OnePointPercent, int TwoPointPercent, int EachPercent, int LinePercent, int InterMPercent)
{
	this->CrossObject.Normalized(OnePointPercent, TwoPointPercent, EachPercent, LinePercent, InterMPercent);
}

Chromosome GA::GetChromosome(int val)
{
	return Population[val];
}