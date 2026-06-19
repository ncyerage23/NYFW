# MAKEFILE FOR NYFW!
# 	Not super great, but it does seem to work, so that's good. I'll 
# 	get better at these dumb things later. 

CC=gcc
CFLAGS=-Iinclude

BDIR=build
LDIR=lib
OBJ=build/canvas.o build/window.o build/input.o build/font.o build/nymg.o


$(LDIR)/libnyfw.a: $(OBJ) | $(LDIR)
	ar rcs $@ $(OBJ)

$(BDIR):
	mkdir -p $(BDIR)

$(LDIR):
	mkdir -p $(LDIR)

$(BDIR)/canvas.o: src/canvas.c | $(BDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BDIR)/window.o: src/window.c | $(BDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BDIR)/input.o: src/input.c | $(BDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BDIR)/font.o: src/font.c | $(BDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BDIR)/nymg.o: src/nymg.c | $(BDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f build/*.o
	rm -f lib/libnyfw.a
