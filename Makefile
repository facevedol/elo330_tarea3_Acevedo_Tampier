default:
	gcc -Wall erp_udp.c buffer_list.c workers.c semaph.c -I. -lpthread