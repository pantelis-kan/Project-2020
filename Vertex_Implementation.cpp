#include <iostream>

#include "Vertex.hpp"

	Vertex::Vertex(){

    }
	
    Vertex::~Vertex(){

    }

	void Vertex::Insert(int id){
	string Vertex::Get_label(){
        return label;
    }

	void Vertex::Set_label(string label_to_set){
        label = label_to_set;
    }
	
    void Vertex::Insert(int id){
        records.push_back(id);
    }

    void Vertex::Search_record(int id){
        
    }
	
    void Vertex::Search_record(int id){
        vector<int>::iterator it;
        it = find(records.begin(), records.end(), id);
        
        if (it != records.end())
            cout << "Element found in myvector: " << *it << '\n';
        else
            cout << "Element not found in myvector\n";

    }

void Vertex::print_records(){
    for (int i = 0; i < records.size(); i++) {
        cout << records.at(i) << ' ';
    }
    cout << endl;
}

