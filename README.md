<!--test machine: atlas.cselabs.umn.edu (ssh)
*   group number: G19
*   name: Timothy Anderson, Luc Rubin, Hank Hanson
*   x500: and09985, rubin206, hans7186 -->

# Project #3 – Multithreading
## Group Number 19

## Test Machine: 
`atlas.cselabs.umn.edu (ssh)`
## Names and x500 addresses
```
Timothy Anderson - and09985
Luc Rubin - rubin206
Hank Hanson - hans7186 
```
## Complete the extra task
If you pass `-b #queue_Size` it will bound the buffer.

## Individual contributions
We split up the work by the 3 source files. Luc Rubin created main.c, Tim Anderson created consumer.c and implemented the linked list in utils.c, and Hank Hanson created the producer.c.
After our individual work was done, we collaborated to find bugs, cleanup the code, and made sure everything ran as expected.

## Assumptions
- A packet is no longer considered in the bounds of the queue once the consumer has it and removed it from the queue. The packet will still be in memory until the consumer is done with it.
  This means when using a bound the actual amount of packets that could be in memory is at most `#queue_Size + #consumer`.  This should be considered in limited memory situations.
- In most cases, semaphores where used instead of mutex locks and condition variables. The exception being mutex locks were used for when a consumer makes changes to an account in the balance array.

## Compile
	> make clean
	> make

## Execution
    > make run1
    > make run2 
    ...
    > make run5

### For testing the results to the expected output (requires the expected output files)
```
> make t1
> make t2
...
> make t5
```
or
```
> make test
```
