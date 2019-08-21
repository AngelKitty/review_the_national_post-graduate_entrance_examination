@echo off

cls
dir *.java
echo Ready to compile the above JAVA programs?
pause

cls
javac *.java

cls
dir TestVector.class
echo Ready to run TestVector?
pause
cls
java TestVector
pause

cls
dir TestList.class
echo Ready to run TestList?
pause
cls
java TestList
pause

cls
echo on