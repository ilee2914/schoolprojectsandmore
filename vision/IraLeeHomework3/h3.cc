// Ira Lee

#include "image.h"
#include <cstdio>
#include <iostream>
#include <string>

using namespace std;
using namespace ComputerVisionProjects;

int
main(int argc, char **argv){

	if (argc!=4) {
		printf("Usage: %s file1 file2 outputarray\n", argv[0]);
		return 0;
	}
	const string input_file(argv[1]);
	const string output_file(argv[2]);
	const string output_array(argv[3]);

	Image an_image;
	if (!ReadImage(input_file, &an_image, 3)) {
		cout <<"Can't open file " << input_file << endl;
		return 0;
	}
	
	if (!printHough(output_file, output_array, an_image)) {
		cout << "Can't write to file " << output_array << endl;
		return 0;
	}
}
