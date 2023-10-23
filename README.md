## UID: 105396892

## Pipe Up

C code implementation for pipe operator

## Building

To build binary:
>make


## Running
Example:
```
    > ./pipe ls
    Makefile  pipe  pipe.c  pipe.o  README.md  test_lab1.py

    > ./pipe ls cat
    Makefile
    pipe
    pipe.c
    pipe.o
    README.md
    test_lab1.py

# Output of command above is equivalent to the following:
    > ls | cat
    Makefile
    pipe
    pipe.c
    pipe.o
    README.md
    test_lab1.py

    > ./pipe ls cat wc
      6       6      51
# Output of command above is equivalent to the following:
    > ls | cat | wc
      6       6      51
```



## Cleaning up

>make clean
