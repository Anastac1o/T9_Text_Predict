#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#define MAX_SIZE 100
#define MAX_WORDSZ 20
#define MAX_MSGSZ 1000
typedef struct node node;
typedef struct list list;
typedef struct hash_table hash_table;
int wordToNum(char* data);

struct node{
    char data[MAX_WORDSZ];
    int key;
    struct node* next;
};

struct list{
    struct node* head;
    struct node* tail;
};

struct hash_table{
    int size;
    struct list* items[MAX_SIZE];
};


node* new_node(char str[MAX_WORDSZ])
{
    struct node* N = (node*) malloc(sizeof(node));
    N->key = wordToNum(str);
    N->next = NULL;
    int i = 0;
    do {
        N->data[i] = str[i];
        i++;   
    } while(str[i] != '\0');

    return N;
}

list* new_list(struct node* N) 
{
    struct list* L = (list*) malloc(sizeof(list));
    L->head = N;
    L->tail = N;
}
void add(struct list* L, struct node* N) 
{
    if(L->head == NULL) {
        L->head = N;
        L->tail = N;
    } else {
        L->tail->next = N;
        L->tail = N;
    }
}

hash_table* new_table(int size_t)
{
    struct hash_table* table = (hash_table*) malloc(sizeof(hash_table));
    table->size = size_t;
    for(int i=0; i < MAX_SIZE; i++) {
        table->items[i] = new_list(NULL);
    }
}

int hash(int key, int t_size) {
    int res = key % t_size;
    if(res < 0) {
        res = 0 - res;
    }
    if(res > MAX_SIZE) {
        res = hash(res, t_size);
    }
    
    return res;
}

void insert(hash_table* table, node* item)
{
    int index = hash(item->key, table->size);
    add(table->items[index], item);
}


void t9_predict(struct hash_table* table)
{
    char mensagem[MAX_MSGSZ];
    bool running = true;
    int num;
    char input[MAX_WORDSZ];
    char pick[2];

    while(running)
    {
        puts("** Escreva a sua mensagem **");
        scanf("%d", &num);

        if(num == 1) {

            printf("\nMensagem: %s", mensagem);

        }
        else if(num == 0) {

            puts("Deseja sair da aplicação (s/n)?");
            scanf("%s", pick);

            if(pick[0] == 's') {
                running = false;
            }

        }
        else {

            int index = hash(num, table->size);
            struct node* item = table->items[index]->head;

            while(item != NULL) {
            
                if(item->key == num) {
                    printf("Sugestao: %s , aceita (s/n)? ", item->data);
                    scanf("%s", pick);
            
                    if(pick[0] == 's') {
                        strcat(mensagem, item->data);
                        strcat(mensagem, " ");  
                        break;
                    }else {
                        item = item->next;
                    }
                }else {
                    item = item->next;
                }
            }

            if(item == NULL) {
                puts("\nNão existem mais sugestões; introduza a palavra no teclado.");
                scanf("%s", input);
                struct node* new_item = new_node(input);
                insert(table, new_item);
                strcat(mensagem, input);
                strcat(mensagem, " ");
            }

        }

    }

}

int wordToNum(char *s)
{
    int num = 0;
    int t = 0;
    
    for(int i=0; s[i] != 0; i++){
        int ok = true;
        switch (s[i])
        {
        case 'a':
        case 'b':
        case 'c':
        case 'A':
        case 'B':
        case 'C':
            t = 2;            
            break;
        case 'd':
        case 'e':
        case 'f':
        case 'D':
        case 'E':
        case 'F':
        //case 'é':
        //case 'É':
            t = 3;
            break;
        case 'g':
        case 'h':
        case 'i':
        case 'G':
        case 'H':
        case 'I':
            t = 4;
            break;
        case 'j':
        case 'k':
        case 'l':
        case 'J':
        case 'K':
        case 'L':
            t = 5;
            break;
        case 'm':
        case 'n':
        case 'o':
        case 'M':
        case 'N':
        case 'O':
            t = 6;
            break;
        case 'p':
        case 'q':
        case 'r':
        case 's':
        case 'P':
        case 'Q':
        case 'R':
        case 'S':
            t = 7;
            break;
        case 't':
        case 'u':
        case 'v':
        case 'T':
        case 'U':
        case 'V':
            t = 8;
            break;
        case 'w':
        case 'x':
        case 'y':
        case 'z':
        case 'W':
        case 'X':
        case 'Y':
        case 'Z':
            t = 9;
            break;
        default:
            ok = false;
            break;
        }

        if(ok){ num = num * 10 + t; }
    }
    return num;
}

void removechar( char str[], char t )
{
    int i,j;
    i = 0;
    while(i<strlen(str))
    {
        if (str[i]==t) 
        { 
            for (j=i; j<strlen(str); j++)
                str[j]=str[j+1];   
        } else i++;
    }
}
   

int main( int argc, char *argv[] ) 
{
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    struct hash_table* table = new_table(MAX_SIZE);

    fp = fopen(argv[1], "r");
    //fp = fopen("small-dict.txt", "r");
    
    if(fp == NULL) {
        puts("Ficheiro não existe.");
        exit(EXIT_FAILURE);
    }

    int w_count = 0;
    clock_t begin = clock();

    while ((read = getline(&line, &len, fp)) != -1) {
        char word[MAX_WORDSZ];
        strcpy(word, line);
        removechar(word, '\n');
        struct node* item = new_node(word);
        insert(table, item);
        //printf("\n %s %d %d - inserido na hashtable no indice - %d", line, i,wordToNum(line),hash(item->key,MAX_SIZE));
        w_count++;
    }
    fclose(fp);

    clock_t end = clock();
    double time_spent = (double) (end - begin) / CLOCKS_PER_SEC;
    printf("\nO programa demorou %lf segundos para colocar %d palavras na tabela. \n", time_spent, w_count);
    puts("\n#########################\n# 1 - Mostrar mensagem  #\n# 2 - a b c á à â ã ç   #\n# 3 - d e f é ê         #\n# 4 - g h i í           #\n# 5 - j k l             #\n# 6 - m n o ó ô õ       #\n# 7 - p q r s           #\n# 8 -  t u v ú          #\n# 9 - w x y z           #\n# 0 - Sair              #\n#########################\n\nAutores:\n - Duarte Anastácio nº40090\n - Beatriz Girão nº 45491\n");

    t9_predict(table);   



    return 0;
}