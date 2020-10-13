SET build_tools_path=%cd%

SET build_path=C:\Plainoldprogrammer\Yasm
CD %build_path%

REM Delete all folders except db
FOR /D %%p IN (*) DO IF NOT %%p == db rmdir "%%p" /s /q

REM Delete all files except the neccesary bat files
FOR %%i IN (*) DO (
	IF NOT %%i == build_release_with_dependencies.bat (
		IF NOT %%i == clean.bat (
			DEL %%i
		)
	)
)

CD %build_tools_path%

