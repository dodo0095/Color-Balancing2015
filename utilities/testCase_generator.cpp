// **************************************************************************
//  File       [testcase_generator.cpp]
//  Author     [Yu-Wei Chang]
//  Synopsis   [The main program of 2017 Spring Algorithm Final Project]
//  Modify     [2017/05/11 Yu-Wei Chang]
// **************************************************************************

/*
	使用方法:
	
	1. 決定 N,ALPHA,BETA,OMEGA,scale,xDiff,yDiff，總共7個參數的值。（在code第42~48行中給值)
	2. 決定 output檔的名稱，在code中第76行的fout.open那邊，將iccad2015_input.case30這串取代成想要命名的名稱。(假設取代成input.case1)
	3. compile。
	4. 執行，即可跑出一個output檔，也就是測資，名為input.case1。

	附註：
	其中的各個參數意思解釋如下，
	N：想要產生的shape數量。
	ALPHA,BETA,OMEGA：如同題目規定的意思。
	scale：x軸和y軸的坐標範圍都設定為0~scale-1。
	xDiff：限定shape的x方向之長度，小於xDiff。
	yDiff：限定shape的y方向之長度，小於yDiff。
	
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <vector>
#include <math.h>
#include <algorithm>

using namespace std; 

int main(int argc, char **argv){
	
	int i,j,N;
	int count=0;
	
	srand(time(NULL));
	
	
	
	int ALPHA,BETA,OMEGA,scale,xDiff,yDiff;
	ALPHA=2;
	BETA=2;
	OMEGA=3;
	N=4;
	scale=10;
	xDiff=4;
	yDiff=4;
	
	int input[4];
	
	int checkX,checkY;	
	int x,y;
	
	bool** exist;
	exist = new bool*[scale];
	for(int n=0;n<scale;n++){
		exist[n] = new bool[scale];
	}
	
	y=0;
	while(1){
		if(y>scale-1){
			break;
		}
		for(x=0;x<=scale-1;x++){
			exist[x][y]=false;
		}
		y++;		
	}
	
	int fail=0;
	
	fstream fout;
	
	fout.open("iccad2015_input.case30",ios::out);
	
	fout<<"ALPHA="<<ALPHA<<endl;
	fout<<"BETA="<<BETA<<endl;
	fout<<"OMEGA="<<OMEGA<<endl;
	
	//create N shapes
	while(1){
		
		for(i=0;i<2;i++){
			input[i]=rand()%scale;
		}
		input[2]=input[0]+(rand()%xDiff);
		input[3]=input[1]+(rand()%yDiff);
		if(input[2]>=scale-1){
			input[2]=scale-1;
		}
		if(input[3]>=scale-1){
			input[3]=scale-1;
		}
		
		
		//check shape是否正確
		checkX=input[2]-input[0];
		checkY=input[3]-input[1];
		
		if((checkX<=0)||(checkY<=0)){
			continue;
		}
		
		x=input[0];
		y=input[1];	
		
		//check是否有重疊的shape
		while(1){ 	
			fail=0;
			if(y>input[3]){
				break;
			}
			for(x=input[0];x<=input[2];x++){
				if(exist[x][y]==true){
					fail=1;
					break;
				}
			}
			if(fail==1){
				break;
			}
			else{
				y++;
				continue;
			}
		}
		
		
		if(fail==1){
			continue;
		}
		//modify exist and write output(將欲佔領的部分的bool改成true,還有寫檔)
		x=input[0];
		y=input[1];	
		while(1){
			if(y>input[3]){
				break;
			}
			for(x=input[0];x<=input[2];x++){
				exist[x][y]=true;
			}
			y++;
			
		}
		fout
		<<input[0]<<","<<input[1]<<","<<input[2]<<","<<input[3]<<endl;
		
		//count the # of shapes
		count++;
		if(count==N){
			break;
		}
	}
	
	
	return 0;
}