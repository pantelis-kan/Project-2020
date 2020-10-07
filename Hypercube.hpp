#ifndef HYPERCUBE_HPP
#define HYPERCUBE_HPP

#include <iostream>
#include <map>
#include <string>

#include "Vertex.hpp"

using namespace std;

// a collection of vertices

class Hypercube{

	public:
		Hypercube();
		~Hypercube();
		void Insert(string tag,int id);
		void Insert_to_F(int h); // create f(h) and store it in map 

	private:
		Vertex* vertex_table;
		map<int,char> F_function;

};

#endif
