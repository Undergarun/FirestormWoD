REM This script, when launched in a folder, look for all the sql files recursively in the folder and merge them in one file called output_sql.

@ECHO OFF

set output_file="output_sql"
type nul >> %output_file%
for /r . %%X in (*.sql) do (
	type "%%X" >> %output_file%
)