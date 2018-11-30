all: run_docker

run: rest_server
	LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib ./rest_server

rest_server: rest_server.c
	gcc -w -o rest_server rest_server.c -lulfius -ljansson

copy_needed_libs:
	mkdir -p libs
	cp /usr/local/lib/libjansson.so.4 ./libs/
	cp /usr/local/lib/libulfius.so.2.3 ./libs/
	cp /usr/lib/libyder.so.1.3.3 ./libs/
	cp /usr/lib/liborcania.so.1.2.3 ./libs/

run_docker: docker_server_make docker_server_run

docker_server_make: rest_server copy_needed_libs
	# Copy systems libs into a local dir so that we can include them
	# in the Docker container
	sudo docker build -t ttt-server .

# Running this way will disconnect docker from your terminal.
# You cannot use getchar() if you run this way
docker_server_run:
	sudo docker run -i -t -p 8080:8080 ttt-server:latest

clean:
	sudo docker images -a | grep "ttt-server" | awk '{print $$3}' | xargs sudo docker rmi --force
	rm -rf ./rest_server ./libs
