// test.java

/*
import java.applet.*;
import java.awt.*;

public class test extends Applet 
{
	public void init() 
	{
		//resize(150,25);
    }
	
	public void paint(Graphics g) 
	{
		g.drawString("Hello world!", 25, 50);
	}
}
*/

import java.applet.*;
import java.awt.*;

public class test extends Applet 
{

	private native void print();

	public static void main( String[] args )
	{
		new test().print();
	}

	static 
	{
		System.loadLibrary( "Debug/HelloJavaDLL" );
	}

	public void init() 
	{
		//resize(150,25);
    }

	public void paint(Graphics g) 
	{
		g.drawString("Hello world from java2!", 25, 50);
	}
}