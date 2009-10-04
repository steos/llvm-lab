
	void main() {
		float x = 9.9f;
		double y = 9e99;
		double z = 3.1415;
		
		byte bar = (byte)1024;
		
		long foo = 3;
		
		iprintln((int)bar);
		iprintln((int)x);
		iprintln((int)y);
		iprintln((int)z);
	}
	
	native void iprintln(int);