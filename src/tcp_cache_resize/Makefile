#these two programs are used to test how to resize the tcp receive/send cache.
server.out:recv_cache_resize.c
	gcc -o server.out recv_cache_resize.c

client.out:send_cache_resize.c
	gcc -o client.out send_cache_resize.c


clear:server.out client.out
	rm -f server.out client.out
