cpp
===

Description
-----------

Code for running nearest neighbor algorithms

Usage
-----

data_set.h & data_set.cpp
```
data_set class
void load(data_set & st, FILE * in)
    loads the vectors from a file
void label(data_set & st, FILE * in)
	loads the labels from a file
max_variance_index(data_set & sub)
	return the index of the vector with the max variance in the passed in data_set
```

vector_math.h & vector_math.cpp
```
typedef vector <double> euclid_vectors
	define euclid_vector as a vector of double
double distance_to(euclid_vector & v1, euclid_vector & v2)
	return euclidean distance between two vectors
double selector(vector<double> s, int k)
	return the kth smallest element in the passed in vector.
```

nn.h & nn.cpp
```
euclid_vector * nn(euclid_vector * query, data_set & data)
	gets the nearest neighbor of query from data
data_set k_nn(int k, euclid_vector * query, data_set & data)
	gets the k nearest neighbor of query from data
data_set c_approx_nn(double c, euclid_vector * query, data_set & data, euclid_vector * nn)
	return the data_set of all vector within c times nearest neighbor distance of the query
euclid_vector * kd_tree_nn(euclid_vector * query, data_set & data, kd_tree_node * root)
	gets nearest neighbor of query through kd_tree
data_set kd_tree_k_nn(int k, euclid_vector * query, data_set & data, kd_tree_node * root)
	gets k nearest neighbor of query through kd_tree
euclid_vector * virtual_spill_tree_nn(euclid_vector * query, data_set & data, virtual_spill_tree_node * root)
	gets nearest neighbor of query through query_tree
data_set virtual_spill_tree_k_nn(int k, euclid_vector * query, data_set & data, virtual_spill_tree_node * root)
	gets k nearest neighbor of query through query_tree
```

kd_tree.h & kd_tree.cpp
```
class kd_tree_node
    vector<int> kd_subdomain(euclid_vector * query, kd_tree_node * root)
    	get subdomain from the leaf of the kd_tree for a given query
    kd_tree_node * kd_tree(double c, data_set & data)
		generates a kd_tree for given data, with leaf size c times train data size
	kd_tree_node * spill_tree(double c, double a, data_set & data)
		generates a spill_tree for given data, with leaf size c times train data size
	void save_kd_tree(kd_tree_node * root, FILE * out)
		saves kd_tree into a given file
    kd_tree_node * load_kd_tree(FILE * in)
		loads kd_tree from a given file
```

virtual_spill_tree.h & virtual_spill_tree.cpp
```
class virtual_spill_tree_node
vector <int> virtual_spill_subdomain(euclid_vector * query, virtual_spill_tree_node * root)
	get subdomain from the leaf of the virtual_spill_tree for a given query
virtual_spill_tree_node * virtual_spill_tree(double a, kd_tree_node * root, data_set & data)
	generates a virtual_spill_tree for given data, with range parameter a based on kd_tree
```

test.h & test.cpp
```
test_setup(char * path)
	load data from files
```
