#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

#define SIDEBARSIZE 57 
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))

int amountOfFiles;
int termLines;
int termColumns;
char *allfiles[100];

struct terminal {
    int lines;
    int columns;
};

struct terminal* initTerm(int lines, int columns) {
    struct terminal term;
    term.lines = lines;
    term.columns = columns;
    
    struct terminal *terminal = &term;
    return terminal;
}

void clearTerminal() {
    system("clear");
}


void renderSideBar(int lines, int columns) {
    clearTerminal();
    for(int yIndex = 0; yIndex < lines; yIndex++) {
        printf("\n");
        for(int xIndex = 0; xIndex < columns; xIndex++) {
            if(xIndex == SIDEBARSIZE)
              printf("|");
            else printf(" ");
        }
    }
}

void openSelectedFile(char *filen)
{
    int fsize = strlen(filen); 
    char filename[fsize];
    for(int index = 0; index < fsize; index++) {
        filename[index] = filen[index];  
    }
    gotoxy(59, 0);
    FILE *fptr;
    fptr = fopen(filename, "r");
    if (fptr == NULL)
    {
        printf("Cannot open file \n");
        return;
    }
    char s;
    while((s=fgetc(fptr))!=EOF) {
        printf("%c",s);
    }
    fclose(fptr);
}

void searchForFile(char* files[]) {
    char input[100];
    scanf("%s", input);
    char *inputstr = &input[0];
    gotoxy(0, 1);
    for(int index = 0; index < amountOfFiles; index++) {
        char *file = strstr(files[index], inputstr);
        if(file) openSelectedFile(file);
    }
}


void openDir() {
    char* files[100]; 
    gotoxy(1,0);
    DIR *d;
    int i = 0;
    struct dirent *dir;
    d = opendir(".");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            char *ret = strstr(dir->d_name, ".kb");
            if(ret) {
                printf("%s\n", dir->d_name);
                files[i] = dir->d_name;
                i++;
                amountOfFiles++;
            }
        }
        closedir(d);
    }
    for(int index = 0; index < amountOfFiles; index++) {
        allfiles[index] = files[index];
    }
}



int main (int argc, char **argv)
{
    struct winsize w;
    int isOpen = 1;
    ioctl(0, TIOCGWINSZ, &w);
    struct terminal *term = initTerm(w.ws_row, w.ws_col);
    renderSideBar(term->lines, term->columns);
    termLines = term->lines;
    termColumns = term->columns;
    openDir();
    searchForFile(allfiles);
    return 0;
}
