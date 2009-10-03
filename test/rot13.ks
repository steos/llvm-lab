
	void main()
	{
		int source = 'x';
		int result = rot13(source);
		int back = rot13(result);
		cprintln(source);
		iprintln(source);
		cprintln(result);
		iprintln(result);
		cprintln(back);
		iprintln(back);
	}

	int rot13(int c) {
		int temp = c;
		int cap = temp & 32;
		temp = temp & (~cap);
		if (temp >= 'A' && temp <= 'Z') {
			temp = (temp - 'A' + 13) % 26 + 'A';
		}
		return temp | cap;
	}

	native void cprintln(char);
	
	native void iprintln(int);