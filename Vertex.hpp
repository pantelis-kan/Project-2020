#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Vertex{

	public:
		Vertex();	//initialize all vertex labels?
		~Vertex();

		string Get_label();
		void print_records();
		void Set_label(string label);

		void Insert(int id);
		void Search_record(int id);
		void Search_record(int id); //searches for an id. only prints for now
		
		string label; // the label of the bucket , example: '00101'
		vector<int> records; // a vector of point ids		

	private:


};


#endif
