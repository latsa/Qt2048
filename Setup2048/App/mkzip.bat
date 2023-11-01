copy C:\Qt\5.15.1\msvc2019_64\bin\Qt5Core.dll .
copy C:\Qt\5.15.1\msvc2019_64\bin\Qt5Gui.dll .
copy C:\Qt\5.15.1\msvc2019_64\bin\Qt5Widgets.dll .
copy C:\Windows\system32\VCRUNTIME140.dll .
copy C:\Windows\system32\VCRUNTIME140_1.dll .
copy Release\2048.exe .
copy "..\..\2048\bin\x64\Release\2048.exe" .

mkdir platforms
cd platforms
copy "C:\Qt\5.15.1\msvc2019_64\plugins\platforms\qwindows.dll" .
cd ..

mkdir styles
cd styles
copy "C:\Qt\5.15.1\msvc2019_64\plugins\styles\qwindowsvistastyle.dll" .
cd ..

zip App.zip *.exe *.dll platforms\*