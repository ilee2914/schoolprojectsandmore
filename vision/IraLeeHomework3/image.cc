
// Class for representing a 2D gray-scale image,
// with support for reading/writing pgm images.
// To be used in Computer Vision class.

#include "image.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

namespace ComputerVisionProjects {

	Image::Image(const Image &an_image){
		AllocateSpaceAndSetSize(an_image.num_rows(), an_image.num_columns());
		SetNumberGrayLevels(an_image.num_gray_levels());

		for (size_t i = 0; i < num_rows(); ++i)
		for (size_t j = 0; j < num_columns(); ++j){
			SetPixel(i,j, an_image.GetPixel(i,j), false);
		}
	}

	Image::~Image(){
		DeallocateSpace();
	}

	void
	Image::AllocateSpaceAndSetSize(size_t num_rows, size_t num_columns) {
		if (pixels_ != nullptr) DeallocateSpace();
		if (sData != nullptr) DeallocateSpace();
		if (houghData != nullptr) DeallocateSpace();
		pixels_ = new int*[num_rows];
		sData = new sVals*[num_rows];
		for (size_t i = 0; i < num_rows; ++i) {
			pixels_[i] = new int[num_columns];
			sData[i] = new sVals[num_columns];
		}
		maxRho = sqrt(pow(num_rows, 2) + pow(num_columns, 2));
		intervals = 180;
		houghData = new double*[maxRho];		//Checking half degrees
		for (int i = 0; i < maxRho; i++) {
			houghData[i] = new double[intervals];
		}
		num_rows_ = num_rows;
		num_columns_ = num_columns;
	}

	void
	Image::DeallocateSpace() {
		for (size_t i = 0; i < num_rows_; i++) {
			delete pixels_[i];
			delete sData[i];
		}
		for (int i = 0; i < intervals; i++) {
			delete houghData[i];
		}
		delete pixels_;
		delete sData;
		delete houghData;
		pixels_ = nullptr;
		sData = nullptr;
		houghData = nullptr;
		num_rows_ = 0;
		num_columns_ = 0;
		maxRho = 0;
		intervals = 0;
	}

	bool ReadImage(const string &filename, Image *an_image, int pgNum) {  
		if (an_image == nullptr) abort();
		FILE *input = fopen(filename.c_str(),"rb");
		if (input == 0) {
			cout << "ReadImage: Cannot open file" << endl;
			return false;
		}

		// Check for the right "magic number".
		char line[1024];
		if (fread(line, 1, 3, input) != 3 || strncmp(line,"P5\n",3)) {
			fclose(input);
			cout << "ReadImage: Expected .pgm file" << endl;
			return false;
		}

		// Skip comments.
		do
		fgets(line, sizeof line, input);
		while(*line == '#');

		// Read the width and height.
		int num_columns,num_rows;
		sscanf(line,"%d %d\n", &num_columns, &num_rows);
		an_image->AllocateSpaceAndSetSize(num_rows, num_columns);


		// Read # of gray levels.
		fgets(line, sizeof line, input);
		int levels;
		sscanf(line,"%d\n", &levels);
		an_image->SetNumberGrayLevels(levels);

		// read pixel row by row.
		for (int i = 0; i < num_rows; ++i) {
			for (int j = 0;j < num_columns; ++j) {
				const int byte=fgetc(input);
				if (byte == EOF) {
					fclose(input);
					cout << "ReadImage: short file" << endl;
					return false;
				}
				an_image->SetPixel(i, j, byte, false);
			}
		}

		if (pgNum == 1) {
			an_image->performSobel();
		} else if (pgNum == 3) {
			an_image->performHoughTransform();
		}

		fclose(input);
		return true; 
	}

	bool WriteImage(const string &filename, const Image &an_image, int pgNum, int threshold) {  
		FILE *output = fopen(filename.c_str(), "w");
		if (output == 0) {
			cout << "WriteImage: cannot open file" << endl;
			return false;
		}
		const int num_rows = an_image.num_rows();
		const int num_columns = an_image.num_columns();
		const int colors = an_image.num_gray_levels();

		// Write the header.
		fprintf(output, "P5\n"); // Magic number.
		fprintf(output, "#\n");  // Empty comment.
		fprintf(output, "%d %d\n%03d\n", num_columns, num_rows, colors);

		for (int i = 0; i < num_rows; ++i) {
			for (int j = 0; j < num_columns; ++j) {
				int byte = 0;
				if (pgNum == 1) {
					byte = an_image.getSobelValue(i, j);
				} else if (pgNum == 2) {
					byte = an_image.GetPixel(i, j);
					if (byte < threshold) {
						byte = 0;
					} else {
						byte = 255;	
					}
				} else {
					byte = an_image.GetPixel(i , j);
				}
				if (fputc(byte,output) == EOF) {
					fclose(output);
					cout << "WriteImage: could not write" << endl;
					return false;
				}
			}
		}
		
		fclose(output);
		return true; 
	}

	bool readArray(const std::string &filename, Image *an_image, const int threshold) {
		ifstream file(filename);

		double **temp; 
		temp = new double*[an_image->getMaxRho()];		//Checking half degrees
		for (int i = 0; i < an_image->getMaxRho(); i++) {
			temp[i] = new double[an_image->getIntervals()];
		}

		for (int i = 0; i < an_image->getMaxRho(); i++) {
			for (int j = 0; j < an_image->getIntervals(); j++) {
				int value;
				file >> value;
				if (value >= threshold)
					temp[i][j] = value;
				else
					temp[i][j] = 0;
				/*	If no local maxima is wanted 
				
				{
					if (value > 255)
						value = 255;
					
					an_image->setHoughPixel(i, j, value, false);*/
				}
			}
		
		
		// Finding local maximas?
		for (int i = 0; i < an_image->getMaxRho(); i+=10) {
			for (int j = 0; j < an_image->getIntervals(); j+=10) {
				int count = 0;
				double totalX = 0;
				double totalY = 0;
				double avgX = 0;
				double avgY = 0;
				for (int x = i; x < i+10; x++) {
					for (int y = j; y < j+10; y++) {
						if (x >= an_image->getMaxRho()) break;
						if (y >= an_image->getIntervals()) break;
						if (temp[x][y] > 0) {
							count += 1 + (temp[x][y]-threshold)*.001 ;
							totalX += x + (temp[x][y]-threshold)*.001;
							totalY += y + (temp[x][y]-threshold)*.001;
						}
					}
				}
				if (count > 0) {
					avgX = totalX/count;
					avgY = totalY/count;
					if (avgX >= an_image->getMaxRho()) avgX = an_image->getMaxRho();
					if (avgY >= an_image->getIntervals()) avgY = an_image->getIntervals();
					an_image->setHoughPixel((int)avgX, (int)avgY, 255, false);
				}
				
				
			}
		}
		
		an_image->drawHoughLines();
		return true;
	}
	
	bool printHough(const std::string &filename, const std::string &filename1, const Image &an_image) {  
		FILE *output = fopen(filename.c_str(), "w");
		ofstream array(filename1);
		
		if (output == 0) {
			cout << "PrintHough: cannot open file" << endl;
			return false;
		}
		const int num_rows = an_image.getMaxRho();
		const int num_columns = an_image.getIntervals();
		const int colors = an_image.num_gray_levels();
		
		// Write the header.
		fprintf(output, "P5\n"); // Magic number.
		fprintf(output, "#\n");  // Empty comment.
		fprintf(output, "%d %d\n%03d\n", num_columns, num_rows, colors);
		
		for (int i = 0; i < num_rows; ++i) {
			for (int j = 0; j < num_columns; ++j) {
				int byte = an_image.getHoughPixel(i, j, false);
				array << setw(4) << byte << " ";
				if (fputc(byte,output) == EOF) {
					fclose(output);
					cout << "PrintHough: could not write" << endl;
					return false;
				}
			}
			array << "\n";
		}
		
		fclose(output);
		return true; 
	}
	// Implements the Bresenham's incremental midpoint algorithm;
	// (adapted from J.D.Foley, A. van Dam, S.K.Feiner, J.F.Hughes
	// "Computer Graphics. Principles and practice", 
	// 2nd ed., 1990, section 3.2.2);  
	void Image::DrawLine(int x0, int y0, int x1, int y1, int color) {  
#ifdef SWAP
#undef SWAP
#endif
#define SWAP(a,b) {a^=b; b^=a; a^=b;}

		const int DIR_X = 0;
		const int DIR_Y = 1;

		// Increments: East, North-East, South, South-East, North.
		int incrE,
		incrNE,
		incrS,
		incrSE,
		incrN;     
		int d;         /* the D */
		int x,y;       /* running coordinates */
		int mpCase;    /* midpoint algorithm's case */
		int done;      /* set to 1 when done */

		int xmin = x0;
		int xmax = x1;
		int ymin = y0;
		int ymax = y1;

		int dx = xmax - xmin;
		int dy = ymax - ymin;
		int dir;

		if (dx * dx > dy * dy) {  // Horizontal scan.
			dir=DIR_X;
			if (xmax < xmin) {
				SWAP(xmin, xmax);
				SWAP(ymin , ymax);
			} 
			dx = xmax - xmin;
			dy = ymax - ymin;

			if (dy >= 0) {
				mpCase = 1;
				d = 2 * dy - dx;      
			} else {
				mpCase = 2;
				d = 2 * dy + dx;      
			}

			incrNE = 2 * (dy - dx);
			incrE = 2 * dy;
			incrSE = 2 * (dy + dx);
		} else {// vertical scan.
			dir = DIR_Y;
			if (ymax < ymin) {
				SWAP(xmin, xmax);
				SWAP(ymin, ymax);
			}
			dx = xmax - xmin;
			dy = ymax-ymin;    

			if (dx >=0 ) {
				mpCase = 1;
				d = 2 * dx - dy;      
			} else {
				mpCase = 2;
				d = 2 * dx + dy;      
			}

			incrNE = 2 * (dx - dy);
			incrE = 2 * dx;
			incrSE = 2 * (dx + dy);
		}

		/// Start the scan.
		x = xmin;
		y = ymin;
		done = 0;

		while (!done) {
			SetPixel(x,y,color, true);

			// Move to the next point.
			switch(dir) {
			case DIR_X: 
				if (x < xmax) {
					switch(mpCase) {
					case 1:
						if (d <= 0) {
							d += incrE;  
							x++;
						} else {
							d += incrNE; 
							x++; 
							y++;
						}
						break;

					case 2:
						if (d <= 0) {
							d += incrSE; 
							x++; 
							y--;
						} else {
							d += incrE;  
							x++;
						}
						break;
					} 
				} else {
					done=1;
				}     
				break;

			case DIR_Y: 
				if (y < ymax) {
					switch(mpCase) {
					case 1:
						if (d <= 0) {
							d += incrE;  
							y++;
						} else {
							d += incrNE; 
							y++; 
							x++;
						}
						break;

					case 2:
						if (d <= 0) {
							d += incrSE; 
							y++; 
							x--;
						} else {
							d += incrE;  
							y++;
						}
						break;
					} // mpCase
				} // y < ymin 
				else {
					done=1;
				}
				break;    
			}
		}
	}

}  // namespace ComputerVisionProjects







