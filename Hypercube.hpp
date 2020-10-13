#ifndef HYPERCUBE_HPP
#define HYPERCUBE_HPP

#include <iostream>
#include <map>
#include <string>

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

#endif
