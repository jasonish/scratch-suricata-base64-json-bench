LIBSURICATA_CONFIG :=	/opt/suricata/bin/libsuricata-config
CFLAGS +=		-DSURICATA_PLUGIN \
			$(shell $(LIBSURICATA_CONFIG) --cflags)
LIBS :=			$(shell $(LIBSURICATA_CONFIG) --libs)

# Yes, the double LIBS is needed as libsuricata-config outputs a few
# in the wrong order.
all:
	gcc -o bench -Wall -std=c11 $(CFLAGS) bench.c $(LIBS) $(LIBS)

clean:
	rm -f bench
	find . -name \*~ -delete
