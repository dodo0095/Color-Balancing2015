#include "graph.h"
#include <iostream>
#include <fstream>
#include <queue>
#include <algorithm>
#include <cmath> 
#include <limits>
#include <bitset>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
using namespace std;


//把input。x1 y1 x2 y2放進來
void Graph::addNode( Node* nptr)
{
	nodesMap[nptr->id]=nptr;
	nodes.push_back(nptr);
}
	





//////////開始畫圖graph///////////////////////////




//graph套件。 檢查二個點xy座標。有沒有連再一起
bool NodeCompByX(const Node* A, const Node* B ) 
{
	if(A->lb_x < B->lb_x) return true;
	else return false;
}

void Graph::sortNodesByX()
{
	sort(nodes.begin(), nodes.end(), NodeCompByX);
}

bool NodeCompByY(const Node* A, const Node* B ) 
{
	if(A->lb_y < B->lb_y) return true;
	else return false;
}

void Graph::sortNodesByY()
{
	sort(nodes.begin(), nodes.end(), NodeCompByY);
}




bool Node::overlap(Node* n,const int & alpha,const int & beta) //從最左邊開始到最右 邊
{
	int check_lb_y=lb_y-beta;
	int check_ru_x=ru_x+alpha;
	int check_ru_y=ru_y+beta;
	//cout<<id<<" calling overlap on "<<n->id<<endl;

	if ((n->lb_x>=lb_x)&&(n->lb_x<=check_ru_x))
	{
		
	   if (((n->lb_y>=check_lb_y)&&(n->lb_y<=check_ru_y))//left bottom inside
		   ||((n->ru_y>=check_lb_y)&&(n->ru_y<=check_ru_y)))//left upper inside
	   {
	   
		   if ((n->lb_x>=ru_x)&&((n->lb_y>=ru_y)||(n->ru_y<=lb_y)))
		   {
		   		//cout<<"invalid !!"<<endl;
				return false;
		   }

		   return true;
	   }
	   else if ((n->lb_y<=check_lb_y)&&(n->ru_y>=check_ru_y))// be included
	   	return true;
	}

	return false;

}


// 根據題目的alpha,beta 創造出圖
void Graph::constructAll()
{
	
	sortNodesByX();
	unsigned int j=0;
	for (unsigned int i=0;i<nodes.size();++i)
	{
		j=i+1;
		
		for (;j<nodes.size();++j)
		{
			//cout<<i<<" calling "<<j<<endl;
			if((nodes[j]->lb_x)>nodes[i]->ru_x+alpha)
			{
				break;
			}
			if (nodes[i]->overlap(nodes[j],alpha,beta))
		    {
			   Edge *e = new Edge(nodes[i], nodes[j]);
	           edges.push_back(e);

	           nodes[i]->edge.push_back(e);
	           nodes[j]->edge.push_back(e);
	        }
	   
		}
	}

}






//////////dfs///////////////////////////




//開始用algorihm走圖形。  這邊使用DFS

void Graph::init() //初始化
{
	map<int, Node *>::iterator itN;
	for ( itN = nodesMap.begin() ; itN != nodesMap.end() ; itN++ )
	{
		Node *node = (*itN).second;
		node->traveled = false;
		node->prev = 0;
        node->color = -1;//把所有點node 都用尚未探訪
	}	
}

void Graph::sortEdgesOfNode()       // 把edge排列
{
	map<int, Node *>::iterator it;
	for ( it = nodesMap.begin() ; it != nodesMap.end() ; it++ )
	{
		Node *node = (*it).second;
		node->sortEdge();
	}

}


bool NodeCompByID( const Node* A, const Node* B ){
	if ( A->id < B->id ) return true;
    else return false;
}

bool edgeComp( const Edge* A, const Edge* B ){
	if ( (*A) < (*B) ) return true;
	return false;
}

void Node::sortEdge()   //用edge排列
{
    sort(edge.begin(), edge.end(), edgeComp);
}


void Graph::sortNodesByID()   //用id排列
{
    sort(nodes.begin(), nodes.end(), NodeCompByID);
}


void Graph::dfs()
{
   init();
   int edgenum=0, group_num = 0;
   sortEdgesOfNode();
   sortNodesByID();
   for(unsigned int i=0;i<nodes.size();++i) {
   	   if(!(nodes[i] -> traveled)) {
   	   	   Group* group = new Group;
           dfsvisit(nodes[i], group_num, group);
           groups.push_back(group);         //  出來的解果會幫忙分群。  相同froup的會放在同一群
           ++group_num;
       }
   }
}


void Graph::dfsvisit(Node* u, int group_num, Group* group)
{
 	u -> group_id = group_num;
	u->traveled = true;
	group -> groupNodes.push_back(u);

	for(unsigned int i = 0; i < u -> edge.size(); ++i) {
		Node* near = u -> edge[i] -> getNeighbor(u);
		if (near->traveled == false) {
			dfsvisit(near, group_num, group);		
		}
	}
}






















//////////////////開始給顏色////////////////////////




Node * Edge::getNeighbor(Node *n)
{
	if ( node[0] == n ) return node[1];
	if ( node[1] == n ) return node[0];

	return 0;	
}



void Graph::colorgraph()
{
	colored_graph=0;
	init();
	for(unsigned int i=0;i<groups.size();i++)
	{
   		Node* sourcenode=groups[i]->groupNodes[0];
		queue<Node*> nodesque;
		sourcenode->color=0;
		sortEdgesOfNode();
		nodesque.push(sourcenode);
    	bool stop=false;
		while (nodesque.empty()==false&&stop==false)
	  {
		Node * u=nodesque.front();
		if(0==u->color)
		{
		for(unsigned int k=0;k<u->edge.size();++k)
		{
			Node* near= u->edge[k]->getNeighbor(u);
			if (-1==near->color)
			{
				near->color=1;
				nodesque.push(near);
	
			}
			else if(0==near->color)
			{
				
				Group* temp=groups[i];
      			groups[i]=groups[colored_graph];
      			groups[colored_graph]=temp;
      			colored_graph++;
      			stop=true;
      			break;
			}
		}
	    }
	    else if(1==u->color)
	    {
	    	for(unsigned int k=0;k<u->edge.size();++k)
		{
			Node* near= u->edge[k]->getNeighbor(u);
			if (-1==near->color)
			{
				near->color=0;
				nodesque.push(near);
				
			}
			else if(1==near->color)
			{
				Group* temp=groups[i];
      			groups[i]=groups[colored_graph];
      			groups[colored_graph]=temp;
      			colored_graph++;
      			stop=true;
      			break;
			}
		}
	    }
		nodesque.pop();
	  }
	}
   
}






















//////////////////////input讀上下界////////////////////////////
//////////////////////上下界存在B_top B_bottom B_right left裡面////////////////////////////

void Graph::getboxsize()
{
  for(unsigned int i=colored_graph;i<groups.size();i++)
{
   for(unsigned int j=0;j<groups[i]->groupNodes.size();j++)
   {
     //lb_x,lb_y,ru_x,ru_y
     if(groups[i]->groupNodes[j]->ru_y>B_top)
     	B_top=groups[i]->groupNodes[j]->ru_y;
     if(groups[i]->groupNodes[j]->lb_y<B_bottom)
      B_bottom=groups[i]->groupNodes[j]->lb_y;
    if(groups[i]->groupNodes[j]->ru_x>B_right)
      B_right=groups[i]->groupNodes[j]->ru_x;
    if(groups[i]->groupNodes[j]->lb_x<B_left)
      B_left=groups[i]->groupNodes[j]->lb_x;
   }
}
}

















//////////////////////////把windows一個一個切開/////////////////////////////



void Graph::mapping()
{
int B_length=B_right-B_left;   //得知整個邊界寬
int B_width=B_top-B_bottom;   //得知整個邊界的長度


////////把window 按照omega大小切出來
	for(int i=0;i<ceil(static_cast<double>(B_length)/omega);i++){
		vector<Window*> a;
		for(int j=0;j<ceil(static_cast<double>(B_width)/omega);j++){
			Window *w=new Window;
			a.push_back(w);	

		}	
		windows.push_back(a);

	}


//////一個一個windows去看對照//////////看每個點屬於哪個window////////////////

	for(unsigned int i=colored_graph;i<groups.size();i++){  
		
		for(unsigned int j=0;j<groups[i]->groupNodes.size();j++){

			int x1=groups[i]->groupNodes[j]->lb_x;
			int x2=groups[i]->groupNodes[j]->ru_x;
			int y1=groups[i]->groupNodes[j]->lb_y;
			int y2=groups[i]->groupNodes[j]->ru_y;
			
			int x1_sec=(x1-B_left)/omega;//x1,x2,y1,y2 in which DSW
			
			int y1_sec=(y1-B_bottom)/omega;
			
			vector <int> x;			
			vector <int> y;
			//x component
			int k=1;
			x.push_back(x2-x1);//x[0]
			int x_tail=x1;
			while
			(
				x2>omega*(k+x1_sec)+B_left){
				x[k-1]=(x1_sec+k)*omega+B_left-x_tail;
				x.push_back(x2-(x1_sec+k)*omega-B_left);
				x_tail=omega*(x1_sec+k)+B_left;
				k++;
			}
			//y component
			k=1;
			y.push_back(y2-y1);//y[0]
			int y_tail=y1;
			while
			(
				y2>omega*(k+y1_sec)+B_bottom){
				y[k-1]=(y1_sec+k)*omega+B_bottom-y_tail;
				y.push_back(y2-(y1_sec+k)*omega-B_bottom);
				y_tail=omega*(y1_sec+k)+B_bottom;
				k++;
			}
			
			if(B_length%omega!=0){
				if(x2>B_right-omega){
					if(x2>((B_right-B_left)/omega)*omega+B_left)
					{
						if(x1<B_right-omega)
						{
 							x[x.size()-1]=x2+omega-B_right;
						}
						else{
							x[x.size()-1]=x2-x1;
						}
					}
					else
					{
						if(x1<B_right-omega)
						{
							x.push_back(x2+omega-B_right);
						}
						else{
							x.push_back(x2-x1);
						}
					}
				}
			}
			if(B_width%omega!=0){
				if(y2>B_top-omega){
					if(y2>((B_top-B_bottom)/omega)*omega+B_bottom){//(y2-y1)/omega= number of y's DW -1
						if(y1<B_top-omega){
 							y[y.size()-1]=y2+omega-B_top;
						}else{
							y[y.size()-1]=y2-y1;
						}
					}else{
						if(y1<B_top-omega){
							y.push_back(y2+omega-B_top);
						}else{
							y.push_back(y2-y1);
						}
					}
				}
			}
			
			for(unsigned int p=0;p<x.size();p++){
				for(unsigned int q=0;q<y.size();q++){

				
					windows[x1_sec+p][y1_sec+q]->windowNodes.push_back(groups[i]->groupNodes[j]);//variable check
					windows[x1_sec+p][y1_sec+q]->area.push_back(x[p]*y[q]);
				}			
			}				
		}
	}

}












/////////////////////先給每個group一個初始的顏色。並且算出density//////////////////////////

bool compare (pair <int,int> i,pair <int,int> j)
		{ return (i.second<j.second); }
bool grpcompare (pair <Group*,int> i,pair <Group*,int> j)
		{ return (i.second<j.second); }
bool wincompare (Window* i,Window* j)
		{ return (i->wgroups.size()<j->wgroups.size()); }


void Graph::windowcalc_grparea() 
{
	for(unsigned int i=0;i<windows.size();i++)
  {
  	for (unsigned int j = 0; j < windows[i].size(); ++j)
  	{
  		vector< pair<int,int> > v;

  		for(unsigned int k=0;k<windows[i][j]->windowNodes.size();k++)
  		{	
  			
  			pair <int,int> group(k,windows[i][j]->windowNodes[k]->group_id);
            v.push_back(group);
 
  		}
  		if(v.size()>0)
  		{
  			sort(v.begin(),v.end(),compare);
  			windows[i][j]->wgroups.push_back(v[0].second);
  			int color=(0==windows[i][j]->windowNodes[v[0].first]->color) ?1:-1;
  			windows[i][j]->wgroupsarea.push_back(windows[i][j]->area[v[0].first]*color);
        for(unsigned int k=1;k<v.size();k++)
  			{
  				if(v[k].second!=v[k-1].second)
  				{
  				windows[i][j]->wgroups.push_back(v[k].second);
  				windows[i][j]->wgroupsarea.push_back(0);
  			    }
  			    int color=(0==windows[i][j]->windowNodes[v[k].first]->color) ?1:-1;
  			    windows[i][j]->wgroupsarea[windows[i][j]->wgroupsarea.size()-1]+=windows[i][j]->area[v[k].first]*color;
  			}
  		}

  	}
  }
}























void Graph::greedycolor()
{
	
  for(unsigned int i=0;i<windows.size();i++)
  {
  	for (unsigned int j = 0; j < windows[i].size(); ++j)
  	{
  		averagecolor(i,j);
  	}
  }
  
}




void Graph::averagecolor(int x,int y)
{
	vector<pair <int,int> > v;
	int avgcolor=0;
    for(unsigned int i=0;i<windows[x][y]->wgroups.size();i++)
    {
    	pair <int,int> area (i,windows[x][y]->wgroupsarea[i]);
    	if(groups[windows[x][y]->wgroups[i]]->locked)
    	{
    		int inv= (!groups[windows[x][y]->wgroups[i]]->invert) ?1:-1;
       		avgcolor+=windows[x][y]->wgroupsarea[i]*inv;
       	}
        v.push_back(area);
    }
    sort(v.begin(),v.end(),compare);
	
	for(int i=v.size()-1;i>=0;i--)
       {
       	
       	int id=windows[x][y]->wgroups[v[i].first];
         if(!groups[id]->locked)
         {
         	
          	if(avgcolor<=0)
          	{
          		if(windows[x][y]->wgroupsarea[v[i].first]<0)
           			groups[id]->invert=true;
           		avgcolor+=windows[x][y]->wgroupsarea[v[i].first];	
            }
           else
           {
           	if(windows[x][y]->wgroupsarea[v[i].first]>=0)
           	groups[id]->invert=true;
           	avgcolor-=windows[x][y]->wgroupsarea[v[i].first];
           }
          groups[id]->locked=true;
         }         
       }




   vector<Window*> sortbygrp;
  for(unsigned int i=0;i<windows.size();i++)
  {
  	for (unsigned int j = 0; j < windows[i].size(); ++j)
  	{
  		sortbygrp.push_back(windows[i][j]);
  	}
  }
  sort(sortbygrp.begin(),sortbygrp.end(),wincompare);
  for(unsigned int i=0;i<sortbygrp.size();i++)
  {
  	ptaveragecolor(sortbygrp[i]);
  }

}






void Graph::ptaveragecolor(Window* win)
{
	vector<pair <int,int> > v;
	int avgcolor=0;
    for(unsigned int i=0;i<win->wgroups.size();i++)
    {
    	pair <int,int> area (i,win->wgroupsarea[i]);
    	if(groups[win->wgroups[i]]->locked)
    	{
    		int inv= (!groups[win->wgroups[i]]->invert) ?1:-1;
       		avgcolor+=win->wgroupsarea[i]*inv;
       	}
        v.push_back(area);
    }
    sort(v.begin(),v.end(),compare);
	
	for(int i=v.size()-1;i>=0;i--)
       {
       	
       	int id=win->wgroups[v[i].first];
         if(!groups[id]->locked)
         {
         	
          	if(avgcolor<=0)
           	{

          		if(win->wgroupsarea[v[i].first]<0)
           			groups[id]->invert=true;
           		avgcolor+=win->wgroupsarea[v[i].first];
           	}
           else
           {
           		if(win->wgroupsarea[v[i].first]>=0)
           	groups[id]->invert=true;
           	avgcolor-=win->wgroupsarea[v[i].first];
           }
          groups[id]->locked=true;
         }         
       }
}



void Graph::reset()
{
	for(size_t i=0;i<groups.size();i++)
	{
		groups[i]->locked=false;
		groups[i]->invert=false;
	}
}



void Graph::saveState()
{	lastState.clear();
	for (unsigned int j=colored_graph;j<groups.size();++j)
	{
		if (groups[j]->invert)
			lastState.push_back(true);
		else
			lastState.push_back(false);
	}
}























///////////////////////////////////輸出//////////////////////////////




void Graph::output(ofstream &fout ){ 
	
	int count=0;
	int B_length=B_right-B_left;
	int B_width=B_top-B_bottom;
	for(int j=0;j<ceil(static_cast<double>(B_width)/omega);j++){//x

		for(int i=0;i<ceil(static_cast<double>(B_length)/omega);i++){//y
			int red_area=0;
			int blue_area=0;
			for(unsigned int k=0;k<windows[i][j]->windowNodes.size();k++){
				if(windows[i][j]->windowNodes[k]->color==1){
					if(groups[windows[i][j]->windowNodes[k]->group_id]->invert==0){
						red_area+=windows[i][j]->area[k];
					}else{//invert:red -> blue
						blue_area+=windows[i][j]->area[k];
					}
				}else if(windows[i][j]->windowNodes[k]->color==0){
					if(groups[windows[i][j]->windowNodes[k]->group_id]->invert==0){
						blue_area+=windows[i][j]->area[k];
					}else{//invert:blue -> red
						red_area+=windows[i][j]->area[k];
					}
				}
						
			}
			
///////////////////////////////////先輸出windows//////////////////////////////
			int D_left=B_left+i*omega;
			int D_right=D_left+omega;
			int D_down=B_bottom+j*omega;
			int D_up=D_down+omega;
			//x most right
			if(D_right>B_right){	
				D_right=B_right;
				D_left=B_right-omega;
			}	
			//y most up
			if(D_up>B_top){
				D_up=B_top;
				D_down=B_top-omega;
			}
			count++;
			fout<<"WIN["<<count<<"]="<<D_left<<","<<D_down<<","<<D_right<<","<<D_up<<"("<<float(red_area*100)/float(omega*omega)<<" "<<float(blue_area*100)/float(omega*omega)<<")"<<endl;
			cout<<"WIN["<<count<<"]="<<D_left<<","<<D_down<<","<<D_right<<","<<D_up<<"("<<float(red_area*100)/float(omega*omega)<<" "<<float(blue_area*100)/float(omega*omega)<<")"<<endl;

		}
	}

	///////////////////////////////////再來輸出會被排除掉的group//////////////////////////////
	for(int i=0;i<colored_graph;i++){
		fout<<"GROUP"<<endl;
		for(unsigned int j=0;j<groups[i]->groupNodes.size();j++){
			fout<<"NO["<<j+1<<"]="<<groups[i]->groupNodes[j]->lb_x<<","<<groups[i]->groupNodes[j]->lb_y<<","<<groups[i]->groupNodes[j]->ru_x<<","<<groups[i]->groupNodes[j]->ru_y<<endl;
			cout<<"NO["<<j+1<<"]="<<groups[i]->groupNodes[j]->lb_x<<","<<groups[i]->groupNodes[j]->lb_y<<","<<groups[i]->groupNodes[j]->ru_x<<","<<groups[i]->groupNodes[j]->ru_y<<endl;
		

		}
	}



	
	for(unsigned int i=colored_graph;i<groups.size();i++){
		fout<<"GROUP"<<endl;
		cout<<"GROUP"<<endl;
		vector <int> red;
		vector <int> blue;
		for(unsigned int j=0;j<groups[i]->groupNodes.size();j++){
			if (groups[i]->invert==false)
			{
			   if(groups[i]->groupNodes[j]->color==1){ 
				   red.push_back(j);
			   }
			   else{
				blue.push_back(j);
			   }
			}
			else{
				if(groups[i]->groupNodes[j]->color==1){ 
				   blue.push_back(j);
			   }
			   else{
				red.push_back(j);
			   }
			}		
		}
		///////////////////////////////////輸出二種顏色的group//////////////////////////////
		for(unsigned int j=0;j<red.size();j++)
		{
			fout<<"CA["<<j+1<<"]="<<groups[i]->groupNodes[red[j]]->lb_x<<","<<groups[i]->groupNodes[red[j]]->lb_y<<","<<groups[i]->groupNodes[red[j]]->ru_x<<","<<groups[i]->groupNodes[red[j]]->ru_y<<endl;
			cout<<"CA["<<j+1<<"]="<<groups[i]->groupNodes[red[j]]->lb_x<<","<<groups[i]->groupNodes[red[j]]->lb_y<<","<<groups[i]->groupNodes[red[j]]->ru_x<<","<<groups[i]->groupNodes[red[j]]->ru_y<<endl;
		}
		for(unsigned int j=0;j<blue.size();j++)
		{
			fout<<"CB["<<j+1<<"]="<<groups[i]->groupNodes[blue[j]]->lb_x<<","<<groups[i]->groupNodes[blue[j]]->lb_y<<","<<groups[i]->groupNodes[blue[j]]->ru_x<<","<<groups[i]->groupNodes[blue[j]]->ru_y<<endl;
			cout<<"CB["<<j+1<<"]="<<groups[i]->groupNodes[blue[j]]->lb_x<<","<<groups[i]->groupNodes[blue[j]]->lb_y<<","<<groups[i]->groupNodes[blue[j]]->ru_x<<","<<groups[i]->groupNodes[blue[j]]->ru_y<<endl;
	
		}



	}
	
}






























//////以下是graph的套件/////////////

Matrix::Matrix(const int & m, const int & n)
{
	rows=m;
	columns=n;
	data = new int* [m];

	for ( int i=0;i<m;++i)
	{
		data[i]=new int [n];
		Row* newrow = new Row(i,0.0);
		rowsVec.push_back(newrow);
		rowsMap[i]=newrow;
		//double score=i;
		for (int j=0;j<n;++j)
		{
			data[i][j]=rand()%2;
		}		

	}

	mutate_column = new double [n];
	b_column = new double [n];

	updateProb();
}




Matrix::~Matrix()
{
	if (data!=NULL)
	{
		for ( int i=0;i<rows;++i)
		{
			if (data[i]!=NULL)
			{
				delete [] data[i];
			}
		}
		delete [] data;
	}
	if (mutate_column!=NULL)
		delete mutate_column;
	if (b_column!=NULL)
		delete b_column;
}




void Matrix::updateProb()
{
	b_board.clear();
	int normalize=(1+rows)*rows/2;
	for ( int j=0;j<columns;++j)
	{
		int accum=0;
		for ( int i=0;i<rows;++i)
		{
			accum+=(rows-i)*data[i][j];			
		}
		//pj1 in the paper
		mutate_column[j]=static_cast<double> (accum)/normalize;
		//bj in paper without normalize
		double thenum = 1.0-fabs(1.0-mutate_column[j]-0.5)-fabs(mutate_column[j]-0.5);
		b_column[j]=1.0-fabs(1.0-mutate_column[j]-0.5)-fabs(mutate_column[j]-0.5);
		b_board.push_back(pair<double, int>(thenum,j)); 
	}
}




//score_board contains row num and the row's score 
void Matrix::setRowscore(const int & row_num,const float & score)
{
	rowsMap[row_num]->row_score=score;
}


void Matrix::mogaR()
{
	//sortRowByScore();
	sort(b_board.begin(),b_board.end());


	for ( int i=0;i<floor(4*rows/5);++i)
	{
	   double x=((double) rand() / (RAND_MAX));
	   double ai=static_cast<double> (i)/(rows-1);
	   if (x<0)
	   	cout<<"GG"<<endl;
	 
	   if (x< ai)
	   {
	   	  assert(i!=0);

	   	  int now_row = rowsVec[i]->row_id;
	   	  int k=floor(ai*columns);
	   	  if (k<0)
	   	  	cout<<"error"<<endl;
	   	//  cout<<"is changing"<<k<<" loci"<<endl;
	   	  for (int j=0;j<k;++j)
	   	  {
	   	  	  int now_col = b_board[j].second;
	   	  	  int tochange=0;
	   	  	  if (data[now_row][now_col]==0)
	   	  	  	tochange=1;
	   	  	  else
	   	  	  	tochange=0;
	   	  	  data[now_row][now_col]=tochange;
	   	  }
	   }
	}


	for (int i=floor(4*rows/5);i<rows;++i)
	{
		//cout<<"Row "<<rowsVec[i]->row_id<<"is randoming"<<endl;
		for (int j=0;j<columns;++j)
		{
			data[i][j]=rand()%2;
		}
	}

	updateProb();

}







void Matrix::mogaC()
{
	sortRowByScore();
	sort(b_board.begin(),b_board.end());

	for (int i=0;i<columns;++i)
	{
		double y=((double) rand() / (RAND_MAX));
	    //double ai=static_cast<double> (i)/(score_board.size()-1);
	    if(y<b_column[i])
	    {
	    	int w=floor(b_column[i]*rows);
	    	if (w<0)
	    		cout<<"error"<<endl;
	    	for (int j=rows-w;j<rows;++j)
	    	{
	    		int now_row=rowsVec[j]->row_id;
	    		int tochange=0;
	    		if (data[now_row][i]==0)
	   	  	  	  tochange=1;
	   	  	    else
	   	  	  	  tochange=0;
	   	  	    data[now_row][i]=tochange;
	    	}
	    }
	}

	updateProb();
}



void Matrix::print()
{

	sortRowByScore();

	cout<<endl;
	cout<<endl;

	cout<<"Matrix : "<<endl;
	for (int i=0;i<rows;++i)
	{
		cout<<"row "<<i<<" :";
	    for ( int j=0;j<columns;++j)
	    {
		   cout<<data[i][j]<<" ";
	    }
	    cout<<endl;
	}

	cout<<"mutate_column ";
	for ( int j=0;j<columns;++j)
	{
		cout<<mutate_column[j]<<" ";
	}
	cout<<endl;

	cout<<"b_column ";
	for (int j=0;j<columns;++j)
	{
		cout<<b_column[j]<<" ";
	}
	cout<<endl;


	cout<<"IN rowsVec"<<endl;
	for (unsigned int i=0;i<rowsVec.size();++i)
	{
		cout<<"Row is "<<rowsVec[i]->row_id<<endl;
		cout<<"Score is "<<rowsVec[i]->row_score<<endl;
	    
	}
	cout<<endl;
	
}


bool RowCompByScore(const Row* a,const Row* b)
{
	if (a->row_score > b->row_score)
		return true;
	else
		return false;
}

void Matrix::sortRowByScore()
{
	sort(rowsVec.begin(),rowsVec.end(),RowCompByScore);
}




Edge::Edge(Node *a, Node *b)
{
	if ( a->id <= b->id ) { node[0] = a; node[1] = b; }
	else { node[0] = b; node[1] = a; }
}


bool Edge::operator < (const Edge& rhs) const{

	int id1a = node[0]->id;
	int id1b = node[1]->id;

	int id2a = rhs.node[0]->id;
	int id2b = rhs.node[1]->id;


	if ( id1a == id2a ) return id1b < id2b;
	if ( id1a == id2b ) return id1b < id2a;
	if ( id1b == id2a ) return id1a < id2b;
	if ( id1b == id2b ) return id1a < id2a;
    return true;
}



Node::Node(const int& i,const int& l_x,const int& l_y,const int& r_x,const int& r_y)
{
	id = i;
	traveled = false;
    color = -1;// not colored
	prev = 0;
	lb_x=l_x;
	lb_y=l_y;
	ru_x=r_x;
	ru_y=r_y;

}

void Node::addEdge(Edge *e)
{
	edge.push_back(e);
}




int Node::areaCalc()
{
	return (ru_x-lb_x)*(ru_y-lb_y);
}






void Graph::printall()
{
	cout << endl << "Shape area :" << endl << endl;
	map<int, Node *>::iterator itN;
	for ( itN = nodesMap.begin() ; itN != nodesMap.end() ; itN++ ) {
		 cout << (*itN).second -> id << ": " << (*itN).second -> areaCalc() << endl;
	}

	cout << endl << "Edges :" << endl << endl;
	for (unsigned int i = 0; i < edges.size(); ++i) {
		cout << "edge " << i << " is " << edges[i] -> node[0] -> id << " -- " << edges[i] -> node[1] -> id << endl;
	}

	cout << endl << "Groups of the graph: " << endl << endl;
	for(unsigned int i = 0; i < groups.size(); ++i) {
		if(groups[i] != NULL) { groups[i] -> printGroup(); }
	}
}



Graph::Graph(const string& n,const int& alp,const int& be,const int& ome)
{
	name = n;
	alpha = alp;
	beta = be;
	omega = ome;
	colored_graph=0;
	B_top=numeric_limits<int>::min();
	B_bottom=numeric_limits<int>::max();
	B_right=numeric_limits<int>::min();
	B_left=numeric_limits<int>::max();
}

Graph::~Graph()
{
	vector<Edge *>::iterator itE;
	for ( itE = edges.begin() ; itE != edges.end() ; itE++ )
	{
		if (*itE!=NULL)
		{
			delete (*itE);
		    (*itE) = 0;
		}
	}
	
	map<int, Node *>::iterator itN;
	for ( itN = nodesMap.begin() ; itN != nodesMap.end() ; itN++ )
	{
		if ((*itN).second!=NULL)
		{delete (*itN).second;
		(*itN).second = 0;}
	}

    vector<Node *>::iterator itN2;
	for ( itN2 = nodes.begin() ; itN2 != nodes.end() ; itN2++ )
	{
		(*itN2) = 0;
	}
}



bool NodeCompByD( const Node* A, const Node* B ){
	if ( A->edge.size() > B->edge.size() ) 
        return true;
    else if (A->edge.size() == B->edge.size()) {
        if (A->id < B->id) 
            return true;
        else 
            return false;
    }
    else
        return false;
}

void Graph::sortNodesByDegree()
{
    sort(nodes.begin(), nodes.end(), NodeCompByD);
}




Node * Graph::getNodeById(const int& id)
{
	return nodesMap[id];
}



Group::~Group() {
	for(unsigned int i = 0; i < groupNodes.size(); ++i) {
		if(groupNodes[i] != NULL) {
			delete groupNodes[i];
			groupNodes[i] = NULL;
		}
	}
}

void
Group::printGroup() {
	for(unsigned int i = 0; i < groupNodes.size(); ++i) {
		cout << "group " << groupNodes[i] -> group_id << ": shape " << groupNodes[i] -> id << endl;
	}
	cout << endl;
}
void
Group::calcgrouparea()
{
	for(size_t i=0;i<groupNodes.size();i++)
	{
		grparea+=groupNodes[i]->area;
	}
}


