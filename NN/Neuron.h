//����� ����������� ������ ���
class Neuron
{
public:

	Neuron();
	Neuron(vector<Input*>* pInputs, vector<double>* pWeights);
	~Neuron(){};

	//������� �������� ��������� �������� �������
	void ProcessNeuron();

	//������� ��������� ����� ��������
	void SetWeights(vector<double>* pWeights);

	//�����
	Output Output;
	vector<Synapse> InputSynapses;
private:

	//������ ������� �������� (������)
	
	
	//�������������� ��������� ����
	//��� ��������� ���������
	double ActivationPotential;
	//�������� ��������
	double OutputValue;

	//������� �������� ������� ��������
	void Adder();
	//������� ���������
	void ActivationFunction();

	//TODO:
	//- ����� ��� ������������� ������� ������
	//- ������� ��� ��������� ����� ��������
};