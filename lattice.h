#pragma once
#include <vector>
#include <string>
#include <random>

class Lattice {
public:
	Lattice();
	std::vector<int> sitesPrev;	// ���������� ������ �����
	std::vector<int> sitesCur; // ������� ������ �����
	std::vector<int> labelsPrev; // ���������� ������ �����
	std::vector<int> labelsCur;	// ������� ������ �����
	std::vector<int> clusterSize; // ������� ���������
	bool lver;	// ���� ���������� �� ���������
	bool lhor;	// ���� ���������� �� �����������

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
	int rows; // ���������� ����� � �������
	int cols; // ���������� �������� � �������
	std::vector<int> parent; // ������ ������� ��� �����
	std::random_device rd;

	// �������������� �������������� ��������
	int classify(int v);
	// ����������� ���� ��������� �� ������
	void merge(int a, int b);
};
