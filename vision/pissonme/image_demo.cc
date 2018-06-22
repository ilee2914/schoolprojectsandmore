// Sample test file for the image class.
// Reads a given pgm image, draws a line, and saves it to
// another pgm image.
#include "image.h"
#include <cstdio>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;
using namespace ComputerVisionProjects;

int
main(int argc, char **argv){
  
  if (argc!=4) {
    printf("Usage: %s file1 file2 file3\n", argv[0]);
    return 0;
  }
   const string input_file(argv[1]);
  const string output_file(argv[3]);
  const string threshold_num(argv[2]);
  //check threshold to make sure int
  for (int i=0; i< threshold_num.size(); i++)
  {
      if (!isdigit(threshold_num[i]))
      {
        cerr<< "Please enter an integer threshold value!";
        return 0;
      }
  }
  //convert string to int
  istringstream buffer(threshold_num);
  int threshold;
  buffer >> threshold;

  Image an_image;
  if (!ReadImage(input_file, &an_image, threshold)) {
    cout <<"Can't open file " << input_file << endl;
    return 0;
  }

  //radius_and_centriods (output_file, an_image);
  // Draw a line from (0, 0) to (100, 240) having gray-value
  // 200.
  //DrawLine(0, 0, 100, 240, 200,
	  // &an_image); 
  
  if (!WriteImage(output_file, an_image)){
    cout << "Can't write to file " << output_file << endl;
    return 0;
  }
}
