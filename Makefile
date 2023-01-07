CFLAGS=-std=c11 -Wall -Wextra -Wsign-conversion -D _DEFAULT_SOURCE

SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

aht25: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJS): aht25.h

clean:
	$(RM) aht25 $(OBJS)

.PHONY: clean
