#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define N 12

extern char **environ;

char *allowed[N] = {"cp", "touch", "mkdir", "ls", "pwd", "cat", "grep", "chmod", "diff", "cd", "exit", "help"};

int isAllowed(const char *cmd) {
	// TODO
	// return 1 if cmd is one of the allowed commands
	// return 0 otherwise

	for (int i = 0; i < 12; i++) {
		if (strcmp(cmd, allowed[i]) == 0) {
			return 1;
		}
	}
	return 0;
}

int main() {

    // TODO
    // Add variables as needed

    char line[256];
	char* argv[21];
	pid_t process_id;
	int status;
	posix_spawnattr_t attr;


    while (1) {

	fprintf(stderr,"rsh>");

	if (fgets(line,256,stdin)==NULL) continue;

	if (strcmp(line,"\n")==0) continue;

	line[strlen(line)-1]='\0';

    	int argc = 0;
    	char *token = strtok(line, " ");

    	while (token != NULL && argc < 20) {
    		argv[argc] = token;
    		argc++;
    		token = strtok(NULL, " ");
    	}
    	argv[argc] = NULL;

    	if (!isAllowed(argv[0])) {
    		printf("NOT ALLOWED!\n");
    		continue;
    	}

    	if (strcmp(argv[0], "exit") == 0) {
    		return 0;
    	}


    	if (strcmp(argv[0], "help") == 0) {
    		printf("The allowed commands are:\n");
    		for (int i = 0; i < N; i++) {
    			printf("%d: %s\n", i + 1, allowed[i]);
    		}
    		continue;
    	}

    	if (strcmp(argv[0], "cd") == 0) {//needs to be built in. If we spawn a process, we will only change the directory of the child, not the parent
    		if (argc > 2) {
    			printf("-rsh: cd: too many arguments\n");
    		} else if (argc == 2) {
    			chdir(argv[1]);
    		}
    		continue;
    	}


    	posix_spawnattr_init(&attr);

    	if (posix_spawnp(&process_id, argv[0], NULL, &attr, argv, environ) != 0) {
    		perror("spawn failed");
    		posix_spawnattr_destroy(&attr);
    		continue;
    	}

    	waitpid(process_id, &status, 0);
    	posix_spawnattr_destroy(&attr);


	// TODO
	// Add code to spawn processes for the first 9 commands
	// And add code to execute cd, exit, help commands
	// Use the example provided in myspawn.c

    }
    return 0;
}
