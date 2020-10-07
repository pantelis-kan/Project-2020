#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Vertex{

	public:
		Vertex();
		~Vertex();
		void Insert(int id);
	

	private:
		string label; // the label of the bucket , example: '00101'
		vector<int> records; // a vector of point ids

};


#endif
