#include "Hypercube.hpp"
#include <random>
#include <bitset>

#include "Point_Table.hpp"


using namespace std;

default_random_engine generator;


//default_random_engine rand_generator(time(NULL));

Hypercube::Hypercube(int k){
	//create Hypercube with vertex_table size 2^k
	vertex_t_size = pow(2,k);
	vertex_table = new Vertex[vertex_t_size];
	

	cout << "vertextable_size is " << vertex_t_size << endl;
}

Hypercube::~Hypercube(){
    delete[] vertex_table;
}


//internally manages the hashing, mapping and insertion of points
void Hypercube::Map_images(Point_Array& input, int N, int k, double** s_params, 
					int M, long long int m, double w, Hypercube* hcube){

	string label;



	//Run compute_f : Returns the necessary bitstring (necessary hashvalue)
	//Insert label and id to vertex
	
	for (int i = 0; i < N; i++){
		label = input.Compute_f(i, k, M, m, w, s_params, hcube);
		Insert(label, i+1);
	}
	

}


//inserting image with id in label=tag
void Hypercube::Insert(string label, int id){
	int i = 0;
	if(vertex_t_size == 0){
		cout << "vertex table not allocated" << endl;
		exit(-1);
	}

	//Find the vertex in vertex_table based on label and call insert with id
	for (i = 0; i < vertex_t_size; i++){
		if (label.compare(vertex_table[i].Get_label()) == 0){
		//found it so we can insert id
			vertex_table[i].Insert(id);
			break;
		}	
	}

	//label not existing so we need to insert label and insert then id	
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
	std::uniform_int_distribution<int> distribution(1,100);
	int coin = distribution(generator);

	coin = coin % 2;

	char binary_value;

	map<int,char>::iterator it;

	// using find() to search for 'b'  
	// key found 
	// "it" has address of key value pair. 
	it = F_function.find(h); 
	
	if(it == F_function.end()){
//		cout << "Key-value pair not present in map" << endl ;
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
		F_function.insert(pair<int,char>(h, binary_value));
		return binary_value;
	}
	else{
//		cout << "Key-value pair present : " << it->first << "->" << it->second << endl;
		return it->second; 
	}	
}

//searches vertex_table and returns position, if not found returns -1
string Hypercube::Search_by_label(string label){
	string not_found;
	not_found = "-1";

	if ((vertex_table[0].Get_label()).empty()){
		return not_found;
	}
	
	for (int i = 0; i < vertex_t_size; i++){
		if (label.compare(vertex_table[i].Get_label()) == 0){
			return vertex_table[i].Get_label();
		}
		else{
			return not_found;
		}
		
	}
	
}

void Hypercube::print_vertex_table(){
	string label_value;

	for (int i = 0; i < vertex_t_size; i++){
		label_value = vertex_table[i].Get_label();
		cout << "Label value is:" << label_value << endl;
		vertex_table[i].print_records();
	}

	


}

vector<int>* Hypercube::retrieve_records_vector(string query_label){

	for (int i = 0; i < vertex_t_size; i++){
		if (query_label.compare(vertex_table[i].Get_label()) == 0){
			return &(vertex_table[i].records);
		}
		
	}

}


Hamming::Hamming(string original_label){
	initial_label = original_label;
	current_label_in_use = original_label;

	labels_Hamming.emplace_back(pair<string, int>(original_label, 0));
	int length = initial_label.length();
	int max_value = pow(2,length)-1;

	int label_bin = stoi (original_label, nullptr, 2);
	int xor_result_int;
	string xor_result_str;
	string new_label;
	string temp;


	for (int i = 1; i <= length; i++){
		for(int j = 0; j <= max_value; j++){
			//do XOR between labels number and j
			xor_result_int = label_bin ^ j;
			xor_result_str = bitset< 32 >( xor_result_int ).to_string();
			//if XOR result is equal to i then add to labels_Hamming with bit sting of j and ham_dist=i
			if (count(xor_result_str.begin(), xor_result_str.end(), '1') == i){
				temp = bitset< 32 >( j ).to_string();
				new_label = temp.substr(32-length);
				labels_Hamming.emplace_back(pair<string, int>(new_label, i));
			}
		}
	}

//	print();
	

}

Hamming::~Hamming(){

}

int Hamming::get_usedprobes(){
	return used_probes;
}

void Hamming::print(){
	cout << "-------Printing Hamming data-------------" << endl;

	cout << "Initial Label was: " << initial_label << endl;

	for(auto it = labels_Hamming.cbegin(); it != labels_Hamming.cend(); ++it){
    	cout << it->first << " -> " << it->second << ", ";
	}

	cout << "-------End of Printing Hamming data-------------" << endl;

}

string Hamming::move_to_next(){
	used_probes++;
	auto it = labels_Hamming.cbegin();
	for (int i = 0; i < used_probes; i++){
		it++;
	}

	current_label_in_use = it->first; 

	return current_label_in_use;

}

