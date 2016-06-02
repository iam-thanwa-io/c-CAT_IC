#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <time.h>


void substring(char s[], char sub[], int p, int l);

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

