C:\Factory\Tools\RDMD.exe /RC out ^
/MD out\bin ^
/MD out\bin\bin ^
/MD out\res

C:\Factory\DevTools\SemiRelease.exe Music.txt       out out\res\Music_       .\res\Music_
C:\Factory\DevTools\SemiRelease.exe Picture.txt     out out\res\Picture_     .\res\Picture_
C:\Factory\DevTools\SemiRelease.exe SoundEffect.txt out out\res\SoundEffect_ .\res\SoundEffect_

COPY RamenX\Release\RamenX.exe out\bin\bin

> out\_StartTheGameForDebug.bat ECHO START /Dbin\bin RamenX.exe

PAUSE
