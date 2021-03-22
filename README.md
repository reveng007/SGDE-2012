### Viewers can follow this link: [SGDE 2012](https://www.youtube.com/playlist?list=PLfpQOoR6-z-rQpwDygoZuHSNCCZBHomlF)

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
# Video 4 - Systemcall Tracing with strace

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


# Video 5 - Breakpoints, Examining registers and Memory

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

#### Actually, till now we haven't done that thing. Now, lets come into breakpoint, We will know why..?

### Adding breakpoints:

1. break function_name
2. break address
3. break line_number
4. ...

#### 1. Applying breakpoint using function_name:
```diff
+(gdb) break main --------------->
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
 ```
 
 #### 2. Applying breakpoint using line_number:

 ```
 (gdb) info breakpoints 
 Num     Type           Disp Enb Address            What
 4       breakpoint     keep y   0x0000555555555151 in EchoInput at main5.c:8
+(gdb) break 13
 Breakpoint 7 at 0x55555555518e: file main5.c, line 15.
 (gdb) list 15
 10		printf("\n\n%s\n\n", buffer);
 11	}
 12	
-13	int main(int argc, char **argv)       ---------------> We mentioned breakpoint at main but breakpoint was applied at line 15..why ?? It was supposed to be line 13
 14	{
 15		EchoInput(argv[1]);
 16	
 17		return 0;
 18	}
 19	
```
#### 3. Applying breakpoint using address:
```
            |-------------------------------- add * in front of address
            V
(gdb) break *0x00005555555551a1
Breakpoint 8 at 0x5555555551a1: file main5.c, line 17.
(gdb) info breakpoints 
Num     Type           Disp Enb Address            What
4       breakpoint     keep y   0x0000555555555151 in EchoInput at main5.c:8
7       breakpoint     keep y   0x000055555555518e in main at main5.c:15
8       breakpoint     keep y   0x00005555555551a1 in main at main5.c:17
```
see this from here: https://youtu.be/qv8M10fsXFg?t=1421

### Checking register Statistics:
```diff
+(gdb) run Reveng         --------------> running the binary
Starting program: /home/kali/Desktop/ASM/Pentester-Academy/GNU_debugger/Video5/main5_debug Reveng

                         |------------------------------------------------>   1. binary file  2. argument => Reveng
Breakpoint 1, main (argc=2, argv=0x7fffffffdf08) at main5.c:15
15		EchoInput(argv[1]);
+(gdb) info registers 
rax            0x55555555517f      93824992235903
rbx            0x0                 0
rcx            0x7ffff7fa5718      140737353766680
rdx            0x7fffffffdf20      140737488346912
rsi            0x7fffffffdf08      140737488346888
rdi            0x2                 2
rbp            0x7fffffffde10      0x7fffffffde10
rsp            0x7fffffffde00      0x7fffffffde00
r8             0x0                 0
r9             0x7ffff7fe2180      140737354015104
r10            0x3                 3
r11            0x2                 2
r12            0x555555555060      93824992235616
r13            0x0                 0
r14            0x0                 0
r15            0x0                 0
rip            0x55555555518e      0x55555555518e <main+15>  -------> pointing to the instruction which would be executed next
eflags         0x206               [ PF IF ]
cs             0x33                51
ss             0x2b                43
ds             0x0                 0
es             0x0                 0
fs             0x0                 0
gs             0x0                 0
```
### To confirm this case: why "argc=2" is outputed in the terminal:-
```
(gdb) print argv[1]
$1 = 0x7fffffffe273 "Reveng"
(gdb) print argv[0]
$2 = 0x7fffffffe22a "/home/kali/Desktop/ASM/Pentester-Academy/GNU_debugger/Video5/main5_debug"

```
```
(gdb) break EchoInput 
Breakpoint 3 at 0x555555555151: file main5.c, line 8.

(gdb) info breakpoints 
Num     Type           Disp Enb Address            What
1       breakpoint     keep y   0x000055555555518e in main at main5.c:15
	breakpoint already hit 1 time
3       breakpoint     keep y   0x0000555555555151 in EchoInput at main5.c:8
```
### To delete breakpoints:
```diff
+(gdb) delete 3

(gdb) info breakpoints 
Num     Type           Disp Enb Address            What
1       breakpoint     keep y   0x000055555555518e in main at main5.c:15
	breakpoint already hit 1 time

(gdb) break EchoInput 
Breakpoint 4 at 0x555555555151: file main5.c, line 8.

(gdb) info breakpoints 
Num     Type           Disp Enb Address            What
1       breakpoint     keep y   0x000055555555518e in main at main5.c:15
	breakpoint already hit 1 time
4       breakpoint     keep y   0x0000555555555151 in EchoInput at main5.c:8
```
### To disable breakpoint:
```diff
+(gdb) disable 4
(gdb) info breakpoints 
Num     Type           Disp Enb Address            What
1       breakpoint     keep y   0x000055555555518e in main at main5.c:15
	breakpoint already hit 1 time
4       breakpoint     keep n   0x0000555555555151 in EchoInput at main5.c:8
                            ^
                            |--------------------------------------------------- Enable mode set to: n
```
### To enable breakpoint:
```diff
+(gdb) enable 4
(gdb) info breakpoints 
Num     Type           Disp Enb Address            What
1       breakpoint     keep y   0x000055555555518e in main at main5.c:15
	breakpoint already hit 1 time
4       breakpoint     keep y   0x0000555555555151 in EchoInput at main5.c:8
                            ^
                            |--------------------------------------------------- Enable mode set to: y
```
### To examine the Memory using "**_examine_**" command:
```diff
+(gdb) help x                   -------------> x = examine
+Examine memory: x/FMT ADDRESS.   -------------------------------> usage format
ADDRESS is an expression for the memory address to examine.
FMT is a repeat count followed by a format letter and a size letter.
Format letters are o(octal), x(hex), d(decimal), u(unsigned decimal),
  t(binary), f(float), a(address), i(instruction), c(char), s(string)
  and z(hex, zero padded on the left).
Size letters are b(byte), h(halfword), w(word), g(giant, 8 bytes).
The specified number of objects of the specified size are printed
according to the format.  If a negative number is specified, memory is
examined backward from the address.

Defaults for format and size letters are those previously used.
Default count is 1.  Default address is following last thing printed
with this command or "print".
+(gdb) x/s argv[1]             ---->   (gdb) print argv[1] would also give the same output
0x7fffffffe273:	"Reveng"
+(gdb) x/o argv[1]
0x7fffffffe273:	0122
+(gdb) x/x argv[1] 
0x7fffffffe273:	0x52
+(gdb) x/d argv[1]
0x7fffffffe273:	82
+(gdb) x/u argv[1]
0x7fffffffe273:	82
+(gdb) x/t argv[1]
0x7fffffffe273:	01010010
+(gdb) x/f argv[1]
0x7fffffffe273:	6.6831191462912155e+91
+(gdb) x/a argv[1]
0x7fffffffe273:	0x5300676e65766552
```
### To diassemble a function:
```diff
+(gdb) disassemble main
Dump of assembler code for function main:
   0x000055555555517f <+0>:	push   %rbp
   0x0000555555555180 <+1>:	mov    %rsp,%rbp
   0x0000555555555183 <+4>:	sub    $0x10,%rsp
   0x0000555555555187 <+8>:	mov    %edi,-0x4(%rbp)
   0x000055555555518a <+11>:	mov    %rsi,-0x10(%rbp)
=> 0x000055555555518e <+15>:	mov    -0x10(%rbp),%rax ------------> Here, '=>' indicates where 'rip' is actually pointing to.
   0x0000555555555192 <+19>:	add    $0x8,%rax
   0x0000555555555196 <+23>:	mov    (%rax),%rax
   0x0000555555555199 <+26>:	mov    %rax,%rdi
   0x000055555555519c <+29>:	call   0x555555555145 <EchoInput>
   0x00005555555551a1 <+34>:	mov    $0x0,%eax
   0x00005555555551a6 <+39>:	leave  
   0x00005555555551a7 <+40>:	ret    
End of assembler dump.
+(gdb) disassemble EchoInput 
Dump of assembler code for function EchoInput:
   0x0000555555555145 <+0>:	push   %rbp
   0x0000555555555146 <+1>:	mov    %rsp,%rbp
   0x0000555555555149 <+4>:	sub    $0x30,%rsp
   0x000055555555514d <+8>:	mov    %rdi,-0x28(%rbp)
   0x0000555555555151 <+12>:	mov    -0x28(%rbp),%rdx
   0x0000555555555155 <+16>:	lea    -0x20(%rbp),%rax
   0x0000555555555159 <+20>:	mov    %rdx,%rsi
   0x000055555555515c <+23>:	mov    %rax,%rdi
   0x000055555555515f <+26>:	call   0x555555555030 <strcpy@plt>
   0x0000555555555164 <+31>:	lea    -0x20(%rbp),%rax
   0x0000555555555168 <+35>:	mov    %rax,%rsi
   0x000055555555516b <+38>:	lea    0xe92(%rip),%rdi        # 0x555555556004
   0x0000555555555172 <+45>:	mov    $0x0,%eax
   0x0000555555555177 <+50>:	call   0x555555555040 <printf@plt>
   0x000055555555517c <+55>:	nop
   0x000055555555517d <+56>:	leave  
   0x000055555555517e <+57>:	ret    
End of assembler dump.
```
### Just onething:
```diff
 (gdb) info registers 

                   x --- snip --- x   

+rip            0x55555555518e      0x55555555518e <main+15>

                   x --- snip --- x

 (gdb) disassemble main
 Dump of assembler code for function main:

                    x --- snip --- x

+=> 0x000055555555518e <+15>:	 mov    -0x10(%rbp),%rax

                    x --- snip --- x
```

#### In the above console, green lines showed us that rip (rip in x86_64, eip in x86) is pointing to 0x55555555518e address.
### But what is this:        
``` 
mov    -0x10(%rbp),%rax 
```
###  in disassemble main ouput?

#### We can use: examine command
```
(gdb) help x

                      x --- snip --- x

  x --- snip --- x    i(instruction)     x --- snip --- x

                      x --- snip --- x

```
##### i(instruction) ---> convert instruction to ASM language 
### => Basically, for decoding to ASM
```
(gdb) x/i 0x55555555518e
=> 0x55555555518e <main+15>:	mov    -0x10(%rbp),%rax
```
##### This is the thing what we got previously in the output of "disassemble main"
```
mov    -0x10(%rbp),%rax
```
### For decoding multiple instructions to ASM
```
        +--------------------->[number of instructions]
        |
(gdb) x/10i 0x55555555518e
=> 0x55555555518e <main+15>:	mov    -0x10(%rbp),%rax
   0x555555555192 <main+19>:	add    $0x8,%rax
   0x555555555196 <main+23>:	mov    (%rax),%rax
   0x555555555199 <main+26>:	mov    %rax,%rdi
   0x55555555519c <main+29>:	call   0x555555555145 <EchoInput>
   0x5555555551a1 <main+34>:	mov    $0x0,%eax
   0x5555555551a6 <main+39>:	leave  
   0x5555555551a7 <main+40>:	ret    
   0x5555555551a8:	nopl   0x0(%rax,%rax,1)
   0x5555555551b0 <__libc_csu_init>:	push   %r15
```
### For decoding multiple instructions to Hex

#### Usage: x/[number]xw [address/instruction/$[register name] ]
```
(gdb) x/10xw 0x7fffffffde00
0x7fffffffde00:	0xffffdf08	0x00007fff	0x00000000	0x00000002
0x7fffffffde10:	0x555551b0	0x00005555	0xf7e0dd0a	0x00007fff
0x7fffffffde20:	0xffffdf08	0x00007fff

(gdb) x/10xw $rsp
0x7fffffffde00:	0xffffdf08	0x00007fff	0x00000000	0x00000002
0x7fffffffde10:	0x555551b0	0x00005555	0xf7e0dd0a	0x00007fff
0x7fffffffde20:	0xffffdf08	0x00007fff

```
# Video 6.- Modifying data in Memory and changing CPU registers at run time

- Two basic steps to gain power:
1. Modify CPU Registers
2. Modify Data in Memory

```diff
$ gdb ./main6_debug 

                                               x --- snip --- x

Reading symbols from ./main6_debug...
(gdb) info breakpoints 
No breakpoints or watchpoints.
(gdb) break main
Breakpoint 2 at 0x5555555551d6: file main6.c, line 26.
(gdb) info breakpoints 
Num     Type           Disp Enb Address            What
2       breakpoint     keep y   0x00005555555551d6 in main at main6.c:26
(gdb) run aaaaaaaaaaa 20 34
The program being debugged has been started already.
Start it from the beginning? (y or n) y
Starting program: /home/kali/Desktop/ASM/Pentester-Academy/GNU_debugger/Video6/main6_debug aaaaaaaaaaa 20 34

Breakpoint 2, main (argc=4, argv=0x7fffffffdef8) at main6.c:26
26		int sum = 0;
+(gdb) print argv[1]
$6 = 0x7fffffffe269 'a' <repeats 11 times>
+(gdb) print argv[2]
$7 = 0x7fffffffe275 "20"
+(gdb) print argv[3]
$8 = 0x7fffffffe278 "34"
(gdb) print argv[4]
$9 = 0x0
                      ascii char
                         |  | 
+(gdb) x/5c argv[1]      V  V
0x7fffffffe269:	97 'a'	97 'a'	97 'a'	97 'a'	97 'a'
+(gdb) set {char} 0x7fffffffe269 = 'B' ---> to change the 1st occuring char to 'B' of the the specified address
+(gdb) x/5c argv[1]
0x7fffffffe269:	66 'B'	97 'a'	97 'a'	97 'a'	97 'a' 
+(gdb) set {char} 0x7fffffffe269 = 'b'
+(gdb) x/5c argv[1]
0x7fffffffe269:	98 'b'	97 'a'	97 'a'	97 'a'	97 'a'
```
### Now if I want to change every value of 'a' to any other character:
##### Use: set {char} address/(address + 1....n) = "N" where N= anything except string
### NOTE:
- Although we can write string in memory but that way is different.
```
(gdb) x/5c argv[1]
0x7fffffffe269:	0 '\000'	0 '\000'	0 '\000'	0 '\000'	97 'a'
(gdb) set {char} 0x7fffffffe269 = 'B'
(gdb) set {char} (0x7fffffffe269 + 1) = 'B'
(gdb) set {char} (0x7fffffffe269 + 2) = 'B'
(gdb) set {char} (0x7fffffffe269 + 3) = 'B'
(gdb) x/5c argv[1]
0x7fffffffe269:	66 'B'	66 'B'	66 'B'	66 'B'	97 'a'

(gdb) set {char} (0x7fffffffe269 + 4) = 'ok'
Invalid character constant.
(gdb) set {char} (0x7fffffffe269 + 4) = '3'      -----------> here 3 is a integer => as ascii value of int 3 is 51
(gdb) x/5c argv[1]
0x7fffffffe269:	66 'B'	66 'B'	66 'B'	66 'B'	51 '3'
(gdb) set {char} (0x7fffffffe269 + 4) = 3 
(gdb) x/5c argv[1]
0x7fffffffe269:	66 'B'	66 'B'	66 'B'	66 'B'	3 '\003'

(gdb) c
Continuing.


BBBB


Sum of 20 + 34 is 54

[Inferior 1 (process 98341) exited normally]
(gdb) 

```
### See the below mentioned text throughly, in this section many mistkes are shown(intensionally), to clear things a bit more:
```
$ gdb ./main6_debug

Reading symbols from ./main6_debug...
(gdb) break main
Breakpoint 1 at 0x11d6: file main6.c, line 26.
(gdb) c
The program is not being run.
(gdb) run AAAA 10 90
Starting program: /home/kali/Desktop/ASM/Pentester-Academy/GNU_debugger/Video6/main6_debug AAAA 10 90

Breakpoint 1, main (argc=4, argv=0x7fffffffdef8) at main6.c:26
26		int sum = 0;

(gdb) c
Continuing.


AAAA

Sum of 10 + 90 is 100

[Inferior 1 (process 100783) exited normally]

(gdb) print sum
No symbol "sum" in current context.
(gdb) print argv[1]
No symbol "argv" in current context.
(gdb) c
The program is not being run.
(gdb) run AAAA 10 80
Starting program: /home/kali/Desktop/ASM/Pentester-Academy/GNU_debugger/Video6/main6_debug AAAA 10 80

Breakpoint 1, main (argc=4, argv=0x7fffffffdef8) at main6.c:26
26		int sum = 0;
(gdb) print argv[1]
$1 = 0x7fffffffe270 "AAAA"
(gdb) print argv[2]
$2 = 0x7fffffffe275 "10"
(gdb) c
Continuing.


AAAA

Sum of 10 + 80 is 90

[Inferior 1 (process 100815) exited normally]

```



### Lets change the interpretation from char to int:
```diff
$ gdb ./main6_debug 

                                               x --- snip --- x

Reading symbols from ./main6_debug...
(gdb) info breakpoints 
No breakpoints or watchpoints.
(gdb) break main
Breakpoint 2 at 0x5555555551d6: file main6.c, line 26.
(gdb) info breakpoints 
Num     Type           Disp Enb Address            What
2       breakpoint     keep y   0x00005555555551d6 in main at main6.c:26
(gdb) run aaaa 10 30
Starting program: /home/kali/Desktop/ASM/Pentester-Academy/GNU_debugger/Video6/main6_debug aaaa 10 30

Breakpoint 1, main (argc=4, argv=0x7fffffffdef8) at main6.c:26
26		int sum = 0;
(gdb) x/5c argv[1]
0x7fffffffe270:	97 'a'	97 'a'	97 'a'	97 'a'	0 '\000'
(gdb) set {char} 0x7fffffffe270 = 'B'
(gdb) set {char} (0x7fffffffe270 + 1) = 'B'
(gdb) set {char} (0x7fffffffe270 + 2) = 'B'
(gdb) set {char} (0x7fffffffe270 + 3) = 'B'
(gdb) x/5c argv[1]
0x7fffffffe270:	66 'B'	66 'B'	66 'B'	66 'B'	0 '\000'
(gdb) c
Continuing.


BBBB

Sum of 10 + 30 is 40

[Inferior 1 (process 101012) exited normally]


```
### We can see here that some change did happened.

### Adding another breakpoint: Please see every step carefully:

```diff
$ gdb ./main6_debug

                                          x --- snip --- x

Reading symbols from ./main6_debug...
+(gdb) run AAAA 10 90                   --------------> without applying any breakpoint (ran 1st time)
Starting program: /home/kali/Desktop/ASM/Pentester-Academy/GNU_debugger/Video6/main6_debug AAAA 10 90


AAAA

Sum of 10 + 90 is 100

[Inferior 1 (process 109749) exited normally]
-(gdb) c                               ---------> Nothing to continue as it has run successfully before
The program is not being run.
+(gdb) break main                   ----------------------> applying 1st break point (ran 2nd time)
Breakpoint 1 at 0x5555555551d6: file main6.c, line 26.
-(gdb) c                                   --------------> Nothing to run as it has ran before sucessfully (fully)
The program is not being run.
+(gdb) run AAAA 10 80             -------------> So we are running it again
Starting program: /home/kali/Desktop/ASM/Pentester-Academy/GNU_debugger/Video6/main6_debug AAAA 10 80

+Breakpoint 1, main (argc=4, argv=0x7fffffffdef8) at main6.c:26  --------> didnot get any output
-warning: Source file is more recent than executable.        ---------> as we applied a breakpoint we have to see output seperately as done below within square bracket.
26		int sum = 0;
-(gdb) print sum           -----------> sum will not give the added value as at main breakpoint was added and sum variable is present within main function. 
$2 = 21845
+(gdb) print argv[1]                           ----+
$1 = 0x7fffffffe270 "AAAA"                         |   
+(gdb) print argv[2]                               | 
$2 = 0x7fffffffe275 "10"                           |
+(gdb) print argv[3]                               |
$3 = 0x7fffffffe278 "80"                           |
+(gdb) c                                           |--------> lets see the outputs...
Continuing.                                        |
                                                   |
                                                   |
AAAA                                               |
                                                   |
Sum of 10 + 80 is 90                               | 
                                                   | 
[Inferior 1 (process 110171) exited normally]  ----+
-(gdb) print argv[2]                   ------> we can not see the ouput again as we have seen it before
No symbol "argv" in current context.
(gdb) l
21		return i + j;
22	}
23	
24	int main(int argc, char **argv)
25	{
26		int sum = 0;
27	
28		EchoInput(argv[1]);
29	
30		sum = AddNumbers(atoi(argv[2]), atoi(argv[3]));
(gdb) 
31	
32		printf("Sum of %s + %s is %d\n\n", argv[2], argv[3], sum );
33	
34		return 0;
35	
36	}
37	
+(gdb) break 32                    ------------------>     applying 2nd break point
Breakpoint 2 at 0x555555555224: file main6.c, line 32.
(gdb) c
-The program is not being run.      -------------------------------> Clearly speaking program is not being run

+(gdb) run AAAA 10 100               ----------------->  ran the binary but as the break point was applied we have to see the result manually
Starting program: /home/kali/Desktop/ASM/Pentester-Academy/GNU_debugger/Video6/main6_debug AAAA 10 100

Breakpoint 1, main (argc=4, argv=0x7fffffffdef8) at main6.c:26
26		int sum = 0;
+(gdb) c                ------------->     Now, we can continue as we have run the binary before
Continuing.


AAAA


Breakpoint 2, main (argc=4, argv=0x7fffffffdef8) at main6.c:32
32		printf("Sum of %s + %s is %d\n\n", argv[2], argv[3], sum );
+(gdb) print sum                        -------------------------------> No we got the sum value as breakpoint was applied at printing statement not on the value assigning line.
$1 = 110      

```
### Now, lets manipulate the result:
##### Use: set variable = [value]
```diff
+(gdb) set sum = 2000
(gdb) c
Continuing.
Sum of 10 + 100 is 2000

[Inferior 1 (process 111050) exited normally]

```
### With gdb we can not only change values, varibles, etc present in memory but also we can change CPU registers at run time.
```diff
(gdb) info functions 
All defined functions:

File main6.c:
19:	int AddNumbers(int, int);
10:	void EchoInput(char *);
5:	void FunctionShouldNotExecute(void);
24:	int main(int, char **);

Non-debugging symbols:
0x0000555555555000  _init
0x0000555555555030  strcpy@plt
0x0000555555555040  puts@plt
0x0000555555555050  printf@plt
0x0000555555555060  atoi@plt
0x0000555555555070  __cxa_finalize@plt
0x0000555555555080  _start
0x00005555555550b0  deregister_tm_clones
0x00005555555550e0  register_tm_clones
0x0000555555555120  __do_global_dtors_aux
0x0000555555555160  frame_dummy
0x0000555555555260  __libc_csu_init
0x00005555555552c0  __libc_csu_fini
0x00005555555552c4  _fini
0x00007ffff7fd3010  _dl_catch_exception@plt
0x00007ffff7fd3020  malloc@plt
0x00007ffff7fd3030  _dl_signal_exception@plt
0x00007ffff7fd3040  calloc@plt
0x00007ffff7fd3050  realloc@plt
0x00007ffff7fd3060  _dl_signal_error@plt
0x00007ffff7fd3070  _dl_catch_error@plt
0x00007ffff7fd3080  free@plt
0x00007ffff7fdbbe0  _dl_rtld_di_serinfo
0x00007ffff7fe2560  _dl_debug_state
0x00007ffff7fe4090  _dl_mcount
0x00007ffff7fe4990  _dl_get_tls_static_info
0x00007ffff7fe4a80  _dl_allocate_tls_init
0x00007ffff7fe4cc0  _dl_allocate_tls
0x00007ffff7fe4d30  _dl_deallocate_tls
0x00007ffff7fe5410  _dl_make_stack_executable
0x00007ffff7fe5760  _dl_find_dso_for_object
0x00007ffff7fe86d0  _dl_exception_create
0x00007ffff7fe87b0  _dl_exception_create_format
0x00007ffff7fe8c50  _dl_exception_free
0x00007ffff7fe9c90  __tunable_get_val
0x00007ffff7fea440  __tls_get_addr
0x00007ffff7fea480  __get_cpu_features
--Type <RET> for more, q to quit, c to continue without paging--q
Quit
(gdb) print FunctionShouldNotExecute 
$2 = {void (void)} 0x555555555165 <FunctionShouldNotExecute>
+(gdb) info registers                 ----------------------------> As program is not running
The program has no registers now.
(gdb) run AA 10 20
Starting program: /home/kali/Desktop/ASM/Pentester-Academy/GNU_debugger/Video6/main6_debug AA 10 20

Breakpoint 1, main (argc=4, argv=0x7fffffffdef8) at main6.c:26
26		int sum = 0;
(gdb) info registers 
+rax            0x5555555551c6      93824992235974   ----------------> let us change value of this register
rbx            0x0                 0
rcx            0x7ffff7fa5718      140737353766680
rdx            0x7fffffffdf20      140737488346912
rsi            0x7fffffffdef8      140737488346872
rdi            0x4                 4
rbp            0x7fffffffde00      0x7fffffffde00
rsp            0x7fffffffddd0      0x7fffffffddd0
r8             0x0                 0
r9             0x7ffff7fe2180      140737354015104
r10            0x3                 3
r11            0x2                 2
r12            0x555555555080      93824992235648
r13            0x0                 0
r14            0x0                 0
r15            0x0                 0
rip            0x5555555551d6      0x5555555551d6 <main+16>
eflags         0x202               [ IF ]
cs             0x33                51
ss             0x2b                43
ds             0x0                 0
es             0x0                 0
fs             0x0                 0
gs             0x0                 0
+(gdb) set $rax = 10       ---------------> Let's change value of this register to 10
(gdb) info registers 
+rax            0xa                 10 ---------------> Changed to 10
rbx            0x0                 0
rcx            0x7ffff7fa5718      140737353766680
rdx            0x7fffffffdf20      140737488346912
rsi            0x7fffffffdef8      140737488346872
rdi            0x4                 4
rbp            0x7fffffffde00      0x7fffffffde00
rsp            0x7fffffffddd0      0x7fffffffddd0
r8             0x0                 0
r9             0x7ffff7fe2180      140737354015104
r10            0x3                 3
r11            0x2                 2
r12            0x555555555080      93824992235648
r13            0x0                 0
r14            0x0                 0
r15            0x0                 0
+rip            0x5555555551d6      0x5555555551d6 <main+16> ----------> We would change the value of rip to 0x555555555165 <FunctionShouldNotExecute>
eflags         0x202               [ IF ]
cs             0x33                51
ss             0x2b                43
ds             0x0                 0
es             0x0                 0
fs             0x0                 0
gs             0x0                 0
(gdb) set $rip = 0x555555555165
(gdb) c
Continuing.


I should not execute!!!


Program received signal SIGSEGV, Segmentation fault.
0x00007fffffffdef8 in ?? ()
```
### We received a segmentation fault:
```
void FunctionShouldNotExecute(void)
{
        printf("\n\nI should not execute!!!\n\n");
}
```
#### We actually, just changed the register value arbitarily, that part does not contain any value to return (there is actually no logical flow to this portion of the program). Hence whatever will be present on the stack would pretty much be garbage.
### To solve this: just add an exit(0):
```
void FunctionShouldNotExecute(void)
{
        printf("\n\nI should not execute!!!\n\n");
        exit(0);
}
```
```diff
- For my case, it didn't happen
```
```diff
(gdb) set $rip = 0x1175
(gdb) c
Continuing.

-Program received signal SIGSEGV, Segmentation fault.
0x0000000000001175 in ?? ()

```
see: https://youtu.be/bFD2uHsO098?t=998

# Video 7: Convinience Variables and Calling Routines
## Convinience Varibles:
* We can create variables in GDB to hold data ----> just like in bash, IP=x.x.x.x OR in fish, set IP x.x.x.x
* We can create **convinience varibles** with: set $i = 10
* malloc usage: set $dyn = (char *)malloc(10)
* demo ="reveng"
* set argv[1] = $demo

```diff
$ gdb ./main7_debug

                              x --- snip --- x

Reading symbols from ./main7_debug...
(gdb) run AAA 10 20
Starting program: /home/kali/Desktop/ASM/Pentester-Academy/GNU_debugger/Video7/main7_debug AAA 10 20


AAA

Sum of 10 + 20 is 30

[Inferior 1 (process 196235) exited normally]
(gdb) break main
Breakpoint 1 at 0x5555555551ed: file main7.c, line 27.
(gdb) run AAA 10 30
Starting program: /home/kali/Desktop/ASM/Pentester-Academy/GNU_debugger/Video7/main7_debug AAA 10 30

Breakpoint 1, main (argc=4, argv=0x7fffffffdef8) at main7.c:27
27		int sum = 0;
(gdb) print argv[1]
$1 = 0x7fffffffe271 "AAA"
+(gdb) set $demo = "BBBB"
(gdb) print $demo
$2 = "BBBB"
+(gdb) set argv[1] = $demo
(gdb) print argv[1]
+$3 = 0x7ffff7faef70 "BBBB"
(gdb) c
Continuing.


BBBB

Sum of 10 + 30 is 40

[Inferior 1 (process 196241) exited normally]
```
Using **malloc** as convinience variable:
```diff
[Inferior 1 (process 196241) exited normally]
(gdb) c
The program is not being run.
(gdb) run AAA 10 20
Starting program: /home/kali/Desktop/ASM/Pentester-Academy/GNU_debugger/Video7/main7_debug AAA 10 20

Breakpoint 1, main (argc=4, argv=0x7fffffffdef8) at main7.c:27
27		int sum = 0;
+(gdb) set $dyn = (char *)malloc(10)
(gdb) print $dyn
+$4 = 0x7ffff7faef70 ""
+(gdb) x/10xb $dyn
+0x7ffff7faef70:	0x00	0x00	0x00	0x00	0x00	0x00	0x00	0x00  ----> 10 
+0x7ffff7faef78:	0x00	0x00                                      ----> locations 
+(gdb) set argv[1] = $dyn
(gdb) print argv[1]
+$5 = 0x7ffff7faef70 ""
```
We can also apply other function call in the operations
```diff
$(gdb) info breakpoints 
Num     Type           Disp Enb Address            What
2       breakpoint     keep y   0x00005555555551ed in main at main7.c:27
	breakpoint already hit 1 time
(gdb) run AAA 10 30
Starting program: /home/kali/Desktop/ASM/Pentester-Academy/GNU_debugger/Video7/main7_debug AAA 10 30

Breakpoint 2, main (argc=4, argv=0x7fffffffdef8) at main7.c:27
27		int sum = 0;
+(gdb) set $dyn = (char *)malloc(10)
(gdb) print $dyn
$8 = 0x7ffff7faef70 ""
(gdb) x/10xb $dyn
0x7ffff7faef70:	0x00	0x00	0x00	0x00	0x00	0x00	0x00	0x00
0x7ffff7faef78:	0x00	0x00
+(gdb) call strcpy($dyn, argv[1])
-'at 0x0x7ffff7f48ff0' has unknown return type; cast the call to its declared return type

```
It didn't gave me satisfactory result...
see: [here](https://youtu.be/Kh1kM1m1kkk?t=373)
```
(gdb) call AddNumbers(10,20)
$9 = 30
(gdb) set $i = 100
(gdb) set $j = 200
(gdb) call AddNumbers($i, $j)
$10 = 300
(gdb) call EchoInput("Holla!")


Holla!

(gdb) call EchoInput("argv[1]")


argv[1]

(gdb) call EchoInput(argv[1])


AAA
```
# Video 8: Cracking a Simple Binary with DEBUG Symbols

### NOTE:
To simply move instruction pointer(rip/eip) to next step,

Use:
```
(gdb) stepi
```

### Strings

1. Display strings in the program
2. Poorly coded ones may reveal private/secret information
3. Secret can be easily hidden by encryption/encoding
4. Not very powerful but is a good starting point

### Note: 
Larger the program, more is the complexity of cracking and more presence of strings

#### I created 4 files:
1. with debug symbols (with -ggdb)
2. binary with ripped symbols
3. binary with stripped symbols
4. binary with full stripped symbols
----------------

#### A) Cracking with "strings" utility    : Static Analysis

1.
```diff
$ strings Video8_debug

/lib64/ld-linux-x86-64.so.2
exit
puts
printf
__cxa_finalize
strcmp
__libc_start_main
libc.so.6
GLIBC_2.2.5
_ITM_deregisterTMCloneTable
__gmon_start__
_ITM_registerTMCloneTable
u/UH
[]A\A]A^A_
+Secret code: 4832-3422-3421-9847
+%s password_to_unlock           --> seems like string passed to printf
+l33tsp3ak
+Incorrect Password! Please try again! 
;*3$"
GCC: (Debian 10.2.1-6) 10.2.1 20210110
Video8.c
long long int
+password
/home/kali/Desktop/ASM/Pentester-Academy/GNU_debugger/Video8
main
+UnlockSecret
long long unsigned int
short unsigned int
GNU C17 10.2.1 20210110 -mtune=generic -march=x86-64 -ggdb -fasynchronous-unwind-tables
unsigned char
result
argv
argc
Video8.c
userInput
checkPass
short int
+IsPasswordCorrect
                     x --- snip --- x
```
### All the juicy infos are marked with green
### Now,
```
$ ./Video8_debug l33tsp3ak
Secret code: 4832-3422-3421-9847
```

2.
```
$ strings Video8_debug_ripped

GCC: (Debian 10.2.1-6) 10.2.1 20210110
Video8.c
long long int
password
/home/kali/Desktop/ASM/Pentester-Academy/GNU_debugger/Video8
main
UnlockSecret
long long unsigned int
short unsigned int
GNU C17 10.2.1 20210110 -mtune=generic -march=x86-64 -ggdb -fasynchronous-unwind-tables
unsigned char
result
argv
argc
Video8.c
userInput
checkPass
short int
IsPasswordCorrect
                    x --- snip --- x
```
## NOTICE:
Here, we cannot see **secret code** and **password to unlock** strings

### Now,
```
$ ./Video8_debug_ripped l33tsp3ak

Failed to execute process './Video8_debug_ripped'. Reason:
exec: Exec format error
The file './Video8_debug_ripped' is marked as an executable but could not be run by the operating system.
```

3.
```diff
$ strings Video8_debug_stripped

/lib64/ld-linux-x86-64.so.2
exit
puts
printf
__cxa_finalize
strcmp
__libc_start_main
libc.so.6
GLIBC_2.2.5
_ITM_deregisterTMCloneTable
__gmon_start__
_ITM_registerTMCloneTable
u/UH
[]A\A]A^A_
+Secret code: 4832-3422-3421-9847
+%s password_to_unlock            --> seems like string passed to printf
+l33tsp3ak
+Incorrect Password! Please try again! 
;*3$"
GCC: (Debian 10.2.1-6) 10.2.1 20210110
deregister_tm_clones
__do_global_dtors_aux
completed.0
__do_global_dtors_aux_fini_array_entry
frame_dummy
__frame_dummy_init_array_entry
__FRAME_END__
__init_array_end
_DYNAMIC
__init_array_start
__GNU_EH_FRAME_HDR
_GLOBAL_OFFSET_TABLE_
__libc_csu_fini
_ITM_deregisterTMCloneTable
puts@GLIBC_2.2.5
_edata
printf@GLIBC_2.2.5
__libc_start_main@GLIBC_2.2.5
__data_start
strcmp@GLIBC_2.2.5
__gmon_start__
__dso_handle
_IO_stdin_used
__libc_csu_init
+IsPasswordCorrect
__bss_start
+main
+UnlockSecret
                          x --- snip --- x
```
Here we can see all the strings.

### Now,
```
$ ./Video8_debug_stripped l33tsp3ak

Secret code: 4832-3422-3421-9847
```
4.
```diff
$ strings Video8_debug_full-stripped 

/lib64/ld-linux-x86-64.so.2
exit
puts
printf
__cxa_finalize
strcmp
__libc_start_main
libc.so.6
GLIBC_2.2.5
_ITM_deregisterTMCloneTable
__gmon_start__
_ITM_registerTMCloneTable
u/UH
[]A\A]A^A_
+Secret code: 4832-3422-3421-9847
+%s password_to_unlock            --> seems like string passed to printf
+l33tsp3ak
+Incorrect Password! Please try again! 
;*3$"
```
We can see important strings are present

### Now,
```
$ ./Video8_debug_full-stripped l33tsp3ak

Secret code: 4832-3422-3421-9847
```
B) Now, with gdb  : run-time analysis/dynamic analysis

1.
```diff
Reading symbols from ./Video8_debug...

(gdb) info functions

All defined functions:

File Video8.c:
10:	int IsPasswordCorrect(char *, char *);
5:	void UnlockSecret(void);
25:	int main(int, char **);

                        x --- snip --- x

+(gdb) break main        --> As in C, from main function program execution starts
Breakpoint 1 at 0x11c1: file Video8.c, line 28.

+(gdb) run somevalue      ---> Lets run the program 

Starting program: /home/kali/Desktop/ASM/Pentester-Academy/GNU_debugger/Video8/Video8_debug somevalue

Breakpoint 1, main (argc=2, argv=0x7fffffffdf28) at Video8.c:28
28		int checkPass = 0;

+ Now, 2 functions are left, one has void input and another one has 2 input, which we don't know. So lets 1st call UnlockSecret() as no value is required to call this.

+(gdb) call UnlockSecret()

Secret code: 4832-3422-3421-9847   -----> we got the secret code
```
### But we still haven't got the passphrase

```diff
Here what we know is main() and UnlockSecret() don't contain any passphrase, if that would have been true, we would see that earlier.

So, now we have to concentrate on IsPasswordCorrect()

(gdb) info breakpoints

Num     Type           Disp Enb Address            What
1       breakpoint     keep y   0x00005555555551c1 in main at Video8.c:28
	breakpoint already hit 1 time

+(gdb) break IsPasswordCorrect
Breakpoint 3 at 0x555555555188: file Video8.c, line 14.

+(gdb) run any_value
The program being debugged has been started already.
Start it from the beginning? (y or n) y
Starting program: /home/kali/Desktop/ASM/Pentester-Academy/GNU_debugger/Video8/Video8_debug asdfa

+Breakpoint 3, IsPasswordCorrect (password=0x7fffffffe293 "any_value", 
+    userInput=0x555555556040 "l33tsp3ak") at Video8.c:14 
14		result = strcmp(password, userInput);

+(gdb) print password
$2 = 0x7fffffffe293 "any_value"

+(gdb) print userInput
$3 = 0x555555556040 "l33tsp3ak"
```
Here we got the password: **"l33tsp3ak"** but it should be equal to password not userInput. Developer did a slight naming error.

password = 0x555555556040 "l33tsp3ak"
userInput = 0x7fffffffe293 "any_value"

### We can also do the same as done above like:
```diff
+(gdb) info scope IsPasswordCorrect
Scope for IsPasswordCorrect:
+Symbol password is a complex DWARF expression: ----> password
     0: DW_OP_fbreg -40
, length 8.
+Symbol userInput is a complex DWARF expression: ----> userInput
     0: DW_OP_fbreg -48
, length 8.
+Symbol result is a complex DWARF expression:   -----> result
     0: DW_OP_fbreg -20
, length 4.

+(gdb) print password
$2 = 0x7fffffffe293 "any_value"

+(gdb) print userInput
$3 = 0x555555556040 "l33tsp3ak"

```
2.
```diff

Reading symbols from ./Video8_debug_ripped...
(gdb) info functions 
All defined functions:

File Video8.c:
10:	int IsPasswordCorrect(char *, char *);
5:	void UnlockSecret(void);
25:	int main(int, char **);

                        x --- snip --- x
(gdb) break main
Breakpoint 1 at 0x11b2: file Video8.c, line 26.

(gdb) run any_value
Starting program: /home/kali/Desktop/ASM/Pentester-Academy/GNU_debugger/Video8/Video8_debug_ripped any_value
/usr/bin/bash: line 1: /home/kali/Desktop/ASM/Pentester-Academy/GNU_debugger/Video8/Video8_debug_ripped: cannot execute binary file: Exec format error
/usr/bin/bash: line 1: /home/kali/Desktop/ASM/Pentester-Academy/GNU_debugger/Video8/Video8_debug_ripped: Success
During startup program exited with code 126.

+ We got the PDB symbols, but then also unable to reverse it ??

```

3.
```diff
Reading symbols from ./Video8_debug_stripped...
(No debugging symbols found in ./Video8_debug_stripped)

(gdb) info functions 
All defined functions:

Non-debugging symbols:
0x0000555555555000  _init
0x0000555555555030  puts@plt
0x0000555555555040  printf@plt
0x0000555555555050  strcmp@plt
0x0000555555555060  exit@plt
0x0000555555555070  __cxa_finalize@plt
0x0000555555555080  _start
0x00005555555550b0  deregister_tm_clones
0x00005555555550e0  register_tm_clones
0x0000555555555120  __do_global_dtors_aux
0x0000555555555160  frame_dummy
0x0000555555555165  UnlockSecret
0x0000555555555178  IsPasswordCorrect
0x00005555555551b2  main

                    x --- snip --- x

+ Functions can not be seen(strings/PDB symbols), reversing can't be done with ease

(gdb) break main 
Breakpoint 1 at 0x11b6

(gdb) info breakpoints 
Num     Type           Disp Enb Address            What
1       breakpoint     keep y   0x00000000000011b6 <main+4>

(gdb) run anything
Starting program: /home/kali/Desktop/ASM/Pentester-Academy/GNU_debugger/Video8/Video8_debug_stripped anything

Breakpoint 1, 0x00005555555551b6 in main ()
(gdb) c
Continuing.


Incorrect Password! Please try again! 


[Inferior 1 (process 149559) exited normally]

```

4.
```diff
Reading symbols from ./Video8_debug_full-stripped...
(No debugging symbols found in ./Video8_debug_full-stripped)

(gdb) info functions 
All defined functions:


+ Functions can not be seen(strings/PDB symbols), reversing can't be done with ease

```
## Strangly enough,
**When I used string utility(statis analysis) with these files, I got strings/PDB symbols on 3/4 (except ripped file, strangely - fully stripped file should be in the excluded portion according to the comparison that I made earlier, among ripped and stripped files and debug files).**

**But anyways, When I used gdb with all those files(dynamic/runtime analysis), I got strings from 1/4 - the one file from which I got the PDB symbols, is the file with debug symbols(Video8_debug)**

But how is that possible??

#### In practical sceanarios, we will get debug symbols with binary very often but sometimes not. But for the sake of the open source community, we have the source code to compare them with the binary

## Video 9: Disassembling and Cracking a Simple Binary

Mainly ASM syntax has **AT&T** ans **Intel** syntax

### Procedure to do that:

```
1. set disassembly-flavor  <tab><tab> 

   set disassembly-flavor <syntax name>

2. disassembly <ADDRESS>
```
**By default, gdb uses AT&T format which is also available for diassemblying code/software with GAS(GNU Assembler)**

see the video: [link](https://www.youtube.com/watch?v=bbVLw6I15f0)

## Video 10: Conditional Breakpoints using Variables and Registers

Sometimes, regular breakpoints don't work:

1. Break only if condition is met
2. Handy in cases where there are loops
3. Conditions can be simple/complex

see the video : [youtube](https://www.youtube.com/watch?v=gIWlGxmVzT0&list=PLfpQOoR6-z-rQpwDygoZuHSNCCZBHomlF&index=10)
see this : [link](https://stackoverflow.com/questions/13064809/the-point-of-test-eax-eax)

### GDB disables ASLR while debugging: [link](https://www.youtube.com/watch?v=pphfcaGnWSA)

## Video14: GDB on 64 Bit Systems

see: [link](https://www.youtube.com/watch?v=M9p5bcmd7PA&t=433s)
 
