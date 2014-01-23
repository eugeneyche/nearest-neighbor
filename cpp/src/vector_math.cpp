#include "vector_math.h"
#include <random>
using namespace std;

double distance_to( euclid_vector & v1, euclid_vector & v2)
{
    if (v1.size() != v2.size())
        return -1;
    double distance = 0;
    for (int i = 0; i < v1.size(); i++)
    {
        double d = v2[i] - v1[i];
        distance += d * d;
    }
    return distance;
}

double selector(vector<double> s, int k)
{
	srand(int(time(NULL))); //random seed
	int size = int(s.size());
	double randomIndex = rand() % size; //random number between 0 and size-1
    
	vector<double> left; //data that are smaller than v
	vector<double> right; // data that are larger than v
	vector<double> v; //data that are equal to v
    
    /* iterate through to push the smaller to left, larger to right, the rest to v */
	for (vector <double>::iterator itr = s.begin(); itr != s.end(); itr++)
	{
		if(*itr == s[randomIndex]){
			v.push_back(*itr);
		}
		else if(*itr < s[randomIndex]){
			left.push_back(*itr);
		}
		else{
			right.push_back(*itr);
		}
	}
    
    /* three conditions */
	if (left.size() >= k)
    { // the kth smallest on the left
		return selector(left, k);
	}
	else if(left.size()+v.size() >= k)
    { // the kth smallest found
		return s[randomIndex];
	}
	else
    { // the kth smallest on the right
		return selector(right, int (k-left.size()-v.size()));
	}
}


