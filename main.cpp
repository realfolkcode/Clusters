#include "lattice.h"
#include <iostream>

using namespace std;

int main()
{
	Lattice lattice;
	//lattice.transpose("example.txt", "example_transposed.txt");
	//lattice.setLabels("example_transposed.txt", "output.txt");
	//lattice.transpose("output.txt", "output_transposed.txt");
	//lattice.beautify("output_transposed.txt");
	//for (auto elem : lattice.clusterSize)
	//{
	//	cout << elem << " ";
	//}
	lattice.generate(30, 0.3, "input.txt", 42);
	return 0;
}