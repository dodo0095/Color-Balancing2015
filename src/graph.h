
#ifndef GRAPH_H
#define GRAPH_H
#include <algorithm>
#include <vector>
#include <map>
#include <string>
#include <cstring>
#include <fstream>
using namespace std;



class Row {
public:
	Row(const int &id=0, const double &score=0):row_id(id),row_score(score){};
	~Row();

	int row_id;
	double row_score;
};



class Matrix {
public:
	//m by n matrix
	Matrix(const int & m, const int & n);
	~Matrix();

	void updateProb();
	void setRowscore(const int & row_num,const float & score);

	void print();

	//bool myComp(pair<double,int*> a, pair<double,int*> b);
	void mogaR();
	void mogaC();
	void sortRowByScore();


	int** data;
	double* mutate_column;//probability of column to change to one!!
	double* b_column;
	vector<pair<double,int> > b_board;
	vector<Row* > rowsVec;
	map<int,Row* > rowsMap;
	int rows,columns;
};


class Node;

class Edge {

	public:
		Edge(Node *a, Node *b);
		Node* node[2];

		Node *getNeighbor(Node *n);

		bool operator < (const Edge& rhs) const;

};

class Node {

	public:
		//left-bottom x,y and right-upper x,y
		Node(const int& i,const int& lb_x,const int& lb_y,const int& ru_x,const int& ru_y);
		void addEdge(Edge *e);
		void sortEdge();
		bool overlap(Node* n,const int & alpha,const int & beta);
		int  areaCalc();

		int id;
        int color;
        int group_id;
		bool traveled;
		vector<Edge *> edge;

		//int d;
		Node *prev;
		//int heap_idx;
		int lb_x,lb_y,ru_x,ru_y;
		int area;
		//int alpha,beta;
};

class Group {

	public:
		Group(bool c = false) { colored = c;invert=false;locked=false;grparea=0;grpwin=0; }
		~Group();
		void printGroup();
		void calcgrouparea();
		vector<Node *> groupNodes;
		bool		   colored;
		bool		   invert;
		bool           locked;
        int            grparea;
        int            grpwin;
};

class Window {
	public:
		Window() {colorarea=0;};
		~Window();

		vector<Node *> windowNodes;
		vector<int>    area;
		vector<int> wgroups;
		vector<int> wgroupsarea;
		int colorarea;

};

class Graph {

	public:
		Graph(const string& n="",const int& alpha=0,const int& beta=0,const int& omega=0);
		~Graph();
		
		//void addEdge(const int& v1, const int& v2);
		void addNode(Node* nptr);
		void sortEdgesOfNode();
        void sortNodesByDegree();
        void sortNodesByID();
        void sortNodesByY();
        void sortNodesByX();

		void init();
		Node * getNodeById(const int& id);

		void dfs();
		void dfsvisit(Node*, int, Group*);
		//void bfs(int);

		void printall();
		void constructAll();

        void colorgraph();
		void getboxsize();

		void windowcalc_grparea();
		void greedycolor();
		void greedywindow();
		void averagecolor(int x,int y);
		void ptaveragecolor(Window* win);
		void largegreedy();
		void largecolor(Group* grp);

		void saveState();
		void recallState();

		bool exhausted();
		void gacolor();
		float gaRowgetScore(int * p);

		void output(ofstream & );//output file
		void mapping();//build windows[][]
		void getScoreMap();
	    float getscore();
	    
		void set(const int& which,const bool& toset ){groups[which]->invert=toset;};
		void reset();
		map<int, Node *> nodesMap;
        vector<Node *>   nodes;
		vector<Edge *>   edges;
		vector<Group *>  groups;
		vector<vector<Window*> > windows;
		vector<bool> lastState;
		//density_window   densWindow;
		int alpha, beta, omega;
		string name;
		int colored_graph;
		int B_top,B_bottom,B_right,B_left;
		vector<vector<int> > scoreMap;//scoreMap[windows W][group G]=differential area in window W for group G
};

#endif
