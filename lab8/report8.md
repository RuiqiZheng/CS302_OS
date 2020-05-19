## 1 
#### a
1. Programmed
2. DMA
#### b
1. Head identifier
2. Track identifier
3. Sector identifier
#### c
1. Seek time
2. Rotational latency 
3.Transfer Time
#### d
1. Response time
2. Throughput

## 2
- work steps
preparation, waiting, working, finish
- process of each step
1. preparation: The CPU set the address register, the data counter register in the DMA controller,
and set the interrupt bit and busy bit in the DMA controller when a device requires writing data.
2. waiting: device waits CPU to distribute CPU resources.
3. working: device consumes CPU resources to store the data
4. finish: DMA controller will send a interrupt signal after input finish

## 3
When sectors near the head is visiting continuously.

## 4
#### 4.1
- track sequence:
100,120,199,0,20,30,60,70,90
- time:
120-100+199-120+199-0+20-0+30-20+60-30+70-60+90-70=388
#### 4.2
FIFO, it writes and reads data as soon as possible.
