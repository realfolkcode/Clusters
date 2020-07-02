#pragma once
#include <vector>
#include <string>

class Lattice {
public:
	std::vector<int> sitesPrev;	// предыдущая строка узлов
	std::vector<int> sitesCur; // текущая строка узлов
	std::vector<int> labelsPrev; // предыдущая строка меток
	std::vector<int> labelsCur;	// текущая строка меток
	std::vector<int> clusterSize; // размеры кластеров
	bool lver = false;	// факт протекания по вертикали
	bool lhor = false;	// факт протекания по горизонтали

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
	int rows = 0; // количество строк в решетке
	int cols = 0; // количество столбцов в решетке
	std::vector<int> parent; // массив предков для меток

	// восстановление идентификатора кластера
	int classify(int v);
	// объединение двух кластеров по меткам
	void merge(int a, int b);
	// генерация случайного числа с плавающей точкой
	double fRand() const;
};
