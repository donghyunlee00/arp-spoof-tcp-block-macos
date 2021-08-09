LDLIBS += -lpcap
CXXFLAGS += -std=c++11

all: arp-spoof-tcp-block

arp-spoof-tcp-block: main.o ip.o dev.o # mac.o
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@

clean:
	rm -f arp-spoof-tcp-block *.o
