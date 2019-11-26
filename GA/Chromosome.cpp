#include "../Headers.h"


Chromosome::Chromosome()
{
	this->fitness = 8888;
	this->percent = 0;
	this->countTrue = 0;
}

Chromosome::Chromosome(int size)
{
	this->alleles.resize(size);
	this->fitness = 8888;
	this->percent = 0;
	this->countTrue = 0;
}

Chromosome::~Chromosome()
{

}

bool Chromosome::operator < (Chromosome &obj)
{
	if (this->fitness < obj.fitness)
		return true;
	return false;
}

bool Chromosome::operator == (Chromosome &obj)
{
	for (int i = 0; i < alleles.size(); i++)
	{
		if (obj.alleles[i] != this->alleles[i])
			return false;
	}

	return true;
}
