/* This programming is used to get the arff files from grens of books
 Author : Yingbo Wang
 ID: 349149
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <regex.h>
#include <string.h>
#include <dirent.h>
#define NULL 0
#define LEN sizeof(struct name)
#define L2 sizeof(struct book)
#define MAXWORD 50
#define MAXBUF 48
int nameofperson = 0;
int namenotperson = 0;
char *filename = "words.txt";
char *path;
char *path2;
char *classpath[20];
char *arffname[20];
char *temp[20];
int headflag = 0;
int headflag1 = 0;
int headflag2 = 0;
int appeartimes = 0;
int xj=0;
int yj=0;
char *name[1000][200];
int namesize = 0;
int featuresize=20;
int pathlength=0;
struct name {/*free list for store the new words which has been found*/
    char *name[200];
    int appeartimes;
    int change;
    struct name *next;
};
struct name *head = NULL;
struct name *p = NULL;
struct book {
    char *name[200][200];
     int nametime;
    int appeartimes[200];
    char *file[20];
    char class;
    struct book *next;
};
struct book *head2 = NULL;
struct book *p2 = NULL;
struct book *ps2 = NULL;
/*dictionray for quick search*/
char *dictionary[] = { "The", "They", "He", "She", "We", "Both", "Most", "Of",
        "So", "That", "There", "Some", "All", "When", "Who", "To", "What",
        "This", "Then", "While", "It", " If", "Only", "About", "Every"
                "How", "Last", "Now", "Here", "In", "For", "Do", "Is", "Our",
        "Are", "A", "An", "That", "Its", "His", "These", "Those", "whose",
        "Whom", "Their", "I", "At", "As", "By", "Besides", "Next", "Each",
        "Everyone", "One", "You", "Web", "Project", "Email", "Was", "Upon",
        "January", "February", "March", "April", "May", "June", "July",
        "August", "September", "October", "November", "December", "Not", "On",
        "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
                "Sunday", "df", "df", "df", "df", "df", "df" };
int number = 84;
FILE *f1, *f2, *f3, *f4;

/*delete the space around the words*/
void Trim(char *ptr) {
    char *p, *q;
    if (ptr == NULL)
        return;
    for (p = ptr; *p == ' ' || *p == '\t' || *p == '\n'; ++p)
        ;
    if (*p == 0) {
        *ptr = 0;
        return;
    }
    for (q = ptr; *p; ++p, ++q) {
        *q = *p;
    }
    for (p = q - 1; *p == ' ' || *p == '\t' || *p == '\n'; --p)
        ;
    *(++p) = '\0';
}
showname() {
    p = head;
    do {
        printf("%s %d\n", p->name, p->appeartimes);
        p = p->next;
    } while (p != NULL);
}
showname1() {
    p2 = head2;
    do {
        printf("%s %d\n", p2->name, p2->appeartimes);
        p2 = p2->next;
    } while (p2 != NULL);
}

/*flag 0 mean that no such name, 1 mean there has name*/
int searchname(char * newname) {
    int flag = 0;
    p = head;
    if (headflag == 0) {
        return flag;
    }
    do {
        if ((strcmp(p->name, newname) == 0)) {
            p->appeartimes = p->appeartimes + 1;
            flag = 1;
        }
        p = p->next;
    } while (p != NULL);
    return flag;
}

/* insert a new word*/
insert(char *newname) {
    struct name *ps;
    p = head;
    if (headflag == 0) {
        p = (struct name *) malloc(LEN);
        strcpy(p->name, newname);
        p->appeartimes = 1;
        head = p;
        p->next = NULL;
        headflag = 1;
        nameofperson = 1;
    } else {
        while ((strcmp(newname, p->name) > 0) && (p->next != NULL)) {
            p = p->next;
        }
        ps = (struct name*) malloc(sizeof(struct name));
        strcpy(ps->name, newname);
        ps->appeartimes = 1;
        ps->next = p->next;
        p->next = ps;
        nameofperson++;
    }
}


int check(char *newname) {
    int i = 0;
    for (i = 0; i < number; i++) {
        if (strcmp(newname, dictionary[i]) == 0) {
            namenotperson++;
            return 0;
        }
    }
    return 1;
}
/* check whether the newname is on the dictionary 1 for no such words 0 for has this words*/
int checkinfile(char *newname) {

    char *words[MAXWORD];
    char *buf[MAXBUF];
    char *lastwords[MAXWORD];
    int flag = 0;
    int i = 0;
    int n = 0;
    if (check(newname) == 0)
        return -1;
    if (f1)
        fclose(f1);
    f1 = fopen(filename, "r");
    while (!feof(f1) && (flag == 0)) {
        for (i = 0; i < MAXWORD; i++)
            words[i] = '\0';
        for (i = 0; i < MAXWORD; i++)
            lastwords[i] = '\0';
        fscanf(f1, "%s", words);
        Trim(words);
        if (strcasecmp(words, newname) == 0) {
            namenotperson++;
            return 0;
        }

        if (strcmp(lastwords, words) == 0)
            flag = 1;
        strcpy(lastwords, words);
        n++;
    }
    fclose(f1);
    return 1;
}
char checkclassinfile(char *filename) {

    char *words1[MAXWORD];
    char *buf1[MAXBUF];
    char *lastwords1[MAXWORD];
    int flag1 = 0;
    int i = 0;
    int n = 0;
         char *word[1];
     int length1=0;
    if (f3)
        fclose(f3);
    f3 = fopen(classpath, "r");
    while (!feof(f3) && (flag1 == 0)) {
        for (i = 0; i < MAXWORD; i++)
            words1[i] = '\0';
        for (i = 0; i < MAXWORD; i++)
            lastwords1[i] = '\0';
        fscanf(f3, "%s", words1);
        Trim(words1);
         Trim(filename);
        if (strstr(words1, filename) != NULL) {
            if(strstr(words1,"A")!=NULL){
		return 'A';
	     } else if(strstr(words1,"B")!=NULL){
		return 'B';
	     } else if(strstr(words1,"C")!=NULL){
		return 'C';
	     } else if(strstr(words1,"D")!=NULL){
		return 'D';
	     } else if(strstr(words1,"E")!=NULL){
		return 'E';
	     } else if(strstr(words1,"F")!=NULL){
		return 'F';
	     } else if(strstr(words1,"G")!=NULL){
		return 'G';
	     } else if(strstr(words1,"H")!=NULL){
		return 'H';
	     } else if(strstr(words1,"I")!=NULL){
		return 'I';
	     } else if(strstr(words1,"J")!=NULL){
		return 'J';
	     } else if(strstr(words1,"K")!=NULL){
		return 'K';
	     } else if(strstr(words1,"L")!=NULL){
		return 'L';
	     } else if(strstr(words1,"M")!=NULL){
		return 'M';
	     } else if(strstr(words1,"N")!=NULL){
		return 'N';
	     } else if(strstr(words1,"O")!=NULL){
		return 'O';
	     } else if(strstr(words1,"P")!=NULL){
		return 'P';
	     }
        }
        if (strcmp(lastwords1, words1) == 0)
            flag1 = 1;
        strcpy(lastwords1, words1);
        n++;
    }
 
    fclose(f3);

    return 'Z';
}
/* get the substring*/
static char* substr(const char*str, unsigned start, unsigned end) {
    unsigned n = end - start;
    static char stbuf[256];
    strncpy(stbuf, str + start, n);
    stbuf[n] = 0;
    return stbuf;
}

int main(int argc, char** argv) {
    int flag; /* flag is keep the default reguler expression*/
    char * pattern; /* pattern is used keep the reguler expression */
    int x, z, lno = 0, cflags = 0; /*lno is line number*/
    char ebuf[256], lbuf[512]; /* lbuf is the char array keep the char in a line.*/
    regex_t reg; /* which keep the reguler expression */
    regmatch_t pm[10];
    const size_t nmatch = 10;
    char *newname[200];
     int di;
    /* build the regular expression*/
    if (argc < 2) {
        printf("you need to use the correct command funcion");
    }
    head = NULL;
    headflag = 0;
    pattern = "[ ][A-Za-z0-9]*[ ]";
    z = regcomp(&reg, pattern, cflags);
    if (z != 0) {
        regerror(z, &reg, ebuf, sizeof(ebuf));
        fprintf(stderr, " %s: pattern '%s' \n", ebuf, pattern);
        return 1;
    }
    if (argc == 3) {
        path = argv[1];
        path2 = argv[2];
         pathlength=strlen(path);
                   strncpy(classpath,path+2,pathlength-2);
                   classpath[pathlength-2]='\0';
           strcat(classpath,".class");
          } else {
        printf("input vaild file directory");
        return 1;
    }
      
    struct dirent* ent = NULL;
    DIR *pDir;
    char dir[512];
    char *files[50];
    int id = 0;
    struct stat statbuf;
    if ((pDir = opendir(path)) == NULL) {
        fprintf(stderr, "Cannot open directory:%s\n", path);
        return;
    }
    while ((ent = readdir(pDir)) != NULL) {
        snprintf(dir, 512, "%s/%s", path, ent->d_name);
        lstat(dir, &statbuf);

        if (S_ISDIR(statbuf.st_mode)) {
        } else {
               
            for (id = 0; id < 50; id++) {
                files[id] = "\0";
            }
              
            strcpy(files, path);
            strcat(files, "\/");
            strcat(files, ent->d_name);
            di = 0;
              lno=0;
            printf("start %s", files);
            f2 = fopen(files, "r");
            while (fgets(lbuf, sizeof(lbuf), f2)) {
                ++lno;
                if ((z = strlen(lbuf)) > 0 && lbuf[z - 1] == '\n')
                    lbuf[z - 1] = 0;
                z = regexec(&reg, lbuf, nmatch, pm, 0);
                if (z == REG_NOMATCH)
                    continue;
                else if (z != 0) {
                    regerror(z, &reg, ebuf, sizeof(ebuf));
                    fprintf(stderr, " %s: regcom('%s') \n", ebuf, lbuf);
                    return 2;
                }

                for (x = 0; x < nmatch && pm[x].rm_so != -1; ++x) {
                    strcpy(newname, substr(lbuf, pm[x].rm_so, pm[x].rm_eo));
                    Trim(newname);
                    if (checkinfile(newname) == 1)
                        if (check(newname) == 1) {
                            if (headflag == 0) {
                                insert(newname);
                            } else if (searchname(newname) == 0) {
                                insert(newname);
                            }

                        }
                }
            }
            
            p = head;
            p2 = head2;
            if (headflag2 == 0) {
                p2 = (struct book *) malloc(L2);
                strcpy(p2->file, ent->d_name);
                appeartimes = 0;
                while ((p->next != NULL) && (appeartimes < 200)) {
                    if (p->appeartimes >= featuresize) {
                        strcpy(p2->name[appeartimes], p->name);
                            p2->nametime++;
                        p2->appeartimes[appeartimes] = p->appeartimes;
                            int flagx=0;
                        for (xj = 0; xj < namesize; xj++) {
                            if (strcmp(name[xj],p2->name[appeartimes])==0)
                                  flagx=1;
                        }
                            if((flagx==0) &&(namesize<1000)){
                               strcpy(name[namesize],p2->name[appeartimes]);
                               namesize++;
                            }
                            appeartimes++;
                    }
                    p = p->next;

                }
                   int sd;
    
                p2->class=checkclassinfile(ent->d_name);
                   printf(" This is type %c\n",p2->class);


                head2 = p2;
                p2->next = NULL;
                headflag2 = 1;
            } else {


                while ((strcmp(ent->d_name, p2->file) > 0)
                        && (p2->next != NULL)) {
                    p2 = p2->next;
                }
                ps2 = (struct book*) malloc(L2);
                strcpy(p2->file, ent->d_name);
                appeartimes = 0;
                while ((p->next != NULL) && (appeartimes < 200)) {
                    if (p->appeartimes >= featuresize) {
                        strcpy(p2->name[appeartimes], p->name);
                            p2->nametime++;
                        p2->appeartimes[appeartimes] = p->appeartimes;
                            int flagx=0;
                            for (xj = 0; xj < namesize; xj++) {
                               if (strcmp(name[xj],p2->name[appeartimes])==0)
                                    flagx=1;
                               }
                            if((flagx==0) &&(namesize<1000)){
                               strcpy(name[namesize],p2->name[appeartimes]);
                               namesize++;
                            }
                        appeartimes++;
                    }
                    p = p->next;
                }
                   
                p2->class=checkclassinfile(ent->d_name);
                   printf(" This is type %c\n",p2->class);
                ps2->next = p2->next;
                p2->next = ps2;
            }
        }
          printf ("  build features success with %d features all\n",namesize);
          printf("  finished:%s\n", ent->d_name);
	
    }
 
     printf("  building arff files for weka\n");
      
     strncpy(arffname,path+2,pathlength-2);

     strcpy(temp,arffname);
     arffname[pathlength-2]='\0';

     strcat(arffname,".arff");
      
      f4 = fopen(arffname, "w");
      
      fprintf(f4,"@relation %s\n",temp);
      fprintf(f4,"@attribute id string\n");
      for(xj=0;xj<namesize;xj++){
      fprintf(f4,"@attribute %s real\n",name[xj]);
      }
      fprintf(f4,"@attribute class string\n");
      fprintf(f4,"@data\n");
      p2=head2;
      int score=0;
      do{
         fprintf(f4,"%s",p2->file);
         
         for(xj=0;xj<namesize;xj++){
             score=0;
             for(yj=0;yj<p2->nametime;yj++){
                if(strcmp(p2->name[yj],name[xj])==0){
                      score=p2->appeartimes[yj];
                }
             }
             fprintf(f4,",%d",score);
          }
          
          fprintf(f4,",%c\n",p2->class);
          p2=p2->next;
      }while(p2->next!=NULL);
      closedir(pDir);
     printf("arff files has been make success");
     fclose(f4);
     namesize=0;
     head2=NULL;
     pathlength=strlen(path2);
    strncpy(classpath,path2+2,pathlength-2);
     classpath[pathlength-2]='\0';
    strcat(classpath,".class");
    if ((pDir = opendir(path2)) == NULL) {
        fprintf(stderr, "Cannot open directory:%s\n", path2);
        return;
    }
    while ((ent = readdir(pDir)) != NULL) {
        snprintf(dir, 512, "%s/%s", path2, ent->d_name);
        lstat(dir, &statbuf);

        if (S_ISDIR(statbuf.st_mode)) {
        } else {

            for (id = 0; id < 50; id++) {
                files[id] = "\0";
            }
              
            strcpy(files, path2);
            strcat(files, "\/");
            strcat(files, ent->d_name);
            di = 0;
              lno=0;
            printf("start %s", files);
            f2 = fopen(files, "r");
            while (fgets(lbuf, sizeof(lbuf), f2)) {
                ++lno;
                if ((z = strlen(lbuf)) > 0 && lbuf[z - 1] == '\n')
                    lbuf[z - 1] = 0;
                z = regexec(&reg, lbuf, nmatch, pm, 0);
                if (z == REG_NOMATCH)
                    continue;
                else if (z != 0) {
                    regerror(z, &reg, ebuf, sizeof(ebuf));
                    fprintf(stderr, " %s: regcom('%s') \n", ebuf, lbuf);
                    return 2;
                }

                for (x = 0; x < nmatch && pm[x].rm_so != -1; ++x) {
                    strcpy(newname, substr(lbuf, pm[x].rm_so, pm[x].rm_eo));
                    Trim(newname);
                    if (checkinfile(newname) == 1)
                        if (check(newname) == 1) {
                            if (headflag == 0) {
                                insert(newname);
                            } else if (searchname(newname) == 0) {
                                insert(newname);
                            }

                        }
                }
            }
            
            p = head;
            p2 = head2;
            if (headflag2 == 0) {
                p2 = (struct book *) malloc(L2);
                strcpy(p2->file, ent->d_name);
                appeartimes = 0;
                while ((p->next != NULL) && (appeartimes < 200)) {
                    if (p->appeartimes >= featuresize) {
                        strcpy(p2->name[appeartimes], p->name);
                            p2->nametime++;
                        p2->appeartimes[appeartimes] = p->appeartimes;
                            int flagx=0;
                        for (xj = 0; xj < namesize; xj++) {
                            if (strcmp(name[xj],p2->name[appeartimes])==0)
                                  flagx=1;
                        }
                            if((flagx==0) &&(namesize<1000)){
                               strcpy(name[namesize],p2->name[appeartimes]);
                               namesize++;
                            }
                            appeartimes++;
                    }
                    p = p->next;

                }
                   
                p2->class=checkclassinfile(ent->d_name);
                   printf(" This is type %c\n",p2->class);
                head2 = p2;
                p2->next = NULL;
                headflag2 = 1;
            } else {
                while ((strcmp(ent->d_name, p2->file) > 0)
                        && (p2->next != NULL)) {
                    p2 = p2->next;
                }
                ps2 = (struct book*) malloc(L2);
                strcpy(p2->file, ent->d_name);
                appeartimes = 0;
                while ((p->next != NULL) && (appeartimes < 200)) {
                    if (p->appeartimes >= featuresize) {
                        strcpy(p2->name[appeartimes], p->name);
                            p2->nametime++;
                        p2->appeartimes[appeartimes] = p->appeartimes;
                            int flagx=0;
                            for (xj = 0; xj < namesize; xj++) {
                               if (strcmp(name[xj],p2->name[appeartimes])==0)
                                    flagx=1;
                               }
                            if((flagx==0) &&(namesize<1000)){
                               strcpy(name[namesize],p2->name[appeartimes]);
                               namesize++;
                            }
                        appeartimes++;
                    }
                    p = p->next;
                }
                   
                p2->class=checkclassinfile(ent->d_name);
                   printf(" This is type %c\n",p2->class);
                ps2->next = p2->next;
                p2->next = ps2;
            }
        }
          printf ("  build features success with %d features all\n",namesize);
          printf("  finished:%s\n", ent->d_name);
    }
     printf("  building arff files for weka\n");
     strncpy(arffname,path2+2,pathlength-2);
     strcpy(temp,arffname);
     arffname[pathlength-2]='\0';
     strcat(arffname,".arff");
      f4 = fopen(arffname, "w");
      fprintf(f4,"@relation %s\n",temp);
      fprintf(f4,"@attribute id string\n");
      for(xj=0;xj<namesize;xj++){
      fprintf(f4,"@attribute %s real\n",name[xj]);
      }
      fprintf(f4,"@attribute class string\n");
      fprintf(f4,"@data\n");
      p2=head2;
      score=0;
      do{
         fprintf(f4,"%s",p2->file);
         
         for(xj=0;xj<namesize;xj++){
             score=0;
             for(yj=0;yj<p2->nametime;yj++){
                if(strcmp(p2->name[yj],name[xj])==0){
                      score=p2->appeartimes[yj];
                }
             }
             fprintf(f4,",%d",score);
          }
          fprintf(f4,",%c\n",p2->class);
          p2=p2->next;
      }while(p2->next!=NULL);
      closedir(pDir);


    regfree(&reg);

    return 0;
}
