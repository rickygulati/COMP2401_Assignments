all: plannerTester.o pathplanner.o display.o
	gcc -o plannerTester plannerTester.o display.o pathplanner.o -lX11

bigEnvironment: bigEnvironment.o pathplanner.o display.o
	gcc -o bigEnvironment bigEnvironment.o pathplanner.o display.o -lX11

pathplanner.o: pathplanner.c obstacles.h
	gcc -c pathplanner.c

bigEnvironment.o: bigEnvironment.c obstacles.h display.h
	gcc -c bigEnvironment.c

plannerTester.o: plannerTester.c obstacles.h display.h
	gcc -c plannerTester.c

display.o: display.c obstacles.h display.h
	gcc -c display.c -lX11

clean:
	rm -f *.o
