/*
	Ryan Tyler
	Date: 10/03/19
	OS: macOS
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/dir.h>

#define MAX_COMMAND_SIZE 255

off_t traverse_directory(char* directory) {
	off_t size = 0;
	struct dirent *dirent_ptr;
	DIR *DIR_ptr;
	struct stat buf;
	
	/* open directory first */
	DIR_ptr = opendir(directory);
	
	/* check to make sure current directory opens */
	if ( DIR_ptr == NULL ) 
	{
		printf("Directory could not be opened.\n");
		exit(1);
	}
	
	while((dirent_ptr = readdir(DIR_ptr)) != NULL) {
		uint8_t fileType = dirent_ptr->d_type;
		char* fileName = malloc(255 * sizeof(char));
		
		strcpy(fileName,dirent_ptr->d_name);
		
		stat(dirent_ptr->d_name,&buf);
		
		/* 
			if the statement below executes the 
			file is either a file or a directory
			
			Rules for execution:
			
			1.) NOT length 1
			2.) NOT length 2
			3.) file name NOT equal "."
			4.) file name NOT equal ".."
			
			** edge case **
			
				=> any hidden file will begin with '.', 
					if file found do NOT execute statement
			
		*/
		if ( strlen(fileName) != 1 && strlen(fileName) != 2 
				&& strcmp(fileName,".") != 0 && strcmp(fileName,"..") != 0 
					&& fileName[0] != '.' ) {
						/* 
							8 => regular file, 4 => directory
						*/
						if(fileType == 4) {
							/* 
								directory was found; append directory name 
								currently being used with the directory name
								just found
							*/
							strcat(directory,"/");
							strcat(directory,fileName);
							/* 
								the new directory name should be passed into 
								traverse_directory function recursively 
								
								function should not be in sub-directory
								and should calculate the sub-total and
								then return the value back to first
								instance of traverse_directory
							*/
							size += traverse_directory(directory);
						/*
				
							~~ error checking ~~
							printf("\nNow entering a new directory...\n");
						*/
			} else {
				/* add file size located in stat buffer to totalSize */
				size += buf.st_size;
				/*
					
					~~ error checking ~~
					printf("\n ~~Current Size: %lld~~\n\n",size);
				*/
				
			}
		}
		
		free(fileName);
	}
	return size;
}

int main (int argc, char**argv) {
	off_t totalSize = 0;
	/*
		this "current directory" array is initialized in memory
		and this will allow us to pass the value into another 
		function as a copy to manipulate the data without touching
		the actual value of "current directory" at its original place
		in memory
	*/
	char currDir[MAX_COMMAND_SIZE];
	/* 
		Let's find out where we are in the file system 
	*/
	getcwd(currDir,sizeof(currDir));
	printf("Starting Directory: %s\n\n", currDir);
	
	totalSize = traverse_directory(currDir);
	printf("Total Size: %lld\n",totalSize);
	return 0;
}
