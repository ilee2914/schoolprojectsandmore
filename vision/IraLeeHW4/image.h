// Class for representing a 2D gray-scale image,
// with support for reading/writing pgm images.
// To be used in Computer Vision class.

#ifndef COMPUTER_VISION_IMAGE_H_
#define COMPUTER_VISION_IMAGE_H_

#include <cstdlib>
#include <string>
#include <math.h>
#include <vector>

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
	class Image {
	public:
		Image(): num_rows_{0}, num_columns_{0}, 
		num_gray_levels_{0}, pixels_{nullptr} { }

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
			if (i >= num_rows_ || j >= num_columns_) {
				if (ignore) return;
				abort();
			}
			pixels_[i][j] = gray_level;
		}

		int GetPixel(size_t i, size_t j) const {
			if (i >= num_rows_ || j >= num_columns_) abort();
			return pixels_[i][j];
		}
		
		int getCenterX() const {return centerX;}
		int getCenterY() const {return centerY;}
		int getRadius() const {return radius;}
		
		void setCenterX(int x) {centerX = x;} 
		void setCenterY(int y) {centerY = y;} 
		void setRadius(int rad) {radius = rad;}
		
		double getNormalX() const {return normalX;}
		double getNormalY() const {return normalY;}
		double getNormalZ() const {return normalZ;}
		
		void setBrightest(int val) {brightest = val;}
		int getBrightestX() const {return brightestX;}
		int getBrightestY() const {return brightestY;}
		
		void findCenterAndRadius() {
			int totalX = 0;
			int totalY = 0;
			
			int minX = 99999;
			int maxX = 0;
			int minY = 99999;
			int maxY = 0;
			int count = 0;
			
			for (int i = 0; i < num_rows_; i++) {
				for (int j = 0; j < num_columns_; j++) {
					if (pixels_[i][j] == 255) {
						totalX += i;
						totalY += j;
						
						if (i < minX) minX = i;
						if (i > maxX) maxX = i;
						if (j < minY) minY = j;
						if (j > maxY) maxY = j;
						
						count++;
					}
				}
			}
			centerX = totalX / count;
			centerY = totalY / count;
			int diameterX = maxX - minX;
			int diameterY = maxY - minY;
			radius = (diameterX + diameterY) / 4.0; // Divide by 2 for the average diameter, divide again by 2 for radius
		}
		
		void calculateCenterOfBrightest() {
			int totalX = 0;
			int totalY = 0;
			int count = 0;
			
			for (int i = 0; i < num_rows_; i++) {
				for (int j = 0; j < num_columns_; j++) {
					if (pixels_[i][j] == brightest) {
						totalX += i;
						totalY += j;
						count++;
					}
				}
			}
			brightestX = totalX/count;
			brightestY = totalY/count;
		}
		// x0, y0 = origin
		void calculateNormal(int x, int y) {
			// Sets the center of the sphere as the origin
			int valueX = x - centerX;
			int valueY = y - centerY;
			
			normalX = valueX;
			normalY = valueY;
			normalZ = sqrt(pow(radius,2) - pow(normalX,2) - pow(normalY, 2));
			
			double uv = sqrt(pow(normalX, 2) + pow(normalY, 2) + pow(normalZ, 2));

			normalX = (normalX * brightest) / uv;
			normalY = (normalY * brightest) / uv;
			normalZ = -1*(normalZ * brightest) / uv;
		}

	private:
		void DeallocateSpace();

		size_t num_rows_; 
		size_t num_columns_; 
		size_t num_gray_levels_;  
		int **pixels_;
		
		int centerX;
		int centerY;
		
		int brightest;
		int brightestX;
		int brightestY;
		
		double normalX;
		double normalY;
		double normalZ;
		
		double radius;
		
	};

	bool ReadImage(const std::string &input_filename, Image *an_image, const int pgNum, const std::string &data);
	bool ReadImage(const std::string &input_filename, Image *an_image, const int pgNum, const int threshold);
	// Reads a pgm image from file input_filename.
	// an_image is the resulting image.
	// Returns true if  everyhing is OK, false otherwise.
	bool ReadImage(const std::string &input_filename, Image *an_image, const int pgNum, const int threshold, const std::string &data);
	

	// Writes image an_iamge into the pgm file output_filename.
	// Returns true if  everyhing is OK, false otherwise.
	bool WriteImage(const std::string &output_filename, const Image &an_image, const int pgNum);
	bool printData(const std::string &filename, const Image &an_image);
	bool printNormal(const std::string &filename, const Image &an_image);

	//  Draws a line of given gray-level color from (x0,y0) to (x1,y1);
	//  an_image is the output_image.
	// IMPORTANT: (x0,y0) and (x1,y1) can lie outside the image 
	//   boundaries, so SetPixel() should check the coordinates passed to it.
	void DrawLine(int x0, int y0, int x1, int y1, int color, Image *an_image);
	
	double computeDeterminant(double inputMatrix[][3]);
	void computeInverseMatrix(double inputMatrix[][3], std::vector< std::vector<double> > &invMatrix);

	Image computeSurfaceNormals(const std::string &input, Image *image1, Image *image2, Image *image3, int step, int threshold);
	Image computeAlbedos(const std::string &input, Image *image1, Image *image2, Image *image3, int threshold);
	Image processImage(const std::string &input, Image *image1, Image *image2, Image *image3, int step, int threshold, int pgNum);
}  // namespace ComputerVisionProjects

#endif  // COMPUTER_VISION_IMAGE_H_
