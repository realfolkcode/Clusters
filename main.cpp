#include "lattice.h"
#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

int main()
{
	srand(time(0));
	int i, j;
	int L = 100; // размер решетки
	int G = 10; // количество разбиений
	int N = 100; // количество генераций решетки с одной и той же концентрацией
	double delta = 0.02; // шаг концентрации
	double phor, pver, phv; // вероятности протекания по соответствующим направлениям

	ofstream outf("test.txt"); // результаты будут записаны в данный файл
	if (!outf)
	{
		std::cerr << "Error while opening file" << "\n";
		exit(1);
	}

	for (double p = 0.1; p < 1; p = p + delta)
	{
		outf << p << "\n";

		for (j = 0; j < G; ++j)
		{
			phor = 0;
			pver = 0;
			phv = 0;
			for (i = 0; i < N; ++i)
			{
				Lattice lattice;
				lattice.generate(L, p, "input.txt"); // генерируем решетку
				lattice.setLabels("input.txt", "output.txt"); // разложение на кластеры
				// вывод размеров кластеров
				for (auto elem : lattice.clusterSize)
				{
					outf << elem << " ";
				}
				outf << "\n";

				lattice.percolation("output.txt"); // проверка протекания
				if (lattice.lhor)
					phor = phor + 1;
				if (lattice.lver)
					pver = pver + 1;
				if (lattice.lhor && lattice.lver)
					phv = phv + 1;
			}
			// нормируем
			phor = phor / N;
			pver = pver / N;
			phv = phv / N;
			outf << phor << " " << pver << " " << phv << "\n";
		}
	}
	outf.close();
	return 0;
}