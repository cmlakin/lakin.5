#Makefile/lakin/4760/Project5

GCC= gcc
CFLAGS= -g -Wall

all: oss user_proc

clean:
	rm *.o oss user_proc

oss: oss.o queue.o osclock.o shm.o resource.o deadlock.o logger.o
	$(GCC) $(CFLAGS) oss.o queue.o osclock.o shm.o resource.o deadlock.o logger.o -o oss

oss.o: oss.c oss.h config.h osclock.h
	$(GCC) $(CFLAGS) -c oss.c

user_proc: user_proc.o deadlock.o shm.o logger.o osclock.o queue.o resource.o
	$(GCC)  $(CFLAGS) user_proc.o deadlock.o shm.o logger.o osclock.o queue.o resource.o -o user_proc

user_proc.o: user_proc.c oss.h
	$(GCC) $(CFLAGS) -c user_proc.c

osclock.o: osclock.c osclock.h shm.h
	$(GCC)  $(CFLAGS) -c osclock.c

shm.o: shm.c shm.h
	$(GCC)  $(CFLAGS) -c shm.c

queue.o: queue.c queue.h
	$(GCC) $(CFLAGS) -c queue.c

resource.o: resource.c resource.h shm.h
	$(GCC) $(CFLAGS) -c resource.c

deadlock.o: deadlock.c shm.h resource.h queue.h
	$(GCC) $(CFLAGS) -c deadlock.c

logger.o: logger.c
	$(GCC) $(CFLAGS) -c logger.c
