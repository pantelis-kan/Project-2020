#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Vertex{

	public:
		Vertex();	//initialize all vertex la
		~Vertex();

		string Get_label();
		void Set_label(string label);

		void Insert(int id);
		void Search_record(int id);

	private:
		string label; // the label of the bucket , example: '00101'
		vector<int> records; // a vector of point ids

};


#endif
