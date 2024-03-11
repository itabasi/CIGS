setup.o: src/DAQ/setup.cxx
		g++  -g -O2 -Wall -pthread -m64 $(shell root-config --cflags --glibs)  -c -o lib/setup.o src/DAQ/setup.cxx

rbcp.o: src/RBCP/rbcp.cxx
		g++  -g -O2 -Wall -pthread -m64 $(shell root-config --cflags --glibs)  -c -o lib/rbcp.o src/RBCP/rbcp.cxx

DAQ.o: src/DAQ/DAQ.cxx 
		g++  -g -O2 -Wall -pthread -m64 $(shell root-config --cflags --glibs)  -c -o lib/DAQ.o src/DAQ/DAQ.cxx

rdf2root.o: src/rdf2root/rdf2root.C
		g++  -g -O2 -Wall -pthread -m64 $(shell root-config --cflags --glibs)  -c -o lib/rdf2root.o src/rdf2root/rdf2root.C

DAQ: lib/rbcp.o lib/setup.o lib/DAQ.o lib/rdf2root.o
		g++ -g -O2 -Wall -o DAQ lib/rbcp.o lib/setup.o lib/DAQ.o $(shell root-config --cflags --glibs) -ltbb 
clean:

		rm -f ./bin/*.o
		rm -f ./lib/*.o
