#include "nn.h"
#include <iostream>
#include <time.h>

using namespace std;

euclid_vector * nn(euclid_vector * test, data_set & train_st)
{
    euclid_vector * mn = NULL;
    double mn_dist = 0;
    double l_dist = 0;
    for (int i = 0; i < train_st.size(); i++)
    {
        l_dist = distance_to(*test, *train_st[i]);
        if (mn == NULL || l_dist < mn_dist)
        {
            mn_dist = l_dist;
            mn = train_st[i];
        }
    }
    return mn;
}

double selector(vector <double> s, double k)
{
	srand(time(NULL)); //random seed
	int size = s.size();
	double randomIndex = rand() % size; //random number between 0 and size-1
	
	vector <double> left; //data that are smaller than v
	vector <double> right; // data that are larger than v
	vector <double> v; //data that are equal to v
    
    /* iterate through to push the smaller to left, larger to right, the rest to v */
	for (vector <double>::iterator itr = s.begin(); itr != s.end(); itr++)
	{
		if (*itr == s[randomIndex])
        {
			v.push_back(*itr);
		}
		else if (*itr < s[randomIndex])
        {
			left.push_back(*itr);
		}
		else
        {
			right.push_back(*itr);
		}
	}

    /* three conditions */
	if (left.size() >= k)
    {// the kth smallest on the left
		return selector(left, k);
	}
	else if (left.size() < k && left.size()+v.size() >= k)
    { // the kth smallest found
		return s[randomIndex];
	}
	else
    { // the kth smallest on the right
		return selector(right, k-left.size()-v.size());
	}
    return 0;
}

data_set k_nn(euclid_vector * test, data_set & train_st, double k)
{
    map <euclid_vector * , double> dist_mp;
    vector <double> dist_vtr; 
    for (int i = 0; i < train_st.size(); i++)
    {
        double dist = distance_to(*test, *(train_st[i]));
        dist_mp[train_st[i]] = dist;
        dist_vtr.push_back(dist);
    }
    double k_dist = selector(dist_vtr, k);
    vector <int> domain;
    for (int i = 0; domain.size() < k && i < train_st.size(); i++)
    {
        if (dist_mp[train_st[i]] <= k_dist)
        {
            vector <int>::iterator itr = domain.begin();
            while (itr != domain.end()&& dist_mp[train_st[*itr]] < dist_mp[train_st[i]])
            {
                itr++;
            }
            if (itr != domain.end())
                domain.insert(itr, i);
            else
                domain.push_back(i);
        }
    }
    return train_st.subset(domain);
}
