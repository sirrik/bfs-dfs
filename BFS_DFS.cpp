/*

Sýrrý Kýrýmlýoðlu
150130117

Analysis of Algorithm 2 
BLG336E


PROJECT 1


*/
#include <iostream>
#include <list>
#include <queue>
#include <stack>
#include <ctime>

using namespace std;

class Node {
	public:
	int farmer,rabbit,fox,carrot; // 0 for left 1 for right
	int node_number;
	
	Node(){ // Constructor
		farmer = 0;
		rabbit = 0;
		fox = 0;
		carrot = 0;
		node_number;
	}
	Node( int farm, int rab, int fo, int car, int numb){ // Constructýr
		farmer = farm;
		rabbit = rab;
		fox = fo;
		carrot = car;
		node_number = numb;
	}
};

class Graph{ 
	public:
	list<Node> *edges;
	int size;
	
	Graph(int x){// Constructor
		edges = new list<Node>[x]; //array of lists to maintain all edges
	}
	
	void insert_newedge( Node x, Node y){ // add edge to both of the nodes
		edges[x.node_number].push_front(y);
		edges[y.node_number].push_front(x);
	}
};

list<Node> solution_tree; // Solution tree
Node last; // last node to determine where farmer goes

bool wrong_conditions(Node x){ // unsuitable states
	if( x.node_number == 8)
		return true;
	if (x.node_number == 7)
		return true;
	if ( x.node_number == 9)
		return true;
	if ( x.node_number == 10)
		return true;
	if ( x.node_number == 5)
		return true;
	if ( x.node_number == 6)
		return true;
	else
		return false;
}

void print(Node x){
	int direction;
	if(x.node_number != 0){ // no move for starting node
		cout << "(Farmer,";
		if(x.rabbit == 1 && last.rabbit == 0) cout << "Rabbit,";
		if(x.rabbit == 0 && last.rabbit == 1) cout << "Rabbit,";
		if(x.fox == 1 && last.fox == 0) cout << "Fox,";
		if(x.fox == 0 && last.fox == 1) cout << "Fox,";
		if(x.carrot == 1 && last.carrot == 0) cout << "Carrot,";
		if(x.carrot == 0 && last.carrot == 1) cout << "Carrot,";
		if( direction == 0) cout << "<)" << endl;
		if( direction == 1) cout << ">)" << endl;
	}

	if(x.farmer == 0){
		cout << "Farmer ";
		direction = 1;	//  where farmer goes
	}
	if(x.rabbit == 0)
		cout << "Rabbit ";
	if(x.fox == 0)
		cout << "Fox ";
	if(x.carrot == 0)
		cout << "Carrot ";
	cout << "|| "; // sides
	if(x.farmer == 1){
		cout << "Farmer ";
		direction = 0;
	}
	if(x.rabbit == 1)
		cout << "Rabbit ";
	if(x.fox == 1)
		cout << "Fox ";
	if(x.carrot == 1)
		cout << "Carrot ";
	cout <<endl;
	
	last = x; 
}

void BFS(Graph edge,Node start){
	queue<Node> neighbours; // neighbor list --- layer lists
	Node current;
	int max_number_kept=0;
	bool discovered[edge.size];
	for(int i = 0; i < edge.size ; i++)
		discovered[i] = false;
	
	discovered[start.node_number]=true; //start node discovered
	neighbours.push(start); //add start node to layer 0
	solution_tree.push_back(start); // add to tree
	Node u;

	int counter_in_layer=0;
	int visited_nodes = 0;
	
	
	while(!neighbours.empty()){
		current = neighbours.front(); 
		neighbours.pop();
		for ( list<Node>::iterator it=edge.edges[current.node_number].begin(); it != edge.edges[current.node_number].end(); ++it){
			u = *it;
			if( discovered[u.node_number] == false && wrong_conditions(u) == false){ // not discovered and not unsuitable
				discovered[u.node_number] = true;
				visited_nodes++;
				if ( counter_in_layer == 0){ //for bracnhing ---- for having one solution ----- one node from a layer
					solution_tree.push_back(u);
					neighbours.push(u);
				}
				counter_in_layer++; //layers
			}
		}
		if ( neighbours.size() > max_number_kept)
			max_number_kept = neighbours.size();
		counter_in_layer = 0;
	}
	int moves=-1;
	cout << "Maximum number of nodes kept in the memory:" << max_number_kept << endl << "Number of visited nodes:" << visited_nodes << endl;
	for( list<Node>::iterator it= solution_tree.begin(); it != solution_tree.end(); ++it) moves++;
	cout << "Solution move count:" << moves << endl;
	for( list<Node>::iterator it= solution_tree.begin(); it != solution_tree.end(); ++it) print(*it);
}



void  DFS(Graph edge, Node start){
	stack<Node> parent; //  for adding edges
	stack<Node> child; //   for adding edges
	Node par,chi;
	Node start_parent(0,0,0,0,-99); //fake parent of the starting node
	child.push(start);
	parent.push(start_parent);
	bool explored[edge.size];
	for( int i = 0; i < edge.size; i++)
		explored[i] = false;
	Node x;
	int max_number_kept=0;
	int visited_nodes = 0;	
	int branching_node=0;
	
	while(!child.empty()){
		chi = child.top(); //taking an edge
		par = parent.top(); // taking an edge
		child.pop();
		parent.pop();
		if ( explored[chi.node_number] == false){
			explored[chi.node_number] = true;
			visited_nodes++;
			solution_tree.push_back(chi);
			for( list<Node>::iterator it= edge.edges[chi.node_number].begin(); it != edge.edges[chi.node_number].end(); ++it){
				x = *it;
				if( explored[x.node_number] == false && wrong_conditions(x) == false && branching_node == 0){ // not discovered, not unsuitable, no branching ----- for only one solution
					parent.push(chi);
					child.push(x);
					branching_node++;
				}
			}
		}
		if ( child.size() > max_number_kept)	max_number_kept = child.size();
		branching_node = 0;
	}
	int moves=-1;
	cout << "Maximum number of nodes kept in the memory:" << max_number_kept << endl << "Number of visited nodes:" << visited_nodes << endl;
	for( list<Node>::iterator it= solution_tree.begin(); it != solution_tree.end(); ++it){
		moves++;
	}
	cout << "Solution move count:" << moves << endl;
	for( list<Node>::iterator it= solution_tree.begin(); it != solution_tree.end(); ++it) print(*it);
	
}


int main(){
	
	Graph farm(16); // create the graph
	Node node0(0,0,0,0,0); // starting node
	Node node1(0,0,0,1,1);
	Node node2(0,0,1,0,2);//add all nodes-states
	Node node3(0,0,1,1,3);
	Node node4(0,1,0,0,4);
	Node node5(0,1,0,1,5);
	Node node6(0,1,1,0,6);
	Node node7(0,1,1,1,7);
	Node node8(1,0,0,0,8);
	Node node9(1,0,0,1,9);
	Node node10(1,0,1,0,10);
	Node node11(1,0,1,1,11);
	Node node12(1,1,0,0,12);
	Node node13(1,1,0,1,13);
	Node node14(1,1,1,0,14);
	Node node15(1,1,1,1,15);
	
	farm.insert_newedge(node0,node8); //adding all edges
	farm.insert_newedge(node0,node12);
	farm.insert_newedge(node0,node10);
	farm.insert_newedge(node0,node9);
	farm.insert_newedge(node12,node4);
	farm.insert_newedge(node10,node2);
	farm.insert_newedge(node9,node1);
	farm.insert_newedge(node4,node14);
	farm.insert_newedge(node4,node13);
	farm.insert_newedge(node2,node14);
	farm.insert_newedge(node2,node11);
	farm.insert_newedge(node1,node13);
	farm.insert_newedge(node1,node11);
	farm.insert_newedge(node14,node6);
	farm.insert_newedge(node13,node5);
	farm.insert_newedge(node11,node3);
	farm.insert_newedge(node6,node15);
	farm.insert_newedge(node5,node15);
	farm.insert_newedge(node3,node15);
	farm.insert_newedge(node15,node7);
	
	clock_t clock1,clock2;
	
	clock1 = clock();
	BFS(farm,node0);
	clock2 = clock();
	cout << endl;
	cout << "Running Time:";
	cout << ((float)(clock2 - clock1)) / CLOCKS_PER_SEC; 
	cout << endl;
	cout << endl;
	solution_tree.clear();
	cout << endl;
	clock1 = clock();
	DFS(farm,node0);
	cout << endl;
	clock2 = clock();
	cout << "Runnig Time:";
	cout << ((float)(clock2 - clock1)) / CLOCKS_PER_SEC; 
}
