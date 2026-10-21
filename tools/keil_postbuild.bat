@echo off
"D:\Keil_v5\ARM\ARMCC\bin\fromelf.exe" --bin --output .\Objects\A39X.bin .\Objects\A39X.axf
if errorlevel 1 exit /b %errorlevel%
python .\tools\merge_hex.py .\tools\A390-BOOTLOADER.hex .\Objects\A39X.hex
exit /b %errorlevel%
