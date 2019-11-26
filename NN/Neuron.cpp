//���������� ������� ������ Neuron
#include "../Headers.h"

Neuron::Neuron()
{
	//TODO: 
	//������� ����������� ��� ������ �������� 
	//������ ������ ���������� ��������
	this->OutputValue = 0;
	this->ActivationPotential = 0;
}

//�����������
Neuron::Neuron(vector<Input*>* pInputs, vector<double>* pWeights)
{
	this->InputSynapses = vector<Synapse>(pInputs->size());

	//��������� ��������������� ��������� �� ����� 
	//��� ������� ������� � ������������� ����
	for (int i = 0; i < pInputs->size(); i++)
	{
		this->InputSynapses[i].pInput = (*pInputs)[i];
		this->InputSynapses[i].Weight = (*pWeights)[i];
	}

	this->Output.Value = 0;
	this->OutputValue = 0;
	this->ActivationPotential = 0;
}

//��������. ���������� ������� �������, 
//���������� ������������ ��������������� �������� �������
void Neuron::Adder()
{
	//�� ������ ������ ������� �������� �������������� ����������
	this->ActivationPotential = 0;

	//��������� �������� ���� ������� ��������,
	//���������� �� ��������������� ���� �������
	for (int i = 0; i < this->InputSynapses.size(); i++)
	{
		this->ActivationPotential += 
			this->InputSynapses[i].Weight * this->InputSynapses[i].pInput->Value;
	}
}

//������� ���������. ������������ ��������� ��������� ������� �������.
//����� ������������ ������������� �������.
void Neuron::ActivationFunction()
{
	//��������������� ������� ��������.
	this->OutputValue = tanh(this->ActivationPotential);
	this->Output.Value = this->OutputValue;
}

void Neuron::ProcessNeuron()
{
	this->Adder();
	this->ActivationFunction();
}

void Neuron::SetWeights(vector<double>* pWeights)
{
	//��������� ��������������� ���� ��� ������� �������
	for (int i = 0; i < pWeights->size(); i++)
	{
		this->InputSynapses[i].Weight = (*pWeights)[i];
	}
}