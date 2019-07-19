set masmhome=d:\masm32
set path=%masmhome%\bin;%masmhome%;.;
set include=%masmhome%\include;%include%
set lib=%masmhome%\lib;%lib%;
ML /c /coff symTDIxploit.asm
Link /SUBSYSTEM:WINDOWS symTDIxploit.obj
pause