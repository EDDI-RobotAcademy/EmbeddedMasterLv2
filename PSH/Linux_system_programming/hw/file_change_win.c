#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int write_to_file(void)
{
    FILE *fp;
    fp = fopen("data", "w");
    if (fp == NULL) {
        perror("fopen error\n");
        return -1;
    }
    printf("after fopen(). offset=%ld\n", ftell(fp));
    fputs("I am gone", fp);
    printf("before fclose(). offset=%ld\n", ftell(fp));
    printf("---------------------------------\n");
    fclose(fp);
    return 0;
}
int read_from_file(void)
{
    FILE *fp;
    char buf[1024];
    char write_buf[1024];
    char tmp_buf[1024];
    char *tok;
    int argc = 0;
    int cmp = 0;
    // char *argv[1024];
    char argv[1024][1024];
    char *delim = " ";

    fp = fopen("data", "r+");
    if (fp == NULL) {
        perror("fopen error\n");
        return -1;
    }

    printf("after fopen(). offset=%ld\n", ftell(fp));

    memset(buf, 0, sizeof(buf));
    fgets(buf, sizeof(buf), fp);

    printf("after fgets(). offset=%ld\n", ftell(fp));

    for (tok = strtok(buf, " "); tok != NULL; tok = strtok(NULL, " "))
    {
        // printf("%s\n",tok);
        strcpy(argv[argc++], tok);
        // argv[argc++] = (char *) tok;
        // argv[argc++] = *tok;
    }

    printf("%s\n", argv[0]);
    printf("%s\n", argv[1]);
    printf("%s\n", argv[2]);

    for (cmp = 0; cmp < argc; cmp++){
        if(strcmp(argv[cmp], "am") == 0) {
            strcpy(argv[cmp], "are");
            // printf("%s\n",argv[cmp]);
        }
    }

    printf("%s\n", argv[0]);
    printf("%s\n", argv[1]);
    printf("%s\n", argv[2]);

    sprintf(write_buf, "%s %s %s", argv[0], argv[1], argv[2]);

    fseek(fp,0,SEEK_SET);
    fputs(write_buf, fp);

    // printf("argv[%d] = %s",argc, argv[argc]);
    // fseek(fp,0,SEEK_SET);
    // printf("after fseek(). offset=%ld\n", ftell(fp));
    fclose(fp);
    // printf("%s\n", buf);
    return 0;
}
int main(int argc, char**argv)
{
    if(write_to_file()){
        perror("write to file error\n");
        return -1;
    }
    if(read_from_file()){
        perror("read to file error\n");
        return -1;
    }
    
    return 0;
}
