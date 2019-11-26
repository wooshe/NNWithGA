#define _CRT_SECURE_NO_WARNINGS

//Подключение всех заголовочных файлов
#include <iostream>
#include <thread>
#include <mutex>
#include <time.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <Windows.h>
#include <vector>
#include <string>
#include <algorithm> 
#include <random>
#include <cstdlib>
#include <sstream>
#include <cstring>
#include <fstream>
using namespace std;

#define ALL_FILES 555

#include "NN\Input.h"
#include "NN\Synapse.h"
#include "NN\Neuron.h"
#include "NN\NeuronNet.h"

#include "GA\Chromosome.h"
#include "GA\GA.h"

vector<vector<double>>* ProcessNetForAllInputFiles(NeuronNet* pNN);
vector<vector<double>>* ProcessNetForAllInputVectors(NeuronNet* pNN);
vector<double> GetTrend(vector<vector<double>>* pResults);
void WriteConfiguration(const char* ConfigurationFileName, NeuronNet* pNN, Chromosome* best);
vector<vector<vector<double>>>* ReadConfiguration(const char* ConfigurationFileName);
extern vector<vector<Input*>*>* g_pInputs;
extern vector <Input*>* g_pExpected;
extern vector <NeuronNet*>* g_pNNs;
extern FILE* GL_STAT;
