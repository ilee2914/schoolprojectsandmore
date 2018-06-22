// Class for representing a 2D gray-scale image,
// with support for reading/writing pgm images.
// To be used in Computer Vision class.

#ifndef COMPUTER_VISION_IMAGE_H_
#define COMPUTER_VISION_IMAGE_H_

#include <cstdlib>
#include <string>
#include <cmath>
#include <iostream>

using namespace std;

namespace ComputerVisionProjects {

	// Class for representing a gray-scale image.
	// Sample usage:
	//   Image one_image;
	//   one_image.AllocateSpaceAndSetSize(100, 200);
	//   one_image.SetNumberGrayLevels(255);
	//   // Creates and image such that each pixel is 150.
	//   for (int i = 0; i < 100; ++i)
	//     for (int j = 0; j < 200; ++j)
	//       one_image.SetPixel(i, j, 150);
	//   WriteImage("output_file.pgm", an_image);
	//   // See image_demo.cc for read/write image.

	//sobel Values
	struct sVals{
		double gX = 0;
		double gY = 0;
		int val = 0;
		
		int computeValues() {
			val = (int)(sqrt(pow(gX,2)+pow(gY,2)));
		}
	};
	
	// rho = sqrt(rows^2 + cols^2)
	// radian from 0 to pi
	class Image {
	public:
		Image(): num_rows_{0}, num_columns_{0}, 
		num_gray_levels_{0}, pixels_{nullptr}, sData{nullptr}, houghData{nullptr}{ }

		Image(const Image &an_image);
		Image& operator=(const Image &an_image) = delete;

		~Image();

		// Sets the size of the image to the given
		// height (num_rows) and columns (num_columns).
		void AllocateSpaceAndSetSize(size_t num_rows, size_t num_columns);

		size_t num_rows() const { return num_rows_; }
		size_t num_columns() const { return num_columns_; }
		size_t num_gray_levels() const { return num_gray_levels_; }
		void SetNumberGrayLevels(size_t gray_levels) {
			num_gray_levels_ = gray_levels;
		}

		// Sets the pixel in the image at row i and column j
		// to a particular gray_level.
		void SetPixel(size_t i, size_t j, int gray_level, bool ignore) {
			if (i >= num_rows_ || j >= num_columns_){
				if (!ignore)
					abort();
				return;
			}
			pixels_[i][j] = gray_level;
		}

		int GetPixel(size_t i, size_t j) const {
			if (i >= num_rows_ || j >= num_columns_) abort();
			return pixels_[i][j];
		}
		
		int getHoughPixel(size_t i, size_t j, bool ignore) const {
			if (i >= maxRho || j >= intervals){
				if (!ignore)
					abort();
				return 0;
			}
			return houghData[i][j];
		}
		
		void setHoughPixel(size_t i, size_t j, int value, bool ignore) {
			if (i >= maxRho || j >= intervals){
				if (!ignore)
					abort();
				return;
			}
			houghData[i][j] = value;
		}
		
		int getSobelValue(size_t i, size_t j) const {                     
			if (i >= num_rows_ || j >= num_columns_) abort();
			return sData[i][j].val;
		}
		
		int getMaxRho() const{
			return maxRho;
		}
		
		int getIntervals() const{
			return intervals;
		}
		
		void performSobel() {	
			// I know I can compute both the X and Y at the same time to save time,
			// but I do it this way to better understand how it works.
			for (int i = 1; i < num_rows_-1; i++) {
				for (int j = 1; j < num_columns_-1;j++) {
					sData[i][j].gX = convolute(sobelX, i, j);
					sData[i][j].gY = convolute(sobelY, i, j);
					sData[i][j].computeValues();
				}
			}
		}
		
		int convolute(int filter[][3], int i, int j)  {
			int total = 0;
			for (int a = -1; a < 2; a++) {
				for (int b = -1; b < 2; b++) {
					total += pixels_[i+a][j+a]*filter[a+1][b+1];
				}
			}
			return total;
		}
		
		void performHoughTransform() {
			for (int i = 0; i < num_rows_; i++) {
				for (int j = 0; j < num_columns_; j++) {
					if (pixels_[i][j] == 255) {				// If current pixel is an edge
						for (int r = 0; r < intervals; r++) {
							double value = i*cos(r*pi/180) + j*sin(r*pi/180);
							setHoughPixel(abs((int)value), r, getHoughPixel(abs((int)value),r, true)+1, true);
							//setHoughPixel(abs((int)value)-1, r-1, getHoughPixel(abs((int)value)-1,r-1, true)+1, true);
							//setHoughPixel(abs((int)value), r-1, getHoughPixel(abs((int)value),r-1, true)+1, true);
							//setHoughPixel(abs((int)value)-1, r, getHoughPixel(abs((int)value)-1,r, true)+1, true);
							//If I want to fill in more than one spot in teh hough array. Not needed, slightly buggy
						}
					}
				}
			}	// Finished filling the acc array
			

		}
		
		void drawHoughLines() {
			//Get all the highest
			// p = currentRho
			// j = radians
			for (int p = 0; p < maxRho; p++) {
				for (int r = 0; r < intervals; r++) {
					if (houghData[p][r] == 255) {
						double radians = r*pi/360;
						int firstY = (int)(p/sin(radians));
						int secondY = -1*((int)((num_columns_)*cos(radians) - p)/(sin(radians)));
						DrawLine(0, firstY, num_columns_, secondY, houghData[p][r]);
					}
				}
			}
		}

	private:
		void DeallocateSpace();

		size_t num_rows_; 
		size_t num_columns_; 
		size_t num_gray_levels_;  
		int **pixels_;

		int sobelX[3][3] = {{-1, 0, 1},
							{-2, 0, 2},
							{-1, 0, 1}};
		int sobelY[3][3] = {{-1, -2, -1},
							{ 0,  0,  0},
							{ 1,  2,  1}};
							
		sVals **sData;
		
		int maxRho;
		int intervals;
		double **houghData;
		double pi = 3.1415926535897;
		
		//  Draws a line of given gray-level color from (x0,y0) to (x1,y1);
		// IMPORTANT: (x0,y0) and (x1,y1) can lie outside the image 
		//   boundaries, so SetPixel() should check the coordinates passed to it.
		void DrawLine(int x0, int y0, int x1, int y1, int color);
		
	};

	// Reads a pgm image from file input_filename.
	// an_image is the resulting image.
	// Returns true if  everyhing is OK, false otherwise.
	bool ReadImage(const std::string &input_filename, Image *an_image, int pgNum);

	// Writes image an_iamge into the pgm file output_filename.
	// Returns true if  everyhing is OK, false otherwise.
	bool WriteImage(const std::string &output_filename, const Image &an_image, int pgNum, int threshold);
	
	bool printHough(const std::string &filename, const std::string &filename1, const Image &an_image);
	bool readArray(const std::string &filename, Image *an_image, const int threshold);
	
}  // namespace ComputerVisionProjects

#endif  // COMPUTER_VISION_IMAGE_H_
