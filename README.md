# OPIchat coding style

The following project is organised in a certain way (and must stay like this!!) in order to keep it reliable, secure and maintainable.

Thus, there is a serie of non-exhaustive mandatory rules that must be followed in order to make the project work.

## Coding style

- The project **must follow the EPITA
coding style** (obviously).
- Macro's must be *UPPERCASE*.
- Functions, enumerations and variables must be *snake_case*.

## Architecture

The project is divided in the following parts:

- `src/`
  - `client/`: the client part.
  - `server/`: the server.
  - `common/`: functions and procedures that are used by both the client and the server.
  - `client_main.c`: the main file of the client.
  - `server_main.c`: the main file of the server.
- `include/`: the header files.
- `tests/`
  - `test_main.c` : the manual main test file.
  - `unit_testing/` : the unit testing folder.

## Header files

A header file must contains for each feature:

- Their structures.
- Their functions
- Their macros.
- Their global variables.
- Their constants.
- Their enumerations.

Concretely, all the informations that must be shared between project's folders must be put in the header files.

Also, each header **MUST** have a standard include guard :

```c
#ifndef __MY_HEADER_FILE_H__
#define __MY_HEADER_FILE_H__
...
#endif
```

NB : `#pragma once` is not standard, then forbiden.

Also, if the name of the header file is `my_header_file.h`, the corresponding include guard must be `MY_HEADER_FILE_H`.

## Isolation and "foldering"

All functions must be isolated in their own logical files.

For example, `src/common/message_free.c` contains all the functions that are used to free a message.

Also, if you create a feature and you want to separate it in different files, you must create a folder for it. For example, if you create a `src/common/message_free.c` and `src/common/message_parser.c` files, you must create a `src/common/message/` folder and put the two files in it.

## Documentation

All functions, structures, structure field, macros, global variables. are documented in their corresponding header file.

The documentation must be written with the [Doxygen](https://www.doxygen.org/) syntax.

## Unit testing

All written functions **MUST HAVE a unit test** which use the *Criterion* framework. The unit test must be in the `tests/unit_testing/` folder. The unit test file must be named like `<something>_test.c`.

## Memory allocations

All functions that allocate memory **MUST BE** freed (again this is obviously obvious).

Also, heap memory allocation **MUST NOT** be done with malloc(), calloc() or realloc(), but with the functions defined in the `src/common/utils/xalloc.c` file.

We won't preload our xmalloc() dude thus there is a tollerance for functions under our control (like syscalls or other libc stuff).

## Recommended practices

If you create a set of methods and structure you don't want another programmer (or hAcKeR user) to manipulate data as their wish, you can use the following practices:

- All *INTERNAL* the methods of a structure **MUST** be static.
- All hidden structures **MUST NOT** be in the header file. Instead, they must be in the .c source file.
- You can create a mempool for your structures and just allow external users to communicate with it through the functions defined in the same file.
