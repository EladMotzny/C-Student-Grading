# C-Student-Grading
Ariel University OS course assignment.
These programs will only use system calls.

## ex11.c
This program will get the path for two text files as parameters for the main function and will check if the files are the same.
Example:
./comp.out /home/user/workspace/files/f1.txt /home/user/workspace/files/f2.txt
Will return 2 if the files are the same and 1 if they are not.

## ex12.c
This program will get the path for a file that contains three lines:
1) The folder in which the students programs are stored (each sub-folder is a different student)
2) Input file path
3) The correct output for the input we provide
Example: ./a.out home/user/Desktop/OSExe/file.txt
![image](https://user-images.githubusercontent.com/33173449/52202425-1b8dae00-2877-11e9-8fcd-357e588d351e.png)

Student folders example:
![image](https://user-images.githubusercontent.com/33173449/52202653-d28a2980-2877-11e9-8c21-9015305dab57.png)

It will then go through the folder, run the students programs and compare the output using ex11.c. Please note that the compilation file for ex11.c needs to be called comp.out otherwise it would not work.
The program will create a CSV file in which the grades will be stored. The possible grades can be:
0 with bad output
0 with compilation error
0 with no C file
100 with great job
Example:
Martziano,0,BAD_OUTPUT
Ortal,0,COMPILATION_ERROR
Shalom,100,GREAT_JOB
Shimi,100,GREAT_JOB
Farjun,0,NO_C_FILE

The program will delete all of the students compilation files so there wont be any junk left behind.
