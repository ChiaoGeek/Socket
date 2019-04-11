all: serverA.cpp  serverB.cpp  client.cpp  aws.cpp monitor.cpp
	g++ -o serverA serverA.cpp
	g++ -o serverB serverB.cpp
	g++ -o client client.cpp
	g++ -o aws aws.cpp
	g++ -o monitor monitor.cpp
	g++ -o udpClient udpClient.cpp
	
serverA: serverA.cpp
	g++ -o serverA serverA.cpp

serverB: serverB.cpp
	g++ -o serverB serverB.cpp

client: client.cpp
	g++ -o client client.cpp

aws: aws.cpp
	g++ -o aws aws.cpp

monitor: monitor.cpp
	g++ -o monitor monitor.cpp


clean:
	$(RM) client serverA serverB client aws monitor udpClient

