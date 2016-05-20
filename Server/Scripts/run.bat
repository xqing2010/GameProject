echo off

protoc.exe --cpp_out=./ PBMessage.proto

lua PacketGen.lua

copy /Y PBMessage.pb.* ..\PacketHandler\
copy /Y PBAutoGen\*    ..\PacketHandler\AutoGen\


echo on

pause