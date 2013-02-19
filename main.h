#include <stdint.h>

#define ROWS 6
#define COLS 27

typedef uint8_t bcell;

typedef struct bstring_t{
	uint8_t len;
	bcell data[COLS];
} bstring;

int main();
void setcell(int, int, bcell);
void setchar(int, int, bcell);
void set(int, int, int, int, bcell);
void clear();
void go(char*, int);
void reader(int);
void browse();
int choose(int, int);
void import();
void resume();
void bookmarks();
void shutdown();
