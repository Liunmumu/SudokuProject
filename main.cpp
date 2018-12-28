#include<iostream>
#include<algorithm>
#include<cstring>
#include<ctime> 
#include<vector>
#include<fstream>

using namespace std;

//  用于生成数独的函数及变量 
void ConnectChanges(); 
void RowChanges(char *rowchange); 
void CreatOutput();
	
	int out_count;
	int a_s_n=0; 
	int requier_count;
	char all_sudoku[200000000];
	char model[10]={ '1','2','3','4','5','7','8','9' };
	
//	用于求解数独的函数及变量 
ofstream solution_outfile("sudoku.txt");
	int s_num=0;
	int row,col;
	char input[20];
	char map[20][20];
	char vis[3][10][10];
	char save_output[800];
	bool isMap=false;

// functions used to create sudoku 	
void ConnectChanges(){
	
	char change1[3][4] = { "063","036" };
	char change2[6][4] = { "258","528","825","852","582","285" };
	char change3[6][4] = { "147","417","714","741","471","174" };
	
	model[8] = '6';  //  (2+3) % 9 + 1 = 6

		for (int i = 0; i < 2; i++){
			for (int j = 0; j < 6; j++){
				for (int k = 0; k < 6; k++){   // calculate all translational changes. 2*6*6 = 72 。获得72种平移组合方式
					
					RowChanges(change1[i]);
					RowChanges(change2[j]);
					RowChanges(change3[k]);
					all_sudoku[a_s_n++] = '\n';

					if (++out_count == requier_count){
					return ;
					}				
				}
			}
		}
		
	while (next_permutation(model, model + 8));   //get the next set of permutation, with TSL .                                                                                           
}

void RowChanges(char *rowchange){
	
	for(int m=0;m<3;m++){
		all_sudoku[a_s_n++] = model[(8+rowchange[m]-'0')%9];
		for(int n=0 ; n<16 ; n++){
			all_sudoku[a_s_n++] = ' ';
			n++;
			all_sudoku[a_s_n++] = model[((15-n)/2 + rowchange[m] - '0') % 9 ]; //将数组变成可左右平移的环形 
		}		
		all_sudoku[a_s_n++] = '\n';
	}
}

void CreatOutput(){
	ofstream outfile("sudoku.txt");
	outfile << all_sudoku ;
	outfile.close();
}

// functions used to create sudoku
void MarkVis(int row, int col, int num,int opt){
		vis[0][row][num] = opt;
		vis[1][col][num] = opt;
		vis[2][row / 3 * 3 + col/3][num] = opt;
}

bool Check(int row, int col, int num){
	if(vis[0][row][num]==0 && vis[1][col][num]==0 && vis[2][row/3*3+col/3][num]==0)
		return true;
	else
		return false;
}

void GetProblem(int row, char *input){
	for(int i=0;i<17;i++){
		if(i%2 == 0){
			map[row][i/2] = input[i];
			MarkVis(row,i/2,input[i]-'0',1);
		}
	}
}

void SolveSudoku(int row,int col){
	while(map[row][col]!= '0'){
		if(col<8) col++;
		else col=0, row++;
		
		if(row==9){
			isMap = true ;
			return;
		}
	}
	bool isSearch = false;
	
	for(int i=1;i<=9;i++) {
		if(Check(row,col,i)){  //判断点(row,col)是否可以放置此数 
			MarkVis(row,col,i,1); //若可放，则对应位置的vis数组设为1 
			map[row][col] = i+'0';
			isSearch = true;
			SolveSudoku(row,col); // 进行下一轮搜索 
		}
		
		if(isSearch){
			isSearch = false;
			if(isMap) return;
			else {
				map[row][col]='0'; //若未找到解，设置此点数字为初始值 
				MarkVis(row,col,i,0);// 回溯并重置vis数组 
			}
		}
	}
}

void SaveSolution(){	
	int p=0;
	if(s_num!=0) solution_outfile << endl;
	s_num++;
	
	for(int i=0;i<9;i++){
		for(int j=0;j<9;j++){
			save_output[p++] = map[i][j];
			if((p+1)%18 == 0)
				save_output[p++] = '\n';
			else
				save_output[p++] = ' ';
		}
	}	
	solution_outfile << save_output;
}


int main(int argc,char* argv[]){
	
//	double duration;
//    clock_t start, finish;
//    start = clock();
	
	if (argc < 2){    //without parameters
		printf("Please enter parameters!\n");
		return 0;
	}

	if (strcmp(argv[1], "-c") == 0){    //判断命令为终局生成 

		if ( requier_count = atoi(argv[2])){
			while( out_count < requier_count)
				ConnectChanges(); 
				
			CreatOutput(); //生成数独终局
		}			
		else
			printf("The number should be 1~1000000 !\n");    
	}

	else if (strcmp(argv[1], "-s") == 0){    //判断命令为残局求解 

		FILE *file = fopen(argv[2], "r");  //获取题目文档 
		char input[100];
		int count = 0;
		
		while(fgets(input,20,file)){
			if(strcmp(input,"\n") == 0) continue;
			GetProblem(count,input);
			count++;
				
			if(count==9){ //已取得一道题目可以开始求解 
			isMap = false;
				SolveSudoku(0,0);
				count=0;
				SaveSolution();  //求解结束 
				memset(vis,0,sizeof(vis));  //将vis数组置零 
			}	
		}
	}	
//	duration = (double)(finish - start);
//	printf( "%f\n", duration );	
	return 0;
}
