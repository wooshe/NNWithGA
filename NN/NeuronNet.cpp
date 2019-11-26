//Реализация функций класса NeuronNet
#include "../Headers.h"

//Конструктор
NeuronNet::NeuronNet(vector<Input*>* pInputs, vector<vector<vector<double>>>* pWeightsLayers)
{
	//Инициализация вектора входов
	this->Inputs = *pInputs;
	vector<vector<vector<double>>> WeightsLayer = *pWeightsLayers;

	//Инициализация вектора первого уровня нейронов
	//this->Neurons = vector<Neuron>();

	vector<Input*> FirstLayerOutputs;
	vector<Input*> SecondLayerOutputs;
	vector<Input*> ThirdLayerOutputs;

	//Добавляем нейроны первого слоя
	for (int i = 0; i < WeightsLayer[0].size(); i++)
	{
		Neuron CurrentNeuron = Neuron(&this->Inputs, &WeightsLayer[0][i]);
		this->FirstNeuronsLayer.push_back(CurrentNeuron);
	}

	//Заполняем вектор выходов первого слоя
	for (int i = 0; i < this->FirstNeuronsLayer.size(); i++)
	{
		FirstLayerOutputs.push_back(&(this->FirstNeuronsLayer[i].Output));
	}

	//Добавляем нейроны второго слоя
	for (int i = 0; i < WeightsLayer[1].size(); i++)
	{
		Neuron CurrentNeuron = Neuron(&FirstLayerOutputs, &WeightsLayer[1][i]);
		this->SecondNeuronsLayer.push_back(CurrentNeuron);
	}

	//Заполняем вектор выходов второго слоя
	for (int i = 0; i < this->SecondNeuronsLayer.size(); i++)
	{
		SecondLayerOutputs.push_back(&(this->SecondNeuronsLayer[i].Output));
	}

	//Добавляем нейроны третьего слоя
	for (int i = 0; i < WeightsLayer[2].size(); i++)
	{
		Neuron CurrentNeuron = Neuron(&SecondLayerOutputs, &WeightsLayer[2][i]);
		this->ThirdNeuronsLayer.push_back(CurrentNeuron);
	}

	//Заполняем вектор выходов третьего слоя
	for (int i = 0; i < this->ThirdNeuronsLayer.size(); i++)
	{
		ThirdLayerOutputs.push_back(&(this->ThirdNeuronsLayer[i].Output));
	}

	//Инициализируем выходные нейроны с соответствующими связями
	for (int i = 0; i < WeightsLayer[3].size(); i++)
	{
		Neuron CurrentNeuron = Neuron(&ThirdLayerOutputs, &WeightsLayer[3][i]);
		this->OutputNeurons.push_back(CurrentNeuron);
	}
}

void NeuronNet::ProcessNet()
{
	//Первый слой
	for (int i = 0; i < this->FirstNeuronsLayer.size(); i++)
	{
		this->FirstNeuronsLayer[i].ProcessNeuron();
	}

	//Второй слой
	for (int i = 0; i < this->SecondNeuronsLayer.size(); i++)
	{
		this->SecondNeuronsLayer[i].ProcessNeuron();
	}

	//Третий слой
	for (int i = 0; i < this->ThirdNeuronsLayer.size(); i++)
	{
		this->ThirdNeuronsLayer[i].ProcessNeuron();
	}

	for (int i = 0; i < this->OutputNeurons.size(); i++)
	{
		this->OutputNeurons[i].ProcessNeuron();
	}
}

void NeuronNet::SetWeights(vector<vector<vector<double>>>* pWeightsLayers)
{
	vector<vector<vector<double>>> WeightsLayer = *pWeightsLayers;

	//Нейроны первого слоя
	for (int i = 0; i < WeightsLayer[0].size(); i++)
	{
		this->FirstNeuronsLayer[i].SetWeights(&WeightsLayer[0][i]);
	}

	//Нейроны второго слоя
	for (int i = 0; i < WeightsLayer[1].size(); i++)
	{
		this->SecondNeuronsLayer[i].SetWeights(&WeightsLayer[1][i]);
	}

	//Нейроны третьего слоя
	for (int i = 0; i < WeightsLayer[2].size(); i++)
	{
		this->ThirdNeuronsLayer[i].SetWeights(&WeightsLayer[2][i]);
	}

	//Выходные нейроны
	for (int i = 0; i < WeightsLayer[3].size(); i++)
	{
		this->OutputNeurons[i].SetWeights(&WeightsLayer[3][i]);
	}
}

void NeuronNet::SetInputs(vector<Input*>* pInputs)
{
	//Так как в нейронах хранится указатель на вход,
	//то достаточно поменять текущие значения в векторе входов
	for (int i = 0; i < this->Inputs.size(); i++)
	{
		this->Inputs[i]->Value = (*pInputs)[i]->Value;
	}
}