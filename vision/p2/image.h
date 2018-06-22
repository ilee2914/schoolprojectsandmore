// Class for representing a 2D gray-scale image,
// with support for reading/writing pgm images.
// To be used in Computer Vision class.

#ifndef COMPUTER_VISION_IMAGE_H_
#define COMPUTER_VISION_IMAGE_H_

#include <cstdlib>
#include <string>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <iomanip> 
#include <vector>

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
struct Node{
	int value = -1;
	Node* prev = nullptr;
	vector<Node*> next;
};
		
class UnionFind {
	public:
		vector<Node*> collection;				//collection of roots
		unordered_map<int, Node*> nodeLocs;		//location of nodes for easy find for values
		Node* add(int num) {
			if (nodeLocs.find(num) == nodeLocs.end()) {
				Node* newNode = new Node();
				newNode->value = num;
				collection.push_back(newNode);
				nodeLocs.insert({newNode->value, newNode});
				return newNode;
			}
			return nullptr;
		}
		Node* find(int num) {
			Node* node;
			auto it = nodeLocs.find(num);
			
			if (it != nodeLocs.end()) {		//If node exists
				node = it->second;
			} else {						//Create a new node if doesn't
				return add(num);
			}
			
			if(node->prev == nullptr)		//if is root
				return node;
			Node* temp = node;
			while (temp->prev != nullptr)		//temp now equals root of its tree
				temp = temp->prev;
			if (node->prev == temp)				//If node was already connected to root
				return temp;
			temp->next.push_back(node);			//ISets node as child to parent
			vector<Node*> x = node->prev->next;
			for (int i = 0; i < x.size(); i++) {	//Removes link from previous parent
				if (x[i]->value == node->value) {
					x.erase(x.begin() + i);
					break;
				}
			}
			if (node == temp) {
				return node;
			}
			node->prev = temp;			//Sets new parent to root
			return temp;
		}
		
		bool Union(int a, int b) {
			Node* aRoot = find(a);
			
			Node* bRoot = find(b);
			if (aRoot == nullptr || bRoot == nullptr) {
				return false;
			}
			if (aRoot == bRoot) {			//already connected
				return true;
			}
			aRoot->next.push_back(bRoot);
			bRoot->prev = aRoot;
			
			for (int i = 0; i < collection.size(); i++) {		//Remove the root from the collection
				if (collection[i]->value == bRoot->value) {
					collection.erase(collection.begin() + i);
					break;
				}
			}
			return true;
		}
		
		void changeValue(Node * a, int num) {
			a->value = num;
		}
		
		void printRoots() {
			for (int i = 0; i < collection.size(); i++) {
				cout << collection[i]->value<< " ";
			}
		}
		
		
};
class Image {
 public:
  Image(): num_rows_{0}, num_columns_{0}, 
	   num_gray_levels_{0}, labelCount{0}, pixels_{nullptr}, labels_{nullptr}{ }
  
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
  
  // -2 for unset
  // -1 for no label
  // labelCount keeps track over number of labels there are.
  //	0 for none
  void setLabel(size_t i, size_t j, int gray_level) {
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
		   } else if (labelTop + labelTopLeft == -2) {							// If left is filled
			   labels_[i][j] = labelLeft;
		   } else if (labelLeft + labelTopLeft == -2) {							// If top is filled
			   labels_[i][j] = labelTop;
		   } else if (labelLeft != labelTop) {				//Found connection
			   labelChart.Union(labelLeft, labelTop);
			   labels_[i][j] = labelLeft;
		   }
	   } else {
		   labels_[i][j] = -1;
	   }
  }
  
  // Merges connected objects
  void secondPass() {
	  //Numbers the objects by changing the value of the root to index
	  cout << labelChart.collection.size() << " objects detected." << endl;
	  for (int i = 0; i < labelChart.collection.size(); i++) {
		  labelChart.changeValue(labelChart.collection[i], i+1);
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
  
  int GetPixel(size_t i, size_t j) const {
    if (i >= num_rows_ || j >= num_columns_) abort();
    return pixels_[i][j];
  }
  
  int getLabel(size_t i, size_t j) const{
	  if (i >= num_rows_ || j >= num_columns_) abort();
	  return labels_[i][j];
  }

 private:
  void DeallocateSpace();

  size_t num_rows_; 
  size_t num_columns_; 
  size_t num_gray_levels_;  
  int **pixels_;
  int **labels_;
  int labelCount;
  UnionFind labelChart;
  //Keeps track of what labels belong where
};
void doIt(Image *an_image) ;
// Reads a pgm image from file input_filename.
// an_image is the resulting image.
// Returns true if  everyhing is OK, false otherwise.
bool ReadImage(const std::string &input_filename, Image *an_image);

// Writes image an_iamge into the pgm file output_filename.
// Returns true if  everyhing is OK, false otherwise.
bool WriteImage(const std::string &output_filename, const Image &an_image);

//  Draws a line of given gray-level color from (x0,y0) to (x1,y1);
//  an_image is the output_image.
// IMPORTANT: (x0,y0) and (x1,y1) can lie outside the image 
//   boundaries, so SetPixel() should check the coordinates passed to it.
void DrawLine(int x0, int y0, int x1, int y1, int color,
	      Image *an_image);

}  // namespace ComputerVisionProjects

#endif  // COMPUTER_VISION_IMAGE_H_
