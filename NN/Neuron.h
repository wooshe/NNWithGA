//Класс описывающий нейрон ИНС
class Neuron
{
public:

	Neuron();
	Neuron(vector<Input*>* pInputs, vector<double>* pWeights);
	~Neuron(){};

	//Функция рассчета выходного значения нейрона
	void ProcessNeuron();

	//Функция установки весов синапсов
	void SetWeights(vector<double>* pWeights);

	//Выход
	Output Output;
	vector<Synapse> InputSynapses;
private:

	//Массив входных синапсов (связей)
	
	
	//Индуцированное локальное поле
	//или потенциал активации
	double ActivationPotential;
	//Выходное значение
	double OutputValue;

	//Функция сумматор входных синапсов
	void Adder();
	//Функция активации
	void ActivationFunction();

	//TODO:
	//- Порог или фиксированный входной сигнал
	//- Функции для настройки весов синапсов
};