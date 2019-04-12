all: serverA.o aws.o monitor.o serverB.o
	g++ -g serverA.cpp -o serverA
	g++ -g aws.cpp -o aws
	g++ -g monitor.cpp -o monitor
	g++ -g serverB.cpp -o serverB
	g++ -g client.cpp -o client

.PHONY: serverB serverA aws monitor

serverB:
	./serverB

serverA:
	./serverA

aws:
	./aws

monitor:
	./monitor
