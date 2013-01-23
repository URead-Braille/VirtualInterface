#include <stdio.h>

class Foo{
public:
	Foo(){
		printf("Foo created\n");
	}
	~Foo(){
		printf("Foo destroyed\n");
	}
	void call(){
		printf("Foo called\n");
	}
};

int main(){
	Foo f;
	f.call();
	return 0;
}
