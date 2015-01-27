@echo off
"c:\bin\tasm32.exe" /ml /l /m3  main.asm main.obj
"c:\bin\tlink32.exe" /Tpe /aa  main.obj ,main.exe,, "c:\bin\zz.lib"
if exist makeex.exe makeex
