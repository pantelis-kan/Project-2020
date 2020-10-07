#ifndef HYPERCUBE_HPP
#define HYPERCUBE_HPP

// a collection of vertices

class Hypercube{

	public:
		Hypercube();
		~Hypercube();
		void Insert(string tag,int id);
		

	private:
		Vertex* vertex_table;

};

#endif
