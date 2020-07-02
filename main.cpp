#include "lattice.h"
#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

int main()
{
	srand(time(0));
	int i, j;
	int L = 100; // ������ �������
	int G = 10; // ���������� ���������
	int N = 100; // ���������� ��������� ������� � ����� � ��� �� �������������
	double delta = 0.02; // ��� ������������
	double phor, pver, phv; // ����������� ���������� �� ��������������� ������������

	ofstream outf("test.txt"); // ���������� ����� �������� � ������ ����
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
				lattice.generate(L, p, "input.txt"); // ���������� �������
				lattice.setLabels("input.txt", "output.txt"); // ���������� �� ��������
				// ����� �������� ���������
				for (auto elem : lattice.clusterSize)
				{
					outf << elem << " ";
				}
				outf << "\n";

				lattice.percolation("output.txt"); // �������� ����������
				if (lattice.lhor)
					phor = phor + 1;
				if (lattice.lver)
					pver = pver + 1;
				if (lattice.lhor && lattice.lver)
					phv = phv + 1;
			}
			// ���������
			phor = phor / N;
			pver = pver / N;
			phv = phv / N;
			outf << phor << " " << pver << " " << phv << "\n";
		}
	}
	outf.close();
	return 0;
}