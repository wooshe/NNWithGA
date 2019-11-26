//����� ����������� ���
class NeuronNet
{
public:
	NeuronNet(vector<Input*>* pInputs, vector<vector<vector<double>>>* pWeightsLayers);
	~NeuronNet() {};

	//������ ������ ���
	vector<Input*> Inputs;

	//������ ���� ��������
	vector<Neuron> FirstNeuronsLayer;

	//������ ���� ��������
	vector<Neuron> SecondNeuronsLayer;

	//������ ���� ��������
	vector<Neuron> ThirdNeuronsLayer;

	//�������� ���� ��������
	vector<Neuron> OutputNeurons;

	//������� ������ ������ ���
	void ProcessNet();

	//������� ��������� ����� ����� ��� ���
	void SetWeights(vector<vector<vector<double>>>* pWeightsLayers);

	//������� ��������� ����� �������� ������ ��� ���
	void SetInputs(vector<Input*>* pInputs);
};