all : osmupdate_simple

osmupdate_simple: osmupdate_simple.c
	g++ osmupdate_simple.c -o osmupdate_simple
