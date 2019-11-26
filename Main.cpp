#include "Headers.h"

vector<vector<Input*>*>* g_pInputs;
vector <Input*>* g_pExpected;
vector <NeuronNet*>* g_pNNs;
FILE* GL_STAT;

//Функция чтения входных сигналов из файла
vector<Input*>* ReadInputs(const char* InputFileName)
{
	//Открываем файл
	FILE* InputFile;
	InputFile = fopen(InputFileName, "r");

	//Читаем количество входов
	int InputsNumber;
	fscanf_s(InputFile, "%d", &InputsNumber);

	vector<Input*>* pInputs = new vector<Input*>(InputsNumber);

	//Создаем вектор входов
	for (int i = 0; i < pInputs->size(); i++)
	{
		(*pInputs)[i] = new Input;
		double Value;
		fscanf_s(InputFile, "%lf", &Value);
		(*pInputs)[i]->Value = Value;
	}

	fclose(InputFile);

	return pInputs;
}

//Функция чтения весов синапсов из конфигурационного файла
vector<vector<vector<double>>>* ReadConfiguration(const char* ConfigurationFileName)
{
	//Открываем файл
	FILE* InputFile;
	InputFile = fopen(ConfigurationFileName, "r");
	if (InputFile == NULL)
		return NULL;

	//Читаем количество входов
	int InputsNumber;
	fscanf_s(InputFile, "%d", &InputsNumber);

	//Читаем количество нейронов первого уровня
	int FirstNeuronsLayerNumber=0;
	fscanf_s(InputFile, "%d", &FirstNeuronsLayerNumber);

	//Читаем количество нейронов второго уровня
	int SecondNeuronsLayerNumber=0;
	fscanf_s(InputFile, "%d", &SecondNeuronsLayerNumber);

	//Читаем количество нейронов третьего уровня
	int ThirdNeuronsLayerNumber=0;
	fscanf_s(InputFile, "%d", &ThirdNeuronsLayerNumber);

	//Читаем количество выходных нейронов
	int OutputNeuronsNumber;
	fscanf_s(InputFile, "%d", &OutputNeuronsNumber);

	vector<vector<vector<double>>>* pWeightsLayers = new vector<vector<vector<double>>>();

	//Создаем вектор весов синапсов первого слоя нейронов
	vector<vector<double>> Weights = vector<vector<double>>();
	for (int i = 0; i < FirstNeuronsLayerNumber; i++)
	{
		Weights.push_back(vector<double>(InputsNumber));
		for (int j = 0; j < InputsNumber; j++)
		{
			double Weight;
			fscanf_s(InputFile, "%lf", &Weight);
			Weights[i][j] = Weight;
		}
	}
	pWeightsLayers->push_back(Weights);

	//Создаем вектор весов синапсов второго слоя нейронов
	Weights = vector<vector<double>>();
	for (int i = 0; i < SecondNeuronsLayerNumber; i++)
	{
		Weights.push_back(vector<double>(FirstNeuronsLayerNumber));
		for (int j = 0; j < FirstNeuronsLayerNumber; j++)
		{
			double Weight;
			fscanf_s(InputFile, "%lf", &Weight);
			Weights[i][j] = Weight;
		}
	}
	pWeightsLayers->push_back(Weights);

	//Создаем вектор весов синапсов третьего слоя нейронов
	Weights = vector<vector<double>>();
	for (int i = 0; i < ThirdNeuronsLayerNumber; i++)
	{
		Weights.push_back(vector<double>(SecondNeuronsLayerNumber));
		for (int j = 0; j < SecondNeuronsLayerNumber; j++)
		{
			double Weight;
			fscanf_s(InputFile, "%lf", &Weight);
			Weights[i][j] = Weight;
		}
	}
	pWeightsLayers->push_back(Weights);

	//Создаем вектор весов синапсов выходного нейрона
	Weights = vector<vector<double>>();
	for (int i = 0; i < OutputNeuronsNumber; i++)
	{
		Weights.push_back(vector<double>(ThirdNeuronsLayerNumber));
		for (int j = 0; j < ThirdNeuronsLayerNumber; j++)
		{
			double Weight;
			fscanf_s(InputFile, "%lf", &Weight);
			Weights[i][j] = Weight;
		}
	}
	pWeightsLayers->push_back(Weights);

	return pWeightsLayers;
}

//Функция записи весов синапсов
void WriteConfiguration(const char* ConfigurationFileName, NeuronNet* pNN, Chromosome* best)
{
	FILE* pFile;
	pFile = fopen(ConfigurationFileName, "w");

	int FirstNeuronsLayerNumber = pNN->FirstNeuronsLayer.size();
	int SecondNeuronsLayerNumber = pNN->SecondNeuronsLayer.size();
	int ThirdNeuronsLayerNumber = pNN->ThirdNeuronsLayer.size();
	int InputsNumber = pNN->Inputs.size();
	int OutputNeuronsNumber = pNN->OutputNeurons.size();

	fprintf(pFile, "%d %d %d %d %d\n", InputsNumber, FirstNeuronsLayerNumber, SecondNeuronsLayerNumber, ThirdNeuronsLayerNumber, OutputNeuronsNumber);

	int point = 0;

	for (int i = 0; i < FirstNeuronsLayerNumber; i++)
	{
		for (int j = 0; j < InputsNumber; j++)
		{
			fprintf(pFile, "%lf ", best->alleles[point]);
			point++;
		}
		fprintf(pFile, "\n");
	}

	fprintf(pFile, "\n");

	for (int i = 0; i < SecondNeuronsLayerNumber; i++)
	{
		for (int j = 0; j < FirstNeuronsLayerNumber; j++)
		{
			fprintf(pFile, "%lf ", best->alleles[point]);
			point++;
		}
		fprintf(pFile, "\n");
	}

	fprintf(pFile, "\n");

	for (int i = 0; i < ThirdNeuronsLayerNumber; i++)
	{
		for (int j = 0; j < SecondNeuronsLayerNumber; j++)
		{
			fprintf(pFile, "%lf ", best->alleles[point]);
			point++;
		}
		fprintf(pFile, "\n");
	}

	fprintf(pFile, "\n");

	for (int i = 0; i < OutputNeuronsNumber; i++)
	{
		for (int j = 0; j < ThirdNeuronsLayerNumber; j++)
		{
			fprintf(pFile, "%lf ", best->alleles[point]);
			point++;
		}
		fprintf(pFile, "\n");
	}

	fclose(pFile);
}

//Функция запуска сети для входного вектора
void ProcessNetForInputVector(NeuronNet* pNN, vector <Input*>* pInputs)
{
	pNN->SetInputs(pInputs);

	pNN->ProcessNet();
}

//Функция запуска сети для всех векторов входов
vector<vector<double>>* ProcessNetForAllInputVectors(NeuronNet* pNN)
{
	vector<vector<double>>* pResults = new vector<vector<double>>();

	for (int i = 0; i < g_pInputs->size(); i++)
	{
		ProcessNetForInputVector(pNN, (*g_pInputs)[i]);

		vector<double> Result = vector<double>(5);
		Result[0] = pNN->OutputNeurons[0].Output.Value;
		Result[1] = pNN->OutputNeurons[1].Output.Value;
		Result[2] = pNN->OutputNeurons[2].Output.Value;
		Result[3] = pNN->OutputNeurons[3].Output.Value;
		Result[4] = pNN->OutputNeurons[4].Output.Value;
		pResults->push_back(Result);
	}

	return pResults;
}

//Функция чтения входных данных в память
vector<vector<Input*>*>* ReadAllInputFilesToMemory()
{
	vector<vector<Input*>*>* pInputs = new vector<vector<Input*>*>();

	for (int i = 1; i <= 285; i++)
	{
		char intStr[5] = { 0 };
		_itoa(i, intStr, 10);
		string fileName = intStr;
		fileName = "Files\\LONGDISTRIBUTED\\" + fileName + "_distr.txt";
		pInputs->push_back(ReadInputs(fileName.c_str()));
	}

	for (int i = 1; i <= 270; i++)
	{
		char intStr[5] = { 0 };
		_itoa(i, intStr, 10);
		string fileName = intStr;
		fileName = "Files\\Distr\\" + fileName + "_2011_distr.txt";
		pInputs->push_back(ReadInputs(fileName.c_str()));
	}

	return pInputs;
}

//Функция чтения результатов
vector<Input*>* ReadExpectedFileToMemory()
{
	//Открываем файл
	FILE* InputFile;
	InputFile = fopen("Files\\DLExpectedOutputs.txt", "r");

	vector<Input*>* pInputs = new vector<Input*>(ALL_FILES);

	//Создаем вектор входов
	for (int i = 0; i < 285; i++)
	{
		(*pInputs)[i] = new Input;
		double Value;
		fscanf_s(InputFile, "%lf", &Value);
		(*pInputs)[i]->Value = Value;
	}

	fclose(InputFile);

	InputFile = fopen("Files\\TExpectedOutputs.txt", "r");

	//Создаем вектор входов
	for (int i = 285; i < 555; i++)
	{
		(*pInputs)[i] = new Input;
		double Value;
		fscanf_s(InputFile, "%lf", &Value);
		(*pInputs)[i]->Value = Value;
	}

	fclose(InputFile);

	return pInputs;
}

string GetFilesNameFromConfiguration()
{
	char intStr[5] = { 0 };
	string fileName = intStr;

	_itoa((*g_pNNs)[0]->Inputs.size(), intStr, 10);
	fileName = fileName + "_" + intStr;
	_itoa((*g_pNNs)[0]->FirstNeuronsLayer.size(), intStr, 10);
	fileName = fileName + "_" + intStr;
	_itoa((*g_pNNs)[0]->SecondNeuronsLayer.size(), intStr, 10);
	fileName = fileName + "_" + intStr;
	_itoa((*g_pNNs)[0]->ThirdNeuronsLayer.size(), intStr, 10);
	fileName = fileName + "_" + intStr;
	_itoa((*g_pNNs)[0]->OutputNeurons.size(), intStr, 10);
	fileName = fileName + "_" + intStr;
	return fileName;
}

string GetFilesNameForTestStat(int iteration)
{
	char intStr[5] = { 0 };
	_itoa(iteration, intStr, 10);
	string buf = intStr;
	string fileName = "Files\\Stat\\";

	fileName += buf;
	fileName += GetFilesNameFromConfiguration();
	fileName += "_TestStat.txt";
	return fileName;
}

string GetFilesNameForOutputWeights(int iteration)
{
	char intStr[5] = { 0 };
	_itoa(iteration, intStr, 10);
	string buf = intStr;
	string fileName = "Files\\Stat\\";

	fileName += buf;
	fileName += GetFilesNameFromConfiguration();
	fileName += "_OutputWeights.txt";
	return fileName;
}

//Функция записи статистики
void WriteStat(vector<double> *trendInfo,int cur)
{
	FILE* pFile;
	pFile = fopen(GetFilesNameForTestStat(cur).c_str(), "a+");
	int result = 0;
	fprintf(pFile, "REAL\t");
	fprintf(pFile, "TEST\t");
	fprintf(pFile, "%d\t", 0);
	fprintf(pFile, "%d\n", 0);
	int priceR = 0;
	int priceV = 0;
	for (int i = 0; i < g_pExpected->size(); i++)
	{
		result = (*g_pExpected)[i]->Value;

		if (result == 1)
			result = 0;
		if (result == 3)
			result = 2;

		fprintf(pFile, "%d\t", result);
		if (result == 0)
			priceR--;
		if (result == 2)
			priceR++;

		result = (*trendInfo)[i + 1];

		fprintf(pFile, "%d\t", result);

		fprintf(pFile, "%d\t", priceR);

		if (result == 0)
			priceV--;
		if (result == 2)
			priceV++;
		fprintf(pFile, "%d\t", priceV);

		fprintf(pFile, "\n");
	}


	fclose(pFile);
}

//Функция получения результата
vector<double> GetTrend(vector<vector<double>>* pResults)
{
	double FastDown;
	double Down;
	double Up;
	double FastUp;
	double NoChange;
	double result;
	int  trend = -1;
	int  trend2 = -1;
	double fitness = 0;
	vector<double> retVector = vector<double>(pResults->size() + 1);

	for (int i = 0; i < pResults->size(); i++)
	{
		FastDown = (*pResults)[i][0];
		Down = (*pResults)[i][1];
		Up = (*pResults)[i][2];
		FastUp = (*pResults)[i][3];
		NoChange = (*pResults)[i][4];

		result = (*g_pExpected)[i]->Value;

		double trueValue = 0.4;
		double falseValue = 0.4;

		if ((FastDown > trueValue || Down > trueValue) && (Up < falseValue && FastUp < falseValue && NoChange < falseValue))
		{
			trend = 0;
			trend2 = 1;
		}
		else if ((FastUp > trueValue || Up > trueValue) && (FastDown < falseValue && Down < falseValue && NoChange < falseValue))
		{
			trend = 2;
			trend2 = 3;
		}

		if (trend == result || trend2 == result)
			fitness++;

		retVector[i + 1] = trend;
		trend = -1;
		trend2 = -1;

	}
	retVector[0] = fitness;
	return retVector;
}

//Функция настройки и запуска ГА
void StartLearning(NeuronNet* pNN, int MaxEpoch , int MaxPercent, string OutputWeightsFile)
{
	Chromosome chromosome;
	GA glb = GA();


	int AllAlleles = pNN->Inputs.size() * pNN->FirstNeuronsLayer.size()
		+ pNN->FirstNeuronsLayer.size() * pNN->SecondNeuronsLayer.size()
		+ pNN->SecondNeuronsLayer.size() * pNN->ThirdNeuronsLayer.size()
		+ pNN->ThirdNeuronsLayer.size() * pNN->OutputNeurons.size();
	

	glb.SetMaxIteration(MaxEpoch);
	glb.SetMaxPopulation(50);
	glb.SetAllelesSize(AllAlleles);
	glb.SetMutatePercent(1);
	glb.SetMutateStep(10);
	glb.SetMutatePercentNewGenom(50);
	glb.SetCrossTPercent(20, 30, 10, 10, 30); //glb.SetCrossTPercent(3, 3, 3, 31, 60);
	glb.SetElitePercent(40);
	glb.SetRTTypeSelectionPercent(60);
	glb.SetTruncationStep(90);
	glb.SetMaxValue(1);

	glb.SetFileNameOutputWeights(OutputWeightsFile);

	glb.SetResultPercent(MaxPercent);

	int ret = glb.Run(pNN);

	printf("\nEpoch %d\n", glb.GetEpoch());
	fprintf(GL_STAT,"\nEpoch %d\n", glb.GetEpoch());

	if (ret == -1)
	{
		printf("Not Found, Best Chromosome:\n");
		fprintf(GL_STAT,"Not Found, Best Chromosome:\n");
		chromosome = glb.GetChromosome(0);
	}
	else
	{
		chromosome = glb.GetChromosome(ret);
	}

	printf("Fitness: %f", chromosome.fitness);
	printf("\tPercent %f",chromosome.percent);
	printf("\tTrue %d from %d\n", chromosome.countTrue, g_pInputs->size());
	fprintf(GL_STAT, "Fitness: %f", chromosome.fitness);
	fprintf(GL_STAT, "\tPercent %f", chromosome.percent);
	fprintf(GL_STAT, "\tTrue %d from %d\n", chromosome.countTrue, g_pInputs->size());
	int find = 0;

	for (int i = 0; i < glb.Population.size(); i++)
	{
		if (glb.Population[i].fitness == 0)
			find++;
	}

	printf("Find: %d\n\n", find);
	fprintf(GL_STAT,"Find: %d\n\n", find);
}

//Функция обучения/Тестирования ГА
void RealTimeLearnTest(int Epoch, int window, int allinput, int FutureDay)
{
	GL_STAT = fopen("Files\\Stat\\GLOBAL_STAT.txt", "a+" );
	fprintf(GL_STAT, "\n\n******START******\n\n");


	//int window = 285; // Окно
	//int allinput = 555; //Общий размер оубчения/тестирования
	//int FutureDay = 270; //На сколько дней прогноз

	int iteration = allinput - window - FutureDay + 1;

	int cur = 1;
	int start = 0;
	int end = start+window;

	vector<vector<Input*>*>* buf_pInputs=new vector<vector<Input *> *>;
	vector <Input*>* buf_pExpected=new vector<Input *>;

	buf_pInputs->insert(buf_pInputs->begin(), g_pInputs->begin(), g_pInputs->end());
	buf_pExpected->insert(buf_pExpected->begin(), g_pExpected->begin(), g_pExpected->end());
	double count = 0;
	FILE* pFile;
	pFile = fopen(GetFilesNameForTestStat(cur).c_str(), "w");
	fclose(pFile);

	fprintf(GL_STAT, "Configuration: %d %d %d %d %d\n", (*g_pNNs)[0]->Inputs.size(), (*g_pNNs)[0]->FirstNeuronsLayer.size(), (*g_pNNs)[0]->SecondNeuronsLayer.size(), (*g_pNNs)[0]->ThirdNeuronsLayer.size(), (*g_pNNs)[0]->OutputNeurons.size());
	fprintf(GL_STAT, "Window %d FutureDay: %d\n", window,FutureDay);
	fprintf(GL_STAT, "\n************\n\n");
	while (cur<=iteration)
	{
		//Подготовка входных данных для обучения на window днях

		string fileName = GetFilesNameForOutputWeights(cur);
		g_pInputs->clear();
		g_pExpected->clear();

		printf("\t****Iteration %d from %d****\n", cur,iteration);
		fprintf(GL_STAT,"\t****Iteration %d from %d****\n", cur, iteration);

		g_pInputs->insert(g_pInputs->begin(),buf_pInputs->begin()+start, buf_pInputs->begin()+end);
		g_pExpected->insert(g_pExpected->begin(), buf_pExpected->begin() + start, buf_pExpected->begin()+end);
		
		//Обучение на window днях
		StartLearning((*g_pNNs)[0],Epoch,100, fileName);

		//Тестирование с найденным весом на FutureDay днях
		g_pInputs->clear();
		g_pExpected->clear();

		for (int i = 0; i < FutureDay; i++)
		{
			g_pInputs->push_back((*buf_pInputs)[end+i]);
			g_pExpected->push_back((*buf_pExpected)[end+i]);
		}

		vector<vector<vector<double>>>* pWeightsLayers;

		pWeightsLayers = ReadConfiguration(fileName.c_str());
		
		vector<vector<double>>* pResults;

		(*g_pNNs)[0]->SetWeights(pWeightsLayers);

		pResults = ProcessNetForAllInputVectors((*g_pNNs)[0]);

		vector<double> trendInfo = GetTrend(pResults);

		double ret = trendInfo[0];
		WriteStat(&trendInfo,cur);
		count += ret;
		
		double percent = (double)count/cur / FutureDay * 100;
		printf("\nALL True: %f from %d \t Percent: %f\n", ret, FutureDay, percent);
		fprintf(GL_STAT,"\nALL True: %f from %d \t Percent: %f\n", ret, FutureDay, percent);
		cur++;
		start++;
		end++;
	}
	fprintf(GL_STAT, "\n\n******END******\n\n");
	fclose(GL_STAT);
}

//Функция прогнозирования на будущее
void Prognosis()
{
	GL_STAT = fopen("Files\\GLOBAL_STAT.txt", "a+");
	fprintf(GL_STAT, "\n\n******START******\n\n");

	int allinput = 220; //Общий размер файлов
	int window = 200; // Размер обучения
	int FutureDay = 20; //На сколько дней прогноз
	int cur = window;
	
	vector<vector<Input*>*>* buf_pInputs = new vector<vector<Input *> *>;
	vector <Input*>* buf_pExpected = new vector<Input *>;

	buf_pInputs->insert(buf_pInputs->begin(), g_pInputs->begin(), g_pInputs->end());
	buf_pExpected->insert(buf_pExpected->begin(), g_pExpected->begin(), g_pExpected->end());

	g_pInputs->clear();
	g_pExpected->clear();

	g_pInputs->insert(g_pInputs->begin(), buf_pInputs->begin(), buf_pInputs->begin() + window);
	g_pExpected->insert(g_pExpected->begin(), buf_pExpected->begin(), buf_pExpected->begin() + window);


	fprintf(GL_STAT, "Configuration: %d %d %d %d %d\n", (*g_pNNs)[0]->Inputs.size(), (*g_pNNs)[0]->FirstNeuronsLayer.size(), (*g_pNNs)[0]->SecondNeuronsLayer.size(), (*g_pNNs)[0]->ThirdNeuronsLayer.size(), (*g_pNNs)[0]->OutputNeurons.size());
	fprintf(GL_STAT, "Window %d FutureDay: %d\n", window, FutureDay);
	fprintf(GL_STAT, "\n************\n\n");

	//StartLearning((*g_pNNs)[0], 0, 90, "Files\\PrognosisWeights.txt");

	vector<vector<vector<double>>>* pWeightsLayers;
	pWeightsLayers = ReadConfiguration("Files\\PrognosisWeights.txt");

	while (cur<=allinput)
	{
		g_pInputs->clear();
		g_pExpected->clear();

		g_pInputs->push_back((*buf_pInputs)[cur]);
		g_pExpected->push_back((*buf_pExpected)[cur]);

		vector<vector<double>>* pResults;

		(*g_pNNs)[0]->SetWeights(pWeightsLayers);

		pResults = ProcessNetForAllInputVectors((*g_pNNs)[0]);

		vector<double> trendInfo = GetTrend(pResults);


		cur++;
	}

	fprintf(GL_STAT, "\n\n******END******\n\n");
	fclose(GL_STAT);

}

int main()
{

	g_pInputs = ReadAllInputFilesToMemory();

	g_pExpected = ReadExpectedFileToMemory();

	SYSTEM_INFO siSysInfo;
	GetSystemInfo(&siSysInfo);

	g_pNNs = new vector<NeuronNet*>;

	vector<void*>* needToFreeInputs= new vector<void *>;
	vector<void*> needToFreeWeights;

	for (int i = 0; i < siSysInfo.dwNumberOfProcessors; i++)
	{
		vector <Input*>* pInputs;
		pInputs = ReadInputs("Files\\Input.txt");

		vector<vector<vector<double>>>* pWeightsLayers;
		pWeightsLayers = ReadConfiguration("Files\\Configuration.txt");

		NeuronNet* pNN = new NeuronNet(pInputs, pWeightsLayers);
		g_pNNs->push_back(pNN);
		needToFreeInputs->push_back(pInputs);
		needToFreeWeights.push_back(pWeightsLayers);
	}
	//Обучение/Тестирование(на существующих данных)
	int epoch = 0;
	int window = 0;
	int allinput = 0;
	int futureday = 0;
	printf("\nCount of Epoch: \n");
	scanf_s("%d", &epoch);
	printf("\nCount of allinput: \n");
	scanf_s("%d", &allinput);
	printf("\nCount of window: \n");
	scanf_s("%d", &window);
	printf("\nCount of futureday: \n");
	scanf_s("%d", &futureday);

	RealTimeLearnTest(epoch, window, allinput, futureday);

	//Обучени->Прогноз (именно прогноз на будущее)
	//Prognosis();

	//Начало обучения
	//StartLearning((*g_pNNs)[0],INT_MAX,100);

	_getch();

	delete(g_pExpected);

	for (int i = 0; i < g_pInputs->size(); i++)
	{
		for (int j = 0; j < (*g_pInputs)[i]->size(); j++)
		{
			delete((*(*g_pInputs)[i])[j]);
		}
	}
	delete(g_pInputs);

	for (int i = 0; i < g_pNNs->size(); i++)
	{
		delete((*g_pNNs)[i]);
	}
	delete(g_pNNs);


	return 0;
}