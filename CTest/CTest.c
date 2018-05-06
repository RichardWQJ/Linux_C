#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <assert.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

void accessTest(const char *filename,int mode)
{
    int result = access(filename, mode);
    char strPrint[50] = { 0 };
    switch (mode)
    {
    case R_OK:
        sprintf(strPrint, "File is %sReadable!", result == 0 ? "" : "Not ");
        break;
    case W_OK:
        sprintf(strPrint, "File is %sWritable!", result == 0 ? "" : "Not ");
        break;
    case X_OK:
        sprintf(strPrint, "File is %sExacutable!", result == 0 ? "" : "Not ");
        break;
    case F_OK:
        sprintf(strPrint, "File is %sExist!", result == 0 ? "" : "Not ");
    	break;
    default:
        assert(0);
        break;
    }
    printf("%s\n", strPrint);
}

char* getwdTest(char *buff)
{
    return getwd(buff);
}

void readDir(DIR *dir, const char *path)
{
    if (dir == NULL) {
        return;
    }

    char buff[100] = { 0 };
    struct dirent *ent;
    while ((ent = readdir(dir)) != NULL) {
        char type = ent->d_type;
        if (type == DT_DIR) {
            //如果是目录，就打开下一级目录
            if (strcmp(ent->d_name, ".") == 0 ||
                strcmp(ent->d_name, "..") == 0) {
                continue;
            }

            if (strlen(path) + strlen(ent->d_name) + 2 > sizeof(buff)) {
                printf("The buffer is full!\n");
                continue;
            }
            sprintf(buff, "%s%c%s", path, "/", ent->d_name);

            printf("Dirname:%s\n", buff);
            DIR *ndir = opendir(buff);
            readDir(ndir, buff);
        }
        else {
            printf("Filename:%s\n", ent->d_name);
        }
    }
}

int main(int argc, char *argv[])
{
	char sz[] = "/home/richard/get.html";	/* Hover mouse over "sz" while debugging to see its contents */
	printf("Filename:%s\n", sz);
    accessTest(sz, R_OK);
    accessTest(sz, W_OK);
    accessTest(sz, X_OK);
    accessTest(sz, F_OK);

    printf("Current dir name:%s\n", get_current_dir_name());
    
    char path[100] = { 0 };
    printf("Current path:%s;\t%s;\n", getwdTest(path), path);

    char samePath[100] = { 0 };
    printf("The same path is:%s;\t%s;\n", getcwd(samePath, 100), samePath); //Return NULL if the directory couldn't be determined or size was too small.

    struct stat buf;
    //判断是否是目录
    int res = stat(path, &buf);
    if (res < 0) {
        printf("Get path stat error!\n");
        exit(1);
    }

    if (!S_ISDIR(buf.st_mode)) {
        printf("The path is not a directory!!\n");
        exit(1);
    }
    DIR *dir = opendir(path);
    readDir(dir, path);

    getchar();
	return 0;
}