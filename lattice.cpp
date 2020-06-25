#include "lattice.h"
#include <vector>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <iomanip>

std::pair<int, int> Lattice::getSize() const
{
	return std::make_pair(rows, cols);
}

int Lattice::classify(int a)
{
	int b, c;
	b = a;
	while (parent[b] != b)
		b = parent[b];
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
	int label_a = classify(a);
	int label_b = classify(b);
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
	else
	{
		clusterSize[label_a]++;
	}
}

void Lattice::setLabels(std::string input, std::string output)
{
	std::ifstream inf(input); //будем читать решетку из этого файла
	std::ofstream outf(output); //будем записывать метки в этот файл

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

	parent.push_back(0);
	clusterSize.push_back(0);

	int i, j, elem;
	int counter = 0; //счетчик меток
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

			if (sitesCur[j] != 0)
			{
				// если среди ранее просмотренных соседей нет занятых узлов
				if (left == 0 && upper == 0)
				{
					counter++;
					labelsCur.push_back(counter);
					parent.push_back(counter);
					clusterSize.push_back(1);
				}
				// если левый сосед занят и верхний нет
				else if (left != 0 && upper == 0)
				{
					label_left = classify(left);
					labelsCur.push_back(label_left);
					clusterSize[label_left]++;
				}
				// если верхний сосед занят и левый нет
				else if (left == 0 && upper != 0)
				{
					label_upper = classify(upper);
					labelsCur.push_back(label_upper);
					clusterSize[label_upper]++;
				}
				// если оба соседа заняты
				else
				{
					merge(left, upper);
					labelsCur.push_back(classify(left));
				}
				outf << labelsCur.back() << " ";
			}
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

double Lattice::fRand() const
{
	double f = (double)rand() / RAND_MAX;
	return f;
}

void Lattice::generate(int L, double p, std::string filename, int seed)
{
	std::ofstream outf(filename);
	if (!outf)
	{
		std::cerr << "Error while opening file" << "\n";
		exit(1);
	}

	outf << L << " " << L << "\n";
	srand(seed);
	int i, j, elem;
	for (i = 0; i < L; ++i)
	{
		for (j = 0; j < L; ++j)
		{
			if (fRand() < p)
				elem = -1;
			else
				elem = 0;
			outf << elem << " ";
		}
		outf << "\n";
	}
}