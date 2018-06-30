
#include <cstring>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "graph.h"
#include "../lib/tm_usage.h"
using namespace std;



void help_message() {
    cout << "usage: ./color_balance <input_file> <output_file>" << endl;
}

int main(int argc, char* argv[])
{
    CommonNs::TmUsage tmusg;
    CommonNs::TmStat stat;
    if(argc != 3) {
       help_message();
       return 0;
    }

    //////////// read the input file /////////////
    ifstream input_file;
    input_file.open(argv[1]);
    if (!input_file.is_open())
    {
      cerr<<"Can not open file "<<argv[1]<<" !!"<<endl;
      return -1;
    }

    int alpha,beta,omega;
    char buf [256];

input_file.read(buf,6);   //截取的文字ALPHA
input_file.getline(buf,200);  //截取後面剩下的數字
alpha=atoi(buf);      //得到ALPHA
cout<<"ALPHA= "<<alpha<<endl;

input_file.read(buf,5);       //截取的文字BETA=
input_file.getline(buf,200);  //截取後面剩下的數字
beta=atoi(buf);       //得到BETA
cout<<"BETA= "<<beta<<endl;

input_file.read(buf,6);       //截取的文字OMEGA=
input_file.getline(buf,200);  //截取後面剩下的數字
omega=atoi(buf);        //得到OMEGA
cout<<"OMEGA= "<<omega<<endl;


//建立graph
    Graph G("color_balance",alpha,beta,omega);
    int shapenum=0;
    int x1,y1,x2,y2;
    while(input_file.getline(buf,256))
    {
        
        size_t start=0;
        size_t end=0; 
        ++shapenum;
        //cout<<"shapenum is "<<shapenum<<endl;
        string ss(buf);

        for (int i=0;i<4;++i)
        { 
           start=end;
           end=ss.find_first_of(',',start);
           if (i==0)
            {
                x1=atoi(ss.substr(start,end-start).c_str()); 
                
            }
           else if (i==1)
            y1=atoi(ss.substr(start,end-start).c_str());
           else if (i==2)
            x2=atoi(ss.substr(start,end-start).c_str());
           else
            y2=atoi(ss.substr(start,end-start).c_str());
          ++end;
        }

        Node* n=new Node(shapenum,x1,y1,x2,y2);
        G.addNode(n);
        //cout<<a<<" "<<b<<" "<<c<<" "<<d<<endl;
    }





    //////////// 把程式碼整理進去graph.cpp////

    G.constructAll();   // 先把上面獨到的node  來組成graph
    G.dfs();          // 用dfs的演算法，  根據上面的graph.    把node都走過一次  把相同group的放在同一群
    G.colorgraph();    // 開始給每個顏色一個
    G.getboxsize();        
    G.mapping();
    G.windowcalc_grparea();
    G.greedycolor();      // 用greedy alrorithm把二邊的density調整一下
    G.saveState();        // 把greddy算完之後的color跟density存起來 


    //////////// write the output file ///////////

    ofstream output_file;
    output_file.open(argv[2]);
    if (!output_file.is_open())
    {
      cerr<<"Can not open output file "<<argv[2]<<" !!"<<endl;
     return -1;
    }

	  G.output(output_file);
    



    cout <<"# run time = " << (stat.uTime + stat.sTime) / 1000000.0 << "sec" << endl;
    cout <<"# memory =" << stat.vmPeak / 1000.0 << "MB" << endl;

    return 0;
}

