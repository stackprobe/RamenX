C:\Factory\Tools\RDMD.exe /RC out

SET RAWKEY=dafecc731331730321f96f40064789fe1344594fd58982838a6c758728271121

C:\Factory\SubTools\resCluster.exe /L Music.txt       /D C:\tmp /K %RAWKEY% /C out\Data1.dat
C:\Factory\SubTools\resCluster.exe /L Picture.txt     /D C:\tmp /K %RAWKEY% /C out\Data2.dat
C:\Factory\SubTools\resCluster.exe /L SoundEffect.txt /D C:\tmp /K %RAWKEY% /C out\Data3.dat

COPY RamenX\Release\RamenX.exe out\しろくまACT.exe
rem COPY IMenu\IMenu\bin\Release\IMenu.exe "out\しろくまACT スタート.exe"
C:\Factory\Tools\xcp.exe doc out
ren out\Manual.txt マニュアル.txt
ren out\Changelog.txt 更新履歴.txt

C:\Factory\SubTools\zip.exe /G out ShirokumaACT
C:\Factory\Tools\summd5.exe /M out

IF NOT "%1" == "/-P" PAUSE
