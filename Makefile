erp_udp.o workers.o semaph.o buffer_list.o: constants.h

erp: erp_udp.o workers.o semaph.o buffer_list.o
	gcc -Wall -o output erp_udp.o workers.o semaph.o buffer_list.o -lpthread -I. -g

clean:
	rm output *.o
	
