
// Class for representing a 2D gray-scale image,
// with support for reading/writing pgm images.
// To be used in Computer Vision class.

#include "image.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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
		pixels_ = new int*[num_rows];
		for (size_t i = 0; i < num_rows; ++i)
		pixels_[i] = new int[num_columns];

		num_rows_ = num_rows;
		num_columns_ = num_columns;
	}

	void
	Image::DeallocateSpace() {
		for (size_t i = 0; i < num_rows_; i++)
		delete pixels_[i];
		delete pixels_;
		pixels_ = nullptr;
		num_rows_ = 0;
		num_columns_ = 0;
	}

	
	bool ReadImage(const string &filename, Image *an_image, const int pgNum, const string &data) {
		return ReadImage(filename, an_image, pgNum, 0, data);
	}	
	
	bool ReadImage(const string &filename, Image *an_image, const int pgNum, const int threshold) { 
		return ReadImage(filename, an_image, pgNum, threshold, "");
	}
	
	bool ReadImage(const string &filename, Image *an_image, const int pgNum, const int threshold, const std::string &data) {  
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
		int brightest = 0;
		for (int i = 0; i < num_rows; ++i) {
			for (int j = 0;j < num_columns; ++j) {
				int byte=fgetc(input);
				if (byte > brightest) {
					brightest = byte;
				}
				if (pgNum == 1) {
					if (byte < threshold) {
						byte = 0;
					} else {
						byte = 255;
					}
				}
				if (byte == EOF) {
					fclose(input);
					cout << "ReadImage: short file" << endl;
					return false;
				}
				an_image->SetPixel(i, j, byte, false);
			}
		}
		
		an_image->setBrightest(brightest);
		an_image->calculateCenterOfBrightest();
		
		if (pgNum == 1) {
			an_image->findCenterAndRadius();
		} else if (pgNum == 2) {
			ifstream input(data);
			int centerX;
			int centerY;
			int radius;
			input >> centerX >> centerY >> radius;
			an_image->setCenterX(centerX);
			an_image->setCenterY(centerY);
			an_image->setRadius(radius);
			an_image->calculateNormal(an_image->getBrightestX(), an_image->getBrightestY());
		}

		fclose(input);
		return true; 
	}

	bool WriteImage(const string &filename, const Image &an_image, const int pgNum) {  
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
				const int byte = an_image.GetPixel(i , j);
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
	
	bool printData(const std::string &filename, const Image &an_image) {
		ofstream output(filename);
		output << an_image.getCenterX() << " " << an_image.getCenterY() << " " << an_image.getRadius();
		output.close();
		return true;	
	}
	
	bool printNormal(const std::string &filename, const Image &an_image) {
		ofstream output(filename, ios_base::app);
		output << an_image.getNormalX() << " " << an_image.getNormalY() << " " << an_image.getNormalZ() << "\r\n";
		output.close();
		return true;	
	}
	
	double computeDeterminant(double inputMatrix[][3]) {
		return ((inputMatrix[0][0] * ((inputMatrix[1][1] * inputMatrix[2][2]) - inputMatrix[1][2] * inputMatrix[2][1]))
		      - (inputMatrix[0][1] * ((inputMatrix[1][0] * inputMatrix[2][2]) - inputMatrix[1][2] * inputMatrix[2][0]))
		      + (inputMatrix[0][2] * ((inputMatrix[1][0] * inputMatrix[2][1]) - inputMatrix[1][1] * inputMatrix[2][0])));
	}
	
	void computeInverseMatrix(double inputMatrix[][3], vector< vector<double> > &invMatrix) {
		double det = computeDeterminant(inputMatrix);
		
		double invDet = 1/det;
		
		invMatrix[0][0] = invDet * ((inputMatrix[1][1] * inputMatrix[2][2]) - (inputMatrix[2][1] * inputMatrix[1][2]));
		invMatrix[0][1] = invDet * ((inputMatrix[0][2] * inputMatrix[2][1]) - (inputMatrix[0][1] * inputMatrix[2][2]));
		invMatrix[0][2] = invDet * ((inputMatrix[0][1] * inputMatrix[1][2]) - (inputMatrix[0][2] * inputMatrix[1][1]));
		invMatrix[1][0] = invDet * ((inputMatrix[1][2] * inputMatrix[2][0]) - (inputMatrix[1][0] * inputMatrix[2][2]));
		invMatrix[1][1] = invDet * ((inputMatrix[0][0] * inputMatrix[2][2]) - (inputMatrix[0][2] * inputMatrix[2][0]));
		invMatrix[1][2] = invDet * ((inputMatrix[1][0] * inputMatrix[0][2]) - (inputMatrix[0][0] * inputMatrix[1][2]));
		invMatrix[2][0] = invDet * ((inputMatrix[1][0] * inputMatrix[2][1]) - (inputMatrix[2][0] * inputMatrix[1][1]));
		invMatrix[2][1] = invDet * ((inputMatrix[2][0] * inputMatrix[0][1]) - (inputMatrix[0][0] * inputMatrix[2][1]));
		invMatrix[2][2] = invDet * ((inputMatrix[0][0] * inputMatrix[1][1]) - (inputMatrix[1][0] * inputMatrix[0][1]));
		
	}
	
	Image computeSurfaceNormals(const string &inputFile, Image *image1, Image *image2, Image *image3, int step, int threshold) {
		return processImage(inputFile, image1, image2, image3, step, threshold, 3);
	}
	
	Image computeAlbedos(const string &inputFile, Image *image1, Image *image2, Image *image3, int threshold) {
		return processImage(inputFile, image1, image2, image3, 1, threshold, 4);
	}
	
	Image processImage(const string &inputFile, Image *image1, Image *image2, Image *image3, int step, int threshold, int pgNum) {
		ifstream input(inputFile);
		vector< vector<double> > invMatrix(3,vector<double>(3,0));
		double inputMatrix[3][3];
		
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				input >> inputMatrix[i][j];
			}
		}
		
		computeInverseMatrix(inputMatrix, invMatrix);
		
		Image tempImage(*image1);
		
		const int num_rows = image1->num_rows();
		const int num_columns = image1->num_columns();
		double highestAlbedo = 0;
		
		for (int i = 0; i < num_rows; i += step) {
			for (int j = 0; j < num_columns; j += step) {
				if (i > num_rows) continue;
				if (j > num_columns) continue;
				
				int pixels[3] = {image1->GetPixel(i, j), image2->GetPixel(i, j), image3->GetPixel(i, j)};	//Pixels brightness for image1, image2, image3
				double I[3] = {0, 0, 0};																	//I values at each image;
				
				if (pixels[0] > threshold && pixels[1] > threshold && pixels[2] > threshold) {
					for (int a = 0; a < 3; a++) {
						for (int b = 0; b < 3; b++) {
							I[a] += invMatrix[a][b] * pixels[b];
						}
					}
					double albedo = sqrt(pow(I[0], 2) + pow(I[1], 2) + pow(I[2], 2));
					
					if (albedo > highestAlbedo) 
						highestAlbedo = albedo;
					
					for (int a = 0; a < 3; a++) {
						I[a] = (I[a] *10) / albedo;
					}
					
					if (pgNum == 3) {
						DrawLine(i, j, (i + I[0]), (j + I[1]), 255, &tempImage);
						tempImage.SetPixel(i-1, j, 255, true);
						tempImage.SetPixel(i+1, j, 255, true);
						tempImage.SetPixel(i, j, 0, true);
						tempImage.SetPixel(i, j-1, 255, true);
						tempImage.SetPixel(i, j+1, 255, true);
					} else if (pgNum == 4) {
						tempImage.SetPixel(i, j, albedo*255, true);		// Set the albedos in to store for later
					}
				} else if (pgNum == 4) {
					tempImage.SetPixel(i, j, 0, true);
				}
			}
		}
		if (pgNum == 4) {
			for (int i = 0; i < num_rows; i++) {
				for (int j = 0; j < num_columns; j++) {
					tempImage.SetPixel(i, j, tempImage.GetPixel(i, j)/highestAlbedo, true);		// Scaling the albedos
				}
			}
		}
		return tempImage;
		
		
		
	}
	// Implements the Bresenham's incremental midpoint algorithm;
	// (adapted from J.D.Foley, A. van Dam, S.K.Feiner, J.F.Hughes
	// "Computer Graphics. Principles and practice", 
	// 2nd ed., 1990, section 3.2.2);  
	void
	DrawLine(int x0, int y0, int x1, int y1, int color,
	Image *an_image) {  
		if (an_image == nullptr) abort();

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
			an_image->SetPixel(x,y,color, true);

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







