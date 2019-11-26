#pragma once
class GA
{
public:

	class CrossT
	{
	public:
		CrossT();
		CrossT(int OnePointPercent, int TwoPointPercent, int EachPercent, int LinePercent, int InterMPercent);
		~CrossT();

		void Normalized(int OnePointPercent, int TwoPointPercent, int EachPercent, int LinePercent, int InterMPercent);

		int GetOnePointPercent() const { return OnePointPercent; }
		void SetOnePointPercent(int val) { OnePointPercent = val; }

		int GetTwoPointPercent() const { return TwoPointPercent; }
		void SetTwoPointPercent(int val) { TwoPointPercent = val; }

		int GetEachPercent() const { return EachPercent; }
		void SetEachPercent(int val) { EachPercent = val; }

		int GetLinePercent() const { return LinePercent; }
		void SetLinePercent(int val) { LinePercent = val; }

		int GetInterMPercent() const { return InterMPercent; }
		void SetInterMPercent(int val) { InterMPercent = val; }



	private:
		int OnePointPercent;
		int TwoPointPercent;
		int EachPercent;
		int LinePercent;
		int InterMPercent;
	};

	GA();
	
	~GA();

	GA(int MaxIteration, int MaxPopulation, int AllelesSize, int ResultPercent, int MutatePercent, int MutateStep, int MutatePercentNewGenom, CrossT object, int MaxValue,int TruncationStep, int ElitePercent, int RTTypeSelectionPercent);
	
	//������ ����������� ����� �����������
	void SetCrossTPercent(int OnePointPercent, int TwoPointPercent, int EachPercent, int LinePercent, int InterMPercent);

	//��������� �������� int
	int GetRand();

	//��������� �������� int � ���������
	int GetRand(int from, int to);

	//��������� �������� double
	double DGetRand();

	//��������� �������� double � ���������
	double GetRand(double from, double to);

	//��������� �����������
	void ThreadCrossing(int l, int r, int TypeSelection);

	//��������� ��������� ���������
	void GenerateStartPopulation();

	//���������� �������-������� ���������
	int Fitness(NeuronNet* pNN);

	//�������-�������
	double FitnessForOne(NeuronNet* pNN, Chromosome &�hromosome);

	//�������� ����� ���������
	void CreateNewPopulation();
	
	//����� �������� ����-�
	double MultInv();

	//���������� �����������
	void GenerateLikelihoods();

	//����� ��������� ������� �������
	int RouletteSelection(double val);

	//����� ��������� ������� �������
	int GA::TournamentSelection(int p1, int p2);

	//����� ���������
	void SelectNewPopulation();

	//�����������
	vector<Chromosome> Crossing(int p1, int p2);

	//�������
	void Mutate(double &val);
	
	//������ ���������
	int Run(NeuronNet* pNN);

	//�������� ���������
	Chromosome GetChromosome(int val);


vector<Chromosome> Population;
vector<Chromosome> OldPopulation;
vector<Chromosome> CrossingPopulation;
Chromosome BestPopulation;
CrossT CrossObject;

int GetMaxPopulation() const { return MaxPopulation; }
void SetMaxPopulation(int val) { MaxPopulation = val; }

int GetMaxIteration() const { return MaxIteration; }
void SetMaxIteration(int val) { MaxIteration = val; }

int GetAllelesSize() const { return AllelesSize; }
void SetAllelesSize(int val) { AllelesSize = val; }

int GetResultPercent() const { return ResultPercent; }
void SetResultPercent(int val) { ResultPercent = val; }

int GetEpoch() const { return Epoch; }
void SetEpoch(int val) { Epoch = val; }

int GetMutatePercent() const { return MutatePercent; }
void SetMutatePercent(int val) { MutatePercent = val; }

int GetMutateStep() const { return MutateStep; }
void SetMutateStep(int val) { MutateStep = val; }

int GetMutatePercentNewGenom() const { return MutatePercentNewGenom; }
void SetMutatePercentNewGenom(int val) { MutatePercentNewGenom = val; }

int GetMaxValue() const { return MaxValue; }
void SetMaxValue(int val) { MaxValue = val; }

int GetTruncationStep() const { return TruncationStep; }
void SetTruncationStep(int val) { TruncationStep = val; }

int GetElitePercent() const { return ElitePercent; }
void SetElitePercent(int val) { ElitePercent = val; }

std::string GetFileNameOutputWeights() const { return FileNameOutputWeights; }
void SetFileNameOutputWeights(std::string val) { FileNameOutputWeights = val; }

int GetRTTypeSelectionPercent() const { return RTTypeSelectionPercent; }
void SetRTTypeSelectionPercent(int val) { RTTypeSelectionPercent = val; }

private:
	string FileNameOutputWeights;
	int ElitePercent;
	int TruncationStep;
	int MaxValue;
	int MaxPopulation;
	int MaxIteration;
	int AllelesSize;
	int ResultPercent;
	int Epoch;
	int MutatePercent;
	int MutateStep;
	int MutatePercentNewGenom;
	int RTTypeSelectionPercent;
};

