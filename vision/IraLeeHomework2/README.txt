This program was designed as a basic binary image processor.
Program 1 converts an image into a binary image with a threshold of 120.
Program 2 uses the Union Find data structure to group objects together. It also assigns a byte to pixels of the same object. Shades in increments of 30.
Program 3 computes useful information about the object. Area, centroid, min inertia, and orientation are calculated. I also tried to calculate roundness.
Program 4 compares data from a database to objects in an image to see if there are any matches. I used the area and inertia to measure the accuracy.

Everything should be working 100%. Please contact me if otherwise.

The instructions are as follows :
./p1 inputFile threshold outputfile
./p2 inputFile outputFile
./p3 inputFile dbFile outputFile
./p4 inputFile dbFile outputFile

The inputs are:
objects.pgm
many_objects_1.pgm
many objects_2.pgm

The outputs are:
output.pgm
output_label.pgm
output_data.pgm
db.txt
output_match.pgm

output1.pgm
output1_label.pgm
output1_data.pgm
db1.txt
output1_match.pgm

output2.pgm
output2_label.pgm
output2_data.pgm
db2.txt
output2_match.pgm


The biggest issue I had with the program was calculating a point on the line. It was trivial, but I thought I messed up somewhere else so I didn't check the error thoroughly.
Another issue were the segmentation faults. I managed to fix it by making sure the objects existed before accessing the properties.
It was nice to have used Union Find. It really showed me how useful data structures are.

