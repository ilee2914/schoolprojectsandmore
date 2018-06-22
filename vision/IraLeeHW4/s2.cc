// Sample test file for the image class.
// Reads a given pgm image, draws a line, and saves it to
// another pgm image.
#include "image.h"
#include <cstdio>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;
using namespace ComputerVisionProjects;

int main(int argc, char **argv){

	if (argc!=6) {
		printf("Usage: %s file1 threshold file2\n", argv[0]);
		return 0;
	}
	const string data(argv[1]);
	const string input1(argv[2]);
	const string input2(argv[3]);
	const string input3(argv[4]);
	const char* output_file(argv[5]);

	Image image1;
	Image image2;
	Image image3;
	
	
	remove(output_file);
	if (!ReadImage(input1, &image1, 2, data)) {
		cout <<"Can't open file " << input1 << endl;
		return 0;
	}
	
	if (!printNormal(output_file, image1)){
		cout << "Can't write to file " << output_file << endl;
		return 0;
	}
	
	if (!ReadImage(input2, &image2, 2, data)) {
		cout <<"Can't open file " << input2 << endl;
		return 0;
	}
	
	if (!printNormal(output_file, image2)){
		cout << "Can't write to file " << output_file << endl;
		return 0;
	}
	
	if (!ReadImage(input3, &image3, 2, data)) {
		cout <<"Can't open file " << input3 << endl;
		return 0;
	}
	
	if (!printNormal(output_file, image3)){
		cout << "Can't write to file " << output_file << endl;
		return 0;
	}
}
