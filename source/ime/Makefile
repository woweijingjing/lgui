TARGET=libime.so
OBJS=pinyin.o imewin.o
CFLAGS=-fPIC -g
LDFLAGS=-lc -lm
%.o:%.c
	$(CC) -c $(CFLAGS) $< -o $@
$(TARGET): $(OBJS)
	$(CC) -g -shared -Wl -o $@ $^ $(LDFLAGS)
clean:
	rm -rf *.o
	rm -rf libime.so
