#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

char braille[] = {
0x00,   // 0b000000 NULL
0x00,   // 0b000000 START OF HEADING
0x00,   // 0b000000 START OF TEXT
0x00,   // 0b000000 END OF TEXT
0x00,   // 0b000000 END OF TRANSMISSION
0x00,   // 0b000000 ENQUIRY
0x00,   // 0b000000 ACK
0x00,   // 0b000000 BELL
0x00,   // 0b000000 BACKSPACE
0x00,   // 0b000000 TAB
0x00,   // 0b000000 \n
0x00,   // 0b000000 Vertical Tab
0x00,   // 0b000000 New Page
0x00,   // 0b000000 \r
0x00,   // 0b000000 Shift Out
0x00,   // 0b000000 Shift In
0x00,   // 0b000000 Data Link Escape
0x00,   // 0b000000 Dev Control 1
0x00,   // 0b000000 Dev Control 2
0x00,   // 0b000000 Dev Control 3
0x00,   // 0b000000 Dev Control 4
0x00,   // 0b000000 NAK
0x00,   // 0b000000 Synchronous Idle
0x00,   // 0b000000 End of Transmission Block
0x00,   // 0b000000 Cancel
0x00,   // 0b000000 End of Medium
0x00,   // 0b000000 Substitute
0x00,   // 0b000000 Escape
0x00,   // 0b000000 File separator
0x00,   // 0b000000 Group Separator
0x00,   // 0b000000 Record Separator
0x00,   // 0b000000 Unit Separator
0x00,   // 0b000000 Space
0x1C,   // 0b011100 !
0x38,   // 0b111000 "
0x00,   // 0b000000 #
0x00,   // 0b000000 $
0x00,   // 0b000000 %
0x00,   // 0b000000 &
0x00,   // 0b010000 '
0x00,   // 0b000000 *
0x04,   // 0b000100 ,
0x30,   // 0b110000 -
0x0D,   // 0b001101 .
0x12,   // 0b010010 /
0x00,   // 0b000000 0
0x00,   // 0b000000 1
0x00,   // 0b000000 2
0x00,   // 0b000000 3
0x00,   // 0b000000 4
0x00,   // 0b000000 5
0x00,   // 0b000000 6
0x00,   // 0b000000 7
0x00,   // 0b000000 8
0x00,   // 0b000000 9
0x0C,   // 0b001100 :
0x14,   // 0b010100 ;
0x00,   // 0b000000 <
0x00,   // 0b000000 =
0x00,   // 0b000000 >
0x34,   // 0b110100 ?
0x00,   // 0b000000 @
0x01,   // 0b000001 A
0x05,   // 0b000101 B
0x03,   // 0b000011 C
0x0B,   // 0b001011 D
0x09,   // 0b001001 E
0x07,   // 0b000111 F
0x0F,   // 0b001111 G
0x0D,   // 0b001101 H
0x06,   // 0b000110 I
0x0E,   // 0b001110 J
0x11,   // 0b010001 K
0x15,   // 0b010101 L
0x13,   // 0b010011 M
0x1B,   // 0b011011 N
0x19,   // 0b011001 O
0x17,   // 0b010111 P
0x1F,   // 0b011111 Q
0x1D,   // 0b011101 R
0x16,   // 0b010110 S
0x1E,   // 0b011110 T
0x31,   // 0b110001 U
0x35,   // 0b110101 V
0x3E,   // 0b101110 W
0x33,   // 0b110011 X
0x3B,   // 0b111011 Y
0x39,   // 0b111001 Z
0x00,   // 0b000000 [
0x00,   // 0b000000 \
0x00,   // 0b000000 ]
0x00,   // 0b000000 ^
0x00,   // 0b000000 _
0x00,   // 0b000000 `
0x01,   // 0b000001 a
0x05,   // 0b000101 b
0x03,   // 0b000011 c
0x0B,   // 0b001011 d
0x09,   // 0b001001 e
0x07,   // 0b000111 f
0x0F,   // 0b001111 g
0x0D,   // 0b001101 h
0x06,   // 0b000110 i
0x0E,   // 0b001110 j
0x11,   // 0b010001 k
0x15,   // 0b010101 l
0x13,   // 0b010011 m
0x1B,   // 0b011011 n
0x19,   // 0b011001 o
0x17,   // 0b010111 p
0x1F,   // 0b011111 q
0x1D,   // 0b011101 r
0x16,   // 0b010110 s
0x1E,   // 0b011110 t
0x31,   // 0b110001 u
0x35,   // 0b110101 v
0x3E,   // 0b101110 w
0x33,   // 0b110011 x
0x3B,   // 0b111011 y
0x39   // 0b111001 z
};

char* def = "/data/sample.pdf";

void setchar(int, int, char);
void set(int, int, int, int, char);
void clear();
void go(char*, int);

int main(){
	clear();
	go(def, strlen(def));
	while(1){
		sleep(1);
	}
}

void go(char* filename, int len){
	FILE *stream;
	if(len >= 4 && strcasecmp(&filename[len-4],".pdf")==0){
		char* argv[] = {"/pdftotext", filename, "/data/temp.txt", NULL};
		char* envp[] = {NULL};
		if(fork()==0){
			execve(argv[0], argv, envp);
			exit(1);
		}
		int status;
		do{
			wait(&status);
		} while(!WIFEXITED(status));
		stream = fopen("/data/temp.txt","r");
	}
	else{
		FILE* stream = fopen(filename, "r");
	}
	int r = 0, c = 0, i = 0;
	for(i = 0; i < 10; i++){
		int ch = fgetc(stream);
		if(ch == EOF){
			break;
		}
		else if(ch == '\n'){
			r++;
			c=0;
		}
		else{
			setchar(0,i,ch);
			c++;
		}
	}
}

void setchar(int cellr, int cellc, char ch){
	char bitmap;
	if(ch >= sizeof(braille)){
		bitmap = 0;
	}
	else{
		bitmap = braille[ch];
	}
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
