//Ryan Chappelle, Professor Shaghaghi, CSCI 163, 5/19/2022
//Dynamic Programming Project
//Worked on with Jack Corley
//QuickSort from GeeksforGeeks, though I altered it so that it could work with
//vector <pair <string, int>>
//AVL struct also from GeeksforGeeks, though again I altered it so that nodes
//could contain int value which represents the value each word has in terms of
//popularity.
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
using namespace std;

// AVL TREE NODE
struct avl {
	struct avl* left;
	struct avl* right;
	string key;
  int value;
	struct avl* par;
	int height;
};

//AVL FUNCTION DECLARATIONS

//Pre: None
//Post: Function to update the height of a node according to its children's
//node's heights
void Updateheight(struct avl* root);

//Pre: None
//Post: Function that executes a Left Left Rotation
struct avl* LLR(struct avl* root);

//Pre: None
//Post: Function that executes a Right Right Rotation
struct avl* RRR(struct avl* root);

//Pre: None
//Post: Function that executes a Left Right Rotation
struct avl* LRR(struct avl* root);

//Pre: None
//Post: Function that executes a Right Left Rotation
struct avl* RLR(struct avl* root);

//Pre: value > 0
//Post: Function that inserts a node into the avl tree
struct avl* insert(struct avl* root, struct avl* parent, string key,
  int value);

// Function to find a key in AVL tree
//Pre: None
//Post: Function that searches for the string key in the avl tree
int search(struct avl* root, string key);

//DYNAMIC PROGRAMMING FUNCTION DECLARATIONS

//Pre: None
//Post: Function that inserts dictionary3.txt to an avl tree
void dictionaryToAVL(avl*& root);

//Pre: None
//Post: Function that returns all possible outcomes from a compressed text file
//into the vector words.
void dynamicProgramming(string input, vector < pair <string, int> >& words,
	avl* root, string previous = "", int pos = 0, int count = 0);

//Pre: None
//Post: returns if character c is punctuation or not.
bool isPunctuation(char c);

//Pre: None
//Post: Function that sorts the pairs in words from largest to smallest by
//their int value.
void quickSortVector(vector < pair <string, int> >& words, int start, int end);

//Pre: None
//Post: Partition function for quickSortVector
int partition(vector < pair <string, int> >& words, int start, int end);

//MAIN FUNCTION

int main() {
	//AVL creation
	struct avl* root;
	root = NULL;
  dictionaryToAVL(root);
	//File operations
	string myString;
	ifstream myFile("testing.txt"); //testing.txt is the compressed file
	ofstream out("answer.txt"); //answer.txt is where the output is sent to
	myFile >> myString;
	//Gathering combinations and sorting them
	vector < pair < string, int > > words;
	dynamicProgramming(myString, words, root);
	quickSortVector(words, 0, words.size());

	//Output
	out << "*IMPORTANT* ONLY DICTIONARY3.TXT IS TO BE USED" << endl;
	out << endl;
	out << "The following are the possible combinations from testing.txt. ";
	out << "The number before each combination represents the sum of the values ";
	out << "of the words in that particular combination." << endl;
	out << endl;


	for (auto e : words) {
		out << "Value of this combination: " << e.second << endl;
		out << e.first << endl;
		out << endl;
	}

	return 0;
}

//AVL FUNCTION DEFINITIONS

void Updateheight(struct avl* root) {
	if (root != NULL) {
		int val = 1;
		if (root->left != NULL) {
      val = root->left->height + 1;
    }
    if (root->right != NULL) {
      val = max(val, root->right->height + 1);
    }
		root->height = val;
	}
}

struct avl* LLR(struct avl* root) {
	struct avl* tmpnode = root->left;
	root->left = tmpnode->right;
	if (tmpnode->right != NULL) {
    tmpnode->right->par = root;
  }
	tmpnode->right = root;
	tmpnode->par = root->par;
	root->par = tmpnode;
	if (tmpnode->par != NULL && root->key < tmpnode->par->key) {
		tmpnode->par->left = tmpnode;
	}
	else {
		if (tmpnode->par != NULL) {
      tmpnode->par->right = tmpnode;
    }
	}
	root = tmpnode;
	Updateheight(root->left);
	Updateheight(root->right);
	Updateheight(root);
	Updateheight(root->par);
	return root;
}

struct avl* RRR(struct avl* root) {
	struct avl* tmpnode = root->right;
	root->right = tmpnode->left;
	if (tmpnode->left != NULL) {
    tmpnode->left->par = root;
  }
	tmpnode->left = root;
	tmpnode->par = root->par;
	root->par = tmpnode;
	if (tmpnode->par != NULL && root->key < tmpnode->par->key) {
		tmpnode->par->left = tmpnode;
	}
	else {
		if (tmpnode->par != NULL) {
      tmpnode->par->right = tmpnode;
    }
	}
	root = tmpnode;
	Updateheight(root->left);
	Updateheight(root->right);
	Updateheight(root);
	Updateheight(root->par);
	return root;
}

struct avl* LRR(struct avl* root) {
	root->left = RRR(root->left);
	return LLR(root);
}

struct avl* RLR(struct avl* root) {
	root->right = LLR(root->right);
	return RRR(root);
}

struct avl* insert(struct avl* root, struct avl* parent, string key,
  int value) {
	if (root == NULL) {
		root = new struct avl;
		if (root == NULL) {
			cout << "Error in memory" << endl;
		}
		else {
			root->height = 1;
			root->left = NULL;
			root->right = NULL;
			root->par = parent;
			root->key = key;
      root->value = value;
		}
	}
	else if (root->key > key) {
		root->left = insert(root->left, root, key, value);
		int firstheight = 0;
		int secondheight = 0;
		if (root->left != NULL) {
      firstheight = root->left->height;
    }
		if (root->right != NULL) {
      secondheight = root->right->height;
    }
		if (abs(firstheight - secondheight) == 2) {
			if (root->left != NULL && key < root->left->key) {
				root = LLR(root);
			}
			else {
				root = LRR(root);
			}
		}
	}
	else if (root->key < key) {
		root->right = insert(root->right, root, key, value);
		int firstheight = 0;
		int secondheight = 0;
		if (root->left != NULL) {
      firstheight = root->left->height;
    }
		if (root->right != NULL) {
      secondheight = root->right->height;
    }
		if (abs(firstheight - secondheight) == 2) {
			if (root->right != NULL && key < root->right->key) {
				root = RLR(root);
			}
			else {
				root = RRR(root);
			}
		}
	}
	Updateheight(root);
	return root;
}

int search(struct avl* root, string key) {
	if (root == NULL) {
    return 0;
  }
	else if (root->key == key) {
    return root->value;
  }
	else if (root->key > key) {
		int val = search(root->left, key);
		return val;
	}
	else {
		int val = search(root->right, key);
		return val;
	}
}

//DYNAMIC PROGRAMMING FUNCTION DEFINITIONS

void dictionaryToAVL(avl*& root) {
  //count = exact number of words in dictionary3.txt
  //initialized to this instead of 0 so that the most popular words are the
  //highest value
  int count = 39241;
  int value;
  string myString;
  ifstream myFile("dictionary3.txt");
  while(!myFile.eof()) {
    myFile >> myString;
    //if the line is a comment or single letter besides 'I', 'a', or 'A',
    //skip that line
    if (myString[0] == '#' || (myString.length() == 1 && (myString[0] != 'a'
    && myString[0] != 'A' && myString[0] != 'I'))) {
      getline(myFile, myString);
    }
    //else, insert that string into the avl tree with the correct value
    else {
      count--;
      //divided by 200 because each word has its capitalized version too
      value = (count / 200) + 1;
      root = insert(root, NULL, myString, value);
    }
  }
}

void dynamicProgramming(string input, vector < pair <string, int> >& words,
	avl* root, string previous, int pos, int count) {
		 pair<string, int> temp;
		 while (pos != input.length()) {
			 previous += input[pos];
			 pos++;
			 if (search(root, previous) != 0) {
				 //Recursive call
				 dynamicProgramming(input, words, root, previous, pos, count);
				 //Increment count to keep track of current value
				 count += search(root, previous);
				 previous = "";
				 //Split input into two parts, considering punctuation
				 string temp1, temp2;
				 if (isPunctuation(input[pos])) {
					 temp1 = input.substr(0, pos + 1);
					 temp2 = input.substr(pos + 1, input.length());
					 pos++;
				 }
				 else {
					 temp1 = input.substr(0, pos);
					 temp2 = input.substr(pos, input.length());
				 }
				 pos++;
				 //Add whitespace to split up words
				 input = temp1 + " " + temp2;
			 }
			 //If at the end of the file and the last entry was a real word, record
			 //this result.
			 if (pos == input.length()) {
				 if (previous == "") {
					 temp.first = input;
					 temp.second = count;
					 words.push_back(temp);
				 }
				 return;
			 }
		 }
	}

//Helper function
bool isPunctuation(char c) {
	if (c == '.' || c == '!' || c == '?' || c == ',') {
		return true;
	}
	else return false;
}

void quickSortVector(vector < pair <string, int> >& words,
	int start, int end) {
	if (start >= end) {
		return;
	}
	//Partitioning the array
	int p = partition(words, start, end);
	//Sort the left part
	quickSortVector(words, start, p - 1);
	//Sort the right part
	quickSortVector(words, p + 1, end);
}

int partition(vector < pair <string, int> >& words, int start, int end) {
	int pivot = words[start].second;
	int count = 0;
	for (int i = start + 1; i <= end; i++) {
		if (words[i].second >= pivot)
			count++;
	}
	// Giving pivot element its correct position
	int pivotIndex = start + count;
	swap(words[pivotIndex], words[start]);
	// Sorting left and right parts of the pivot element
	int i = start, j = end;
	while (i < pivotIndex && j > pivotIndex) {
		while (words[i].second >= pivot) {
			i++;
		}
		while (words[j].second < pivot) {
			j--;
		}
		if (i < pivotIndex && j > pivotIndex) {
			swap(words[i++], words[j--]);
		}
	}
	return pivotIndex;
}
