//Класс описывающий ИНС
class NeuronNet
{
public:
	NeuronNet(vector<Input*>* pInputs, vector<vector<vector<double>>>* pWeightsLayers);
	~NeuronNet() {};

	//Массив входов ИНС
	vector<Input*> Inputs;

	//Первый слой нейронов
	vector<Neuron> FirstNeuronsLayer;

	//Второй слой нейронов
	vector<Neuron> SecondNeuronsLayer;

	//Третий слой нейронов
	vector<Neuron> ThirdNeuronsLayer;

	//Выходной слой нейронов
	vector<Neuron> OutputNeurons;

	//Функция начала работа ИНС
	void ProcessNet();

	//Функция установки новых весов для ИНС
	void SetWeights(vector<vector<vector<double>>>* pWeightsLayers);

	//Функция установки новых значений входов для ИНС
	void SetInputs(vector<Input*>* pInputs);
};