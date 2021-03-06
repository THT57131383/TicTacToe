#include <iostream>
#include <conio.h>
#include <windows.h>

#define MAX 1
#define N 5

using namespace std;

int map[N][N] = {0};
bool maxTurn = true;
int player, cpu;

//find possible routes to win of player
int checkRoute(int player){
	int routes = 0;
	int count = 0;
	//check horizontal routes to win
	for(int i=0; i<N; i++){
		for(int j=0; j<N; j++){
			if(map[i][j]==0 or map[i][j]==player) count += 1;
			else count = 0;
		}
		if(count == N) routes += 1;
	}
	//check vertical routes to win
	for(int i=0; i<N; i++){
		for(int j=0; j<N; j++){
			if(map[j][i]==0 or map[j][i]==player) count += 1;
			else count = 0;
		}
		if(count == N) routes += 1;
	}
	//check angle routes to win
	for(int i=0; i<N; i++){
		if(map[i][i]==0 or map[i][i]==player) count += 1;
		else count = 0;
	}
	if(count == N) routes += 1;
	for(int i=0; i<N; i++){
		if(map[i][N-1-i]==0 or map[i][N-1-i]==player) count += 1;
		else count = 0;
	}
	if(count == N) routes += 1;
	return routes;
}
//check if a player won
bool checkWin(int player){
	int count;
	//check horizontal routes
	for(int i=0; i<N; i++){
		count = 0;
		for(int j=0; j<N; j++){
			if(map[i][j]==player) count += 1;
			else count = 0;
		}
		if(count == N) return true;
	}
	//check vertical routes
	for(int i=0; i<N; i++){
		count = 0;
		for(int j=0; j<N; j++){
			if(map[j][i]==player) count += 1;
			else count = 0;
		}
		if(count == N) return true;
	}
	count = 0;
	//check angle routes
	for(int i=0; i<N; i++){
		if(map[i][i]==player) count += 1;
		else count = 0;
	}
	if(count == N) return true;
	count = 0;
	for(int i=0; i<N; i++){
		if(map[i][N-1-i]==player) count += 1;
		else count = 0;
	}
	if(count == N) return true;
	//if no win route found
	return false;
}
//check if board is full
bool checkDraw(){
	int count = 0;
	for(int i=0; i<N; i++){
		for(int j=0; j<N; j++){
			if(map[i][j]!=0) count+=1;
		}
	}
	if(count<N*N-1) return false;
	return true;
}
//calculate advantage of current turn over next turn
int getAdvantage(int target, int opponent){
	int score = checkRoute(target) - checkRoute(opponent);
}

void showBoard(){
	for(int i=0; i<N; i++){
		for(int j=0; j<N; j++){
			if(j>0) cout<<"|";
			if(map[i][j]==0) cout<<" ";
			else if(map[i][j]==MAX) cout<<"X";
			else cout<<"O";
		}
		cout<<endl;
		if(i<N-1) for(int k=0; k<2*N-1; k++) cout<<"-";
		cout<<endl;
	}
}

void menu(){
	cout<<"TIC TAC TOE: PLAYER VS CPU\n";
	cout<<"Choose turn:\n";
	cout<<"1.Player firt (X)\n";
	cout<<"2.CPU first (O)";
	bool run = true;
	while(run)
	if(_kbhit()){
		if(_getch()=='1'){
			player = MAX;
			cpu = 2;
			run = false;
		}
		else if(_getch()=='2'){
			cpu = MAX;
			player = 2;
			run = false;
		}
	}
}

void getInput(){
	int input = 0;
	int x, y;
	cout<<"Enter position(XY):";
	while(x>=N || y>=N || map[x][y]!=0){
		cin>>input;
		if(input>10) input -= 11;
		else input -= 1;
		x = input/10;
		y = input%10;
		if(x<N && y<N && map[x][y]==0){
			map[x][y] = player;
			break;
		}
		else{
			cout<<"Enter valid position(XY):";
			input = 0;
		}
	}
}

void sortTrace(int *trace, int adv[][N]){
	int traceHead = 0;
	for(int header=0; header<N*N; header++){
		int min = trace[header]; //min position
		for(int i=min/10; i<N; i++){
			int j;
			if(i>min/10) j = 0;
			else j = min%10;
			for(j; j<N; j++){ //find adv point < min
				if(adv[i][j]>(-2*N-2) && adv[i][j]<adv[min/10][min%10])
					min = i*10+j; //min = position
			}
		}
		int p = 0;
		while(p<N*N && trace[p]!=min) p++; //find min position in traces
		if(p!=traceHead){
			int t = trace[p];
			trace[p] = trace[traceHead];
			trace[traceHead] = t;
		}
		traceHead++;
	}
}

void generateAdv(int adv[][N], int *trace){
	int p = 0;
	for(int i=0; i<N*N;i++) trace[i] = -1;
	for(int i=0; i<N; i++){
		for(int j=0; j<N; j++){
			//get player's advantage for every possible cpu move
			if(map[i][j]==0){
				map[i][j]=cpu;	//try step
				adv[i][j] = getAdvantage(player,cpu);
				trace[p] = i*10+j;	//add trace
				p++;
				map[i][j]=0;	//undo step
			}
			else adv[i][j] = -2*N-2;
		}
	}
}

int alphaEstimation(bool cpuTurn, int trace, int aTrace ,int alpha, int current, int depth){
	int x, y;
	int cAdv = getAdvantage(player,cpu);
		
	if(cAdv < alpha) return trace;
	if(current >= depth) return aTrace;
	
	int t[N*N];
	int a[N][N];
	generateAdv(a,t);
	sortTrace(t,a);
	int min = aTrace;
	int p;
	if(cpuTurn){
		p = 0;
		while(p<N*N){
			x = t[p]/10;
			y = t[p]%10;
			if(t[p]>=0 && map[x][y]==0){
				map[x][y] = cpu; //try step;
				cAdv = getAdvantage(player, cpu);
				map[x][y] = 0;
				if(cAdv>=alpha)
					min = alphaEstimation(false,t[p],aTrace,alpha,current+1,depth);
				else min = t[p];
				if(current==depth || min != aTrace) break;
			}
			p++;
		}
	}
	else{
		p = N*N-1;
		while(p>=0){
			x = t[p]/10;
			y = t[p]%10;
			if(t[p]>=0 && map[x][y]==0){
				map[x][y] = player;
				cAdv = getAdvantage(player, cpu); 
				map[x][y] = 0;
				if(cAdv>=alpha)
				min = alphaEstimation(true,t[p],aTrace,alpha,current+1,depth);
				else min = t[p];
			}
			if(current==depth || min != aTrace) break;
			p--;
		}
	}
	
	if(min!=alpha) return trace;
	return aTrace;
}

int betaEstimation(bool cpuTurn, int trace, int bTrace ,int beta, int current, int depth){
	
}

void cpuMove(){
	int adv[N][N];
	int trace[N*N];
	generateAdv(adv, trace);
	sortTrace(trace, adv);
	int x, y;
	int p;
	int move;
	if(cpu==MAX){ //alpha cut
		int alpha;
		x = trace[0]/10;
		y = trace[0]%10;
		if(trace[0]>=0 && map[x][y]==0) alpha = adv[x][y];
		move = trace[0];
		p = 0;
		while(p<N*N && move==trace[0]){
			x = trace[p]/10;
			y = trace[p]%10;
			if(trace[p]>=0 && map[x][y]==0){
				map[x][y] = cpu; //try step;
				move = alphaEstimation(false,trace[p],trace[0],alpha,0,N*N/2);
				map[x][y] = 0;
			}
			p++;
		}
		x = move/10;
		y = move%10;
		if(map[x][y]==0) map[x][y] = cpu;
	}
	Sleep(1000);
}

void start(){
	bool win = false;
	bool lose = false;
	bool draw = false;
	while(!win && !lose && !draw){
		if(maxTurn){
			if(player==MAX) getInput();
			else cpuMove();
			maxTurn = false;
		}
		else{
			if(player==MAX) cpuMove();
			else getInput();
			maxTurn = true;
		}
		if(checkWin(player)) win = true;
		else if(checkWin(cpu)) lose = true;
		else if(checkDraw()) draw = true;
		system("cls");
		showBoard();
	}
	if(win) cout<<"You won";
	else if(lose) cout<<"You lost";
	else if(draw) cout<<"Draw";
}

int main(){
	menu();
	system("cls");
	start();
	_getch();
	return 0;
}
