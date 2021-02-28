# Video 1.

 Debug Symbols is used to provide specification:
 -----------------------------------------------
 -----------------------------------------------

### Without debug symbol:

Eg: Description of the user

1. Boy

2. age

3. programming


### With debug symbols/ PDB(program database) symbols/ strings:

Eg: Description of user

1. Boy --> Soumyanil

2. age ---> 20

3. programming ---> C, python

**_✓ NOTE: More specific in presence of debug symbols._**

# Video 2.
```
1. We need to explicitly mention it at compile time.

2. Debug Symbol File types:

   - DWARF 2
   - COFF
   - XCOFF
   - Stabs

3. GCC use the -g option

4. GCC --ggdb for GDB specific symbols
```

## If :

### Condition1:
```
$ gcc main2.c -ggdb -o main2_with_debug

$ ls
main2.c  main2_no_debug*  main2_with_debug*

$ gdb ./main2_with_debug

                              x -- snip -- x

Reading symbols from ./main2_with_debug...
(gdb) list
10	}
11	
12	int SubsNum(int n1, int n2)
13	{
14		int sub = 0;
15		
16		sub = n1 - n2;
17		
18		return sub;
19	}
(gdb) <enter>
20	
21	
22	int main (int argc, char **argv)
23	{
24		int input_1 = atoi(argv[1]);
25		int input_2 = atoi(argv[2]);
26		
27		printf("\n\n Simple Add/Substract\n\n");
28	
29		printf("Sum of %d + %d = %d\n\n", input_1, input_2, AddNum(input_1, input_2));
(gdb) <enter>
30		
31		printf("Difference of %d - %d = %d\n\n", input_1, input_2, SubsNum(input_1, input_2));	
32		getchar();
33	
34		return 0;
35	}
```
### Condition2:

#### If now, we rename the main2.c to main2.c.renamed

### **_we gonna see a change_**
```
$ mv main2.c main2.c.renamed

$ ls
main2.c.renamed  main2_no_debug*  main2_with_debug*

$ gdb ./main2_with_debug

                              x -- snip -- x

Reading symbols from ./main2_with_debug...
(gdb) list
10	main2.c: No such file or directory.
(gdb) list
10	in main2.c
(gdb) 
10	in main2.c
(gdb) 
10	in main2.c
(gdb) 
10	in main2.c
(gdb) 
10	in main2.c
(gdb) 
10	in main2.c
```
### So, that means: Binary file(main2_with_debug) does not contain GDB symbols within it, as if that would have been true then in "**_Condition2_**" the result would have been the sameas of "**_Condition1_**". But <span style="color:red"> **ACTUALLY** </span>, it <span style="color:red">**isn't the Case!!**</span>

### And if we again, rewrite the name of the file to previous name(main2.c), then again would work perfectly like in  "**Condition1**"

### Different commands:

```
(gdb) list
(gdb) info functions
(gdb) info sources
(gdb) info variables (not local variables)
(gdb) info scope <tab><tab> (local variables)
(gdb) info scope function_name
```
## Ripping debug Symbols off a Binary:

- objcopy  (objectcopy--> part of binutils)
### usage: 
```
$ objcopy --only-keep-debug rip_from_binary_file ripped_binary_file
```
## Stripping debug Symbols off a Binary:

### usage:
```
$ strip --strip-debug strip_from_binary_file
```

## Eg: 

```diff
$ strip --strip-debug main2_debug
$ ls
main2.c  main2_debug*  ripped_symbols*

$ gdb ./main2_debug

            x --- snip ---- x

Reading symbols from ./main2_debug...
+ (No debugging symbols found in ./main2_debug)
(gdb)
```
### So, debugging symbols are removed.

## Now just take a look below:

### Comparison among debug symbols: image has the result of the files present in the Video2 > part1 folder(though the concept is same for part1 and part2):
![](https://github.com/reveng007/SGDE-2012/blob/main/GNU_debugger/Video2/part1/debug_symbols_comparison.png?raw=true)

### Why are we concerned about debug symbols, how is it imp. from security perspective ?

Ans: A Smart developer(from security point of view) should not ship/share binary files with debug symbols with the rest of the world.

They will use something like this to strip those unnecessary symbols: 
```
$ strip --strip-debug --strip-unneeded  main2_debug_rename
```
- This will not only reduce the size of the file but also reduce the security flaws.
- Reduce Security flaws in this sense (according to me): 
```
If we perform reverse engineering of the 4 types of files with "string" utility, 4 types of file will show 4 different number of strings.

$ strings main2_debug | wc -l
102

$ strings ripped_symbols | wc -l
84

$ strings stripped_symbols | wc -l
78

$ strings Full_stripped_symbols | wc -l
44
```
### We can see that Full_stripped_symbols produce least number of strings with which probabality of successfully reverse engineering the code will become difficult than the other 3 codes.

## Though even after stripping off debug symbols, many things are remained within the binary in real world scenario.


### NOTE: 
#### Interestingly, number of strings produced by stripped_symbols binary file have become more or less equal to the main2_no_debug binary file.
```
$ strings main2_no_debug | wc -l
80
```
# From Here, I used files present in Video2 > part2 folder:
# What if We want to covert no_debugged_mode to debugged_mode

## Adding Debug symbols to binary file:

### 2 options:

#### 1. Adding it in the Binary itself
  - objcopy --add-gnu-debuglink=debug_file  binary

```diff
$ objcopy --add-gnu-debuglink=main2_debug   convert-no_debug_to_debug_objcopy

$ gdb ./convert-no_debug_to_debug_objcopy

                      x --- snip --- x

Reading symbols from ./convert-no_debug_to_debug_objcopy...
+Reading symbols from /home/kali/Desktop/ASM/Pentester-Academy/GNU_debugger/Video2/part2/main2_debug...
(gdb) info functions 
All defined functions:

+File main2.c:
+5:	int AddNum(int, int);
+14:	int SubsNum(int, int);
+24:	int main(int, char **);

Non-debugging symbols:
0x0000000000001000  _init
0x0000000000001080  _start
0x00000000000010b0  deregister_tm_clones
0x00000000000010e0  register_tm_clones
0x0000000000001120  __do_global_dtors_aux
0x0000000000001160  frame_dummy
0x0000000000001260  __libc_csu_init
0x00000000000012c0  __libc_csu_fini
0x00000000000012c4  _fini
0x0000000000001000  _init
0x0000000000001030  puts@plt
0x0000000000001040  printf@plt
0x0000000000001050  getchar@plt
0x0000000000001060  atoi@plt
0x0000000000001070  __cxa_finalize@plt
0x0000000000001080  _start
0x00000000000010b0  deregister_tm_clones
0x00000000000010e0  register_tm_clones
0x0000000000001120  __do_global_dtors_aux
0x0000000000001160  frame_dummy
0x0000000000001260  __libc_csu_init
0x00000000000012c0  __libc_csu_fini
0x00000000000012c4  _fini
(gdb) info variables 
All defined variables:

+File main2.c:
+3:	int IamGlobalVariable;

Non-debugging symbols:
0x0000000000002000  _IO_stdin_used
0x0000000000002050  __GNU_EH_FRAME_HDR
0x00000000000021e4  __FRAME_END__
0x0000000000003de8  __frame_dummy_init_array_entry
0x0000000000003de8  __init_array_start
0x0000000000003df0  __do_global_dtors_aux_fini_array_entry
0x0000000000003df0  __init_array_end
0x0000000000003df8  _DYNAMIC
0x0000000000004000  _GLOBAL_OFFSET_TABLE_
0x0000000000004038  __data_start
0x0000000000004038  data_start
0x0000000000004040  __dso_handle
0x0000000000004048  __TMC_END__
0x0000000000004048  __bss_start
0x0000000000004048  _edata
0x0000000000004048  completed
0x0000000000004050  _end
0x0000000000002000  _IO_stdin_used
0x0000000000002050  __GNU_EH_FRAME_HDR
0x00000000000021e4  __FRAME_END__
0x0000000000003de8  __frame_dummy_init_array_entry
0x0000000000003de8  __init_array_start
0x0000000000003df0  __do_global_dtors_aux_fini_array_entry
0x0000000000003df0  __init_array_end
0x0000000000003df8  _DYNAMIC
0x0000000000004000  _GLOBAL_OFFSET_TABLE_
0x0000000000004038  __data_start
0x0000000000004038  data_start
0x0000000000004040  __dso_handle
--Type <RET> for more, q to quit, c to continue without paging--q
Quit
(gdb) quit
```

#### 2. Load the symbol file within GDB
  - symbol-file file_name

```diff
$ gdb ./convert-no_debug_to_debug_symbol-file

                                        x --- snip --- x

Reading symbols from ./convert-no_debug_to_debug...
-(No debugging symbols found in ./convert-no_debug_to_debug)
(gdb) info functions 
All defined functions:

Non-debugging symbols:
0x0000000000001000  _init
0x0000000000001030  puts@plt
0x0000000000001040  printf@plt
0x0000000000001050  getchar@plt
0x0000000000001060  atoi@plt
0x0000000000001070  __cxa_finalize@plt
0x0000000000001080  _start
0x00000000000010b0  deregister_tm_clones
0x00000000000010e0  register_tm_clones
0x0000000000001120  __do_global_dtors_aux
0x0000000000001160  frame_dummy
0x0000000000001165  AddNum
0x0000000000001186  SubsNum
0x00000000000011a5  main
0x0000000000001260  __libc_csu_init
0x00000000000012c0  __libc_csu_fini
0x00000000000012c4  _fini
(gdb) info variables 
All defined variables:

Non-debugging symbols:
0x0000000000002000  _IO_stdin_used
0x0000000000002050  __GNU_EH_FRAME_HDR
0x00000000000021e4  __FRAME_END__
0x0000000000003de8  __frame_dummy_init_array_entry
0x0000000000003de8  __init_array_start
0x0000000000003df0  __do_global_dtors_aux_fini_array_entry
0x0000000000003df0  __init_array_end
0x0000000000003df8  _DYNAMIC
0x0000000000004000  _GLOBAL_OFFSET_TABLE_
0x0000000000004038  __data_start
0x0000000000004038  data_start
0x0000000000004040  __dso_handle
0x0000000000004048  __TMC_END__
0x0000000000004048  __bss_start
0x0000000000004048  _edata
0x0000000000004048  completed
0x000000000000404c  IamGlobalVariable
0x0000000000004050  _end

+(gdb) symbol-file main2_debug -----> We have to write like this => gdb has the CAPABILITY to read the debug symbols seperately from the binary.

++ Onething, if we want to add debug symbols via any other files among 4 files mentioned above, we can, but debug symbols will be added accordingly. main2_debug has the most number of debug symbols than any other files.

Reading symbols from main2_debug...                    
(gdb) info functions 
All defined functions:

+File main2.c:            -----------------> All debug symbols appeared
+5:	int AddNum(int, int);
+14:	int SubsNum(int, int);
+24:	int main(int, char **);

Non-debugging symbols:
0x0000000000001000  _init
0x0000000000001030  puts@plt
0x0000000000001040  printf@plt
0x0000000000001050  getchar@plt
0x0000000000001060  atoi@plt
0x0000000000001070  __cxa_finalize@plt
0x0000000000001080  _start
0x00000000000010b0  deregister_tm_clones
0x00000000000010e0  register_tm_clones
0x0000000000001120  __do_global_dtors_aux
0x0000000000001160  frame_dummy
0x0000000000001260  __libc_csu_init
0x00000000000012c0  __libc_csu_fini
0x00000000000012c4  _fini
(gdb) info variables 
All defined variables:

+File main2.c:               -------------------------------> All debug symbols appeared
+3:	int IamGlobalVariable;

Non-debugging symbols:
0x0000000000002000  _IO_stdin_used
0x0000000000002050  __GNU_EH_FRAME_HDR
0x00000000000021e4  __FRAME_END__
0x0000000000003de8  __frame_dummy_init_array_entry
0x0000000000003de8  __init_array_start
0x0000000000003df0  __do_global_dtors_aux_fini_array_entry
0x0000000000003df0  __init_array_end
0x0000000000003df8  _DYNAMIC
0x0000000000004000  _GLOBAL_OFFSET_TABLE_
0x0000000000004038  __data_start
0x0000000000004038  data_start
0x0000000000004040  __dso_handle
0x0000000000004048  __TMC_END__
0x0000000000004048  __bss_start
0x0000000000004048  _edata
0x0000000000004048  completed
0x0000000000004050  _end
(gdb)quit
```
# Assignment:

## What does "info file" command perform in gdb? How is it useful?
```diff
$ gdb ./main2_debug

Reading symbols from ./main2_debug...
+(gdb) info files
Symbols from "/home/kali/Desktop/ASM/Pentester-Academy/GNU_debugger/Video2/part2/main2_debug".
Local exec file:
	`/home/kali/Desktop/ASM/Pentester-Academy/GNU_debugger/Video2/part2/main2_debug', file type elf64-x86-64.
	Entry point: 0x1080
	0x00000000000002a8 - 0x00000000000002c4 is .interp
	0x00000000000002c4 - 0x00000000000002e8 is .note.gnu.build-id
	0x00000000000002e8 - 0x0000000000000308 is .note.ABI-tag
	0x0000000000000308 - 0x000000000000032c is .gnu.hash
	0x0000000000000330 - 0x0000000000000420 is .dynsym
	0x0000000000000420 - 0x00000000000004b6 is .dynstr
	0x00000000000004b6 - 0x00000000000004ca is .gnu.version
	0x00000000000004d0 - 0x00000000000004f0 is .gnu.version_r
	0x00000000000004f0 - 0x00000000000005b0 is .rela.dyn
	0x00000000000005b0 - 0x0000000000000610 is .rela.plt
	0x0000000000001000 - 0x0000000000001017 is .init
	0x0000000000001020 - 0x0000000000001070 is .plt
	0x0000000000001070 - 0x0000000000001078 is .plt.got
	0x0000000000001080 - 0x00000000000012c1 is .text
	0x00000000000012c4 - 0x00000000000012cd is .fini
	0x0000000000002000 - 0x0000000000002050 is .rodata
	0x0000000000002050 - 0x000000000000209c is .eh_frame_hdr
	0x00000000000020a0 - 0x00000000000021e8 is .eh_frame
	0x0000000000003de8 - 0x0000000000003df0 is .init_array
	0x0000000000003df0 - 0x0000000000003df8 is .fini_array
	0x0000000000003df8 - 0x0000000000003fd8 is .dynamic
	0x0000000000003fd8 - 0x0000000000004000 is .got
	0x0000000000004000 - 0x0000000000004038 is .got.plt
	0x0000000000004038 - 0x0000000000004048 is .data
	0x0000000000004048 - 0x0000000000004050 is .bss
+(gdb) info target 
Symbols from "/home/kali/Desktop/ASM/Pentester-Academy/GNU_debugger/Video2/part2/main2_debug".
Local exec file:
	`/home/kali/Desktop/ASM/Pentester-Academy/GNU_debugger/Video2/part2/main2_debug', file type elf64-x86-64.
	Entry point: 0x1080
	0x00000000000002a8 - 0x00000000000002c4 is .interp
	0x00000000000002c4 - 0x00000000000002e8 is .note.gnu.build-id
	0x00000000000002e8 - 0x0000000000000308 is .note.ABI-tag
	0x0000000000000308 - 0x000000000000032c is .gnu.hash
	0x0000000000000330 - 0x0000000000000420 is .dynsym
	0x0000000000000420 - 0x00000000000004b6 is .dynstr
	0x00000000000004b6 - 0x00000000000004ca is .gnu.version
	0x00000000000004d0 - 0x00000000000004f0 is .gnu.version_r
	0x00000000000004f0 - 0x00000000000005b0 is .rela.dyn
	0x00000000000005b0 - 0x0000000000000610 is .rela.plt
	0x0000000000001000 - 0x0000000000001017 is .init
	0x0000000000001020 - 0x0000000000001070 is .plt
	0x0000000000001070 - 0x0000000000001078 is .plt.got
	0x0000000000001080 - 0x00000000000012c1 is .text
	0x00000000000012c4 - 0x00000000000012cd is .fini
	0x0000000000002000 - 0x0000000000002050 is .rodata
	0x0000000000002050 - 0x000000000000209c is .eh_frame_hdr
	0x00000000000020a0 - 0x00000000000021e8 is .eh_frame
	0x0000000000003de8 - 0x0000000000003df0 is .init_array
	0x0000000000003df0 - 0x0000000000003df8 is .fini_array
	0x0000000000003df8 - 0x0000000000003fd8 is .dynamic
	0x0000000000003fd8 - 0x0000000000004000 is .got
	0x0000000000004000 - 0x0000000000004038 is .got.plt
	0x0000000000004038 - 0x0000000000004048 is .data
	0x0000000000004048 - 0x0000000000004050 is .bss
(gdb)
```
### NOTE: "_info file_" command = "_info target_" command in gdb

### Ans: Print the names of the files in the debuggee(A process or application upon which a debugger acts).

### Files are:

1. the executable file
2. the core dump files that the debugger is currently using
3. the files from which the debugger loaded symbols

# Video 3. - Inspecting Symbols with NM

### NM --> Lists Symbols from Binary Files
```
$ nm ./main2_debug                                         ----------+
0000000000001165 T AddNum                                            |
                 U atoi@GLIBC_2.2.5                                  |
0000000000004048 B __bss_start                                       |
0000000000004048 b completed.0                                       | 
                 w __cxa_finalize@GLIBC_2.2.5                        | 
0000000000004038 D __data_start                                      | 
0000000000004038 W data_start                                        |
00000000000010b0 t deregister_tm_clones                              |
0000000000001120 t __do_global_dtors_aux                             |
0000000000003df0 d __do_global_dtors_aux_fini_array_entry            |
0000000000004040 D __dso_handle                                      |
0000000000003df8 d _DYNAMIC                                          |
0000000000004048 D _edata                                            |
0000000000004050 B _end                                              | 
00000000000012c4 T _fini                                             | 
0000000000001160 t frame_dummy                                       |
0000000000003de8 d __frame_dummy_init_array_entry                    |
00000000000021e4 r __FRAME_END__                                     |
                 U getchar@GLIBC_2.2.5                               | 
0000000000004000 d _GLOBAL_OFFSET_TABLE_                             |---- Symbol Name
                 w __gmon_start__                                    | 
0000000000002050 r __GNU_EH_FRAME_HDR                                |
000000000000404c B IamGlobalVariable                                 |
0000000000001000 t _init                                             | 
0000000000003df0 d __init_array_end                                  |
0000000000003de8 d __init_array_start                                |
0000000000002000 R _IO_stdin_used                                    |
                 w _ITM_deregisterTMCloneTable                       |
                 w _ITM_registerTMCloneTable                         | 
00000000000012c0 T __libc_csu_fini                                   | 
0000000000001260 T __libc_csu_init                                   |  
                 U __libc_start_main@GLIBC_2.2.5                     |
00000000000011a5 T main                                              | 
                 U printf@GLIBC_2.2.5                                |
                 U puts@GLIBC_2.2.5                                  | 
00000000000010e0 t register_tm_clones                                |
0000000000001080 T _start                                            |
0000000000001186 T SubsNum                                           | 
0000000000004048 D __TMC_END__                                  -----+
                 |________                                              
|_______________|        |
        |             Symbol Type
Virtual address of
 various symbols.
```
### Symbol Types:

| Symbol TYPE  |                   Meaning                          |
| ------------ | -------------------------------------------------- |
|      A       |                Absolute Symbol                     |
|      B       |   In the Uninitialized Data Section (BSS)          |
|      D       |        In the initialized Data Section             |
|      N       |               Debugging Symbol                     |
|      T       |              In the Text Section                   |
|      U       |        Symbol Undefined right now                  |

- Lowercase is Local Symbol
- Uppercase is External

### Usage:
```
$ nm ./main3_debug
0000000000001165 T AddNum
                 U atoi@GLIBC_2.2.5 ----> Undefined
000000000000404c B __bss_start
000000000000404c b completed.0
                 w __cxa_finalize@GLIBC_2.2.5
0000000000004038 D __data_start
0000000000004038 W data_start
00000000000010b0 t deregister_tm_clones
0000000000001120 t __do_global_dtors_aux
0000000000003df0 d __do_global_dtors_aux_fini_array_entry
0000000000004040 D __dso_handle
0000000000003df8 d _DYNAMIC
000000000000404c D _edata
0000000000004058 B _end
00000000000012c4 T _fini
0000000000001160 t frame_dummy
0000000000003de8 d __frame_dummy_init_array_entry
00000000000021e4 r __FRAME_END__
                 U getchar@GLIBC_2.2.5  --------------> Undefined
0000000000004000 d _GLOBAL_OFFSET_TABLE_
                 w __gmon_start__
0000000000002050 r __GNU_EH_FRAME_HDR
0000000000004050 B IamGlobalVariable
0000000000004048 D IamInitializedGlobalVariable
0000000000001000 t _init
0000000000003df0 d __init_array_end
0000000000003de8 d __init_array_start
0000000000002000 R _IO_stdin_used
                 w _ITM_deregisterTMCloneTable
                 w _ITM_registerTMCloneTable
00000000000012c0 T __libc_csu_fini
0000000000001260 T __libc_csu_init
                 U __libc_start_main@GLIBC_2.2.5 ---------------> Undefined
00000000000011a5 T main
                 U printf@GLIBC_2.2.5  ---> ...
                 U puts@GLIBC_2.2.5  ------> ....
00000000000010e0 t register_tm_clones
0000000000001080 T _start
0000000000001186 T SubsNum
0000000000004050 D __TMC_END__
```
### To arrange the gaps in an order (the blanks are there because some of the symbols are not resolved with any virtual address)
#### use: nm -n _[binary]_
```
$ nm -n ./main3_debug
                 U atoi@GLIBC_2.2.5
                 w __cxa_finalize@GLIBC_2.2.5
                 U getchar@GLIBC_2.2.5
                 w __gmon_start__
                 w _ITM_deregisterTMCloneTable
                 w _ITM_registerTMCloneTable
                 U __libc_start_main@GLIBC_2.2.5
                 U printf@GLIBC_2.2.5
                 U puts@GLIBC_2.2.5
0000000000001000 t _init
0000000000001080 T _start
00000000000010b0 t deregister_tm_clones
00000000000010e0 t register_tm_clones
0000000000001120 t __do_global_dtors_aux
0000000000001160 t frame_dummy
0000000000001165 T AddNum
0000000000001186 T SubsNum
00000000000011a5 T main
0000000000001260 T __libc_csu_init
00000000000012c0 T __libc_csu_fini
00000000000012c4 T _fini
0000000000002000 R _IO_stdin_used
0000000000002050 r __GNU_EH_FRAME_HDR
00000000000021e4 r __FRAME_END__
0000000000003de8 d __frame_dummy_init_array_entry
0000000000003de8 d __init_array_start
0000000000003df0 d __do_global_dtors_aux_fini_array_entry
0000000000003df0 d __init_array_end
0000000000003df8 d _DYNAMIC
0000000000004000 d _GLOBAL_OFFSET_TABLE_
0000000000004038 D __data_start
0000000000004038 W data_start
0000000000004040 D __dso_handle
0000000000004048 D IamInitializedGlobalVariable
000000000000404c B __bss_start
000000000000404c b completed.0
000000000000404c D _edata
0000000000004050 B IamGlobalVariable
0000000000004050 D __TMC_END__
0000000000004058 B _end

$ nm -nr ./main3_debug
0000000000004058 B _end
0000000000004050 D __TMC_END__
0000000000004050 B IamGlobalVariable
000000000000404c D _edata
000000000000404c b completed.0
000000000000404c B __bss_start
0000000000004048 D IamInitializedGlobalVariable
0000000000004040 D __dso_handle
0000000000004038 W data_start
0000000000004038 D __data_start
0000000000004000 d _GLOBAL_OFFSET_TABLE_
0000000000003df8 d _DYNAMIC
0000000000003df0 d __init_array_end
0000000000003df0 d __do_global_dtors_aux_fini_array_entry
0000000000003de8 d __init_array_start
0000000000003de8 d __frame_dummy_init_array_entry
00000000000021e4 r __FRAME_END__
0000000000002050 r __GNU_EH_FRAME_HDR
0000000000002000 R _IO_stdin_used
00000000000012c4 T _fini
00000000000012c0 T __libc_csu_fini
0000000000001260 T __libc_csu_init
00000000000011a5 T main
0000000000001186 T SubsNum
0000000000001165 T AddNum
0000000000001160 t frame_dummy
0000000000001120 t __do_global_dtors_aux
00000000000010e0 t register_tm_clones
00000000000010b0 t deregister_tm_clones
0000000000001080 T _start
0000000000001000 t _init
                 U puts@GLIBC_2.2.5
                 U printf@GLIBC_2.2.5
                 U __libc_start_main@GLIBC_2.2.5
                 w _ITM_registerTMCloneTable
                 w _ITM_deregisterTMCloneTable
                 w __gmon_start__
                 U getchar@GLIBC_2.2.5
                 w __cxa_finalize@GLIBC_2.2.5
                 U atoi@GLIBC_2.2.5
```
### List of all the external symbols:

#### Use: nm -g _[binary]_
```
$ nm -g ./main3_debug

0000000000001165 T AddNum
                 U atoi@GLIBC_2.2.5
000000000000404c B __bss_start
                 w __cxa_finalize@GLIBC_2.2.5
0000000000004038 D __data_start
0000000000004038 W data_start
0000000000004040 D __dso_handle
000000000000404c D _edata
0000000000004058 B _end
00000000000012c4 T _fini
                 U getchar@GLIBC_2.2.5
                 w __gmon_start__
0000000000004050 B IamGlobalVariable
0000000000004048 D IamInitializedGlobalVariable
0000000000002000 R _IO_stdin_used
                 w _ITM_deregisterTMCloneTable
                 w _ITM_registerTMCloneTable
00000000000012c0 T __libc_csu_fini
0000000000001260 T __libc_csu_init
                 U __libc_start_main@GLIBC_2.2.5
00000000000011a5 T main
                 U printf@GLIBC_2.2.5
                 U puts@GLIBC_2.2.5
0000000000001080 T _start
0000000000001186 T SubsNum
0000000000004050 D __TMC_END__
```
## Finding the size:

```
$ nm -S ./main3_debug

0000000000001165 0000000000000021 T AddNum
                 U atoi@GLIBC_2.2.5
000000000000404c B __bss_start
000000000000404c 0000000000000001 b completed.0
                 w __cxa_finalize@GLIBC_2.2.5
0000000000004038 D __data_start
0000000000004038 W data_start
00000000000010b0 t deregister_tm_clones
0000000000001120 t __do_global_dtors_aux
0000000000003df0 d __do_global_dtors_aux_fini_array_entry
0000000000004040 D __dso_handle
0000000000003df8 d _DYNAMIC
000000000000404c D _edata
0000000000004058 B _end
00000000000012c4 T _fini
0000000000001160 t frame_dummy
0000000000003de8 d __frame_dummy_init_array_entry
00000000000021e4 r __FRAME_END__
                 U getchar@GLIBC_2.2.5
0000000000004000 d _GLOBAL_OFFSET_TABLE_
                 w __gmon_start__
0000000000002050 r __GNU_EH_FRAME_HDR
0000000000004050 0000000000000004 B IamGlobalVariable
0000000000004048 0000000000000004 D IamInitializedGlobalVariable
0000000000001000 t _init
0000000000003df0 d __init_array_end
0000000000003de8 d __init_array_start
0000000000002000 0000000000000004 R _IO_stdin_used
                 w _ITM_deregisterTMCloneTable
                 w _ITM_registerTMCloneTable
00000000000012c0 0000000000000001 T __libc_csu_fini
0000000000001260 000000000000005d T __libc_csu_init
                 U __libc_start_main@GLIBC_2.2.5
00000000000011a5 00000000000000b1 T main
                 U printf@GLIBC_2.2.5
                 U puts@GLIBC_2.2.5
00000000000010e0 t register_tm_clones
0000000000001080 000000000000002b T _start
0000000000001186 000000000000001f T SubsNum
0000000000004050 D __TMC_END__
```

### Finding "**_Symbol types_**" with **grep**:

```
$ nm ./main3_debug | grep 'D'
0000000000004038 D __data_start
0000000000004040 D __dso_handle
0000000000003df8 d _DYNAMIC
000000000000404c D _edata
00000000000021e4 r __FRAME_END__
0000000000002050 r __GNU_EH_FRAME_HDR
0000000000004048 D IamInitializedGlobalVariable ----------------> As IamInitializedGlobalVariable was intialised to 20
0000000000004050 D __TMC_END__
```
# Video 4.(Systemcall Tracing with strace)

- Helper tool to understand how program interact with the OS
- Traces all System calls made by the program
- Tells us about arguments passed and has great filtering capabilities

### Use: strace ./[binary_file] [arguments]
- "-o" output_file
- "-t" for timestamp
- "-r" for raltive timestamping
- "-e" for tracing specific system calls ---> Use: strace -e [specifc system call] ./[binary] [arguments]
#### or we can also combine multiple options. 

### NOTE: "-o" ---> Unable to find how to use

```
$ strace ./main4_debug 20 30 

execve("./main4_debug", ["./main4_debug", "20", "30"], 0x7ffd3b2e6460 /* 50 vars */) = 0  ---> Invokes the program
brk(NULL)                               = 0x563a33bd9000
access("/etc/ld.so.preload", R_OK)      = -1 ENOENT (No such file or directory)
openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
fstat(3, {st_mode=S_IFREG|0644, st_size=119068, ...}) = 0
mmap(NULL, 119068, PROT_READ, MAP_PRIVATE, 3, 0) = 0x7f5d21550000
close(3)                                = 0
openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libc.so.6", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0@n\2\0\0\0\0\0"..., 832) = 832
fstat(3, {st_mode=S_IFREG|0755, st_size=1839792, ...}) = 0
mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7f5d2154e000
mmap(NULL, 1852680, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7f5d21389000
mprotect(0x7f5d213ae000, 1662976, PROT_NONE) = 0
mmap(0x7f5d213ae000, 1355776, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x25000) = 0x7f5d213ae000
mmap(0x7f5d214f9000, 303104, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x170000) = 0x7f5d214f9000
mmap(0x7f5d21544000, 24576, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1ba000) = 0x7f5d21544000
mmap(0x7f5d2154a000, 13576, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x7f5d2154a000
close(3)                                = 0
arch_prctl(ARCH_SET_FS, 0x7f5d2154f540) = 0
mprotect(0x7f5d21544000, 12288, PROT_READ) = 0
mprotect(0x563a32e18000, 4096, PROT_READ) = 0
mprotect(0x7f5d21598000, 4096, PROT_READ) = 0
munmap(0x7f5d21550000, 119068)          = 0
fstat(1, {st_mode=S_IFCHR|0620, st_rdev=makedev(0x88, 0), ...}) = 0
brk(NULL)                               = 0x563a33bd9000
brk(0x563a33bfa000)                     = 0x563a33bfa000
write(1, "\n", 1
)                       = 1
write(1, "\n", 1
)                       = 1
write(1, " Simple Add/Substract\n", 22 Simple Add/Substract  
) = 22
write(1, "\n", 1
)                       = 1
write(1, "Sum of 20 + 30 = 50\n\n", 21Sum of 20 + 30 = 50    -------------> Addition

) = 21
write(1, "Difference of 20 - 30 = -10\n\n", 29Difference of 20 - 30 = -10  ----> Substraction

) = 29
fstat(0, {st_mode=S_IFCHR|0620, st_rdev=makedev(0x88, 0), ...}) = 0
read(0, 0x563a33bd96b0, 1024)           = ? ERESTARTSYS (To be restarted if SA_RESTART is set)
--- SIGWINCH {si_signo=SIGWINCH, si_code=SI_KERNEL} ---
read(0, progressbar0x563a33bd96b0, 1024)           = ? ERESTARTSYS (To be restarted if SA_RESTART is set)
--- SIGWINCH {si_signo=SIGWINCH, si_code=SI_KERNEL} ---

read(0, | |                                 --------> Curser stays here -------> then click <enter>

"progressbar\n", 1024)          = 12
lseek(0, -11, SEEK_CUR)                 = -1 ESPIPE (Illegal seek)
exit_group(0)                           = ?
+++ exited with 0 +++

$
```
### This is because, the program is meant to be work like that way only.
```
$ ./main4_debug 20 30

Simple Add/Substract

Sum of 20 + 30 = 50

Difference of 20 - 30 = -10 | | ------> Curser stays here --> then click <enter>

             <---- Program terminates
$
```
- mmap ----> memory mapping

#### strace -e [specific systemcall] ./[binary_file] [arguments]

```
$ strace -e write ./main4_debug 20 30

write(1, "\n", 1
)                       = 1
write(1, "\n", 1
)                       = 1
write(1, " Simple Add/Substract\n", 22 Simple Add/Substract
) = 22
write(1, "\n", 1
)                       = 1
write(1, "Sum of 20 + 30 = 50\n\n", 21Sum of 20 + 30 = 50

) = 21
write(1, "Difference of 20 - 30 = -10\n\n", 29Difference of 20 - 30 = -10

) = 29

| | ------> curser stoped here ----> press <enter>

+++ exited with 0 +++

```

### NOTE: 
- "_strace_" can be used as to analyze interpretted language as well.

### Attaching to a running process:
```
$ strace -p [PID]
```

### See this:
![](https://github.com/reveng007/SGDE-2012/blob/main/GNU_debugger/Video4/strace_pid.png?raw=true)

### Showing statistical data:
### Use:
#### strace -c [executable arguments]
```
$ strace -c nc google.com 80
> GET \

HTTP/1.0 500 Internal Server Error
Content-Type: text/html; charset=UTF-8
Date: Sun, 28 Feb 2021 05:33:02 GMT
Server: gws
Content-Length: 1730
X-XSS-Protection: 0
X-Frame-Options: SAMEORIGIN

<!DOCTYPE html>
<html lang=en>
  <meta charset=utf-8>
  <meta name=viewport content="initial-scale=1, minimum-scale=1, width=device-width">
  <title>Error 500 (Server Error)!!1</title>
  <style>
    *{margin:0;padding:0}html,code{font:15px/22px arial,sans-serif}html{background:#fff;color:#222;padding:15px}body{margin:7% auto 0;max-width:390px;min-height:180px;padding:30px 0 15px}* > body{background:url(//www.google.com/images/errors/robot.png) 100% 5px no-repeat;padding-right:205px}p{margin:11px 0 22px;overflow:hidden}ins{color:#777;text-decoration:none}a img{border:0}@media screen and (max-width:772px){body{background:none;margin-top:0;max-width:none;padding-right:0}}#logo{background:url(//www.google.com/images/branding/googlelogo/1x/googlelogo_color_150x54dp.png) no-repeat;margin-left:-5px}@media only screen and (min-resolution:192dpi){#logo{background:url(//www.google.com/images/branding/googlelogo/2x/googlelogo_color_150x54dp.png) no-repeat 0% 0%/100% 100%;-moz-border-image:url(//www.google.com/images/branding/googlelogo/2x/googlelogo_color_150x54dp.png) 0}}@media only screen and (-webkit-min-device-pixel-ratio:2){#logo{background:url(//www.google.com/images/branding/googlelogo/2x/googlelogo_color_150x54dp.png) no-repeat;-webkit-background-size:100% 100%}}#logo{display:inline-block;height:54px;width:150px}
  </style>
  <a href=//www.google.com/><span id=logo aria-label=Google></span></a>
  <p><b>500.</b> <ins>That’s an error.</ins>
  <p>The server encountered an error and could not complete your request.<p>If the problem persists, please <A HREF="http://www.google.com/support/">report</A> your problem and mention this error message and the query that caused it.  <ins>That’s all we know.</ins>
% time     seconds  usecs/call     calls    errors syscall
------ ----------- ----------- --------- --------- ----------------
 32.90    0.000351         117         3           select
 32.80    0.000350         175         2           write
 15.28    0.000163           9        18           read
 12.09    0.000129           5        23         1 close
  6.94    0.000074          37         2           alarm
  0.00    0.000000           0        34        28 stat
  0.00    0.000000           0        16           fstat
  0.00    0.000000           0         2           poll
  0.00    0.000000           0         4           lseek
  0.00    0.000000           0        29           mmap
  0.00    0.000000           0         9           mprotect
  0.00    0.000000           0         5           munmap
  0.00    0.000000           0         3           brk
  0.00    0.000000           0         7           rt_sigaction
  0.00    0.000000           0         1           rt_sigprocmask
  0.00    0.000000           0         1           ioctl
  0.00    0.000000           0         1         1 access
  0.00    0.000000           0         2           getpid
  0.00    0.000000           0         6           socket
  0.00    0.000000           0         6         4 connect
  0.00    0.000000           0         1           sendto
  0.00    0.000000           0         1           recvfrom
  0.00    0.000000           0         3           setsockopt
  0.00    0.000000           0         1           execve
  0.00    0.000000           0         1           uname
  0.00    0.000000           0         1           arch_prctl
  0.00    0.000000           0        48        32 openat
------ ----------- ----------- --------- --------- ----------------
100.00    0.001067           4       230        66 total
```
## So, Why all this is important ?

- It can be useful to set break points to many of systemcalls while running gdb, i.e., changing/manipoulating things at run time.


# Video 5.(Breakpoints, Examining registers and Memory)

## What is Breakpoint ?

- Technique used to "Pause" the program during execution based on certain criteria
- Criteria can be "about to execute an instruction" (which we want to examine)
- Debugger allows you to inspect/ modify CPU Registers, Memory, Data, etc.

```
$ cat main5.c 

#include<stdio.h>
#include<string.h>

void EchoInput(char *userInput)
{
	char buffer[20];

	strcpy(buffer, userInput);

	printf("\n\n%s\n\n", buffer);
}

int main(int argc, char **argv)
{
	EchoInput(argv[1]);

	return 0;
}
```
```
$ gcc main5.c -ggdb -o main5_debug
$ ./main5_debug "Reveng Hiiiiiiiiiiiiiiiiiiiiiii"

Reveng Hiiiiiiiiiiiiiiiiiiiiiii                              ----------> within 20 char buffer size

$ ./main5_debug "Reveng Hiiiiiiiiiiiiiiiiiiiiiiii"

Reveng Hiiiiiiiiiiiiiiiiiiiiiiii      -------> excided 20 char buffer size, segmentation fault ==> buffer overflow

sh: “./main5_debug "Reveng Hiiiiiiii…” terminated by signal SIGSEGV (Address boundary error)

```

### Opening binary in gdb
```
$ gdb ./main5_debug

                               x -- snip -- x

Type "apropos word" to search for commands related to "word"...
Reading symbols from ./main5_debug...
(gdb) run Reveng
Starting program: /home/kali/Desktop/ASM/Pentester-Academy/GNU_debugger/Video5/main5_debug Reveng


Reveng

[Inferior 1 (process 467424) exited normally]
```
```
(gdb) list 1
1	#include<stdio.h>
2	#include<string.h>
3	
4	void EchoInput(char *userInput)
5	{
6	  char buffer[20];
7	
8		strcpy(buffer, userInput);
9	
10  printf("\n\n%s\n\n", buffer);
(gdb) 
11	}
12	
13	int main(int argc, char **argv)
14	{
15		EchoInput(argv[1]);
16	
17		return 0;
18	}
19	
20
```
### So, whats the difference between running binary in gdb and in terminal

### Actually, till now we haven't done that thing. Now, lets come into breakpoint

```diff
 (gdb) break main
 Breakpoint 1 at 0x118e: file main5.c, line 15.
 (gdb)  list 15
 10		 printf("\n\n%s\n\n", buffer);
 11	 }
 12	
-13	int main(int argc, char **argv)      ---------------> We mentioned breakpoint at main but breakpoint was applied at line 15..why ?? It was supposed to be line 13
 14	 {
 15	 	 EchoInput(argv[1]);
 16	
 17		 return 0;
 18	 }
 19	
 (gdb) 
```
