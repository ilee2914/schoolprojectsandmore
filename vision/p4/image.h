// Class for representing a 2D gray-scale image,
// with support for reading/writing pgm images.
// To be used in Computer Vision class.

#ifndef COMPUTER_VISION_IMAGE_H_
#define COMPUTER_VISION_IMAGE_H_

#include <cstdlib>
#include <string>
#include <iostream>
#include <unordered_map>
#include <cmath>
#include <fstream>

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
	struct objData{
		int label;
		int centerRow = 0;
		int centerCol = 0;
		int totalX = 0;
		int totalY = 0;
		int area = 0;
		double totalXPrime = 0;
		double totalYPrime = 0;
		double a = 0;
		double b = 0;
		double c = 0;
		// in radians
		double theta = 0;
		double thetaneg = 0;
		double E = 0;
		double Emax = 0;
		double roundness;
	};

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
		void SetPixel(size_t i, size_t j, int gray_level) {
			if (i >= num_rows_ || j >= num_columns_) abort();
			pixels_[i][j] = gray_level;
		}

		int GetPixel(size_t i, size_t j) const {
			if (i >= num_rows_ || j >= num_columns_) abort();
			return pixels_[i][j];
		}

		void recordData(int i, int j, int label) {
			auto it = objects.find(label);
			objData* obj;
			if (it == objects.end()) {
				obj = new objData();
				obj->label = label;
				objects.insert({label, obj});
			} else {
				obj = it->second;
			}
			obj->totalX += j;
			obj->totalY += i;
			obj->area++;
		}

		void findCenters() {
			// 1st moment of area
			for (auto it = objects.begin(); it != objects.end(); it++) {
				findCenter(it->second);
			}	  
		}
		
		void findCenter(objData* obj) {
			obj->centerCol = obj->totalX / obj->area;
			obj->centerRow = obj->totalY / obj->area;
		}
		
		void findSecondMoment() {
			recordSecondData();		// Finds A, B, and C
			for (auto it = objects.begin(); it != objects.end(); it++) {
				objData* obj = it->second;
				obj->theta = atan2((obj->b),(obj->a - obj->c))/2;
				obj->E = obj->a * pow(sin(obj->theta),2) - obj->b*sin(obj->theta)*cos(obj->theta) + obj->c*pow(cos(obj->theta),2);
				obj->thetaneg = obj->theta*-1;
				obj->Emax = obj->a * pow(sin(obj->thetaneg),2) - obj->b*sin(obj->thetaneg)*cos(obj->thetaneg) + obj->c*pow(cos(obj->thetaneg),2);
				obj->roundness = obj->E/obj->Emax;
			}
		}

		void recordSecondData() {
			for (int i = 0; i < num_rows_; ++i) {
				for (int j = 0;j < num_columns_; ++j) {
					int byte = pixels_[i][j];
					if (byte > 0) {
						auto it = objects.find(byte);
						if (it == objects.end())
							continue;
						objData* obj = it->second;
						obj->totalXPrime += pow(j - obj->centerCol, 2);
						obj->totalYPrime += pow(i - obj->centerRow, 2);
						obj->b += ((j - obj->centerCol) * (i - obj->centerRow));
					}		
				}
			}
			for (auto it = objects.begin(); it != objects.end(); it++) {
				objData* obj = it->second;
				obj->a = obj->totalXPrime;
				obj->b *= 2;
				obj->c = obj->totalYPrime;
			}
		}
		
		void compareData(const string& input_filename) {
			string throwaway;
			ifstream input(input_filename);
			if (input.is_open()) {
				input >> throwaway;
				while (!input.eof()) {
					objData obj;
					input >> obj.label;
					input >> obj.area;
					input >> obj.centerCol;
					input >> obj.centerRow;
					input >> obj.E;
					input >> obj.theta;
					input >> obj.roundness;
		
					// Checking if obj in db exists in image
					for (auto it = objects.begin(); it != objects.end(); it++) {
						double areaAcc = abs(obj.area - it->second->area)/(double)obj.area;
						double EAcc = abs(obj.E - it->second->E)/obj.E;
						if (areaAcc <= .15 &&  EAcc <= .15) {		//Found something 85% accurate
							objData* objs = it->second;
							//cout << "Label : " << obj.label << " Label: " << objs->label << endl;
							//cout << areaAcc << " " << EAcc << endl;
							double row = objs->centerRow;
							double col = objs->centerCol;
							int nextCol = (col+num_columns_);
							int nextRow = row+num_columns_*tan(objs->theta);
							DrawLine(row, col, nextRow, nextCol, 255, objs->label, false);
						}
					}
				}
				input.close();
			} else {
				cout << "Unable to open file";
			}
		}
		void printData(const string& input_filename) const {
			ofstream db(input_filename);
			db << "Label|Area|CenterCol|centerRow|minIntertia|orientation|roundness" << "\r\n";
			for (auto it = objects.begin(); it != objects.end(); it++) {
				objData* obj = it->second;
				db << fixed
				<< obj->label << " " 
				<< obj->area << " "
				<< obj->centerCol << " " 
				<< obj->centerRow << " "
				<< obj->E << " "
				<< obj->theta << " "
				<< obj->roundness << "\r\n";
			}
			db.close();
		}

		void markCenters() {
			for (auto it = objects.begin(); it != objects.end(); it++) {
				objData* obj = it->second;
				pixels_[obj->centerRow][obj->centerCol] = 255;
			}
		}
		
		void drawOrientation() {
			for (auto it = objects.begin(); it != objects.end(); it++) {
				objData* obj = it->second;
				double row = obj->centerRow;
				double col = obj->centerCol;
				int nextCol = (col+num_columns_);
				int nextRow = row+num_columns_*tan(obj->theta);
				DrawLine(row, col, nextRow, nextCol, 255, obj->label, false);
			}
		}
	private:
		void DeallocateSpace();

		size_t num_rows_; 
		size_t num_columns_; 
		size_t num_gray_levels_;  
		int **pixels_;
		unordered_map<int, objData*> objects;
		
		//  Draws a line of given gray-level color from (x0,y0) to (x1,y1);
		//  an_image is the output_image.
		// IMPORTANT: (x0,y0) and (x1,y1) can lie outside the image 
		//   boundaries, so SetPixel() should check the coordinates passed to it.
		void DrawLine(int x0, int y0, int x1, int y1, int color, int label, bool drawHoles);
	};

	// Reads a pgm image from file input_filename.
	// an_image is the resulting image.
	// Returns true if  everyhing is OK, false otherwise.
	bool ReadImage(const std::string &input_filename, Image *an_image);
	
	bool p3(const string &input_filename, Image *an_image);
	bool p4(const string &input_filename, Image *an_image);
	// Writes image an_iamge into the pgm file output_filename.
	// Returns true if  everyhing is OK, false otherwise.
	bool WriteImage(const std::string &output_filename, const Image &an_image);

	

}  // namespace ComputerVisionProjects

#endif  // COMPUTER_VISION_IMAGE_H_
