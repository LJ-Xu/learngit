md src
for %%i in (*.proto) do ( protoc.exe --cpp_out=..\..\..\..\Modules\Business\src %%i )
pause 
