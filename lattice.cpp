#include "lattice.h"
#include <vector>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include <random>

std::pair<int, int> Lattice::getSize() const
{
	return std::make_pair(rows, cols);
}

int Lattice::classify(int a)
{
	int b, c;
	b = a;
	// идем до корня - "настоящей" метки кластера
	while (parent[b] != b)
		b = parent[b];
	// сжимаем путь, присваивая найденный корень в качестве родителя всем обойденным вершинам
	while (parent[a] != a)
	{
		c = parent[a];
		parent[a] = b;
		a = c;
	}
	return b;
}

void Lattice::merge(int a, int b)
{
	// восстанавливаем id кластеров меток
	int label_a = classify(a);
	int label_b = classify(b);

	// находим наименьший id среди них и присваиваем другой метке в качестве родителя, обновляем размеры
	if (label_b < label_a)
	{
		parent[label_a] = label_b;
		clusterSize[label_b] = clusterSize[label_b] + clusterSize[label_a] + 1;
		clusterSize[label_a] = 0;
	}
	else if (label_a < label_b)
	{
		parent[label_b] = label_a;
		clusterSize[label_a] = clusterSize[label_a] + clusterSize[label_b] + 1;
		clusterSize[label_b] = 0;
	}
	// если одинаковые метки, то нет нужды в объединении, сразу увеличиваем размер на 1 (вклад текущего узла)
	else
	{
		clusterSize[label_a]++;
	}
}

void Lattice::setLabels(std::string input, std::string output)
{
	std::ifstream inf(input);
	std::ofstream outf(output);

	if (!inf)
	{
		std::cerr << "Error while opening file" << "\n";
		exit(1);
	}
	if (!outf)
	{
		std::cerr << "Error while opening file" << "\n";
		exit(1);
	}

	int rows_, cols_;
	inf >> rows_ >> cols_;
	rows = rows_;
	cols = cols_;
	outf << rows << " " << cols << "\n";

	// будем считать, что индексация в данных векторах начинается с 1, заполняем нулевой элемент мусором
	parent.push_back(0);
	clusterSize.push_back(0);

	int i, j, elem;
	int counter = 0;
	int left, upper, label_left, label_upper;
	for (i = 0; i < rows; ++i)
	{
		sitesPrev = sitesCur;
		sitesCur.clear();
		labelsPrev = labelsCur;
		labelsCur.clear();
		for (j = 0; j < cols; ++j)
		{
			inf >> elem;
			sitesCur.push_back(elem);

			if (i > 0)
				upper = labelsPrev[j];
			else
				upper = 0;
			if (j > 0)
				left = labelsCur[j - 1];
			else
				left = 0;

			// если узел занят
			if (sitesCur[j] != 0)
			{
				if (left == 0 && upper == 0)
				{
					counter++;
					labelsCur.push_back(counter);
					parent.push_back(counter);
					clusterSize.push_back(1);
				}
				else if (left != 0 && upper == 0)
				{
					label_left = classify(left);
					labelsCur.push_back(label_left);
					clusterSize[label_left]++;
				}
				else if (left == 0 && upper != 0)
				{
					label_upper = classify(upper);
					labelsCur.push_back(label_upper);
					clusterSize[label_upper]++;
				}
				else
				{
					merge(left, upper);
					labelsCur.push_back(classify(left));
				}
				outf << labelsCur.back() << " ";
			}
			// если узел свободен, то просто добавляем 0 к текущему вектору меток
			else
			{
				labelsCur.push_back(0);
				outf << 0 << " ";
			}
		}
		outf << "\n";
	}
}

void Lattice::transpose(std::string input, std::string output) const
{
	std::ifstream inf(input);
	if (!inf)
	{
		std::cerr << "Error while opening file" << "\n";
		exit(1);
	}
	int rows, cols, i, j, elem;
	std::vector<std::vector<int>> matrix;
	std::vector<int> tmp;
	inf >> rows >> cols;
	for (i = 0; i < rows; ++i)
	{
		tmp.clear();
		for (j = 0; j < cols; ++j)
		{
			inf >> elem;
			tmp.push_back(elem);
		}
		matrix.push_back(tmp);
	}
	inf.close();

	std::ofstream outf(output);
	if (!outf)
	{
		std::cerr << "Error while opening file" << "\n";
		exit(1);
	}
	outf << cols << " " << rows << "\n";
	for (i = 0; i < cols; ++i)
	{
		for (j = 0; j < rows; ++j)
		{
			outf << matrix[j][i] << " ";
		}
		outf << "\n";
	}
}

void Lattice::beautify(std::string filename) const
{
	std::ifstream inf(filename);
	if (!inf)
	{
		std::cerr << "Error while opening file" << "\n";
		exit(1);
	}
	int rows, cols, i, j, elem;
	std::vector<std::vector<int>> matrix;
	std::vector<int> tmp;
	inf >> rows >> cols;
	int width = 0;
	for (i = 0; i < rows; ++i)
	{
		tmp.clear();
		for (j = 0; j < cols; ++j)
		{
			inf >> elem;
			tmp.push_back(elem);
			width = std::max(width, int(log10(abs(elem))));
		}
		matrix.push_back(tmp);
	}
	width += 2;
	inf.close();

	std::ofstream outf(filename);
	if (!outf)
	{
		std::cerr << "Error while opening file" << "\n";
		exit(1);
	}
	outf << rows << " " << cols << "\n";
	for (i = 0; i < rows; ++i)
	{
		for (j = 0; j < cols; ++j)
		{
			outf << std::setw(width);
			outf << matrix[i][j] << " ";
		}
		outf << "\n";
	}
}

void Lattice::generate(int L, double p, std::string filename)
{
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0.0, 1.0);
	std::ofstream outf(filename);
	if (!outf)
	{
		std::cerr << "Error while opening file" << "\n";
		exit(1);
	}

	outf << L << " " << L << "\n";
	int i, j, elem;
	for (i = 0; i < L; ++i)
	{
		for (j = 0; j < L; ++j)
		{
			if (dis(gen) < p)
				elem = -1;
			else
				elem = 0;
			outf << elem << " ";
		}
		outf << "\n";
	}
}

void Lattice::percolation(std::string filename)
{
	// векторы, хранящие информацию о протекании
	// если равно 0 - кластер не встречается на границе
	// если равно 1 - кластер встречается на одной из границ
	// если >= 2 - кластер встречается на обоих границах
	std::vector<int> hor(clusterSize.size(), 0);
	std::vector<int> ver(clusterSize.size(), 0);
	// загружаем файл с метками
	std::ifstream inf(filename);
	if (!inf)
	{
		std::cerr << "Error while opening file" << "\n";
		exit(1);
	}
	int i, j, elem, rows, cols;
	inf >> rows >> cols;
	int cluster;
	for (i = 0; i < rows; ++i)
	{
		for (j = 0; j < cols; ++j)
		{
			inf >> elem;
			if (i == 0)
			{
				cluster = classify(elem);
				ver[cluster] = 1;
			}
			else if (j == 0)
			{
				cluster = classify(elem);
				hor[cluster] = 1;
			}
			else if (i == rows - 1)
			{
				cluster = classify(elem);
				ver[cluster] *= 2;
			}
			else if (j == cols - 1)
			{
				cluster = classify(elem);
				hor[cluster] *= 2;
			}
		}
	}

	for (i = 1; i < clusterSize.size(); ++i)
	{
		if (hor[i] >= 2)
		{
			lhor = true;
		}
		if (ver[i] >= 2)
		{
			lver = true;
		}
	}
}

void Lattice::clear()
{
	rows = 0;
	cols = 0;
	sitesPrev.clear();
	sitesCur.clear();
	labelsPrev.clear();
	labelsCur.clear();
	clusterSize.clear();
	parent.clear();
	lhor = false;
	lver = false;
}