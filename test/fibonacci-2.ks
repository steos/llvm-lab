
	void main() {
		fib(42);
	}
	
	void fib(int limit) {
		int n = 0;
		int m = 1;
		int x = 0;
		while (x < limit) {
			x = n + m;
			n = m;
			m = x;
			iprint(x);
			cprint(' ');
		}
		cprint('\n');
	}
	
	native void iprint(int);
	native void cprint(char);