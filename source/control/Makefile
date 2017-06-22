TARGET=libcontrol.so
OBJS=button.o deskicon.o listbox.o idlecontrol.o \
	picturebox.o scrollbar.o static.o sledit.o mledit.o startmenu.o tabcontrol.o \
	taskbar.o 
CFLAGS=-fPIC -g
LDFLAGS=-lc -lm
%.o:%.c
	$(CC) -c $(CFLAGS) $< -o $@
$(TARGET): $(OBJS)
	$(CC) -g -shared -Wl -o $@ $^ $(LDFLAGS)
clean:
	rm -rf *.o
	rm -rf libcontrol.so
