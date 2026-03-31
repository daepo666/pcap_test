#Makefile
all: pcap-test
	
pcap-test: main.o translate.o
        gcc -o pcap-test main.o translate.o -lpcap

main.o: my_struct.h main.c
        gcc -c -o main.o main.c

translate.o: my_struct.h translate.c
        gcc -c -o translate.o translate.c

clean:
	rm -f pcap-test
        rm -f *.o

