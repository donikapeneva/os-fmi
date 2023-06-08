# os-fmi

## Directories
- /c 
	- tasks from os-problems.pdf (current)
	- archived tasks from previous years
- /shell
	- tasks from os-problems.pdf (current)
	- archived tasks from previous years


## Creating new solution file

~/makesol \<task\> [-d \<dir\>] 
- creates a directory with \<task\> as name
- creates a file named sol in the task directory
- if -d is passed, it will create it under \<dir\>
- gives x rights to user on the file sol

### required
\<task\> : task name
- if pattern is matching the one from the os-problems.pdf file then the new file will be created under \/shell or \/c-task folder. The script decides if the task number is < FIRST_C_TAKS number, it will go in \/shell, otherwise in \/c-tasks
- if the pattern doesn't match, then the default folder will be used 

### options
-d : directory
- if the directory parameter is passed, then the task will be created under this folder. 

### example: 
- ./makesol 23 -d shell
	- will create file sol in \~\/shell/playground/23
- ./makesol 23 
	- will create file sol in ~/playground/23
- ./makesol 23_2017-SE-06 -d shell
	- will create file sol in ~/shell/23_2017-SE-06
-  ./makesol 58_2017-SE-06 
	- will create file sol in ~/c-tasks/58_2017-SE-06


## Defaults

- DEFAULT_ROOT=~
- DEFAULT_DIR=playground : default folder task to be created in
- FIRST_C_TAKS=56 : according to the os-problems.pdf
