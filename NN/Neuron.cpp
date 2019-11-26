//Реализация функций класса Neuron
#include "../Headers.h"

Neuron::Neuron()
{
	//TODO: 
	//Сделать возможность для разных нейронов 
	//делать разное количество синапсов
	this->OutputValue = 0;
	this->ActivationPotential = 0;
}

//Конструктор
Neuron::Neuron(vector<Input*>* pInputs, vector<double>* pWeights)
{
	this->InputSynapses = vector<Synapse>(pInputs->size());

	//Сохраняем соответствующие указатели на входы 
	//для каждого синапса и устанавливаем веса
	for (int i = 0; i < pInputs->size(); i++)
	{
		this->InputSynapses[i].pInput = (*pInputs)[i];
		this->InputSynapses[i].Weight = (*pWeights)[i];
	}

	this->Output.Value = 0;
	this->OutputValue = 0;
	this->ActivationPotential = 0;
}

//Сумматор. Складывает входные сигналы, 
//взвешенные относительно соответствующих синапсов нейрона
void Neuron::Adder()
{
	//На всякий случай обнулим значение активационного потенциала
	this->ActivationPotential = 0;

	//Суммируем значение всех входных сигналов,
	//умноженных на соответствующие веса синапса
	for (int i = 0; i < this->InputSynapses.size(); i++)
	{
		this->ActivationPotential += 
			this->InputSynapses[i].Weight * this->InputSynapses[i].pInput->Value;
	}
}

//Функция активации. Ограничивает амплитуду выходного сигнала нейрона.
//Будем использовать сигмоидальную функцию.
void Neuron::ActivationFunction()
{
	//Гиперболический тангенс подойдет.
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
	//Сохраняем соответствующие веса для каждого синапса
	for (int i = 0; i < pWeights->size(); i++)
	{
		this->InputSynapses[i].Weight = (*pWeights)[i];
	}
}