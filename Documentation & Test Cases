# Grace Biemans (geb965) & Nick Heleta (nwh397)


    Initialize memory allocator (init.c)
- freelisthead will hold the space we get back from mmap
    - We put the space into a freechunk and initialize the header and footer values
- lastChecked is a pointer that will keep track for next fit policy during allocation later
    
    Error Handling:
- We can only call this routine once, so error if it is called and freelisthead is already initialized
    



    Allocate memory (alloc.c)
- If the freelist only has one chunk, we allocate from that
    - *There is a special case if the user wants all of that chunk because that is the only thing in freelist (see limitations before)
    - If the user doesn't want the whole chunk, we split it and return a pointer to it
- If there are multiple chunks in freelist, we use next fit policy to find a chunk in freelist to use (lastChecked is the pointer we use to keep track of the last chunk we checked)
    - Loop through until lastChecked points to a chunk that is big enough for the user
    - We have three cases for whether the chunk in freelist is the first, middle, or last node in freelist
        - For each case, we return the whole chunk if it is perfect size or split and return a newly made chunk

    Error Handling:
- If there is not enough space in the freelist for the user's request

    Limitations:
- *Didn't get to implementing the special case where the user takes the last chunk from the freelist, leaving freelist empty
    - I have an if statement that catches the case and returns NULL to user
- Does not work if we initialize a space bigger than 1360 bytes - I don't know why, it might just be on my computer




    Free memory (free.c)
- Take the allocated chunk and convert it to a free chunk so that it has next and previous pointers in the header and footer rather than magic number
- Create a start pointer and set it to the head of freelist so we can stop looping through the chunks when we get back to start
    - Looping is to check if we find a chunk in the freelist that is before or after the chunk we are freeing - for coalescing
    - If we find one, we loop through freelist again to see if there is another chunk on the other side - the case where we coalesce 3 chunks into 1
        - If we come across the 3 in 1 coalesce case, we remove one of the free chunks from freelist 
            - Have 3 cases for this - removing first, middle, or last node of freelist
        - If we only have a 2 in 1 coalesce case, we don't remove anything from freelist, just make the one chunk bigger
    - If there is no coalescing to do, we just add the chunk to the freelist as a new node

    Error Handling:
- If the chunk we are given is not valid (magic number is not right)

    Limitations:
- We did not implement the special case in alloc.c where we delete the last chunk in freelist, so there is also no code in free.c to deal with the return of a chunk to the empty freelist
- some of the coalescing cases work on some examples and not on others, didn't have time to fix all of them, I think it is because I forgot to consider header and footer size somewhere



    Display freelist (display.c)
- Initialize a start pointer at the head of freelist and loop though all of the nodes in freelist, printing the address and size for each
- There is a special case for if there is only one chunk in freelist, so we just print that one and don't loop

    Error Handling:
- if the freelist is empty, print a message to the console informing the user (more of a special case than an error)




    Test Cases:

1: Initialize with 360 as the size argument (will have to round up to mult of 16 and then initialize)

Nodes in the freelist:
  Node 1:
    Address: 0x10f68f000
    Size: 336


2: Initialize with 1360 as the size argument

Nodes in the freelist:
  Node 1:
    Address: 0x105068000
    Size: 1328


3: Continuing off of test 2, allocate a chunk with 400 as the size argument in M_Alloc

Nodes in the freelist:
  Node 1:
    Address: 0x106dc3000
    Size: 896


4: Continuing off of test 3, allocate a chunk with 658 as the size argument in M_Alloc (will have to round up to mult of 16 first)

Nodes in the freelist:
  Node 1:
    Address: 0x106f7c000
    Size: 192


5: Continuing off of test 4, free the chunk allocated in test 3 (there will not be any coalescing necessary)

Nodes in the freelist:
  Node 1:
    Address: 0x10d5abb20
    Size: 400
  Node 2:
    Address: 0x10d5ab000
    Size: 192


6: Continuing off of test 5, free the chunk allocated in test 4 (3 chunks will be coalesced into 1)

Nodes in the freelist:
  Node 1:
    Address: 0x101656000
    Size: 1328


7: Continuing off of test 4, free the chunk allocated in test 4 (Will be coalescing with a freechunk in the address space before it)

Nodes in the freelist:
  Node 1:
    Address: 0x1043d7000
    Size: 896


8: Freeing node that is above node 1 in address space (will be coalescing with a freechunk in the address space after it)

Nodes in the freelist:
  Node 1:
    Address: 0x10a5d8b20
    Size: 400
  Node 2:
    Address: 0x10a5d8000
    Size: 176

Nodes in the freelist:
  Node 1:
    Address: 0x104ba1500
    Size: 896
  Node 2:
    Address: 0x104ba1000
    Size: 176


9: Free all of the allocated chunks (4) in random order - should coalesce all into one
- includes combining with above and combining with above and below at once

Nodes in the freelist:
  Node 1:
    Address: 0x10a4f7000
    Size: 256

Nodes in the freelist:
  Node 1:
    Address: 0x10a4f7000
    Size: 336

Nodes in the freelist:
  Node 1:
    Address: 0x10a4f7b20
    Size: 400
  Node 2:
    Address: 0x10a4f7000
    Size: 336

Nodes in the freelist:
  Node 1:
    Address: 0x10a4f7b20
    Size: 400
  Node 2:
    Address: 0x10a4f7000
    Size: 400

Nodes in the freelist:
  Node 1:
    Address: 0x10a4f7000
    Size: 1328


10: Next fit policy test. lastChecked node is the one of size 400, the user is requesting one of size 401. The lastChecked node will not have enough space, so will have to go to the node of size 688

Nodes in the freelist:
  Node 1:
    Address: 0x10cf2da00
    Size: 400
  Node 2:
    Address: 0x10cf2d000
    Size: 688

Nodes in the freelist:
  Node 1:
    Address: 0x10cf2da00
    Size: 400
  Node 2:
    Address: 0x10cf2d000
    Size: 240


11: Test next fit policy in different way. The first printed freelist has 3 chunks and the user is going to ask for a chunk of 113 bytes. Only two of the chunks have enough space to share
- the 112 printed between shows where lastChecked is, so we started looking from the first node and ended up allocating from the second node.

Nodes in the freelist:
  Node 1:
    Address: 0x10e7b2d20
    Size: 112
  Node 2:
    Address: 0x10e7b2b20
    Size: 304
  Node 3:
    Address: 0x10e7b2000
    Size: 496

112
Nodes in the freelist:
  Node 1:
    Address: 0x10e7b2d20
    Size: 112
  Node 2:
    Address: 0x10e7b2b20
    Size: 144
  Node 3:
    Address: 0x10e7b2000
    Size: 496


12: This next fit policy test starts at the same points as test 11, except lastChecked points to node 3, so we will end up allocating from node 3

Nodes in the freelist:
  Node 1:
    Address: 0x102c6dd20
    Size: 112
  Node 2:
    Address: 0x102c6db20
    Size: 304
  Node 3:
    Address: 0x102c6d000
    Size: 496

496
Nodes in the freelist:
  Node 1:
    Address: 0x102c6dd20
    Size: 112
  Node 2:
    Address: 0x102c6db20
    Size: 304
  Node 3:
    Address: 0x102c6d000
    Size: 336
