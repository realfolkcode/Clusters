#pragma once
#include <vector>
#include <string>

class Lattice {
public:
	std::vector<int> sitesPrev;	// ���������� ������ �����
	std::vector<int> sitesCur; // ������� ������ �����
	std::vector<int> labelsPrev; // ���������� ������ �����
	std::vector<int> labelsCur;	// ������� ������ �����
	std::vector<int> clusterSize; // ������� ���������
	bool lver = false;	// ���� ���������� �� ���������
	bool lhor = false;	// ���� ���������� �� �����������

	// �������� ������-����������
	void setLabels(std::string input, std::string output);
	// ������ �������
	std::pair<int, int> getSize() const;
	// ���������������� �������
	void transpose(std::string input, std::string output) const;
	// �������������� ����� � �������� ���
	void beautify(std::string filename) const;
	// ��������� ������� � ��������� �������������
	void generate(int L, double p, std::string filename);
	// �������� ����������
	void percolation(std::string filename);
	// �������� ����������
	void clear();

private:
	int rows = 0; // ���������� ����� � �������
	int cols = 0; // ���������� �������� � �������
	std::vector<int> parent; // ������ ������� ��� �����

	// �������������� �������������� ��������
	int classify(int v);
	// ����������� ���� ��������� �� ������
	void merge(int a, int b);
	// ��������� ���������� ����� � ��������� ������
	double fRand() const;
};
