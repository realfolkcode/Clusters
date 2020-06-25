#pragma once
#include <vector>
#include <string>

class Lattice {
public:
	std::vector<int> sitesPrev;
	std::vector<int> sitesCur;
	std::vector<int> labelsPrev;
	std::vector<int> labelsCur;
	std::vector<int> clusterSize;
	std::vector<int> pHor;
	std::vector<int> pVer;
	// основной алгоритм разбиения на кластеры (расставление меток)
	void setLabels(std::string input, std::string output);
	// размеры решетки
	std::pair<int, int> getSize() const;
	// транспонирование матрицы
	void transpose(std::string input, std::string output) const;
	// форматирование текстового файла
	void beautify(std::string filename) const;
	// генерация случайной решетки с заданной вероятностью
	void generate(int L, double p, std::string filename, int seed);

private:
	int rows = 0;
	int cols = 0;
	// дерево меток
	std::vector<int> parent;
	// определение номера кластера по метке
	int classify(int v);
	// слияние двух кластеров
	void merge(int a, int b);
	// генерация случайного double в диапазоне от 0 до 1
	double fRand() const;
	// находится ли метка на границе
	void percolation(std::string filename);
};