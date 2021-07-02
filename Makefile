default: build

build: clean
	gcc -Wall -o curl -l curl main.c util.c cJSON.c

clean:
	rm -rf curl

test: build 
	./curl https://freegeoip.app/json/
