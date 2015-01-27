@echo off
kpasm.exe ex.kpasm
tasm32.exe /ml /l /m3  main.asm main.obj
tlink32.exe /Tpe /aa  main.obj ,main.exe,, 
makeex
if exist main.obj del main.obj
if exist main.map del main.map
if exist main.LST del main.LST
pause
