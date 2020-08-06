#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio_ext.h>
#include <unistd.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SHMSZ 27

int main()
{
	int shmid, i;
	key_t key;
	char *shm, *s;
	key = 1000;

	if ((shmid = shmget(key, SHMSZ, 0666)) < 0)
	{
        	perror("ERROR: shmget failed !");
        	exit(1);
    	}

	if ((shm = shmat(shmid, NULL, 0)) == (char *) -1)
	{
        	perror("ERROR: shmat failed !");
        	exit(1);
	}

	*shm = '!';

	while(*shm!='*')
	{
		while(*shm!='@' && *shm!='*')
			sleep(1);

		if(*shm!='*')
		{
			printf("SERVER: ");
			for (s = shm+1; *s != '\0'; s++)
		    	putchar(*s);
			putchar('\n');

			char a[100];

			printf("Message[press * to END]: ");
			scanf("%[^\n]%*c", a);

			if(a[0]!='*')
			{
				s = shm+1;
				for (i=0; i<strlen(a); i++)
					 *s++ = a[i];
				*s = '\0';

				*shm = '#';
			}

			else
				*shm = '*';
		}

		else
			printf("\nConnection CLOSED by SERVER\n");
	}

	shmid = shmdt(shm);

	return 0;
}