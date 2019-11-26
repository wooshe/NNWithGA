
class Chromosome
{
public:
	Chromosome();

	Chromosome(int size);

	~Chromosome();

	bool operator < (Chromosome &objr);
	bool operator == (Chromosome &objr);

	vector<double> alleles;
	double fitness;
	double chance;
	double percent;
	int countTrue;
};

