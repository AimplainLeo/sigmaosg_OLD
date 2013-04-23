rem ************ cleanup ************

del test.class
del test.jar
del test.keystore

rem ************ build ************

javac test.java

javah -classpath . -jni -o test.h test

rem need to compile the dll here

jar cf test.jar test.java Debug/HelloJavaDLL.dll

keytool -genkey -keystore test.keystore -alias signkey -keypass password -storepass password -keyalg RSA -validity 365 -dname "CN=John Doe, OU=Development, O=ACME"

jarsigner -keystore test.keystore -storepass password -keypass password test.jar signkey