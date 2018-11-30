all: run

run: Client
	java -cp bin:./libs/* edu.northeastern.cs5007.Client

Client: src/edu/northeastern/cs5007/Client.java
	javac -d bin -sourcepath src -cp ./libs/*:. src/edu/northeastern/cs5007/Client.java

clean:
	rm -rf bin/*
