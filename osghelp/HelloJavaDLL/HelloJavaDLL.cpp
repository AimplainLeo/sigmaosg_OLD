// HelloJavaDLL.cpp : Defines the exported functions for the DLL application.
//

/*
	Notes: 

		1. compile the java class
			javac test.java

		2. create the c++ header code
			javah -classpath . -jni -o test.h test

		3. compile the c++ dll

		4. create a jar file that contains both the class and dll
			jar cf test.jar test.java Debug/HelloJavaDLL.dll

		5. create a unique key
			keytool -genkey -keystore test.keystore -alias signkey -keypass password -storepass password -keyalg RSA -validity 365 -dname "CN=John Doe, OU=Development, O=ACME"

		6. sign the jar file with the unique key generated in step 5
			jarsigner -keystore test.keystore -storepass password -keypass password test.jar signkey



		http://www.java-tips.org/other-api-tips/jni/simple-example-of-using-the-java-native-interface.html
		http://vkessels.home.xs4all.nl/articles/jnijarapplet.html

		perhaps:
		http://www.codeproject.com/Articles/791/Embed-OpenGL-inside-Java-AWT-Canvas

*/


#include "stdafx.h"
#include "test.h"
#include <jni.h>
#include <stdio.h>
#include <Windows.h>

JNIEXPORT void JNICALL
Java_test_print( JNIEnv* env, jobject obj )
{
	//printf( "Hello World from C++ the new one\n" );
	MessageBoxA(NULL,"Hello World from c++!", "Hello Popup", 0);
	return;
}


