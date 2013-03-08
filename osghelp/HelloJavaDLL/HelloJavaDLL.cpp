// HelloJavaDLL.cpp : Defines the exported functions for the DLL application.
//

/*
	Notes: 

		javac test.java
		javah test


*/


#include "stdafx.h"
#include "test.h"
#include <jni.h>
#include <stdio.h>

JNIEXPORT void JNICALL
Java_test_myout( JNIEnv* env, jobject obj )
{
	printf("Hello World from C++\n");
	return;
}


