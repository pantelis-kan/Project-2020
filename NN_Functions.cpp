
#include "NN_Functions.hpp"

#include <chrono>

using namespace std;

/* 	Assigns all images to buckets
	int N : is the number of total images in the input 
	int TableSize :
	double** s_params: it's the s(i) necessary for the aplification of LSH
*/
void Preprocessing(Hash_Table** H_Tables, Point_Array& input, int N, int TableSize, double** s_params, int L, 
					int k, int M, long long int m, double w ){


	int bucket_position;

	// for each point x in the input
	for(int j = 0; j < N; j++ ){

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





void Nearest_Neighbors(Hash_Table** H_Tables,Point_Array& input, Point_Array& queries,
				int N_q,int TableSize, double** s_params,int L,int k,int M, long long int m,double w){

	int query_bucket_position, nearest_neighbor_id;
	double min_distance;


	// for each query
	for (int q = 0; q < N_q; q++){

		min_distance  = std::numeric_limits<double>::max();
		bool found_nn = false;

		// for each hash table
		for (int l = 0; l < L; l++){
			
			// find the position of the query in the lth hash table
			query_bucket_position = queries.Compute_g(q,k,M,m,w,TableSize,s_params,l);

			//cout << "Query id " << q+1 << " fell into bucket " << query_bucket_position <<endl;

			// find how many elements the bucket has
			int size_of_bucket = H_Tables[l]->SizeofBucket(query_bucket_position);

			// if the query fell on an empty bucket, ignore
			if (size_of_bucket == 0) continue; 
			found_nn = true;

			//cout << "Bucket " << query_bucket_position <<  " of g" <<l<< " has " << size_of_bucket << " elements "<<endl; 
			// for each element in the bucket
			for(int i = 0; i < size_of_bucket; i++ ){

				if (i > 10*L) break; // 20*L are enough points

				// pop id from the query's bucket
				int id = H_Tables[l]->Pop_ID(query_bucket_position,i); 

				Point& query_point = queries.Retrieve(q);
				Point& input_point = input.Retrieve(id-1);

				// compute Manhattan Distance for the query and the popped id
				double distance = Distance(query_point,input_point,1); 
				//cout << "Computed distance from point " << q+1 << " to point " << id-1 << " = " << distance <<endl;
				if (distance < min_distance){
					min_distance = distance;
					nearest_neighbor_id = id;
				}

			}
			
		}

		if(found_nn != false)
			cout << "Approximate NN for query " << q+1 << " = " << nearest_neighbor_id << " with distance " << min_distance <<endl;
		else
			cout << "Could not find approximate nearest neighbor for query " << q+1 <<endl;
	}


}



void Exact_NN(Point_Array& input, Point_Array& queries, int N, int N_q){

	int nearest_neighbor_id;
	double min_distance;
	double distance;

	int num_of_points = N/4;

	long double dist_sum = 0.0;
	
	auto t1 = std::chrono::high_resolution_clock::now();
	for(int j = 0; j < N_q; j++){
		min_distance = std::numeric_limits<double>::max();
		for(int i = 0; i < N; i++){

			Point& query_point = queries.Retrieve(j);
			Point& input_point = input.Retrieve(i);

	
			distance = Distance(query_point,input_point,1);
			
			if(j == 0) dist_sum += distance;
			//distance = Distance(input_point,query_point,1);

			if (distance < min_distance){
					min_distance = distance;
					nearest_neighbor_id = i+1;
	 			}

	 	}
	
	auto t2 = std::chrono::high_resolution_clock::now();		
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();

	//cout << "Time taken : " << duration <<endl;
	cout << "Exact NN for query " << j+1 << " = " << nearest_neighbor_id << " with distance " << min_distance <<endl;

	}

	//cout << "distance sum : " << dist_sum <<endl;
	cout << "Best w : " << dist_sum/N << endl;

}


double compute_w(Point_Array& input, int N){
	double distance_sum = 0.0;
	double distance_a_b = 0.0;
	int count_of_sums = 0;

	Point* image_a;
	Point* image_b;

	cout << "Calculating started now!!" << endl;
//    auto start = high_resolution_clock::now(); 

/*
	for (int i = 0; i < N; i++){
		image_a = input.Retrieve_ptr(i);
		cout << "we are at " << i << endl;
		for (int j = i+1; j < N; j++){
			image_b = input.Retrieve_ptr(j);
			distance_a_b = Distance(*image_a, *image_b, 1);
			distance_sum += distance_a_b;
			count_of_sums++;
		}
	}
*/

	for (int i = 0; i < N; i += 2){
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
void Cube_Nearest_Neighbors(Hypercube* hcube, Point_Array& input, int input_count, Point_Array& queries, int queries_count, double** s_params, int M_lsh, long long int m_lsh, double w, int k, int M, int probes, int N, double R){
	string query_label;
	vector<int> *input_records;
	double min_distance;
	int nearest_neighbor_id;

	// for each query
//	for (int q = 0; q < queries_count; q++){
	for (int q = 0; q < 1; q++){
		min_distance  = std::numeric_limits<double>::max();
		bool found_nn = false;
		
		// find the position of the query in the cube table
		query_label = queries.Compute_f(q, k, M_lsh, m_lsh, w, s_params, hcube);

		//Initialize Hamming class needed for the probes. Make and delete for every query
		Hamming* hamming = new Hamming(query_label);
		bool do_hamming_now = false;

	cout << "Query label is: " << query_label << endl;

		//retrieve pointer to a Vertex which is the actual bucket corresponding to the query_label
		input_records = hcube->retrieve_records_vector(query_label);
	cout << "Records vector has size:" << input_records->size() << endl;
		//check here that query_label is same as retrieved vertex label

		// if the query fell on an empty bucket, ignore or .........?

		Point& query_image = queries.Retrieve(q);
		int count_images_checked = 0;
		double min_distance_previous = 0;


		//Finding N Nearest Neibhors until found or thresolds are met (max probes and/or max images M)
		for(int c=0; c < N; c++){
			found_nn = false;
			do_hamming_now = false; 
			
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
					count_images_checked++;
				}
				else{
					do_hamming_now = true;
					break;
				}

				//check if maximum number of images to be checked has been reached
				if(M == count_images_checked)
					break;
			}
			if(found_nn == true){
				cout << "Approximate NN for query " << q+1 << " = " << nearest_neighbor_id << " with distance " << min_distance <<endl;
				min_distance_previous = min_distance;
				min_distance  = numeric_limits<double>::max();
			}

			if(M == count_images_checked)
				break;

			//if the count of NN already found is equal to sizeof the current vertex/bucket then probe with hamming
/*			if (do_hamming_now == true){
				
				if (hamming->get_usedprobes() == probes){
					break;		//Thresold reached: we cannot go further so searching has to stop
				}

				//move_to_next: should actually check next in map, change the current_in_use and increase used_probes
				//Returns the new label of the bucket we move to
				query_label = hamming->move_to_next();
				//Change bucket to the next one to be checked
				input_records = hcube->retrieve_records_vector(query_label);
				
				
			}
*/		}
		delete hamming;	

	}




}

