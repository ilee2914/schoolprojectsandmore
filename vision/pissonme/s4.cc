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
  
  if (argc!=7) {
    printf("Usage: %s file1 file2 file3 file4 file5 file6 file7\n", argv[0]);
    return 0;
  }
  
  const string textfile(argv[1]);
  const string input_file1(argv[2]);
  const string input_file2(argv[3]);
  const string input_file3(argv[4]);
  const string threshold(argv[5]);
  const string output_file(argv[6]);


  for (int i=0; i< threshold.size(); i++)
  {
      if (!isdigit(threshold[i]))
      {
        cerr<< "Please enter an integer threshold value!";
        return 0;
      }
  }
  //convert string to int

  int thres= std::stoi(threshold);


   Image an_image, an_image1, an_image2;

  if (!ReadImage(input_file1, &an_image)) {
    cout <<"Can't open file " << input_file1 << endl;
    return 0;
  }

  if (!ReadImage(input_file2, &an_image1)) {
    cout <<"Can't open file " << input_file2 << endl;
    return 0;
  }

  if (!ReadImage(input_file3, &an_image2)) {
    cout <<"Can't open file " << input_file3 << endl;
    return 0;
  }

  albedo(&an_image, &an_image1, &an_image2,thres, textfile, output_file);


  return 0; 
  
}