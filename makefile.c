CFLAGS = -Wall -g -ansi -O2
OBJS1 = main_noise.o denoise.o timing.o


programs = denoise

all : $(programs)

denoise: $(OBJS1)
	$(CC) -o $@ $?


clean:
	rm -rf $(OBJS1) $(programs) *~
