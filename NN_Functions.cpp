
#include "NN_Functions.hpp"

#include <algorithm>
#include <chrono>

using namespace std;

/* 	Assigns all images to buckets
	int input_count : is the number of total images in the input 
	int TableSize :
	double** s_params: it's the s(i) necessary for the aplification of LSH
*/
void Preprocessing(Hash_Table** H_Tables, Point_Array& input, int input_count, int TableSize, double** s_params, int L, 
					int k, int M, long long int m, double w){


	int bucket_position;

	// for each point x in the input
	for(int j = 0; j < input_count; j++ ){

		auto t1 = std::chrono::high_resolution_clock::now();

		// Compute g1(x) ... gL(x)
		for (int l = 0; l < L; l++){
		
			bucket_position = input.Compute_g(j, k, M, m, w, TableSize, s_params, l);
			

			//cout << "Function g(x) computed in " << duration <<endl;

			H_Tables[l]->InsertToBucket(bucket_position, j+1); // Insert to bucket point with id j+1	

			//cout << " point id " << j+1 << " has bucket " << bucket_position <<endl;
		}

		auto t2 = std::chrono::high_resolution_clock::now();		
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();

		//cout << "Point " << j+1 << " inserted in " << duration << " microseconds " << endl;
		//cout << endl;

	}


}





void LSH_Nearest_Neighbors(Results* results, Hash_Table** H_Tables,Point_Array& input, Point_Array& queries,
				int queries_count,int TableSize, double** s_params,int L,int k,int M, long long int m, double w, int N){

	int query_bucket_position, nearest_neighbor_id;
	double min_distance;


	// for each query
//	for (int q = 0; q < queries_count; q++){
	for (int q = 0; q < 1; q++){

		min_distance  = std::numeric_limits<double>::max();
		bool found_nn = false;
		multimap<double, int> all_NN_storage;
		double min_distance_previous = 0;

		for(int c = 0; c < N; c++){
			// for each hash table
			for (int l = 0; l < L; l++){

				// find the position of the query in the lth hash table
				query_bucket_position = queries.Compute_g(q,k,M,m,w,TableSize,s_params,l);

				//cout << "Query id " << q+1 << " fell into bucket " << query_bucket_position <<endl;

				// find how many elements the bucket has
				int size_of_bucket = H_Tables[l]->SizeofBucket(query_bucket_position);

				// if the query fell on an empty bucket, ignore
				if (size_of_bucket == 0) continue; 

				

				//cout << "Bucket " << query_bucket_position <<  " of g" <<l<< " has " << size_of_bucket << " elements "<<endl; 
				// for each element in the bucket
				for(int i = 0; i < size_of_bucket; i++ ){

					if (i > 10*L) break; // 10*L are enough points

					// pop id from the query's bucket
					int id = H_Tables[l]->Pop_ID(query_bucket_position,i); 

					Point& query_point = queries.Retrieve(q);
					Point& input_point = input.Retrieve(id-1);

					// compute Manhattan Distance for the query and the popped id
					double distance = Distance(query_point,input_point,1); 
					//cout << "Computed distance from point " << q+1 << " to point " << id-1 << " = " << distance <<endl;
					if (distance < min_distance && distance > min_distance_previous){
						min_distance = distance;
						nearest_neighbor_id = id;
						found_nn = true;
					}

				}
			}	

			if(found_nn == true){
				cout << "Approximate NN for query " << q+1 << " = " << nearest_neighbor_id << " with distance " << min_distance <<endl;
				//make a multimap and insert there. then in the end results take N from there
  				all_NN_storage.insert( pair<double, int>(min_distance, nearest_neighbor_id) );
				min_distance_previous = min_distance;
				min_distance  = numeric_limits<double>::max();
			}
			else
				cout << "Could not find approximate nearest neighbor for query " << q+1 <<endl;
		}

		results[q].set_query_id(q+1);
		
		for (auto it = all_NN_storage.cbegin(); it != all_NN_storage.cend(); it++){
			results[q].insert_N_nearest((*it).second, (*it).first);
		}

		//check if empty, means that no nearest found which is weird
		if (all_NN_storage.empty()){
			cout << "No Approximate nearest found for query image: " << q+1 << endl; 
		}
		

	}


}



void Exact_NN(Point_Array& input, Point_Array& queries, int input_count, int queries_count,ofstream& outfile,int* time_passed){

	int nearest_neighbor_id;
	double min_distance;
	double distance;

	int num_of_points = input_count/4;

	long double dist_sum = 0.0;
	
	double distances[input_count]; // each query has maximum input_count neighbors 

	
	for(int j = 0; j < queries_count; j++){
		auto t1 = std::chrono::high_resolution_clock::now();
		//min_distance = std::numeric_limits<double>::max();
		Point& query_point = queries.Retrieve(j);

		for(int i = 0; i < input_count; i++){
	
			Point& input_point = input.Retrieve(i);
			distance = Distance(query_point,input_point,1);
			
			distances[i] = distance;

	 	}

		auto t2 = std::chrono::high_resolution_clock::now();		
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();	 
	
		sort(distances, distances+input_count);
		for(int i = 0; i < 50; i++){

			if( i == 49) outfile << distances[i] << " " << duration << endl;
			else outfile << distances[i] << endl;
		}

		//cout << "Time taken : " << duration <<endl;
		//cout << "Exact NN for query " << j+1 << " = " << nearest_neighbor_id << " with distance " << min_distance <<endl;

	}


	//cout << "distance sum : " << dist_sum <<endl;
	//cout << "Best w : " << dist_sum/input_count << endl;

}


double compute_w(Point_Array& input, int input_count){
	double distance_sum = 0.0;
	double distance_a_b = 0.0;
	int count_of_sums = 0;

	Point* image_a;
	Point* image_b;

	cout << "Calculating started now!!" << endl;
//    auto start = high_resolution_clock::now(); 

/*
	for (int i = 0; i < input_count; i++){
		image_a = input.Retrieve_ptr(i);
		cout << "we are at " << i << endl;
		for (int j = i+1; j < input_count; j++){
			image_b = input.Retrieve_ptr(j);
			distance_a_b = Distance(*image_a, *image_b, 1);
			distance_sum += distance_a_b;
			count_of_sums++;
		}
	}
*/

	for (int i = 0; i < input_count; i += 2){
		image_a = input.Retrieve_ptr(i);
		image_b = input.Retrieve_ptr(i+1);
		distance_a_b = Distance(*image_a, *image_b, 1);
		distance_sum += distance_a_b;
		count_of_sums++;
		cout << "we are at " << i << endl;
	}


	cout << "average distance is:" << distance_sum/count_of_sums << endl;
	cout << "count of sums:" << count_of_sums << endl;
  //  auto stop = high_resolution_clock::now(); 

//	auto duration = duration_cast(stop - start); 
  
//    cout << "Time taken by function: " << duration.count() << " microseconds" << endl;

	return distance_sum/count_of_sums;

}


//probably change it so it returns a pointer to array storing the results for N nearest neibhors?
void Cube_Nearest_Neighbors(Results* results, Hypercube* hcube, Point_Array& input, int input_count, Point_Array& queries, int queries_count, double** s_params, int M_lsh, long long int m_lsh, double w, int k, int M, int probes, int N, double R){
	string query_label;
	vector<int> *input_records;
	double min_distance;
	int nearest_neighbor_id;

	// for each query
//	for (int q = 0; q < queries_count; q++){
	for (int q = 0; q < 2; q++){
		min_distance  = std::numeric_limits<double>::max();
		bool found_nn = false;
		multimap<double, int> all_NN_storage;
		
		// find the position of the query in the cube table
		query_label = queries.Compute_f(q, k, M_lsh, m_lsh, w, s_params, hcube);

		//Initialize Hamming class needed for the probes. Make and delete for every query
		Hamming* hamming = new Hamming(query_label);

	cout << "Query label is: " << query_label << endl;

		//retrieve pointer to a Vertex which is the actual bucket corresponding to the query_label
		input_records = hcube->retrieve_records_vector(query_label);
	cout << "Records vector has size:" << input_records->size() << endl;
		//check here that query_label is same as retrieved vertex label

		// if the query fell on an empty bucket, ignore or .........?

		Point& query_image = queries.Retrieve(q);
		int count_images_checked = 0;
		double min_distance_previous = 0;
		int count_found_nn = 0;

		//Finding N Nearest Neibhors until found or thresolds are met (max probes and/or max images M)
		for(int c=0; c < N; c++){
			found_nn = false;
			
			// for each element in the vector measure distance with query
			for(int i = 0; i < input_records->size(); i++ ){

				// pop id from the query's bucket
				int id = input_records->at(i); 
				Point& input_image = input.Retrieve(id-1);

				// compute Manhattan Distance for the query and the popped id
				double distance = Distance(query_image, input_image, 1); 

				//cout << "Computed distance from point " << q+1 << " to point " << id-1 << " = " << distance <<endl;
				
				//Checking for a NN excluding the previously found one
				//Else means that we have already taken all of them so we need to move to another vertex
				if (distance < min_distance && distance > min_distance_previous){
					min_distance = distance;
					nearest_neighbor_id = id;
					found_nn = true;
				}

				//because you want to count the checked images only once and not N times
				if(c == 0)	
					count_images_checked++;

				//check if maximum number of images to be checked has been reached
				if(M == count_images_checked)
					break;
			}
			if(found_nn == true){
				cout << "Approximate NN for query " << q+1 << " = " << nearest_neighbor_id << " with distance " << min_distance <<endl;
				//make a multimap and insert there. then in the end results take N from there
  				all_NN_storage.insert( pair<double, int>(min_distance, nearest_neighbor_id) );
				min_distance_previous = min_distance;
				min_distance  = numeric_limits<double>::max();
				count_found_nn++;
			}

			if(M == count_images_checked)
				break;

			//if the count of NN already found is equal to sizeof the current vertex/bucket then probe with hamming
			//also in case we fell in an empty bucket then we change vertex
			if (count_found_nn == input_records->size() || input_records->size() == 0){
				count_found_nn = 0; //changing vertex so initialize again the count
				if (hamming->get_usedprobes() == probes){
					break;		//Thresold reached: we cannot go further so searching has to stop
				}

				//move_to_next: should actually check next in map, change the current_in_use and increase used_probes
				//Returns the new label of the bucket we move to
				query_label = hamming->move_to_next();
				cout << "New query label after probing is: " << query_label << endl;
				//Change bucket to the next one to be checked
				input_records = hcube->retrieve_records_vector(query_label);
				min_distance_previous = 0;
				min_distance  = numeric_limits<double>::max();

				
			}
		}

		results[q].set_query_id(q+1);
		//		multimap<double, int>::iterator it = all_NN_storage.cbegin();
		
		for (auto it = all_NN_storage.cbegin(); it != all_NN_storage.cend(); it++){
			results[q].insert_N_nearest((*it).second, (*it).first);
		}

		//check if empty, means that no nearest found which is weird
		if (all_NN_storage.empty()){
			cout << "No Approximate nearest found for query image: " << q+1 << endl; 
		}

		delete hamming;	
		cout << "Total images checked before stopping:" << count_images_checked << endl;

	}




}



void Cube_Range_Search(Results* results, Hypercube* hcube, Point_Array& input, int input_count, Point_Array& queries, int queries_count, double** s_params, int M_lsh, long long int m_lsh, double w, int k, int M, int probes, int N, double R){
	string query_label;
	vector<int> *input_records;
	double min_distance;
	int nearest_neighbor_id;

	// for each query
//	for (int q = 0; q < queries_count; q++){
	for (int q = 0; q < 2; q++){
		min_distance  = R;
		bool found_nn = false;
		
		// find the position of the query in the cube table
		query_label = queries.Compute_f(q, k, M_lsh, m_lsh, w, s_params, hcube);

	cout << "Query label is: " << query_label << endl;

		//retrieve pointer to a Vertex which is the actual bucket corresponding to the query_label
		input_records = hcube->retrieve_records_vector(query_label);
	cout << "Records vector has size:" << input_records->size() << endl;
		//check here that query_label is same as retrieved vertex label

		// if the query fell on an empty bucket, ignore or .........?

		Point& query_image = queries.Retrieve(q);
		int count_images_checked = 0;
		double min_distance_previous = 0;

		//Finding all Neibhors inside a specific range R or thresolds are met (max images M)
		for(int c=0; c < input_records->size(); c++){
			found_nn = false;
			
			// for each element in the vector measure distance with query
			for(int i = 0; i < input_records->size(); i++ ){

				// pop id from the query's bucket
				int id = input_records->at(i); 
				Point& input_image = input.Retrieve(id-1);

				// compute Manhattan Distance for the query and the popped id
				double distance = Distance(query_image, input_image, 1); 

				//cout << "Computed distance from point " << q+1 << " to point " << id-1 << " = " << distance <<endl;
				
				//Checking for a NN inside the range excluding the previously found one
				//Else means that we have already taken all of them so we need to move to another vertex
				if (distance < min_distance && distance > min_distance_previous){
					min_distance = distance;
					nearest_neighbor_id = id;
					found_nn = true;
				}
	
				//because you want to count the checked images only once and not N times
				if(c == 0)	
					count_images_checked++;

				//check if maximum number of images to be checked has been reached
				if(M == count_images_checked)
					break;
			}

			if(found_nn == true){
				cout << "Range NN for query " << q+1 << " = " << nearest_neighbor_id << " with distance " << min_distance <<endl;
				results[q].insert_Range_nearest(nearest_neighbor_id);
				
				min_distance_previous = min_distance;
				min_distance  = R;
			}
			else{
				//means that it didnt find any other element in the range so we break;
				break;
			}

			if(M == count_images_checked)
				break;

		}

		cout << "Total images checked before stopping:" << count_images_checked << endl;

	}





}

