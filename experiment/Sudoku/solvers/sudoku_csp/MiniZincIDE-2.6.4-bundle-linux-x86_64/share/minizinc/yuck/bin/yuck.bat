@REM Yuck launcher script

@echo off

set "APP_HOME=%~dp0\.."
set "LIB_DIR=%APP_HOME%\lib\"
set "YUCK_JAVA_OPTS=-Djava.lang.Integer.IntegerCache.high=10000 -XX:+UseParallelGC"
set "CLASS_PATH=%LIB_DIR%\rtree-1.0.5.jar;%LIB_DIR%\scopt_3-4.0.1.jar;%LIB_DIR%\jgrapht-core-1.4.0.jar;%LIB_DIR%\scala-parser-combinators_3-2.1.0.jar;%LIB_DIR%\scala3-library_3-3.1.3.jar;%LIB_DIR%\jheaps-0.11.jar;%LIB_DIR%\scala-library-2.13.8.jar;%LIB_DIR%\yuck-20221101.jar"
set "MAIN_CLASS=yuck.flatzinc.runner.FlatZincRunner"

java %YUCK_JAVA_OPTS% %JAVA_OPTS% -cp %CLASS_PATH% %MAIN_CLASS% %*
