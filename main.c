#include <stdio.h>

void setchar(int, int, char);
void set(int, int, int, int, char);
void clear();

int main(){
	clear();
	setchar(0,0,'b');
	setchar(0,1,'a');
	setchar(0,2,'d');

	setchar(1,0,'c');
	setchar(1,1,'a');
	setchar(1,2,'b');
	while(1){
		sleep(0);
	}
}

char letters[] = {
0x1, // 000001
0x5, // 000101
0x3, // 000011
0xB, // 001011
0x9};// 001001

void setchar(int cellr, int cellc, char ch){
	unsigned char bitmap = letters[ch-'a'];
	int r, c;
	for(r=0; r<3; r++){
		for(c=0; c<2; c++){
			set(cellr, cellc, r, c, bitmap&1);
			bitmap = bitmap >> 1;
		}
	}
}

void clear(){
	printf("\033[2J");
}

void set(int cellr, int cellc, int r, int c, char on){
	int row = r+cellr*4+1;
	int col = c+cellc*3+1;
	printf("\033[%d;%dH",row,col);
	if(on){
		printf("+");
	}
	else{
		printf("-");
	}
	fflush(stdout);
}
