#pragma once
#include <vector>
#include <string>
#include <random>

class Lattice {
public:
	Lattice();
	std::vector<int> sitesPrev;	// предыдущая строка узлов
	std::vector<int> sitesCur; // текущая строка узлов
	std::vector<int> labelsPrev; // предыдущая строка меток
	std::vector<int> labelsCur;	// текущая строка меток
	std::vector<int> clusterSize; // размеры кластеров
	bool lver;	// факт протекания по вертикали
	bool lhor;	// факт протекания по горизонтали

	// алгоритм Хошена-Копельмана
	void setLabels(std::string input, std::string output);
	// размер решетки
	std::pair<int, int> getSize() const;
	// транспонирование решетки
	void transpose(std::string input, std::string output) const;
	// форматирование файла в читаемый вид
	void beautify(std::string filename) const;
	// генерация решетки с указанной концентрацией
	void generate(int L, double p, std::string filename);
	// проверка протекания
	void percolation(std::string filename);
	// очищение переменных
	void clear();

private:
	int rows; // количество строк в решетке
	int cols; // количество столбцов в решетке
	std::vector<int> parent; // массив предков для меток
	std::random_device rd;

	// восстановление идентификатора кластера
	int classify(int v);
	// объединение двух кластеров по меткам
	void merge(int a, int b);
};
