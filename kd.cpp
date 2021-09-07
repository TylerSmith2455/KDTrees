#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<iomanip>
#include<vector>
#include<cmath>
using namespace std;

const int b = 10;

// Node structure
struct Node {
	bool innerNode;
	int median;
	int dimension;
	vector<vector<int>> bucket;
	Node *left, *right;
};

// Creates a new internal node
struct Node* newInternalNode(int med, int dem) {
	struct Node* temp = new Node;
	temp->median = med;
	temp->dimension = dem;
	temp->innerNode = 1;
	temp->left = temp->right = NULL;
	return temp;
}

// Creates a new leaf node
struct Node* newLeafNode(vector<vector<int>> newBucket) {
	struct Node* temp = new Node;
	temp->bucket = newBucket;
	temp->innerNode = 0;
	temp->left = temp->right = NULL;
	return temp;
}

// Finds best dimension to discriminate on
int bestDimension(vector<vector<int>> points) {
	int BestDimension = 0;   // Hold best dimension to discrinate on
	int widestRange = 0;     // Hold widest range to determine best dimension
	
	// Double For loop calculates the range for each dimensiona nd then determines which range is greater
	for (double i = 0; i < points[0].size(); i++) {
		int range = 0;
		int lowVal = points[0][i];  // lowVal set to first element of given dimension
		int highVal = points[0][i]; // highVal set to first element of given dimension
		for (double j = 1; j < points.size(); j++) {  // Find lowVal and highVal
			if (points[j][i] < lowVal)
				lowVal = points[j][i];
			if (points[j][i] > highVal)
				highVal = points[j][i];
		}
		range = highVal - lowVal;        // Calculate the range
		if (range > widestRange) {       // Ties go to the earlier dimension
			widestRange = range;
			BestDimension = i;
		}
	}
	return BestDimension;       // Return best dimension
}

// Standard partitioning method for quick sort to be used for quick select
int partition(vector<vector<int>> &points, int dimension, int left, int right) {
	int x = points[right][dimension]; 
	int i = (left - 1);
	for (double j = left; j <= right - 1; j++) {
		if (points[j][dimension] <= x) {
			i++;
			swap(points[i][dimension], points[j][dimension]);
		}
	}
	swap(points[i + 1][dimension], points[right][dimension]);
	return (i + 1);
}

int findMedianValue(vector<vector<int>> points, int dimension, int left, int right, int med) {
	vector<vector<int>> temp = points;
	while (left <= right) {
		// partition around last element and get position of pivot
		int index = partition(temp, dimension, left, right);

		// If position of the pivot is same as median
		if (index == med - 1)
			return temp[index][dimension];

		// If pivot is more than median, the median must be on the left
		else if (index > med - 1)
			right = index - 1;
		
		// else smallest is on the right
		else
			left = index + 1;
	}
	return -1; // Return -1 on invalid values
}	

void makeKDTree(struct Node** root, vector<vector<int>> points) {
	int bestDim, medianValOne, medianValTwo;
	vector<vector<int>> leftPoints, rightPoints;
	
	// Make sure the vector is greater than 10
	if (points.size() > b) {	// Inner Node 
		bestDim = bestDimension(points);   // Find the best dimension to discriminate on

		if (points.size() % 2 == 0) {      // If the size of vector is even, then calculate the average of the middle values
			medianValOne = findMedianValue(points, bestDim, 0, points.size() - 1, points.size()/2);
			medianValTwo = findMedianValue(points, bestDim, 0, points.size() - 1, points.size()/2 + 1);
			medianValOne = (medianValOne + medianValTwo) / 2;
		}
		else                               // If the size of the vector is odd, calculate the median
			medianValOne = findMedianValue(points, bestDim, 0, points.size() - 1, ceil(points.size()/2));
		
		// Double for loop to break the vector into two seperate vectors based upon the best dimension and the median value
		for (double i = 0; i < points.size(); i++) {
			vector<int> leftTemp, rightTemp;
			for (double j = 0; j < points[0].size(); j++) {
				if (points[i][bestDim] <= medianValOne) {
					leftTemp.push_back(points[i][j]);
				}
				else { 
					rightTemp.push_back(points[i][j]);
				}
			}
			if (points[i][bestDim] <= medianValOne)
				leftPoints.push_back(leftTemp);
			else rightPoints.push_back(rightTemp);
		} 
		
		// If the root node is null, make one
		if (!(*root))
			*root = newInternalNode(medianValOne,bestDim);
		
		// Recur for each sub vector to keep building the tree
		makeKDTree(&( (*root)->left), leftPoints);
		makeKDTree(&( (*root)->right), rightPoints);
	}
	else {		// Leaf Node
		// If the root node is NULL, make one
		if (!(*root)) {
			*root = newLeafNode(points);
			return;
		}
	}
}

void printProbeBucket(struct Node** root, vector<int> probe) {
	// If root node is an inner node
	if ((*root)->innerNode == 1) {   // Inner Node
		// If the probes value is less than or equal to the roots based on dimension, recur ti the left
		if (probe[(*root)->dimension] <= (*root)->median)
			printProbeBucket(& ( (*root)->left), probe);
		else  // Otherwise recur to the right
			printProbeBucket(& ( (*root)->right), probe);
	}
	else {  // Leaf Node 
		cout << "probe ";

		// Print the probes values
		for (double i = 0; i < probe.size(); i++)
			cout << probe[i] << " ";

		// Print the buckets size
		cout << "reaches bucket: " << (*root)->bucket.size() << " elements\n";

		// Print all the buckets values
		for (double i = 0; i < (*root)->bucket.size(); i++) {
			for (double j = 0; j < (*root)->bucket[0].size(); j++) {
				cout << (*root)->bucket[i][j] << " ";
			}
			cout << "\n";
		}
	}		
}

int main(int argc, char *argv[]) {
	int k = stoi(argv[1]);     // number of dimensions
	int n = stoi(argv[2]);     // number of data points
	int p = stoi(argv[3]);     // number of probes

	// 2D vector to hold points
	vector<vector<int>> points;
	int point;
	struct Node *root = NULL;

	// populate vector to hold points
       	for (int i = 0; i < n; i++) {
		vector<int> temp;    // temporary vector to populate the 2D vector
		for (int j = 0; j < k; j++) {
			cin >> point;
			temp.push_back(point);
		}
		points.push_back(temp);
	}

	// Create the KD Tree by using a 2D vector of points and a root Node
	makeKDTree(&root,points);
	
	// Get each probe's value
	for (int i = 0; i < p; i++) {
		vector<int> temp;  // temporary vector to hold the probe point
		for (int j = 0; j < k; j++) {
			cin >> point;
			temp.push_back(point);
		}
		printProbeBucket(&root, temp);    // Print the probe's point and the bucket it would go in
	}
	
	return 0;
}
