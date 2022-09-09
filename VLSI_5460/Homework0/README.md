# Calvin Passmore - A02107892 - Homework 0

*I didn't get rotation working.*
Other than that everything should be working fine.
I worked on rotations for about 4-5 hours alone but it wouldn't stop segfaulting for me.
Due to this, it's not self balancing.
I have commented out tree.cpp:465 which turns off balancing.
If you want to see how far I got with balancing, uncomment that line and rebuild with 

    make

The executable name is 
    
    homework0

Execute as normal

    ./homework0

-----------------
-----------------
When running you'll see the command prompt

    tree $

When you run the program you'll see the following menu only once

    Choose a number from the following options, followed by any arguments:
        (1) Create_Tree
        (2) Insertion
        (3) Deletion
        (4) Search
        (5) Traversal
        (6) Delete_Tree
        (7) Check_Balance
        (q) Quit

To choose an option, only input the number. Then follow it up by any necessary arguments

    tree $1 input.txt

Here an outline of acceptable arguments for each option

## Options

### Create Tree

Acceptable arguments: input file names

Example

    tree $1 input.txt

### Insertion

Acceptable arguments: a single number

Example

    tree $2 14

### Search

Acceptable arguments: a single number

Example

    tree $3 14

### Deletion

Acceptable arguments: a single number

Example

    tree $4 14

### Traversal

Acceptable arguments: the words: 'pre' 'post' 'in'

    tree $5 post

### Delete Tree

Acceptable arguments: none

Example

    tree $6

### Check Balance

Acceptable arguments: none

Example

    tree $7

### Quit

Acceptable arguments: none

Example

    tree $q

**NOTE This is how the output.txt file is saved. If you quit the program prematurely with 'CTL + C' for example, the output file might not be saved because it is buffered output.**