@echo off
echo ������ʱ�ļ������ڴ���ת��

del  UI2016.sdf
rmdir Build /S /Q
rmdir ipch  /S /Q

del  Bin\Debug\UI\*.dll
del  Bin\Debug\UI\*.exe
del  Bin\Debug\UI\*.ilk

pause