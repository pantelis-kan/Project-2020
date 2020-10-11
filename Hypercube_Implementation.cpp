#include "Hypercube.hpp"

using namespace std;

//default_random_engine rand_generator(time(NULL));

Hypercube::Hypercube(int k){
	vertex_table = new Vertex[k];
}

Hypercube::~Hypercube(){
//    delete[] vertex_table;
}


//internally manages the hashing, mapping and insertion of points
void Hypercube::Map_images(Point_Array& input, int N, int k, double** s_params, 
					int M, long long int m, double w){

	string label;

	//create Hypercube with vertex_table size 2^k
	vertex_t_size = pow(2,k);
	vertex_table = new Vertex[vertex_t_size];
	

	cout << "vertextable_size is" << vertex_t_size << endl;

	//Run compute_f : Returns the necessary bitstring (necessary hashvalue)
	//Insert label and id to vertex
	//for (int i = 0; i < N; i++){
	for (int i = 0; i < 5; i++){
		label = input.Compute_f(i, k, M, m, w, s_params, vertex_table);
		Insert(label, i+1);
	}
	

}


//inserting image with id in label=tag
void Hypercube::Insert(string label, int id){
	
	if(vertex_t_size == 0){
		cout << "vertex table not allocated" << endl;
		exit(-1);
	}

	//Find the vertex in vertex_table based on label and call insert with id
	for (int i = 0; i < vertex_t_size; i++){
		if (label.compare(vertex_table[i].Get_label()) == 0){
		//found it so we can insert id
			vertex_table[i].Insert(id);
			break;
		}	
	}

	//label not existing so we need to save it and insert id	
	if (i == vertex_t_size){ 
		for (int j = 0; j < vertex_t_size; j++){
			if ((vertex_table[j].Get_label()).empty()){
				vertex_table[j].Set_label(label);
				vertex_table[j].Insert(id);
				break;
			}
		}
	}
	
}


/* Checks if key (LSH hash value) exists in map, 
	if not flips a coin for 0,1
	if yes returns the binary value
*/
char Hypercube::Insert_to_F(int h){
	srand (time(NULL));
	int coin;
	char binary_value;

	map<int,char>::iterator it;

	// using find() to search for 'b'  
	// key found 
	// "it" has address of key value pair. 
	it = F_function.find(h); 
	
	if(it == F_function.end()){
		cout << "Key-value pair not present in map" ;
		coin = rand() % 2; 
		if(coin == 0){
			binary_value= '0';
		}
		else if (coin == 1){
			binary_value= '1';
		}
		else{
			cout << "something went very wrong! Hypercube_Implementation.cpp" << endl;
			exit(-1);
		}
		F_function.insert(pair<char,int>(h, binary_value));
	
	}
	else{
//		cout << "Key-value pair present : " << it->first << "->" << it->second ; 
	}
	return binary_value;
	
}

//searches vertex_table and returns position, if not found returns -1
int Search_by_label(string label){




}




