#ifndef HYPERCUBE_HPP
#define HYPERCUBE_HPP

#include <iostream>
#include <map>
#include <string>
#include <vector>

class Point_Array;
#include "Point.hpp"
#include "Vertex.hpp"

using namespace std;

/* Collection of k vertices
*/
class Hypercube{
	public:
		Hypercube(int k);
		~Hypercube();

		inline int get_vertex_t_size(){
			return vertex_t_size;
		}

		void Map_images(Point_Array& input, int N, int k, double** s_params, 
					int M, long long int m, double w, Hypercube* hcube); //internally manages the hashing, mapping and insertion of points
		
		void Insert(string label, int id); //inserting image with id in label=tag
		string Search_by_label(string label); //searches vertex_table and returns position, if not found returns -1
		
		void print_vertex_table();

		vector<int>* retrieve_records_vector(string query_label);

		char Insert_to_F(int h); // create f(h) and store it in map

	private:
		Vertex* vertex_table;
		map<int, char> F_function; //mapping h values with the 0 or 1
		int vertex_t_size = 0;
};

//based on initial_label we create a map with all the possible labels with ham_dist = 1,2,etc
class Hamming{
	public:
		Hamming(string initial_label);
		~Hamming();
		
		int get_usedprobes();
		void print();


		string move_to_next();

	private:
		string initial_label;
		string current_label_in_use;
		int used_probes = 0;		//everytime we use hamming we add a probe so we can stop when thresold is reached
//		map<string, int> labels_Hamming;			//label, ham_dist; 
		vector <pair <string, int> > labels_Hamming;

};


#endif
