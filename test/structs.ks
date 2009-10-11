
	struct foo 
	{
		int x;
		new(int y) 
		{
			x = y * 2 - 8;
			foo(x);
			bar(x);
		}
		void foo(int bar) {
			iprintln(bar + x);
		}
		static void bar(int x) {
			iprintln(x);
		}
	}
	
	void main() 
	{
		foo f = new foo(9);	
	}
	
	native void iprintln(int);
	native void cprint(char);
	native void cprintln(char);