gcc stat.c -o stat -DDEBUG -pthread
gcc deq.c -o deq -DDEBUG -pthread
gcc enq.c -o enq -DDEBUG -pthread
gcc sample.c -o sample -DDEBUG -pthread
gcc scheduler.c -o scheduler -DWavator -pthread
