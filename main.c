#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct email{
    int id;
    char poluchatel[50];
    char copy_to[50];
    char podatel[50];
    char title[100];
    char content[500];
    int day;
    int month;
    int year;
    int hour;
    int min;
    int sec;
};
typedef struct email email;


struct List
{
    email email;
    struct List* next;
};
typedef struct List List;



List *addNewEmail()
{
    List *new_email = NULL;
    new_email = (List*)malloc(sizeof(List));
    if(new_email==NULL)
    {
        perror("Not enough memory for list.");
        exit(1);
    }

    printf("Enter your Email: \n");
    printf("Enter id of email: ");
    scanf("%d", &new_email->email.id);

    printf("Enter poluchatel of email: ");
    fflush(stdin);
    if(fgets(new_email->email.poluchatel,50,stdin)!=NULL){
        new_email->email.poluchatel[strlen(new_email->email.poluchatel)-1]='\0';
    }

    printf("Enter copy_to of email: ");
    fflush(stdin);
    if(fgets(new_email->email.copy_to,50,stdin)!=NULL){
        new_email->email.copy_to[strlen(new_email->email.copy_to)-1]='\0';
    }

    printf("Enter podatel of email: ");
    fflush(stdin);
    if(fgets(new_email->email.podatel,50,stdin)!=NULL){
        new_email->email.podatel[strlen(new_email->email.podatel)-1]='\0';
    }

    printf("Enter title of email: ");
    fflush(stdin);
    if(fgets(new_email->email.title,100,stdin)!=NULL){
        new_email->email.title[strlen(new_email->email.title)-1]='\0';
    }

    printf("Enter content of email: ");
    fflush(stdin);
    if(fgets(new_email->email.content,500,stdin)!=NULL){
        new_email->email.content[strlen(new_email->email.content)-1]='\0';
    }

    printf("Enter date of email:\n ");
    printf("FORMAT: DD.MM.YYYY HH:MM:SS.  ");
    scanf("%d", &new_email->email.day);
    printf(".");
    scanf("%d" ,&new_email->email.month);
    printf(".");
    scanf("%d" ,&new_email->email.year);
    printf("\t");
    scanf("%d" ,&new_email->email.hour);
    printf(":");
    scanf("%d" ,&new_email->email.min);
    printf(":");
    scanf("%d" ,&new_email->email.sec);

    new_email->next = NULL;

    return new_email;

}

int BadWord(char *word)
{
    FILE* fp;
        if((fp = fopen("bad_words.txt", "r"))==NULL)
        {
        perror("Problem with bad_words.txt.\n");
        exit(5);
       }
    char bad_words_from_file[50];
    while(fscanf(fp,"%s",bad_words_from_file) != EOF)
        {
            if(strcmp(word,bad_words_from_file) == 0)
                {
                    fclose(fp);
                    return 1;
                }
        }

    fclose(fp);
        return 0;
}


List *delete_spam(List *Spam)
{
    FILE *fin;
    if((fin=fopen("Gmail.bin","rb")) == NULL)
    {
        perror("Cannot open a bin file for read.");
        exit(1);
    }

    List *root = NULL;
    root = (List*)malloc(sizeof(List));
    if(root == NULL)
    {
        perror("Memory error (root).");
        exit(4);
    }
    while(fread(&root->email,sizeof(email),1,fin)==1){}

    if(root == NULL)
        {
            printf("No emails in file.\n");
            return root;
        }

    List* temp = root;
    List* prev = root;
    for(; temp != NULL; prev=temp, temp=temp->next)
        {
            if(temp == Spam)
                {
                    if(root == Spam){
                    root = root->next;
                    free(Spam);
                    fclose(fin);
                    return root;
                    }

                    prev->next = temp->next;
                    free(temp);
                    temp = prev;
                    fclose(fin);
                    return temp;
                }

        }
        fclose(fin);
        return root;
    }




int main()
{
    int options = 0;
    while(options!=5)
    {
    printf(" 1 -> If you have to add a email.\n 2 -> If you have to visit all emails\n 3 -> if you should s visit a email.\n 4 -> Delete all spam emails.\n 5-> End of the program.\n");
    scanf("%d", &options);

    if(options == 1){
    FILE *fp;
    if((fp=fopen("Gmail.bin","ab+"))==NULL){
        perror("Cannot open a bin file for add.");
        exit(2);
    }

    List *root=NULL;

    root=addNewEmail();
    if(fwrite(root,sizeof(email),1,fp)!=1){
        perror("Write error");
        exit(3);
    }

    fclose(fp);

    } else if(options == 2)
        {
            FILE *fin;
            if((fin=fopen("Gmail.bin","rb"))==NULL){
            perror("Cannot open a bin file for read.");
            exit(3);
            }

            List *all_emails = NULL;
            all_emails = (List*)malloc(sizeof(List));
            if(all_emails == NULL)
                {
                    perror("Memory error (all_emails).");
                    exit(4);
                }

            while(fread(&all_emails->email,sizeof(email),1,fin)==1)
            {
                printf("ID: %d\nPodatel: %s\nTitle: %s\nDate: %02d.%02d.%d\t%02d:%02d:%02d.\n\n", all_emails->email.id, all_emails->email.podatel, all_emails->email.title, all_emails->email.day, all_emails->email.month, all_emails->email.year, all_emails->email.hour, all_emails->email.min, all_emails->email.sec);
            }
            fclose(fin);
        }else if(options == 3)
            {
                int my_id;
                int search = 0;
                printf("ID you are looking for: ");
                scanf("%d", &my_id);

                FILE *fs;
                if((fs=fopen("Gmail.bin","rb"))==NULL){
                perror("Cannot open a bin file for search.");
                exit(5);
                }

                List *search_email = NULL;
                search_email = (List*)malloc(sizeof(List));

                while(fread(&search_email->email,sizeof(email),1,fs) == 1){
                if(my_id == search_email->email.id)
                    {
                        printf("\nContent: %s\n", search_email->email.content);
                        search++;
                    }
                }
                if(search==0)
                    {
                        printf("\nNo email with this id.\n");
                    }
                fclose(fs);
            }else if(options == 4)
                {
                    FILE *fsp;
                    if((fsp=fopen("Gmail.bin","rb"))==NULL)
                    {
                        perror("Cannot open a bin file for read.");
                        exit(3);
                    }

                    List *spam_emails = NULL;
                    spam_emails = (List*)malloc(sizeof(List));
                    if(spam_emails == NULL)
                        {
                            perror("Memory error (spam_emails).");
                            exit(4);
                        }

                    while(fread(&spam_emails->email,sizeof(email),1,fsp)==1)
                        {
                                    int all_words_count = 0;
                                    int bad_words_count = 0;
                                    int test;
                                    char *word = strtok(spam_emails->email.content," ");
                                    while(word != NULL)
                                    {
                                        test = BadWord(word);
                                        if(test == 1)
                                        {
                                            all_words_count++;
                                            bad_words_count++;
                                        }else if(test == 0)
                                        {
                                            all_words_count++;
                                        }
                                        word = strtok(NULL," ");
                                    }

                                        float result = (float)bad_words_count/all_words_count;
                                        if(result > 1/5)
                                        {
                                            delete_spam(spam_emails);
                                            printf("Spam email are deleted.\n");
                                        }else
                                            {
                                               printf("Email is not spam.\n");
                                            }

                                }

                            fclose(fsp);
                            }
                    }

    return 0;
}



