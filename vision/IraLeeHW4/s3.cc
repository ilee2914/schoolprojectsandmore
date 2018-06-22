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

	if (argc!=8) {
		printf("Usage: %s file1 image1 image2 image3 step threshold file2\n", argv[0]);
		return 0;
	}
	const string normals(argv[1]);
	const string img1(argv[2]);
	const string img2(argv[3]);
	const string img3(argv[4]);
	const int step = atoi(argv[5]);
	const int threshold = atoi(argv[6]);
	const string output_file(argv[7]);

	Image image1, image2, image3;
	if (!ReadImage(img1, &image1, 3, 0)) {
		cout <<"Can't open file " << img1 << endl;
		return 0;
	}
	if (!ReadImage(img2, &image2, 3, 0)) {
		cout <<"Can't open file " << img2 << endl;
		return 0;
	}
	if (!ReadImage(img3, &image3, 3, 0)) {
		cout <<"Can't open file " << img3 << endl;
		return 0;
	}
	
	Image temp = computeSurfaceNormals(normals, &image1, &image2, &image3, step, threshold);
	
	if (!WriteImage(output_file, temp, 3)) {
		cout << "Can't write to file" << endl;
	}

}
