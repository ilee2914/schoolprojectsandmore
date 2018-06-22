// Sample test file for the image class.
// Reads a given pgm image, draws a line, and saves it to
// another pgm image.
#include "image.h"
#include <cstdio>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
using namespace std;
using namespace ComputerVisionProjects;

int
main(int argc, char **argv){

	if (argc!=6) {
		printf("Usage: %s file1 file2 file3 file4 file5\n", argv[0]);
		return 0;
	}

	const string textfile(argv[1]);
	const string input_file1(argv[2]);
	const string input_file2(argv[3]);
	const string input_file3(argv[4]);
	const string output_file(argv[5]);

	int out;
	vector<int> output;
	ifstream myReadFile;
	myReadFile.open(textfile);
	if (myReadFile.is_open()) {
		while (!myReadFile.eof()) {
			myReadFile >> out;
			output.push_back(out);
		}
	}
	myReadFile.close();

	int center_x= output.at(0);
	cout<<center_x<<endl;
	int center_y= output.at(1);
	cout<<center_y<<endl;
	int rad= output.at(2);
	cout<<rad<<endl;

	vector<double> result; 
	vector<double> temp;

	Image an_image, an_image1, an_image2;
	if (!ReadImage(input_file1, &an_image)) {
		cout <<"Can't open file " << input_file1 << endl;
		return 0;
	}

	temp= calculate_normal(&an_image, center_x,center_y,rad);
	for (int i=0; i<temp.size(); i++)
	{
		result.push_back(temp.at(i));
	}

	if (!ReadImage(input_file2, &an_image1)) {
		cout <<"Can't open file " << input_file2 << endl;
		return 0;
	}

	temp= calculate_normal(&an_image1, center_x,center_y,rad);
	for (int i=0; i<temp.size(); i++)
	{
		result.push_back(temp.at(i));
	}

	if (!ReadImage(input_file3, &an_image2)) {
		cout <<"Can't open file " << input_file3 << endl;
		return 0;
	}

	temp= calculate_normal(&an_image2, center_x,center_y,rad);
	for (int i=0; i<temp.size(); i++)
	{
		result.push_back(temp.at(i));
	}

	int counter=0;
	ofstream myfile;
	myfile.open (output_file);
	for(int i=0; i<result.size(); i++)
	{
		myfile << result.at(i);
		counter++;
		if(counter ==3)
		{
			counter =0;
			myfile<< "\n";
		}
		else
		myfile << "   ";
	}
	myfile.close();

	return 0; 

}