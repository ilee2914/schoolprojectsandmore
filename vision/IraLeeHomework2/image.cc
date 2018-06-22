// Ira Lee

// xsin - ycos + p
// Class for representing a 2D gray-scale image,
// with support for reading/writing pgm images.
// To be used in Computer Vision class.

#include "image.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

using namespace std;

namespace ComputerVisionProjects {

	Image::Image(const Image &an_image){
		AllocateSpaceAndSetSize(an_image.num_rows(), an_image.num_columns());
		SetNumberGrayLevels(an_image.num_gray_levels());

		for (size_t i = 0; i < num_rows(); ++i)
		for (size_t j = 0; j < num_columns(); ++j){
			SetPixel(i,j, an_image.GetPixel(i,j));
		}
	}

	Image::~Image(){
		DeallocateSpace();
	}

	void Image::AllocateSpaceAndSetSize(size_t num_rows, size_t num_columns) {
		if (pixels_ != nullptr) DeallocateSpace();
		if (labels_ != nullptr) DeallocateSpace();
		pixels_ = new int*[num_rows];
		labels_ = new int*[num_rows];
		for (size_t i = 0; i < num_rows; ++i) {
			pixels_[i] = new int[num_columns];
			labels_[i] = new int[num_columns];
		}
		num_rows_ = num_rows;
		num_columns_ = num_columns;
	}

	void Image::DeallocateSpace() {
		for (size_t i = 0; i < num_rows_; i++) {
			delete pixels_[i];
			delete labels_[i];
		}
		delete pixels_;
		delete labels_;
		pixels_ = nullptr;
		labels_ = nullptr;
		num_rows_ = 0;
		num_columns_ = 0;
		labelCount = 0;
	}

	void Image::setLabel(size_t i, size_t j, int gray_level) {
			if (i >= num_rows_ || j >= num_columns_) abort();
			labels_[i][j] = -1;
			// Top left corner
			if (i == 0 && j == 0) {
				if (gray_level == 255) {
					labelCount++;
					labels_[i][j] = labelCount;
				} else {
					labels_[i][j] = -1;
				}
			}
			int labelTop = -2;
			int labelLeft = -2;
			int labelTopLeft = -2;
			
			if (i == 0) {
				labelTop = -1;
				labelTopLeft = -1;
			}
			else 
			labelTop = labels_[i-1][j];
			
			if (j == 0) {
				labelLeft = -1;
				labelTopLeft = -1;
			}
			else
			labelLeft = labels_[i][j-1];
			
			if (labelTopLeft == -2)
			labelTopLeft = labels_[i-1][j-1];
			
			if (gray_level == 255) {
				if ((labelTop + labelLeft + labelTopLeft) == -3) {		// Found new item
					labelCount++;
					labels_[i][j] = labelCount;
					labelChart.find(labelCount);
				} else if (labelTopLeft > 0) {						// If diagonal is filled
					labels_[i][j] = labelTopLeft;
				} else if (labelTop + labelTopLeft == -2) {			// If left is filled
					labels_[i][j] = labelLeft;
				} else if (labelLeft + labelTopLeft == -2) {		// If top is filled
					labels_[i][j] = labelTop;
				} else if (labelLeft != labelTop) {					//Found connection
					labelChart.Union(labelLeft, labelTop);
					labels_[i][j] = labelLeft;
				}
			} else {
				labels_[i][j] = -1;
			}
		}
		int Image::getLabel(size_t i, size_t j) const{
			if (i >= num_rows_ || j >= num_columns_) abort();
			return labels_[i][j];
		}

		// Merges connected objects
		void Image::secondPass() {
			//Numbers the objects by changing the value of the root to index
			cout << labelChart.getCollection().size() << " objects detected." << endl;
			for (int i = 0; i < labelChart.getCollection().size(); i++) {
				labelChart.changeValue(labelChart.getCollection()[i], i+1);
			}
			for (int i = 0;i < num_rows_; i++) {
				for (int j = 0; j < num_columns_; j++) {
					int val = labelChart.find(labels_[i][j])->value;
					if (val < 0) {
						val = 0;
					}
					labels_[i][j] = val;
				}
			}
		}
		void Image::recordData(int i, int j, int label) {
			auto it = objects.find(label);
			objData* obj;
			if (it == objects.end()) {
				obj = new objData();
				obj->label = label;
				objects.insert({label, obj});
			} else {
				obj = it->second;
			}
			// Recording total Xs, Ys, and counting pixels
			obj->totalX += j;
			obj->totalY += i;
			obj->area++;
		}

		void Image::findCenters() {
			// 1st moment of area
			for (auto it = objects.begin(); it != objects.end(); it++) {
				findCenter(it->second);
			}	  
		}
		
		void Image::findSecondMoment() {
			recordSecondData();		// Finds A, B, and C
			for (auto it = objects.begin(); it != objects.end(); it++) {
				objData* obj = it->second;
				obj->theta = atan2((obj->b),(obj->a - obj->c))/2;
				//Calculating Emin and Emaxby asin^2(theta) - bsin(theta)cos(theta) + ccos^2(theta)
				obj->E = obj->a * pow(sin(obj->theta),2) - obj->b*sin(obj->theta)*cos(obj->theta) + obj->c*pow(cos(obj->theta),2);
				//Just testing. If pos theta is b/a-c then the neg value must be -b/a-c
				obj->thetaneg = obj->theta*-1;
				obj->Emax = obj->a * pow(sin(obj->thetaneg),2) - obj->b*sin(obj->thetaneg)*cos(obj->thetaneg) + obj->c*pow(cos(obj->thetaneg),2);
				obj->roundness = obj->E/obj->Emax;
			}
		}
		
		void Image::compareData(const string& input_filename) {
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
							//Debugging purposes
							//cout << "Label : " << obj.label << " Label: " << objs->label << endl;
							//cout << areaAcc << " " << EAcc << endl;
							double row = objs->centerRow;
							double col = objs->centerCol;
							int nextCol = (col+50);
							int nextRow = row+50*tan(objs->theta);
							DrawLine(row, col, nextRow, nextCol, 255, objs->label, false);
						}
					}
				}
				input.close();
			} else {
				cout << "Unable to open file";
			}
		}
		void Image::printData(const string& input_filename) const {
			ofstream db(input_filename);
			db << "Label|Area|CenterCol|centerRow|minInertia|orientation|roundness" << "\r\n";
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

		void Image::markCenters() {
			for (auto it = objects.begin(); it != objects.end(); it++) {
				objData* obj = it->second;
				pixels_[obj->centerRow][obj->centerCol] = 255;
			}
		}
		
		void Image::drawOrientation() {
			for (auto it = objects.begin(); it != objects.end(); it++) {
				objData* obj = it->second;
				double row = obj->centerRow;
				double col = obj->centerCol;
				//Mark the point 50 pixels to the right
				int nextCol = (col+50);
				int nextRow = row+50*tan(obj->theta);
				//cout << row << " " << col << " " << nextRow << " " << nextCol << endl;
				DrawLine(row, col, nextRow, nextCol, 255, obj->label, false);
			}
		}
	bool ReadImage(const string &filename, Image *an_image, bool p2Flag) {  
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
				an_image->SetPixel(i, j, byte);
				if (p2Flag)
					an_image->setLabel(i, j, byte);
				if (byte > 0) 
					an_image->recordData(i, j, byte);
			}
		}
		fclose(input);
		return true; 
	}
	
	bool p2(Image *an_image) {
		an_image->secondPass();
	}
	
	bool p3(const string &input_filename, Image *an_image) {
		an_image->findCenters();
		an_image->findSecondMoment();
		
		an_image->drawOrientation();
		an_image->markCenters();
		an_image->printData(input_filename);
	}
	bool p4(const string &input_filename, Image *an_image) {
		an_image->findCenters();
		an_image->findSecondMoment();
		an_image->compareData(input_filename);
		
	}
	bool WriteImage(const string &filename, const Image &an_image, int threshold, bool p1Flag, bool p2Flag) {  
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
				int byte = an_image.GetPixel(i , j);
				if (p1Flag) {
					if (byte < threshold) {
						  byte = 0;
					  } else {
						  byte = 255;
					  }
				}
				if (p2Flag)
					byte = an_image.getLabel(i, j)*30;
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

	// Implements the Bresenham's incremental midpoint algorithm;
	// (adapted from J.D.Foley, A. van Dam, S.K.Feiner, J.F.Hughes
	// "Computer Graphics. Principles and practice", 
	// 2nd ed., 1990, section 3.2.2);  
	void Image::DrawLine(int x0, int y0, int x1, int y1, int color, int label, bool drawHoles) {

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
		
		int savedx = x , savedy = y;
		bool hole = false;
		int byte;
		while (!done) {
			if (x >= num_rows_ || y >= num_columns_)
			break;
			if (drawHoles != true) {
				byte = GetPixel(x, y);
				if (byte == 0) {
					hole = true;
					color = 0;
				} else if (byte == label) {
					color = 255;
					if (hole)
					DrawLine(savedx, savedy, x, y, 255, label, true);
					savedx = x;
					savedy = y;
					hole = false;
				}
				
			}
			SetPixel(x,y,color);
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







