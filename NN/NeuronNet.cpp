//���������� ������� ������ NeuronNet
#include "../Headers.h"

//�����������
NeuronNet::NeuronNet(vector<Input*>* pInputs, vector<vector<vector<double>>>* pWeightsLayers)
{
	//������������� ������� ������
	this->Inputs = *pInputs;
	vector<vector<vector<double>>> WeightsLayer = *pWeightsLayers;

	//������������� ������� ������� ������ ��������
	//this->Neurons = vector<Neuron>();

	vector<Input*> FirstLayerOutputs;
	vector<Input*> SecondLayerOutputs;
	vector<Input*> ThirdLayerOutputs;

	//��������� ������� ������� ����
	for (int i = 0; i < WeightsLayer[0].size(); i++)
	{
		Neuron CurrentNeuron = Neuron(&this->Inputs, &WeightsLayer[0][i]);
		this->FirstNeuronsLayer.push_back(CurrentNeuron);
	}

	//��������� ������ ������� ������� ����
	for (int i = 0; i < this->FirstNeuronsLayer.size(); i++)
	{
		FirstLayerOutputs.push_back(&(this->FirstNeuronsLayer[i].Output));
	}

	//��������� ������� ������� ����
	for (int i = 0; i < WeightsLayer[1].size(); i++)
	{
		Neuron CurrentNeuron = Neuron(&FirstLayerOutputs, &WeightsLayer[1][i]);
		this->SecondNeuronsLayer.push_back(CurrentNeuron);
	}

	//��������� ������ ������� ������� ����
	for (int i = 0; i < this->SecondNeuronsLayer.size(); i++)
	{
		SecondLayerOutputs.push_back(&(this->SecondNeuronsLayer[i].Output));
	}

	//��������� ������� �������� ����
	for (int i = 0; i < WeightsLayer[2].size(); i++)
	{
		Neuron CurrentNeuron = Neuron(&SecondLayerOutputs, &WeightsLayer[2][i]);
		this->ThirdNeuronsLayer.push_back(CurrentNeuron);
	}

	//��������� ������ ������� �������� ����
	for (int i = 0; i < this->ThirdNeuronsLayer.size(); i++)
	{
		ThirdLayerOutputs.push_back(&(this->ThirdNeuronsLayer[i].Output));
	}

	//�������������� �������� ������� � ���������������� �������
	for (int i = 0; i < WeightsLayer[3].size(); i++)
	{
		Neuron CurrentNeuron = Neuron(&ThirdLayerOutputs, &WeightsLayer[3][i]);
		this->OutputNeurons.push_back(CurrentNeuron);
	}
}

void NeuronNet::ProcessNet()
{
	//������ ����
	for (int i = 0; i < this->FirstNeuronsLayer.size(); i++)
	{
		this->FirstNeuronsLayer[i].ProcessNeuron();
	}

	//������ ����
	for (int i = 0; i < this->SecondNeuronsLayer.size(); i++)
	{
		this->SecondNeuronsLayer[i].ProcessNeuron();
	}

	//������ ����
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

	//������� ������� ����
	for (int i = 0; i < WeightsLayer[0].size(); i++)
	{
		this->FirstNeuronsLayer[i].SetWeights(&WeightsLayer[0][i]);
	}

	//������� ������� ����
	for (int i = 0; i < WeightsLayer[1].size(); i++)
	{
		this->SecondNeuronsLayer[i].SetWeights(&WeightsLayer[1][i]);
	}

	//������� �������� ����
	for (int i = 0; i < WeightsLayer[2].size(); i++)
	{
		this->ThirdNeuronsLayer[i].SetWeights(&WeightsLayer[2][i]);
	}

	//�������� �������
	for (int i = 0; i < WeightsLayer[3].size(); i++)
	{
		this->OutputNeurons[i].SetWeights(&WeightsLayer[3][i]);
	}
}

void NeuronNet::SetInputs(vector<Input*>* pInputs)
{
	//��� ��� � �������� �������� ��������� �� ����,
	//�� ���������� �������� ������� �������� � ������� ������
	for (int i = 0; i < this->Inputs.size(); i++)
	{
		this->Inputs[i]->Value = (*pInputs)[i]->Value;
	}
}