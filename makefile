output: MyTaskScheduler.o
	gcc MyTaskScheduler.o -o output

MyTaskScheduler.o: MyTaskScheduler.c
	gcc -c MyTaskScheduler.c

clean:
	rm *.o output