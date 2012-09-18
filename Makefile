<<<<<<< HEAD
osmupdate: osmupdate_simple.c
	g++ osmupdate_simple.c -g -o osmupdate


test : osmupdate
	./osmupdate -v --minutely --max-days=15551 --keep-tempfiles --planet-url=http://pine02.fosm.org/planet/  2011-07-15T23:30:00Z osm.osm 

debug : osmupdate
	gdb --args ./osmupdate -v --minutely --max-days=15551 --keep-tempfiles --planet-url=http://pine02.fosm.org/planet/  2011-07-15T23:30:00Z osm.osm 

clean :
	rm osmupdate

=======
all : osmupdate_simple

osmupdate_simple: osmupdate_simple.c
	g++ osmupdate_simple.c -o osmupdate_simple
>>>>>>> 77aaa3201dfd80ad1451810541f4dcd1f96dd041
