
	void main() {
		fib(24);
	}
	
	void fib(int limit) {
		int last = 0;
		int x = 1;
		while (x < limit) {
			iprint(x + last);
			cprint(' ');
			last = x;
			x = x + 1;
		}
		cprint('\n');
	}
	
	native void iprint(int);
	native void cprint(char);