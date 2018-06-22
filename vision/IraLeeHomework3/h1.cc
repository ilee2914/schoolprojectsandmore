// Sample test file for the image class.
// Reads a given pgm image, draws a line, and saves it to
// another pgm image.
#include "image.h"
#include <cstdio>
#include <iostream>
#include <string>

using namespace std;
using namespace ComputerVisionProjects;

int main(int argc, char **argv){

	if (argc!=3) {
		printf("Usage: %s file1 file2\n", argv[0]);
		return 0;
	}
	const string input_file(argv[1]);
	const string output_file(argv[2]);

	Image an_image;
	if (!ReadImage(input_file, &an_image, 1)) {
		cout <<"Can't open file " << input_file << endl;
		return 0;
	}

	if (!WriteImage(output_file, an_image, 1, 0)){
		cout << "Can't write to file " << output_file << endl;
		return 0;
	}
}
