#include "../Headers.h"

GA::CrossT::CrossT()
{
	Normalized(20, 20, 20, 20, 20);
}

GA::CrossT::CrossT(int OnePointPercent, int TwoPointPercent, int EachPercent, int LinePercent, int InterMPercent)
{
	Normalized(OnePointPercent, TwoPointPercent, EachPercent, LinePercent, InterMPercent);
}
GA::CrossT::~CrossT()
{

}

void GA::CrossT::Normalized(int OnePointPercent, int TwoPointPercent, int EachPercent, int LinePercent, int InterMPercent)
{
	double sum = 0;
	sum += 1 / ((double)OnePointPercent);
	sum += 1 / ((double)TwoPointPercent);
	sum += 1 / ((double)EachPercent);
	sum += 1 / ((double)LinePercent);
	sum += 1 / ((double)InterMPercent);

	double last = 0;
	OnePointPercent = last = last + (double)OnePointPercent;
	TwoPointPercent = last = last + (double)TwoPointPercent;
	EachPercent = last = last + (double)EachPercent;
	LinePercent = last = last + (double)LinePercent;
	InterMPercent = last = last + (double)InterMPercent;

	this->SetOnePointPercent(OnePointPercent);
	this->SetTwoPointPercent(TwoPointPercent);
	this->SetEachPercent(EachPercent);
	this->SetLinePercent(LinePercent);
	this->SetInterMPercent(InterMPercent);
}