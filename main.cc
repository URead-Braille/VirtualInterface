#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <termios.h>
#include "main.h"

bcell capital = 0x20;  //0b100000
bcell number = 0x17; //0b010111
bcell blank = 0x00; //0b000000

bcell braille[] = {
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
0x00,   // 0b111100 )
0x00,   // 0b111100 (
0x00,   // 0b000000 *
0x04,   // 0b000000 +
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
0x2E,   // 0b101110 W
0x33,   // 0b110011 X
0x3B,   // 0b111011 Y
0x39,   // 0b111001 Z
0x00,   // 0b000000 [
0x00,   // 0b000000 '\'
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
0x2E,   // 0b101110 w
0x33,   // 0b110011 x
0x3B,   // 0b111011 y
0x39   // 0b111001 z
};

bcell mainmenu[] = {capital, braille['M'], braille['a'], braille['i'], braille['n'], blank, capital, braille['M'], braille['e'], braille['n'], braille['u']};
bcell resume[] = {capital, braille['R'], braille['e'], braille['s'], braille['u'], braille['m'], braille['e']};
bcell bookmarks[] = {capital, braille['B'], braille['o'], braille['o'], braille['k'], braille['m'], braille['a'], braille['r'], braille['k'], braille['s']};
bcell browse[] = {capital, braille['B'], braille['r'], braille['o'], braille['w'], braille['s'], braille['e']};
bcell import[] = {capital, braille['I'], braille['m'], braille['p'], braille['o'], braille['r'], braille['t']};
bcell shutdown[] = {capital, braille['S'], braille['h'], braille['u'], braille['t'], braille['d'], braille['o'], braille['w'], braille['n']};

struct termios oldattrs;

main(){
	struct termios attrs;
	tcgetattr(0,&oldattrs);
	memcpy(&attrs, &oldattrs, sizeof(struct termios));
	attrs.c_lflag &= ~(ICANON | ECHO);
	attrs.c_cc[VMIN] = 1;
	attrs.c_cc[VTIME] = 0;
	tcsetattr(0, TCSANOW, &attrs);
	uint8_t r,c;
	uint8_t s = 1;
	while(1){
		clear();
		for(c=0; c<sizeof(mainmenu); c++){
			setchar(r,c+3,mainmenu[c]);

		}
		for(c=0; c<sizeof(resume); c++){
			setchar(r,c+1,resume[c]);
		}
		for(c=0; c<sizeof(bookmarks); c++){
			setchar(r,c+1,bookmarks[c]);
		}
		for(c=0; c<sizeof(browse); c++){
			setchar(r,c+1,browse[c]);
		}
		for(c=0; c<sizeof(import); c++){
			setchar(r,c+1,import[c]);
		}
		for(c=0; c<sizeof(shutdown); c++){
			setchar(r,c+1,shutdown[c]);
		}
		s = choose(s,6);
		switch(s){
			case 0:
				s=1;
				break;
			case 1:
				resume();
				break;
			case 2:
				bookmarks();
				break;
			case 3:
				browse();
				break;
			case 4:
				import();
				break;
			case 5:
				shutdown();
				break;
		}
	}
}

bstring *loctopage(uint32_t loc){
	bstring *ans = (bstring*)malloc(sizeof(bstring));
	int x = loc/(COLS*(ROWS-1))+1;
	int r;
	for(r = ROWS-1; r>=0; r--){
		int t = x/10;
		ans->data[r] = braille['0'+(x-10*t)];
		x = t;
		if(x==0) break;
	}
	ans->data[r] = number;
	while(r>=0){
		ans->data[r]=blank;
		r--;
	}
	ans->len = ROWS;
	return ans;
}

void shutdown(){
	tcsetattr(0, TCSANOW, &oldattrs);
	exit(0);
}

int choose(int start, int num){
	uint8_t s=start;
	setchar(s,0,0xFF);
	char c;
	while(1){
		uint8_t newselect=s;
		read(0, &c, 1);
		switch(c){
			case 'a':
				newselect = s-1;
				break;
			case 'd':
				newselect = s+1;
				break;
			case '\n':
				return s;
			case ' ':
				return -1;
			default:
				newselect = c-'0';
		}
		if(newselect>=1 && newselect<num){
			setchar(s,0,0);
			s=newselect;
			setchar(s,0,0xFF);
		}
	}
}

void resume(){
	int fd = open("/lastfile", O_RDWR);
	if(fd < 0){
		fd = open("/help", O_RDWR);
	}
	reader(fd);
	close(fd);
}

void bookmarks(){
	int s=1;
	int c;
	while(1){
		clear();
		for(c=0; c<mainmenu[2].len; c++){
			setchar(0,c+3,mainmenu[2].data[c]);
		}
		int bfd = open("/bookmarks", O_RDONLY);
		int r;
		int p = 0;
		int fd[ROWS-1];
		for(r=1; r<ROWS; r++){
			uint32_t loc;
			char filename[71];
			filename[0] = '/';
			filename[1] = 'd';
			filename[2] = 'a';
			filename[3] = 't';
			filename[4] = 'a';
			filename[5] = '/';
			filename[70]= '\0';
			read(bfd, &filename[6], 64);
			read(bfd, &loc, 4);
			bstring *line = loctopage(loc);
			fd[r-1] = open(filename, O_RDWR);
			read(fd[r-1], &line->data[1], ROWS-4);
			int c;
			for(c=0; c<COLS; c++){
				setchar(r,c+1,line->data[c]);
			}
		}
		s = choose(s,ROWS-1);
		if(s == -1) return;
		reader(fd[s-1]);
	}
}

void browse(){
	int s=1;
	int c;
	while(1){
		clear();
		for(c=0; c<mainmenu[3].len; c++){
			setchar(0,c+3,mainmenu[3].data[c]);
		}
		char* a = " hello";
		char* b = " world";
		for(c=0; c<strlen(a); c++){
			setchar(1,c,braille[a[c]]);
		}
		for(c=0; c<strlen(b); c++){
			setchar(1,c,braille[b[c]]);
		}
		s = choose(s, 2);
		if(s == -1) return;
		else if(s == 1){
			int fd = open("/hello",O_RDWR);
			reader(fd);
			close(fd);
		}
		else{
			int fd = open("/world", O_RDWR);
			reader(fd);
			close(fd);
		}
	}
}

void import(){
	clear();
	char* msg = "No usb drive detected";
	int c;
	for(c=0; c<mainmenu[4].len; c++){
		setchar(0,c+3,mainmenu[4].data[c]);
	}
	setchar(1,0,capital);
	for(c=0; c<strlen(msg); c++){
		setchar(1,c+1,braille[msg[c]]);
	}
	char ch;
	while(1){
		read(0, &ch, 1);
		if(ch==' '){
			return;
		}
	}
}

void reader(int fd){
	clear();
	int r,c;
//	for(c=0; c<COLS; c++){
//		setchar(0,c,top[c]);
//	}
	bcell grid[ROWS-1][COLS];
	while(1){
		read(fd, grid, sizeof(grid));
		for(r=1; r<ROWS; r++){
			for(c=0; c<COLS; c++){
				setchar(r,c,grid[r][c]);
			}
		}
		char ch;
		while(1){
			read(0, &ch, 1);
			if(ch == ' ') return;
			if(ch == 'a'){
				//Not implemented
			}
			if(ch == 'd'){
				//Not implemented
			}
		}
	}
}

void setchar(int cellr, int cellc, bcell cell){
	int r, c;
	for(r=0; r<3; r++){
		for(c=0; c<2; c++){
			set(cellr, cellc, r, c, cell&1);
			cell = cell >> 1;
		}
	}
}

void clear(){
	printf("\033[2J");
}

void set(int cellr, int cellc, int r, int c, bcell on){
	int row = r+cellr*4+1;
	int col = c+cellc*3+1;
	printf("\033[%d;%dH",row,col);
	if(on){
		printf("\333");
	}
	else{
		printf(" ");
	}
	printf("\033[%d;%dH",25,0);
	fflush(stdout);
}
