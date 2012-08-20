@ECHO OFF

for %%f in (*.asm) do (
	nasm -O0 -f bin -o "%%~nf.bin" "%%~nf.asm"
	echo Compiling %%~nf.asm
)