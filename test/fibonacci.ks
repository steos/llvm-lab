
	void main()
	{	
		iprintln(fibonacci(24));
	}

	int fibonacci(int x)
	{
		if (x <= 2) {
			return 1;
		}

		return fibonacci(x - 1) + fibonacci(x - 2);
	}
	
	native void iprintln(int);
	
	
