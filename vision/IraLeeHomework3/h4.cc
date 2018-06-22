// Ira Lee

#include "image.h"
#include <cstdio>
#include <iostream>
#include <string>

using namespace std;
using namespace ComputerVisionProjects;

int main(int argc, char **argv){
	if (argc!=5) {
		printf("Usage: %s file1 hougharray threshold file2\n", argv[0]);
		return 0;
	}
	const string input_file(argv[1]);
	const string input_array(argv[2]);
	const int threshold = atoi(argv[3]);
	const string output_file(argv[4]);
	string output_array = "filler.txt";

	Image an_image;
	if (!ReadImage(input_file, &an_image, 4)) {
		cout <<"Can't open file " << input_file << endl;
		return 0;
	}
	
	if (!readArray(input_array, &an_image, threshold)) {
		cout << "readArray broken" << endl;
		return 0;
	}
	
	if (!WriteImage(output_file, an_image, 4, threshold)){
		cout << "Can't write to file " << output_file << endl;
		return 0;
	}
	
	// Will print out the hough array after local maximas are found
	/*
	if (!printHough("s"+output_file, output_array, an_image)){
		cout << "Can't write to file " << output_file << endl;
		return 0;
	}*/
	
}
