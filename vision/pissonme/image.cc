
// Class for representing a 2D gray-scale image,
// with support for reading/writing pgm images.
// To be used in Computer Vision class.

#include "image.h"
#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <math.h> 
#include <vector>
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

	bool ReadImage(const string &filename, Image *an_image) {  
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
			}
		}
		fclose(input);
		return true; 
	}

	void radius_and_centriods (const string &filename, Image *an_image, int threshold)
	{

		//Covert to binary
		for (int i = 0; i < an_image->num_rows(); ++i)
		{
			for (int j = 0; j < an_image->num_columns(); ++j)
			{
				int byte= an_image->GetPixel(i,j);
				int byte_binary;
				if (byte >= threshold)
				{
					byte_binary = 255;
				}
				if (byte < threshold)
				{
					byte_binary = 0;
				}
				an_image->SetPixel(i, j, byte_binary);
			}
		}

		//FIND CENTER POINTS
		int Xcenter=0;
		int Ycenter=0;
		int area=0;
		for (int i = 1; i < an_image->num_rows(); ++i)
		{
			for (int j = 1; j < an_image->num_columns(); ++j)
			{
				if(an_image->GetPixel(i,j)== 255)
				{
					Xcenter= Xcenter+i;
					Ycenter= Ycenter+j;
					area++;
				}
			}
		}
		//X CENTER
		Xcenter = Xcenter/area;
		//Y CENTER
		Ycenter = Ycenter/area;


		//calculate radius
		int top_i, top_j, left_i, left_j, right_i, right_j, bottom_i, bottom_j, counter;
		counter =0;
		for (int i = 0; i < an_image->num_rows(); ++i) {
			for (int j = 0; j < an_image->num_columns(); ++j) {
				if (an_image->GetPixel(i , j) == 255)
				{
					counter++;
					if (counter==1)
					{
						top_i=i;
						bottom_i =i;
						right_i =i;
						left_i =i;

						top_j=j;
						bottom_j=j;
						right_j=j;
						left_j =j;
					}
					else
					{
						//most bottom
						if(i < top_i)
						{
							top_i=i;
							top_j=j;
						}
						if(i > bottom_i)
						{
							bottom_i=i;
							bottom_j=j;
						}
						if(j < left_j)
						{
							left_i=i;
							left_j=j;
						}
						if(j > right_j)
						{
							right_i=i;
							right_j=j;
						}
					}
				}
			} 
		} 

		if (top_j != bottom_j)
		{
			top_j = bottom_j;
		}

		if (right_i != left_i)
		{
			right_i = left_i;
		}

		double lenghtdifference, widthdifference;
		lenghtdifference = bottom_i - top_i;
		widthdifference = right_j - left_j;
		double temp = lenghtdifference + widthdifference;
		double avg = temp/2;
		double radi = avg/2;
		int radius = ceil(radi);

		ofstream myfile;
		myfile.open (filename);
		myfile << Xcenter;
		myfile << " ";
		myfile << Ycenter;
		myfile << " ";
		myfile << radius;
		myfile.close();
	}

	std::vector<double> calculate_normal(Image *image, int center_x, int center_y, int rad)
	{
		int bright_i, bright_j;
		int brightness=0;
		for (int i = 0; i < image->num_rows(); ++i)
		{
			for (int j = 0; j < image->num_columns(); ++j) 
			{
				int current = image->GetPixel(i , j);
				if (current > brightness)
				{
					brightness= current;
					bright_i=i;
					bright_j=j;
				}
			}
		}
		cout <<bright_i << "  "<<bright_j<<"   "<<brightness<<endl;
		int bright_x = bright_i - center_x;
		int bright_y = bright_j - center_y;
		//(z - zcenter)^2 = -(x - xcenter)^2 -(y - ycenter)^2 + r^2
		int result = pow(rad,2) - pow(bright_x,2)- pow(bright_y,2); 
		double z = sqrt (result);
		//double z = temp +1;

		//scaling the vector
		double sum= pow(bright_x,2) + pow(bright_y,2) + pow(z,2);
		double denominator = sqrt(sum);
		int k = image->GetPixel(bright_i, bright_j);

		cout<<"unscaled  "<<bright_x<<"  "<<bright_y<<endl;

		bright_i = (k* bright_x)/denominator;
		bright_j = (k* bright_y)/denominator;
		z = (k* z)/denominator;

		vector<double> new_vec;
		new_vec.push_back(bright_i);
		new_vec.push_back(bright_j);
		new_vec.push_back(z);

		return new_vec;

	}

	void surface_normals(Image *an_image, Image *an_image1, Image *an_image2, int n_skip, int threshold, const std::string &input_filename, const std::string &filename)
	{
		double out;
		vector<double> output;
		ifstream myReadFile;
		myReadFile.open(input_filename);
		if (myReadFile.is_open()) {
			while (!myReadFile.eof()) {
				myReadFile >> out;
				output.push_back(out);
			}
		}
		myReadFile.close();

		cout<<"My output: "<< output.size()<<endl;

		int size= 3;
		double source_matrix[size][size];
		int value=0;

		for(int i=0; i<size; i++)
		{
			for(int j=0; j<size; j++)
			{ 
				source_matrix[i][j]= output.at(value);
				value++;
			}
		}

		//calculate determinant and find inverse of matrix

		int var = (source_matrix[1][1] * source_matrix[2][2]);
		int var1= (source_matrix[1][2] * source_matrix[2][1]);
		int result = var- var1;
		int result1= result * source_matrix[0][0];

		var = (source_matrix[1][0] * source_matrix[2][2]);
		var1= (source_matrix[1][2] * source_matrix[2][0]);
		result = var- var1;
		int result2= result * source_matrix[0][1];
		result2= result2* (0-1);


		var = (source_matrix[1][0] * source_matrix[2][1]);
		var1= (source_matrix[1][1] * source_matrix[2][0]);
		result = var- var1;
		int result3= result * source_matrix[0][2];

		double determinant= result1 + result2 + result3;
		double inverse_det = (1/determinant);

		//calculate inverse 
		double inverse_matrix[3][3];

		inverse_matrix[0][0] = (source_matrix[1][1] * source_matrix[2][2]) - (source_matrix[2][1] * source_matrix[1][2]);
		inverse_matrix[0][1] = (source_matrix[0][2] * source_matrix[2][1]) - (source_matrix[0][1] * source_matrix[2][2]);
		inverse_matrix[0][2] = (source_matrix[0][1] * source_matrix[1][2]) - (source_matrix[0][2] * source_matrix[1][1]);
		inverse_matrix[1][0] = (source_matrix[1][2] * source_matrix[2][0]) - (source_matrix[1][0] * source_matrix[2][2]);
		inverse_matrix[1][1] = (source_matrix[0][0] * source_matrix[2][2]) - (source_matrix[0][2] * source_matrix[2][0]);
		inverse_matrix[1][2] = (source_matrix[1][0] * source_matrix[0][2]) - (source_matrix[0][0] * source_matrix[1][2]);
		inverse_matrix[2][0] = (source_matrix[1][0] * source_matrix[2][1]) - (source_matrix[2][0] * source_matrix[1][1]);
		inverse_matrix[2][1] = (source_matrix[2][0] * source_matrix[0][1]) - (source_matrix[0][0] * source_matrix[2][1]);
		inverse_matrix[2][2] = (source_matrix[0][0] * source_matrix[1][1]) - (source_matrix[1][0] * source_matrix[0][1]);

		for(int i=0; i<3; i++)
		{
			for(int j=0; j<3; j++)
			{
				inverse_matrix[i][j]=inverse_matrix[i][j]* inverse_det;  
			}
		}

		double I_1, I_2, I_3, variable1, variable2, variable3;
		Image image;
		image.AllocateSpaceAndSetSize(an_image->num_rows(), an_image->num_columns());
		image.SetNumberGrayLevels(255); 

		for (int i = 0; i < an_image->num_rows(); i++)
		{
			for (int j = 0; j < an_image->num_columns(); j++) 
			{
				image.SetPixel(i,j, an_image->GetPixel(i,j));
			}
		}
		int count=0;

		for (int i = 0; i < an_image->num_rows(); i= i+ n_skip)
		{
			for (int j = 0; j < an_image->num_columns(); j= j+ n_skip) 
			{
				//count++;
				//if(count == n_skip)
				//{
				//count =0;
				if((an_image->GetPixel(i , j) > threshold) && (an_image1->GetPixel(i , j) > threshold) && (an_image2->GetPixel(i , j) > threshold))
				{
					const double an_image_pixel = an_image->GetPixel(i,j);
					const double an_image1_pixel = an_image1->GetPixel(i,j);
					const double an_image2_pixel = an_image2->GetPixel(i,j);
					//calculate normal 
					variable1 = inverse_matrix[0][0] * an_image_pixel;
					variable2 = inverse_matrix[0][1] * an_image1_pixel;
					variable3 = inverse_matrix[0][2] * an_image2_pixel;
					I_1 = variable1 + variable2 + variable3;

					variable1 = inverse_matrix[1][0] * an_image_pixel;
					variable2 = inverse_matrix[1][1] * an_image1_pixel;
					variable3 = inverse_matrix[1][2] * an_image2_pixel;
					I_2 = variable1 + variable2 + variable3;

					variable1 = inverse_matrix[2][0] * an_image_pixel;
					variable2 = inverse_matrix[2][1] * an_image1_pixel;
					variable3 = inverse_matrix[1][2] * an_image2_pixel;
					I_3 = variable1 + variable2 + variable3;

					double temp = (I_1*I_1)+ (I_2*I_2) + (I_3*I_3);
					double len = sqrt(temp);

					I_3 = I_3 / len;
					I_2 = I_2 / len;
					I_1 = I_1 / len;

					I_3 = I_3 * 10;
					I_2 = I_2 * 10;
					I_1 = I_1 * 10;

					DrawLine(i,j,(i+I_1),(j+I_2),255,&image);  //draw the needles on the image
					image.SetPixel(i, j, 0);
				}
				//}
			}
		}

		if (!WriteImage(filename, image)){
			cout << "Can't write to file " << filename << endl;
		}
	}



	void albedo(Image *an_image, Image *an_image1, Image *an_image2, int threshold, const std::string &input_filename, const std::string &filename)
	
		double out;
		vector<double> output;
		ifstream myReadFile;
		myReadFile.open(input_filename);
		if (myReadFile.is_open()) {
			while (!myReadFile.eof()) {
				myReadFile >> out;
				output.push_back(out);
			}
		}
		myReadFile.close();

		cout<<"My output: "<< output.size()<<endl;

		int size= 3;
		double source_matrix[size][size];
		int value=0;

		for(int i=0; i<size; i++)
		{
			for(int j=0; j<size; j++)
			{ 
				source_matrix[i][j]= output.at(value);
				value++;
			}
		}

		//calculate determinant and find inverse of matrix

		int var = (source_matrix[1][1] * source_matrix[2][2]);
		int var1= (source_matrix[1][2] * source_matrix[2][1]);
		int result = var- var1;
		int result1= result * source_matrix[0][0];

		var = (source_matrix[1][0] * source_matrix[2][2]);
		var1= (source_matrix[1][2] * source_matrix[2][0]);
		result = var- var1;
		int result2= result * source_matrix[0][1];
		result2= result2* (0-1);


		var = (source_matrix[1][0] * source_matrix[2][1]);
		var1= (source_matrix[1][1] * source_matrix[2][0]);
		result = var- var1;
		int result3= result * source_matrix[0][2];

		double determinant= result1 + result2 + result3;
		double inverse_det = (1/determinant);

		//calculate inverse 
		double inverse_matrix[3][3];

		inverse_matrix[0][0] = (source_matrix[1][1] * source_matrix[2][2]) - (source_matrix[2][1] * source_matrix[1][2]);
		inverse_matrix[0][1] = (source_matrix[0][2] * source_matrix[2][1]) - (source_matrix[0][1] * source_matrix[2][2]);
		inverse_matrix[0][2] = (source_matrix[0][1] * source_matrix[1][2]) - (source_matrix[0][2] * source_matrix[1][1]);
		inverse_matrix[1][0] = (source_matrix[1][2] * source_matrix[2][0]) - (source_matrix[1][0] * source_matrix[2][2]);
		inverse_matrix[1][1] = (source_matrix[0][0] * source_matrix[2][2]) - (source_matrix[0][2] * source_matrix[2][0]);
		inverse_matrix[1][2] = (source_matrix[1][0] * source_matrix[0][2]) - (source_matrix[0][0] * source_matrix[1][2]);
		inverse_matrix[2][0] = (source_matrix[1][0] * source_matrix[2][1]) - (source_matrix[2][0] * source_matrix[1][1]);
		inverse_matrix[2][1] = (source_matrix[2][0] * source_matrix[0][1]) - (source_matrix[0][0] * source_matrix[2][1]);
		inverse_matrix[2][2] = (source_matrix[0][0] * source_matrix[1][1]) - (source_matrix[1][0] * source_matrix[0][1]);

		for(int i=0; i<3; i++)
		{
			for(int j=0; j<3; j++)
			{
				inverse_matrix[i][j]=inverse_matrix[i][j]* inverse_det;  
			}
		}

		double I_1, I_2, I_3, variable1, variable2, variable3;
		double albedo_max = 0;
		Image image;
		image.AllocateSpaceAndSetSize(an_image->num_rows(), an_image->num_columns());
		image.SetNumberGrayLevels(255); 



		for (int i = 0; i < an_image->num_rows(); i++)
		{
			for (int j = 0; j < an_image->num_columns(); j++) 
			{
				//count++;
				//if(count == n_skip)
				//{
				//count =0;
				if((an_image->GetPixel(i , j) > threshold) && (an_image1->GetPixel(i , j) > threshold) && (an_image2->GetPixel(i , j) > threshold))
				{
					const double an_image_pixel = an_image->GetPixel(i,j);
					const double an_image1_pixel = an_image1->GetPixel(i,j);
					const double an_image2_pixel = an_image2->GetPixel(i,j);
					//calculate normal 
					variable1 = inverse_matrix[0][0] * an_image_pixel;
					variable2 = inverse_matrix[0][1] * an_image1_pixel;
					variable3 = inverse_matrix[0][2] * an_image2_pixel;
					I_1 = variable1 + variable2 + variable3;

					variable1 = inverse_matrix[1][0] * an_image_pixel;
					variable2 = inverse_matrix[1][1] * an_image1_pixel;
					variable3 = inverse_matrix[1][2] * an_image2_pixel;
					I_2 = variable1 + variable2 + variable3;

					variable1 = inverse_matrix[2][0] * an_image_pixel;
					variable2 = inverse_matrix[2][1] * an_image1_pixel;
					variable3 = inverse_matrix[1][2] * an_image2_pixel;
					I_3 = variable1 + variable2 + variable3;

					double temp = (I_1*I_1)+ (I_2*I_2) + (I_3*I_3);
					double len = sqrt(temp);

					if(len > albedo_max)
					{
						albedo_max=len;
					}
					I_3 = I_3 / len;
					I_2 = I_2 / len;
					I_1 = I_1 / len;

					I_3 = I_3 * 10;
					I_2 = I_2 * 10;
					I_1 = I_1 * 10;


					image.SetPixel(i,j,len * 255); //set pixel to the value of albedo, scale it with 255 (max gray level)
				}
				else
				{
					image.SetPixel(i,j, 0);
				}
				//}
			}
		}

		//scale 
		for(int i = 0; i < image.num_rows(); i++)
		{
			for (int j = 0; j < image.num_columns(); j++)
			{
				double current_pixel = image.GetPixel(i,j);
				double value = current_pixel/albedo_max;
				image.SetPixel(i,j,value);
			}
		}

		if (!WriteImage(filename, image)){
			cout << "Can't write to file " << filename << endl;
		}

	}


	bool WriteImage(const string &filename, const Image &an_image) {  
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
			an_image->SetPixel(x,y,color);

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







