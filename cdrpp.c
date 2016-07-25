#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include <sys/wait.h>


void substring(char s[], char sub[], int p, int l);
void uncompressFile(char *folder);
void copy(char *source, char *dest);
void uncompressFileSys(char *folder);
void copySys(char *source, char *dest);
void cpFileToRTCPath();

int main(void) {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) printf("directory: %s\n", cwd);
    char path[1024];
    struct stat st = {0};       
        
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (cwd)) != NULL) {
        strcpy(path, cwd);
        strcat(path, "/cdrpptemp");
        if (stat(path, &st) == -1) {
            printf("create temp dir: %s\n", path);
            mkdir(path, 0755);
        }
        struct tm * timeinfo;
        char datef[70];
        time_t now = time(0);
        timeinfo = localtime(&now);
        strftime(datef, sizeof datef, "%Y%m%d%H%M%S", timeinfo);
        
        char pathITSC5[1024];
        char pathITSC4A[1024];
        char pathITSC4B[1024];
        
        strcpy(pathITSC5, path);
        strcat(pathITSC5,"/ITSC5_");
        strcat(pathITSC5,datef);
        
        strcpy(pathITSC4A, path);
        strcat(pathITSC4A,"/ITSC4A_");
        strcat(pathITSC4A,datef);
        
        strcpy(pathITSC4B, path);
        strcat(pathITSC4B,"/ITSC4B_");
        strcat(pathITSC4B,datef);
        
        FILE *fITSC5 = fopen(pathITSC5, "w+");
        if (fITSC5 == NULL) {
            printf("Error opening file!\n");
            return 1;
        }
        FILE *fITSC4A = fopen(pathITSC4A, "w+");
        if (fITSC4A == NULL) {
            printf("Error opening file!\n");
            return 1;
        }
        FILE *fITSC4B = fopen(pathITSC4B, "w+");
        if (fITSC4B == NULL) {
            printf("Error opening file!\n");
            return 1;
        }
        while ((ent = readdir (dir)) != NULL) {
            if (strcmp(".",ent->d_name) != 0 && strcmp("..",ent->d_name) != 0 && ent->d_type != 4) {
                if ((ent->d_name[0] == 'h' && ent->d_name[1] == 'i') || (ent->d_name[0] == 'T' && ent->d_name[1] == 'T')){
                    if (strstr(ent->d_name,"ITSC5")) {
                        fprintf(fITSC5, "%s\n", ent->d_name);
                    } else if (strstr(ent->d_name,"ITSC4A")) {
                        fprintf(fITSC4A, "%s\n", ent->d_name);
                    } else if (strstr(ent->d_name,"ITSC4B")) {
                        fprintf(fITSC4B, "%s\n", ent->d_name);
                    }
                }
            }
        }
        closedir(dir);
        
        rewind(fITSC5);rewind(fITSC4A);rewind(fITSC4B);
        
        char folderITSC5[1024];
        strcpy(folderITSC5, cwd);
        strcat(folderITSC5, "/ITSC5_");
        int c = 0;
        char line [256];char sub[256];char srcPath[1024];
        while ((fgets(line, sizeof line, fITSC5) != NULL) && (c < 999999))
        {
            c++;
            substring(line, sub, 29, 8);
            strcpy(path, folderITSC5);
            strcat(path, sub);
            if (stat(path, &st) == -1) {
                printf("create dest dir: %s\n", path);
                mkdir(path, 0755);
            }
            line[strcspn(line, "\n")] = 0;
            strcpy(srcPath,cwd);strcat(srcPath,"/");strcat(srcPath,line);
            strcat(path,"/");strcat(path,line);
            rename(srcPath, path);
            //printf("'%s' '%s' %d\n", srcPath, path, rename(srcPath, path));
        }
        
        char folderITSC4A[1024];
        strcpy(folderITSC4A, cwd);
        strcat(folderITSC4A, "/ITSC4A_");
        c = 0;
        while((fgets(line, sizeof line, fITSC4A) != NULL) && (c < 999999))
        {
            c++;
            substring(line, sub, 22, 8);
            strcpy(path, folderITSC4A);
            strcat(path, sub);
            if (stat(path, &st) == -1) {
                printf("create dest dir: %s\n", path);
                mkdir(path, 0755);
            }
            line[strcspn(line, "\n")] = 0;
            strcpy(srcPath,cwd);strcat(srcPath,"/");strcat(srcPath,line);
            strcat(path,"/");strcat(path,line);
            rename(srcPath, path);
        }

        char folderITSC4B[1024];
        strcpy(folderITSC4B, cwd);
        strcat(folderITSC4B, "/ITSC4B_");
        c = 0;
        while((fgets(line, sizeof line, fITSC4B) != NULL) && (c < 999999))
        {
            c++;
            substring(line, sub, 22, 8);
            strcpy(path, folderITSC4B);
            strcat(path, sub);
            if (stat(path, &st) == -1) {
                printf("create dest dir: %s\n", path);
                mkdir(path, 0755);
            }
            line[strcspn(line, "\n")] = 0;
            strcpy(srcPath,cwd);strcat(srcPath,"/");strcat(srcPath,line);
            strcat(path,"/");strcat(path,line);
            rename(srcPath, path);
        }
        
        fclose(fITSC5);
        fclose(fITSC4A);
        fclose(fITSC4B);
    } else {
        perror ("");
        return 1;
    }

    cpFileToRTCPath();
    
    printf("Done.\n");
        
    return 0;
}

void substring(char s[], char sub[], int p, int l) {
   int c = 0;
   while (c < l) {
      sub[c] = s[p+c-1];
      c++;
   }
   sub[c] = '\0';
}

void cpFileToRTCPath() {
    struct tm * timeinfo;
    char datef[70];
    time_t now = time(0);
    timeinfo = localtime(&now);
    timeinfo->tm_mday -= 1;
    strftime(datef, sizeof datef, "%Y%m%d", timeinfo);
    printf("Copy CDR from yesterday: %s\n",datef);
    
    int ssleep = 10;
    
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) printf("directory: %s\n", cwd);
    
    char folderRTC4A[1024]; char folderRTC4B[1024]; char folderRTC5[1024];
    strcpy(folderRTC4A, cwd); strcat(folderRTC4A, "/ITSC4A");
    strcpy(folderRTC4B, cwd); strcat(folderRTC4B, "/ITSC4B");
    strcpy(folderRTC5, cwd); strcat(folderRTC5, "/SRC5");
    
    char folderITSC4A[1024];
    strcpy(folderITSC4A, cwd);strcat(folderITSC4A, "/ITSC4A_");strcat(folderITSC4A, datef);
    //uncompressFile(folderITSC4A);sleep(ssleep);
    //strcat(folderITSC4A, "/.");
    //copy(folderITSC4A, folderRTC4A);
    uncompressFileSys(folderITSC4A);
    copySys(folderITSC4A, folderRTC4A);
    
    char folderITSC4B[1024];
    strcpy(folderITSC4B, cwd);strcat(folderITSC4B, "/ITSC4B_");strcat(folderITSC4B, datef);
    //uncompressFile(folderITSC4B);sleep(ssleep);
    //strcat(folderITSC4B, "/.");
    //copy(folderITSC4B, folderRTC4B);
    uncompressFileSys(folderITSC4B);
    copySys(folderITSC4B, folderRTC4B);
    
    char folderITSC5[1024];
    strcpy(folderITSC5, cwd);strcat(folderITSC5, "/ITSC5_");strcat(folderITSC5, datef);
    //uncompressFile(folderITSC5);sleep(ssleep);
    //strcat(folderITSC5, "/.");
    //copy(folderITSC5, folderRTC5);
    uncompressFileSys(folderITSC5);
    copySys(folderITSC5, folderRTC5);
}

void uncompressFile(char *folder) {
    int childExitStatus;
    pid_t pid;
    int status;
    if (!folder) {
        /* handle as you wish */
    }

    pid = fork();

    if (pid == 0) { /* child */
        execl("/bin/uncompress", "/bin/uncompress", "-r", folder, (char *)0);
    }
    else if (pid < 0) {
        /* error - couldn't start process - you decide how to handle */
    }
    else {
        /* parent - wait for child - this has all error handling, you
         * could just call wait() as long as you are only expecting to
         * have one child process at a time.
         */
        pid_t ws = waitpid( pid, &childExitStatus, WNOHANG);
        if (ws == -1)
        { /* error - handle as you wish */
        }

        if( WIFEXITED(childExitStatus)) /* exit code in childExitStatus */
        {
            status = WEXITSTATUS(childExitStatus); /* zero is normal exit */
            /* handle non-zero as you wish */
        }
        else if (WIFSIGNALED(childExitStatus)) /* killed */
        {
        }
        else if (WIFSTOPPED(childExitStatus)) /* stopped */
        {
        }
    }     
}

void copy(char *source, char *dest) {
    int childExitStatus;
    pid_t pid;
    int status;
    if (!source || !dest) {
        /* handle as you wish */
    }

    pid = fork();

    if (pid == 0) { /* child */
        execl("/bin/cp", "/bin/cp", "-R", source, dest, (char *)0);
    }
    else if (pid < 0) {
        /* error - couldn't start process - you decide how to handle */
    }
    else {
        /* parent - wait for child - this has all error handling, you
         * could just call wait() as long as you are only expecting to
         * have one child process at a time.
         */
        pid_t ws = waitpid( pid, &childExitStatus, WNOHANG);
        if (ws == -1)
        { /* error - handle as you wish */
        }

        if( WIFEXITED(childExitStatus)) /* exit code in childExitStatus */
        {
            status = WEXITSTATUS(childExitStatus); /* zero is normal exit */
            /* handle non-zero as you wish */
        }
        else if (WIFSIGNALED(childExitStatus)) /* killed */
        {
        }
        else if (WIFSTOPPED(childExitStatus)) /* stopped */
        {
        }
    }
}

void uncompressFileSys(char *folder) {
    char command[100];
    //strcpy(command, "/bin/uncompress -r ");
    strcpy(command, "/usr/bin/uncompress -r ");
    strcat(command, folder);
    system(command);
}

void copySys(char *source, char *dest) {
    char command[100];
    strcpy(command, "/bin/cp ");
    strcat(command, source);
    strcat(command, "/* ");
    strcat(command, dest);
    system(command);
}
