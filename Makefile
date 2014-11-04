all: workers.o semaph.o buffer_list.o erp

erp: erp_udp.c semaph.c workers.c buffer_list.c
	gcc -o erp_udp erp_udp.c workers.o semaph.o buffer_list.o -lpthread -I. -g

semaph.o: semaph.c
	gcc -c semaph.c -o semaph.o
	
workers.o: workers.c
	gcc  -c workers.c -o workers.o
	
buffer_list.o: buffer_list.c
	gcc  -c buffer_list.c -o buffer_list.o

clean:
	rm erp_udp *.o
	
