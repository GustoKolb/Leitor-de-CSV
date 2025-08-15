#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX 1024
#define NUMBER 1
#define STRING 0
#define DIGTLINES 6 
#define COLUMNS 8
#define VALID 1
#define INVALID 0

// ---------------- Funcões de Inicialização e Destruição -----------

//fff
int* biggest_words(char*** file_data, int lines, int columns){

    unsigned int  string_size;
    int* words;

    if (!file_data)
        return NULL;

    words = malloc(columns * sizeof (int));
    if (!words)
        return NULL;

    string_size = 0;
    for (int i = 0; i < columns; i++) {
        for (int j = 0; j < lines; j++) {
            if (strlen(file_data[j][i]) > string_size)
                string_size = strlen(file_data[j][i]);
        }
        words[i] = string_size;
        string_size = 0;
    }
    return words;
}

//fff
//------------------------------------------------------------------
char*** allocate_file_data( int lines, int columns){

    char*** file_data;
    
    
    //Alocar vetor de ponteiros
    file_data = malloc(lines * sizeof(char**));
    if (!file_data){
        printf("Matriz não alocada\n");
        return NULL;
    }
    
    //Alocar linhas e strings
    for (int i = 0; i < lines; i++) {
        file_data[i] = malloc(columns * sizeof(char*));
        if (!file_data[i]){
        printf("Matriz não alocada\n");
        return NULL;
        }
    }
    return file_data;

}

//fff
//------------------------------------------------------------------
char*** fill_data(char* name, char*** file_data, int lines){

    char string[MAX - 1];
    FILE* file = fopen(name,"r");
    char* rest;
    char* token;
    int col;
    char* nan = "NaN";

    if (!file){
        printf("arquivo inexistente\n");
        return NULL;
    }

    if (!file_data)
        return NULL;

    for (int i = 0; i < lines;  i++) {

        fgets(string, MAX - 1, file);

        col = 0;
        rest = string;
        while ((token = strchr(rest, ',')) != NULL) {
            *token = '\0';
            if (strlen(rest) == 0)
                file_data[i][col] = strdup(nan);
            else 
                file_data[i][col] = strdup(rest);
            rest = token + 1;
            col++;
        }
        file_data[i][col] = strdup(rest);
        file_data[i][col][strlen(file_data[i][col]) - 1] = '\0';
    }

        fclose(file);
    return file_data;

}

//fff
//------------------------------------------------------------------
struct information* get_info(char* name){


    struct information* info;
    int lines;
    int columns;
    char string[MAX - 1];
    FILE* file = fopen(name,"r");
    char* token;
    int cols;
    unsigned int string_size;
    char* rest;
    char*** file_data;
    int* valid_lines;
    int* valid_cols;

    if (!file){
        printf("arquivo inexistente\n");
        return NULL;
    }
    
    //Testar se o arquivo está vazio
    int ch = fgetc(file);
    if (ch == EOF) {
        fclose(file);   
        return NULL;
    }
    
    info = malloc(sizeof(struct information));
    if (!info){
        printf("Info não alocada\n");
        return NULL;
    }

    strcpy(info->name, name);
    //info->name = name;
    //Descobre o num de linhas
    lines = 0;
    while (fgets(string, MAX - 1, file))
        lines++;
    rewind(file);
    info->lines = lines;

    //Descobre o num de colunas
    columns = 0;
    fgets(string, MAX - 1, file);
    rest = string;
    while ((token = strchr(rest, ',')) != NULL) {
        *token = '\0';
        if (rest){
            columns++; 
            rest = token + 1;
        }
    }
    if (strlen(string) != 0)
        columns++;




    //token = strtok(string, ",");
    //while (token){
    //    columns++; 
    //    token = strtok(NULL,",");
    //}
    info->columns = columns;

    //Descobre os tipos
    fgets(string, MAX - 1, file);
    info->types = malloc( info->columns * sizeof(int));
    if (!info->types){
        printf("vetor não alocado\n");
        return NULL;
    }

    rest = string;
    cols = 0;
//    while ((token = strchr(rest,',')) != NULL) {
//        *token = '\0';
//        double value = atof(rest);
//        if (value == 0)
//            info->types[cols] = STRING;
//        else 
//            info->types[cols] = NUMBER;
//        rest = token + 1;
//        cols++;
//        if (rest[0] == ',') {
//            info->types[cols] = NUMBER;
//            cols++;
//            rest = token + 1;
//        }
//
//    }
//    if (strlen(string) != 0){
//        double value = atof(rest);
//        if (value == 0)
//            info->types[cols] = STRING;
//        else if (strlen(rest) == 0)
//            info->types[cols] = NUMBER;
//        else 
//            info->types[cols] = NUMBER;
//        cols++;
//    }
        
    while (rest != NULL && *rest != '\0') {
        char *token = strchr(rest, ',');
        if (token != NULL) {
            *token = '\0';
        }
        
        // Check if rest is empty (",,")
        if (strlen(rest) == 0) {
            info->types[cols] = NUMBER;
        } else {
            double value = strtod(rest, NULL);
            if (value == 0.0) {
                info->types[cols] = STRING;
            } else {
                info->types[cols] = NUMBER;
            }
        }
        
        cols++;
        if (token != NULL) {
            rest = token + 1;
        } else {
            rest = NULL;
        }
    }




   // token = strtok(string, ",");
   // cols = 0;
   // while (token){
   //     double value = atof(token);
   //     if (value == 0)
   //         info->types[cols] = STRING;
   //     else 
   //         info->types[cols] = NUMBER;
   //     token = strtok(NULL,",");
   //     cols++;
   // }
    rewind(file);

    //Achar a maior palavra
    string_size = 0;
    while (fgets(string, MAX - 1, file)){

        rest = string;
        while ((token = strchr(rest, ',')) != NULL) {
            *token = '\0';
            if (strlen(rest) > string_size)
                string_size = strlen(rest);
            rest = token + 1;
        }
    }
    info->word = string_size;

    //Aloca uma matriz e preenche com os dados
    file_data = allocate_file_data(lines,columns);
    info->file_data = fill_data(name, file_data,lines);
    
    //Acha as maiores palavras por coluna
    info->bwords = biggest_words(file_data,lines,columns);


    //Validar linhas
    valid_lines = malloc((lines - 1) * sizeof(int));
    if (!valid_lines)
        return NULL;
    for (int i = 0; i < lines - 1; i++) 
        valid_lines[i] = 1;
    info->valid_lines = valid_lines;

    valid_cols = malloc(columns * sizeof(int));
    if (!valid_cols)
        return NULL;
    for (int i = 0; i < columns; i++) 
        valid_cols[i] = 1;
    info->valid_cols = valid_cols;

    fclose(file);
    return info;
}
  

//fff
//------------------------------------------------------------------
void destroy_file_data(char*** file_data, int lines, int columns){

    if (file_data){

        for (int i = 0; i < lines; i++) {
            for (int j = 0; j < columns; j++) {
            free(file_data[i][j]);
            }
        }

        for (int i = 0; i < lines; i++) {
            free(file_data[i]);
            file_data[i] = NULL;
        }
        free(file_data);
    }
    
}
//fff
//------------------------------------------------------------------
void destroy_info(struct information** info){

    if (*info){
        free((*info)->bwords);
        free((*info)->types);
        free((*info)->valid_lines);
        free((*info)->valid_cols);
        destroy_file_data((*info)->file_data, (*info)->lines, (*info)->columns);
        free(*info);
        *info = NULL;
    }
}
//---------- Funções Pequenas --------------------------------------

//fff
int print_line(struct information* info, int line){

    if (!info)
        return 0;

    printf("%-*d ",DIGTLINES, line);

    for (int j = 0; j < info->columns; j++) {
        if (info->valid_cols[j] == VALID)
            printf("%*s ",info->bwords[j],info->file_data[line + 1][j]);
    }
    printf("\n");
    return 1;
}

//fff

//-------------------------------------------------------------------
int print_dots(struct information* info){
 
    if (!info)
        return 0;

    char dots[5] = "...";
    printf("%-*s ",DIGTLINES, dots);

    for (int j = 0; j < info->columns; j++) {
        if (info->valid_cols[j] == VALID)
          printf("%*s ",info->bwords[j],dots);
    }
    printf("\n");
    return 1;
}

//-------------------------------------------------------------------
//fff
int print_vabs(struct information* info){

    if (!info)
        return 0;

    printf("%*s ",DIGTLINES,"");
    for (int i = 0; i < info->columns; i++) 
        if (info->valid_cols[i] == VALID)
            printf("%*s ",info->bwords[i],info->file_data[0][i]);
    printf("\n");
    return 0;
}
//-------------------------------------------------------------------
//fff
int compara_num(const void* a, const void* b) {
    struct string *sa = (struct string *)a;
    struct string *sb = (struct string *)b;

    if (strcmp(sa->value,"NaN") == 0){
        return -1;
    }
    if (strcmp(sb->value,"NaN") == 0)
        return 1;

    float num_sa = atof(sa->value);
    float num_sb = atof(sb->value);

    if (num_sa == 0 || num_sb == 0)
        return strcmp(sa->value,sb->value);

    if (num_sa > num_sb)
        return 1;
    else if (num_sa < num_sb)
        return -1;
    return 0;
}
//fff
//-------------------------------------------------------------------
int compara_num_dec(const void* a, const void* b) {
    
    int i = compara_num(a,b);
    if (i == 0)
        return 0;
    return -i;
}

//-------------------------------------------------------------------
//fff
int record_sorted(struct information* info, char* name, struct string* v, char* save){

    char file_name[MAX + 1];
    FILE* file;
    char string[MAX + 1];
    char* token;
    char nan[] = "NaN";
   
    if (!info || !v)
        return -1;

    while ((strcmp(save,"S\n") !=  0 && strcmp(save,"N\n") != 0)) {
        printf("Opção não existe, tente novamente: ");
        fgets(save, sizeof(save), stdin);
    }

    if (strcmp(save, "S\n") == 0){
    
        if (!name){
            printf("Entre com o nome do arquivo: ");
            fgets(file_name, sizeof(file_name),stdin);
            char* token = strchr(file_name,'\n');
            *token = '\0';
        } else
            strcpy(file_name,name);

        file = fopen(file_name,"w");
        if (!file){
            printf("Arquivo inexistente\n");
            return 0;
        }
        
        string[0] = '\0';
        for (int i = 0; i < info->columns; i++) {
             strcat(string,info->file_data[0][i]);
             strcat(string,",");
        }
        token = strrchr(string,',');
        *token = '\n';
        fputs(string,file);

        string[0] = '\0';
        for (int i = 0; i < info->lines - 1; i++){

             for (int j = 0; j < info->columns; j++){ 
                if (strcmp(info->file_data[v[i].line][j],nan) != 0)
                    strcat(string,info->file_data[v[i].line][j]);
                strcat(string,",");
             }
             token = strrchr(string,',');
             *token = '\n';
             fputs(string,file);
             string[0] = '\0';
        }
       
        fclose(file);
    }
    return 1;
}

//------------------------------------------------------------------
//fff
float calt_desvio(struct information* info, int col, float average){

    float sum;
    float value;
    int valid_lines;

    if (!info)
        return -1;
    
    valid_lines = 0;
    sum = 0;
    for (int i = 1; i < info->lines; i++) {
        if (strcmp(info->file_data[i][col],"NaN") != 0){
            valid_lines++;
            value = atof(info->file_data[i][col]) - average;
            value = value * value;
            sum = sum + value;
        }
    }
    return sqrt((sum / valid_lines - 1));
}

//------------------------------------------------------------------
//fff
int compara_med(const void* a, const void* b) {
    const char * const *sa = (const char* const *)a;
    const char * const *sb = (const char* const *)b;

    if (strcmp(*sa,"NaN") == 0){
        return -1;
    }
    if (strcmp(*sb,"NaN") == 0)
        return 1;

    return strcmp(*sa,*sb);
}
//------------------------------------------------------------------
//fff
void press_enter(){

    char enter;
    
    printf("\nPressione Enter para continuar\n");
    scanf("%c",&enter);
    while (enter != '\n')
        scanf("%c",&enter);
    return;
}
//------------------------------------------------------------------
//fff
struct mode* count_times(struct information* info, int line, int col, struct mode* head){

    int found;
    struct mode* aux;
    struct mode* aux_prev;

    if (!info)
        return NULL;

    //Cria a cabeça da lista se ela não existe
    if (!head){
        head = malloc(sizeof(struct mode));
        if (!head)
            return NULL;
        head->value = info->file_data[line][col];
        head->times = 1;
        head->next = NULL;
        return head;
    }

    //Procura o termo e acrescenta caso haja
    found = 0;
    aux = head;
    aux_prev = head;
    while (aux != NULL && found == 0){
        if (strcmp(info->file_data[line][col],aux->value) == 0){
            aux->times++;
            found = 1;
        } else {
            aux = aux->next;
            if (aux)
                aux_prev = aux;
        }
    }

    //Se não achar cria um novo nodo
    if (found == 0){
        aux = malloc(sizeof(struct mode));
        if (!aux)
            return NULL;
        aux_prev->next = aux;
        aux->value = info->file_data[line][col];
        aux->times = 1;
        aux->next = NULL;
    }
    return head;
}

//------------------------------------------------------------------
//fff
int smaller(char* s1, char* s2){

    if (!s1 || !s2)
        return -1;

    if (strcmp(s1,"NaN") == 0){
        return 0;
    }
    if (strcmp(s2,"NaN") == 0)
        return 1;
 
    float num_s1 = atof(s1);
    float num_s2 = atof(s2);

    if (num_s1 == 0 || num_s2 == 0){
        if (strcmp(s1,s2) > 0)
            return 1;
        return 0;
    }

    if (num_s1 > num_s2)
        return 1;
    return 0;

}

int bigger(char* s1, char* s2){

    if (!s1 || !s2)
        return -1;

    if (strcmp(s1,"NaN") == 0){
        return 1;
    }
    if (strcmp(s2,"NaN") == 0)
        return 0;

    float num_s1 = atof(s1);
    float num_s2 = atof(s2);

    if (num_s1 == 0 || num_s2 == 0){
        if (strcmp(s1,s2) < 0)
            return 1;
        return 0;
    }

    if (num_s1 < num_s2)
        return 1;
    return 0;

}

int equal(char* s1, char* s2){

    if (!s1 || !s2)
        return -1;

    if (strcmp(s1,s2) == 0)
        return 1;
    return 0;
}

int different(char* s1, char* s2){

    if (!s1 || !s2)
        return -1;

    if (strcmp(s1,s2) != 0)
        return 1;
    return 0;
}

int small_equal(char* s1, char* s2){

    if (!s1 || !s2)
        return -1;
    
    if (strcmp(s1,"NaN") == 0 && strcmp(s2,"NaN") != 0){
        return 0;
    }
    if (strcmp(s1,"NaN") != 0 && strcmp(s2,"NaN") == 0){
        return 1;
    }
    if (strcmp(s2,"NaN") == 0 && strcmp(s1,"NaN") == 0)
        return 1;

    float num_s1 = atof(s1);
    float num_s2 = atof(s2);

    if (num_s1 == 0 || num_s2 == 0){
        if (strcmp(s1,s2) >= 0)
            return 1;
        return 0;
    }


    if (num_s1 >= num_s2)
        return 1;
    return 0;
}

int bigg_equal(char* s1, char* s2){

    if (!s1 || !s2)
        return -1;
    
    if (strcmp(s1,"NaN") == 0 && strcmp(s2,"NaN") != 0){
        return 1;
    }
    if (strcmp(s1,"NaN") != 0 && strcmp(s2,"NaN") == 0){
        return 0;
    }
    if (strcmp(s2,"NaN") == 0 && strcmp(s1,"NaN") == 0)
        return 0;
 
    float num_s1 = atof(s1);
    float num_s2 = atof(s2);

    if (num_s1 == 0 || num_s2 == 0){
        if (strcmp(s1,s2) <= 0)
            return 1;
        return 0;
    }


    if (num_s1 <= num_s2)
        return 1;
    return 0;

}
//------------------------------------------------------------------//fff
int filtro(struct information* info, int column, char* value, int (*func)(char*, char*)){

    int result;

    if (!info || !func)
        return 0;

    for (int i = 1; i < info->lines;i++) {
        result = func(value, info->file_data[i][column]);
        if (result == 1)
           info->valid_lines[i - 1] = VALID;
        printf("\n");
    }
    return 1;
}

//-------------------------------------------------------------------
//fff
int record(struct information* info, char* name, char* save){
    
    char file_name[MAX + 1] = {0};
    FILE* file;
    char string[MAX + 1] = {0};
    char* token;
    char nan[] = "NaN";
   
    if (!info)
        return 0;

    while ((strcmp(save,"S\n") !=  0 && strcmp(save,"N\n") != 0)) {
        printf("Opção não existe, tente novamente: ");
        fgets(save, sizeof(save), stdin);
    }
    
    if (strcmp(save, "S\n") == 0){
    
        if (!name){
            printf("Entre com o nome do arquivo: ");
            fgets(file_name, sizeof(file_name),stdin);
            char* token = strchr(file_name,'\n');
            *token = '\0';
        } else{
            strcpy(file_name,name);
        }

        file = fopen(file_name,"w");
        if (!file){
            printf("Arquivo inexistente\n");
            return 0;
        }
        
        string[0] = '\0';
        for (int i = 0; i < info->columns; i++) {
            if (info->valid_cols[i] == VALID){
                strcat(string,info->file_data[0][i]);
                strcat(string,",");
            }
        }
        token = strrchr(string,',');
        *token = '\n';
        fputs(string,file);

        string[0] = '\0';
        for (int i = 1; i < info->lines; i++){

            if (info->valid_lines[i - 1] == VALID){

                for (int j = 0; j < info->columns; j++){ 
                    if (info->valid_cols[j] == VALID){
                        if (strcmp(info->file_data[i][j],nan) != 0)
                            strcat(string,info->file_data[i][j]);
                        strcat(string,",");
                    }
                }
                token = strrchr(string,',');
                *token = '\n';
                fputs(string,file);
                string[0] = '\0';
            }
        }
       
        printf("Arquivo gravado com sucesso\n");
        fclose(file);
    }
    return 1;
}
//-------------------------------------------------------------------
//fff
int discard_orgn(struct information** info){

    char save[5];
    char name[MAX + 1];

 
    if (!*info)
        return -1;

    printf("Deseja descartar os dados originais? [S|N] ");
    fgets(save, sizeof(save), stdin);
    while ((strcmp(save,"S\n") !=  0 && strcmp(save,"N\n") != 0)) {
        printf("Opção não existe, tente novamente: ");
        fgets(save, sizeof(save), stdin);
    }
 
    if (strcmp(save, "S\n") == 0){
        record(*info,(*info)->name,save);
        strcpy(name,(*info)->name);
        destroy_info(info);
        (*info) = get_info(name);
        return 1;
    }
    
    return 0;
}
//-------------------------------------------------------------------
//fff
int print_nan(struct information** info) {
 
    int prt_array[5];
    int array_size;
    int cont;
    int i;
    char save[5];
   
    print_vabs(*info);

    //Joga no vetor as primeiras 5 linhas que têm NaN
    cont = 0;
    i = 0;
    array_size = 0;
    while ((i < (*info)->lines - 1) && cont < 5){

        if ((*info)->valid_lines[i] == VALID){
                prt_array[cont] = i;
                array_size++;
                cont++;
            }
            i++;
    }
    for (int i = 0; i < array_size;i++) 
        print_line(*info,prt_array[i]);

       print_dots(*info);
    
    //Joga no vetor as últimas 5 linhas que têm NaN
    i = (*info)->lines - 2;
    cont = 0;
    array_size = 0;
    while ((i >= 0) && cont < 5){
            if ((*info)->valid_lines[i] == VALID){
                prt_array[cont] = i;
                array_size++;
                cont++;
            }
            i--;
    }
    for (int i = array_size - 1; i >= 0 ;i--) 
        print_line(*info,prt_array[i]);

    cont = 0;//Contar o número de NaNs
    for (int i = 1; i < (*info)->lines - 1;i++) {
        if ((*info)->valid_lines[i] == VALID)
            cont++;
    }

    printf("\n[%d rows x %d columns]\n",cont,(*info)->columns);

    printf("\nDeseja gravar um arquivo com os dados alterados? [S|N] ");
    
    fgets(save, sizeof(save), stdin);
    record((*info),NULL,save);
    
    discard_orgn(info);
   // if (discard == 0){
   //     for (int i = 0; i < (*info)->lines - 1;i++) 
   //         (*info)->valid_lines[i] = VALID;
   // }


    press_enter();


    return 1;
}

//-------------------------------------------------------------------
//fff
int change_average(struct information** info){

    int cont;
    float sum;
    double average;
    char string[(*info)->word];
    char save[5] = "S\n";

    if (!info)
        return 0;

    //strcpy((*info)->file_data[135][4],"Teste");
    cont = 0;
    sum = 0;
    for (int i = 0; i < (*info)->columns; i++) {

        //Encontra os não NaNs e faz a média
        if ((*info)->types[i] == NUMBER) {
            for (int j = 1; j < (*info)->lines;j++) {
                if (strcmp((*info)->file_data[j][i],"NaN") != 0){
                    cont++;
                    float num = atof((*info)->file_data[j][i]);
                    sum = sum + num;
                }
            }
            average = sum / cont;
            //Substitui NaN pela média
            
            for (int k = 1; k < (*info)->lines; k++) {
        
                if ((*info)->valid_lines[k - 1] == VALID){

                    if (strcmp((*info)->file_data[k][i],"NaN") == 0){
                        sprintf(string, "%.1f",average);
                        (*info)->file_data[k][i] = realloc((*info)->file_data[k][i], strlen(string) + 1);
                        strcpy((*info)->file_data[k][i],string);
                        //print_line((*info),k - 1);
                    }
                }
            }
            sum = 0;
            cont = 0;
        }
    }
    for (int i = 0; i < (*info)->lines - 1;i++) 
        (*info)->valid_lines[i] = VALID;
    record((*info),(*info)->name,save);
    char name[MAX + 1];
    strcpy(name,(*info)->name);
    destroy_info(info);
    *info = get_info(name);
    press_enter();
    return 1;

}//-------------------------------------------------------------------
//fff
int change_next(struct information** info){

    char save[5] = "S\n";
    char* next = NULL;
    int p;

    if (!info)
        return 0;

    for (int i = 0; i < (*info)->columns; i++) {

        if ((*info)->types[i] == NUMBER) {
            
          for (int k = 1; k < (*info)->lines; k++) {
        
            if ((*info)->valid_lines[k - 1] == VALID){

              if (strcmp((*info)->file_data[k][i],"NaN") == 0){
                
                /*Checa se o próximo é nulo repetidamente até o 
                o final do arquivo */
                if ((k + 1) <= (*info)->lines - 1){
                    next = (*info)->file_data[k+1][i];
                    p = k + 1;
                    while (strcmp(next,"NaN") == 0 && p + 1 <= (*info)->lines - 1){
                        next = (*info)->file_data[p + 1][i];
                        p++;
                    }
                    strcpy((*info)->file_data[k][i],next); 

                }
              }
            }
          }
        }
    }
   
    for (int i = 0; i < (*info)->lines - 1;i++) 
        (*info)->valid_lines[i] = VALID;
   
    record((*info),(*info)->name,save);
    char name[MAX + 1];
    strcpy(name,(*info)->name);
    destroy_info(info);
    *info = get_info(name);
    return 1;

}

//-------------------------------------------------------------------
//fff
int remove_nan(struct information** info){

    char save[5] = "S\n";

    if (!info)
        return 0;

    //Ignora os NaNs
    for (int i = 0; i < (*info)->lines - 1;i++) {
        if ((*info)->valid_lines[i] == VALID)
            (*info)->valid_lines[i] = INVALID;
        else 
            (*info)->valid_lines[i] = VALID;
    }

    record((*info),(*info)->name,save);
    char name[MAX + 1];
    strcpy(name,(*info)->name);
    destroy_info(info);
    (*info) = get_info(name);
    return 1;

}

int find_col(struct information* info, char* word){

    if (!info || !word)
        return -1;
    
    for (int i = 0; i < info->columns;i++){
        if (strcmp(word,info->file_data[0][i]) == 0){
            info->valid_cols[i] = VALID;
            return 1;
        }
    }
    
    return 0;

}

//// --------- Funções do Trabalho -----------------------------------
//fff
int summary(struct information** info){


    if (!(*info))
        return 0;

    for (int i = 0; i < (*info)->columns; i++) {
        if ((*info)->types[i] == NUMBER)
            printf("%s [N]\n",(*info)->file_data[0][i]);
        else 
            printf("%s [S]\n",(*info)->file_data[0][i]);
    }
    printf("\n%d variáveis encontradas\n",(*info)->columns);
    press_enter();
    

    return 1;
}

//------------------------------------------------------------------
//fff
int show( struct information** info){

    int cont;
    int i;
    int array_size;
    int prt_array[5];
    
    if (!info)
        return 0;
    
    print_vabs(*info);
 
    //Joga no vetor as 5 primeiras linhas válidas
    cont = 0;
    i = 0;
    array_size = 0;
    while ((i < (*info)->lines - 1) && cont < 5){
            if ((*info)->valid_lines[i] == VALID){
                prt_array[cont] = i;
                array_size++;
                cont++;
            }
            i++;
    }
    for (int i = 0; i < array_size;i++) 
        print_line(*info,prt_array[i]);

    print_dots(*info);
    
    //Joga no vetor as 5 últimas linhas válidas
    i = ((*info)->lines) - 2;
    cont = 0;
    array_size = 0;
    while ((i >= 0) && cont < 5){
            if ((*info)->valid_lines[i] == VALID){
                prt_array[cont] = i;
                array_size++;
                cont++;
            }
            i--;
    }
    for (int i = array_size - 1; i > -1 ;i--) 
        print_line(*info,prt_array[i]);

   printf("\n");
    
    printf("\n[%d rows x %d columns]\n",(*info)->lines - 1,(*info)->columns);

    press_enter();
    return 1;

}

//fff
//------------------------------------------------------------------
int filter(struct information** info){

    func_t func_array[6];
    char *token;
    char variable[(*info)->word];
    char filter[(*info)->word];
    char value[MAX + 1];
    int vabcol;
    int p;
    int cont;
    int i;
    int prt_array[5];
    int array_size;
    char save[5];
    int discard;
    
    if (!info)
        return 0;

    func_array[0] = smaller;
    func_array[1] = bigger;
    func_array[2] = equal;
    func_array[3] = different;
    func_array[4] = bigg_equal;
    func_array[5] = small_equal;
  
    //Input correto da variável
    vabcol = -1;
    getchar();
    while (vabcol == -1){
        printf("Entre com a variável: ");
        fgets(variable, sizeof(variable),stdin);
        token = strchr(variable,'\n');
        if (token)
            *token = '\0';
   
        //Acha a coluna correta
        for (int i = 0; i < (*info)->columns; i++) {
            if (strcmp(variable, (*info)->file_data[0][i]) == 0)
                vabcol = i;
        }
    }

    
    printf("Escolha um filtro ( == > >= < <= != ): ");
    fgets(filter, sizeof(filter),stdin);
    token = strchr(filter,'\n');
    if (token)
        *token = '\0';
 
    

    printf("Digite um valor: ");
    fgets(value, sizeof(value),stdin);
    token = strchr(value,'\n');
    if (token)
        *token = '\0';
 
    printf("\n");
   
    for (int i = 0; i < (*info)->lines - 1; i++) 
        (*info)->valid_lines[i] = INVALID;

    p = 0;
    while ( p == 0){
        if (strcmp(filter,"<") == 0){
                filtro(*info,vabcol,value,func_array[0]);
                p = 1;
        } else if (strcmp(filter,">") == 0){
                filtro(*info,vabcol,value,func_array[1]);
                p = 1;
        } else if (strcmp(filter,"==") == 0){
                filtro(*info,vabcol,value,func_array[2]);
                p = 1;
        } else if (strcmp(filter,"!=") == 0){
                filtro(*info,vabcol,value,func_array[3]);
                p = 1;
        } else if (strcmp(filter,">=") == 0){
                filtro(*info,vabcol,value,func_array[4]);
                p = 1;
        } else if (strcmp(filter,"<=") == 0){
                filtro(*info,vabcol,value, func_array[5]);
                p = 1;
        } else {
            printf("Filtro errado, tente de novo: ");
            fgets(filter, sizeof(filter),stdin);
            token = strchr(filter,'\n');
            if (token)
            *token = '\0';
        }
    }
    print_vabs(*info);
    

    //Joga no vetor as 5 primeiras linhas válidas
    cont = 0;
    i = 0;
    array_size = 0;
    while ((i < (*info)->lines - 1) && cont < 5){
            if ((*info)->valid_lines[i] == VALID){
                prt_array[cont] = i;
                array_size++;
                cont++;
            }
            i++;
    }
    for (int i = 0; i < array_size;i++) 
        print_line(*info,prt_array[i]);

    print_dots(*info);
    
    //Joga no vetor as 5 últimas linhas válidas
    i = ((*info)->lines) - 2;
    cont = 0;
    array_size = 0;
    while ((i >= 0) && cont < 5){
            if ((*info)->valid_lines[i] == VALID){
                prt_array[cont] = i;
                array_size++;
                cont++;
            }
            i--;
    }
    for (int i = array_size - 1; i > -1 ;i--) 
        print_line(*info,prt_array[i]);

    cont = 0;//Contar o número de válidos
    for (int i = 0; i < (*info)->lines - 1;i++) {
        if ((*info)->valid_lines[i] == VALID)
            cont++;
    }

    printf("\n[%d rows x %d columns]\n",cont,(*info)->columns);


    printf("\nDeseja gravar um arquivo com os dados filtrados? [S|N] ");
    
    fgets(save, sizeof(save), stdin);
    record(*info,NULL,save);
   
    discard = discard_orgn(info);
    if (discard == 0){
        for (int i = 0; i < (*info)->lines - 1;i++) 
            (*info)->valid_lines[i] = VALID;
    } 
    //printf("Linhas:%d\n",info->lines);
    press_enter();

    
    return 1;

}
//-------------------------------------------------------------------
//fff
int describe(struct information** info){
    
    char variable[(*info)->word];
    int col;
    char* token;
    float type;
    int cont;
    float sum;
    struct mode* head;
    char** array;
    float average;
    struct mode* aux;
    float min,max;
    int amount;
    float med;
 
    
    //Input correto da variável
    col = -1;
    getchar();
    while (col == -1){
        printf("Entre com a variável: ");
        fgets(variable, sizeof(variable),stdin);
        token = strchr(variable,'\n');
        if (token)
            *token = '\0';
   
        //Acha a coluna correta
        for (int i = 0; i < (*info)->columns; i++) {
            if (strcmp(variable, (*info)->file_data[0][i]) == 0)
                col = i;
        }
    }

 
   type = atof((*info)->file_data[1][col]);

    cont = 0;
    sum = 0;
    head = NULL;
    //Faz o somatório e cria uma lista com os valores únicos
    for (int i = 1; i < (*info)->lines;i++) {
        if (strcmp((*info)->file_data[i][col],"NaN") != 0){
            cont++;//contador
            float num = atof((*info)->file_data[i][col]);
            if (num != 0)
                sum = sum + num;//soma
            head = count_times((*info),i,col,head);//structs com quanti
        }
    }

    //Vetor a ser ordenado
    array = malloc(((*info)->lines - 1) * sizeof(char*));
    if (!array)
        return 0;
    for (int i = 0; i < (*info)->lines - 1;i++) 
        array[i] = (*info)->file_data[i + 1][col];
    qsort(array,(*info)->lines - 1,sizeof(char*),compara_med);
   
    printf("Contador: %d\n",cont);
    if (type != 0){
        average = sum / cont;
        float desvio = calt_desvio(*info,col,average);
        printf("Média: %.1f \n",average);
        printf("Desvio: %.1f \n",desvio);

   
        //Calculo da mediana
        if ((cont) % 2 == 0){
            float m1 = atof(array[cont / 2]);
            float m2 = atof(array[ (cont / 2) - 1]);
            printf("%f %f \n",m1,m2);
            med = (m1 + m2) / 2;
        } else 
            med = atof(array[(cont) / 2]);

        printf("Mediana: %.1f \n",med);
    }
    free(array);
    

    min = atof(head->value);
    max = 0;
    amount = 0;
    aux = head;

    //Achar moda, min e max
    while (aux) {
        float value = atof(aux->value);
        if (amount < aux->times){
            amount = aux->times; 
        }
        if (type != 0){
            if (min > value)
                min = value;
            if (max < value)
                max = value;
        }
        aux = aux->next;
    }

    printf("Moda: ");
    aux = head;
    while (aux){
        if (aux->times == amount)
            printf("%s ",aux->value);
        aux = aux->next;
    }

    printf("%d vezes\n",amount);

    if (type != 0){
        printf("Min: %.1f\n",min);
        printf("Max: %.1f\n",max);
    }

    printf("Valores unicos: [ ");
    aux = head;
    while (aux){
        printf("%s ",aux->value);
        aux = aux->next;
    }
    printf("]\n");

    //Deletar structs
    aux = head;
    while (aux){
        aux = head->next;
        free(head);
        head = aux;
    }

    press_enter();
    return 1;
}

//-------------------------------------------------------------------
//fff
int sorting(struct information** info){

    char variable[(*info)->word];
    char order[(*info)->word];
    int col;
    char* token;
    struct string* array;
    char save[5];

    if (!info)
        return 0;
    
    //Input correto da variável
    col = -1;
    getchar();
    while (col == -1){
        printf("Entre com a variável: ");
        fgets(variable, sizeof(variable),stdin);
        token = strchr(variable,'\n');
        if (token)
            *token = '\0';
   
        //Acha a coluna correta
        for (int i = 0; i < (*info)->columns; i++) {
            if (strcmp(variable, (*info)->file_data[0][i]) == 0)
                col = i;
        }
    }

    strcpy(order,"0");//Qualquer coisa;
    while ((strcmp(order,"A") != 0) && (strcmp(order,"D") != 0)){
            printf("Selecione uma opção [A]scendente ou [D]escendente: ");
            fgets(order, sizeof(order),stdin);
            token = strchr(order,'\n');
            if (token)
                *token = '\0';
    }
 
    //Preeche vetor de structs
    array = malloc(((*info)->lines - 1) * sizeof(struct string));
    if (!array)
        return 0;

    for (int i = 0; i < (*info)->lines - 1; i++) {
        array[i].line = i + 1;
        strcpy(array[i].value, (*info)->file_data[i + 1][col]);
    }
   

    print_vabs(*info);
    
    //Ordenação
    if (strcmp(order,"A") == 0)
        qsort(array,(*info)->lines - 1, sizeof(struct string),compara_num);
    else {
        qsort(array,(*info)->lines - 1, sizeof(struct string),compara_num_dec);
    }
    for (int i = 0; i < 5; i++) 
        print_line(*info, array[i].line - 1);

    print_dots(*info);
 
    for (int i = (*info)->lines - 6; i < (*info)->lines - 1; i++) 
        print_line(*info, array[i].line - 1);
    
    printf("\n[%d rows x %d columns]\n",(*info)->lines - 1,(*info)->columns);

    printf("\nDeseja gravar um arquivo com os dados ordenados? [S|N] ");
    
    fgets(save, sizeof(save), stdin);
    record_sorted(*info, NULL, array, save);

    printf("Deseja descartar os dados originais? [S|N] ");
    fgets(save, sizeof(save), stdin);
    while ((strcmp(save,"S\n") !=  0 && strcmp(save,"N\n") != 0)) {
        printf("Opção não existe, tente novamente: ");
        fgets(save, sizeof(save), stdin);
    }

    if (strcmp(save, "S\n") == 0){
        record_sorted(*info,(*info)->name, array, save);

        destroy_file_data((*info)->file_data, (*info)->lines, (*info)->columns);
        (*info)->file_data = allocate_file_data((*info)->lines, (*info)->columns);
        fill_data((*info)->name,(*info)->file_data,(*info)->lines);
    } 

    free(array);
    press_enter();
    
    return 1;
    
    
}
//-------------------------------------------------------------------
//fff
int selecting(struct information** info){

    char string[MAX + 1];
    char save[5];
    int discard;
    char* token;
    int check;
    char* rest;
    int i;
    int cont;
    int array_size;
    int prt_array[5];

    if (!info)
        return 0;

    //Invalida todas as colunas primeiro
    for (int i = 0 ; i < (*info)->columns;i++) 
        (*info)->valid_cols[i] = INVALID;

    getchar();
    while (1) {
        printf("Entre com as variáveis que deseja selecionar (separadas por espaço): ");
        fgets(string, sizeof(string), stdin);
        check = 1;
        rest = string;
        token = strchr(string,'\n');
        *token = '\0';
        while ((token = strchr(rest, ' ')) != NULL) {
            *token = '\0';
            if (find_col(*info,rest) == 0){
                check = 0;
                break;
            } else 
                rest = token + 1;
        }

        if (find_col(*info,rest) == 0)
                check = 0;
        if (check == 1)
            break;
        for (int i = 0; i < (*info)->columns; i++) 
            (*info)->valid_cols[i] = INVALID;
            
    }

    print_vabs(*info); 

    //Joga no vetor as 5 primeiras linhas válidas
    cont = 0;
    i = 0;
    array_size = 0;
    while ((i < (*info)->lines - 1) && cont < 5){
            if ((*info)->valid_lines[i] == VALID){
                prt_array[cont] = i;
                array_size++;
                cont++;
            }
            i++;
    }
    for (int i = 0; i < array_size;i++) 
        print_line(*info,prt_array[i]);

    print_dots(*info);
    
    //Joga no vetor as 5 últimas linhas válidas
    i = ((*info)->lines) - 2;
    cont = 0;
    array_size = 0;
    while ((i >= 0) && cont < 5){
            if ((*info)->valid_lines[i] == VALID){
                prt_array[cont] = i;
                array_size++;
                cont++;
            }
            i--;
    }
    for (int i = array_size - 1; i > -1 ;i--) 
        print_line(*info,prt_array[i]);

    printf("\nDeseja gravar um arquivo com as variáveis selecionadas? [S|N] ");
    
    fgets(save, sizeof(save), stdin);
    record(*info,NULL,save);

    printf("%s\n",(*info)->name);
    discard = discard_orgn(info);
    if (discard == 0){
        for (int i = 0; i < (*info)->columns; i++) 
            (*info)->valid_cols[i] = VALID;
    }
    return 1;
}
//------------------------------------------------------------------
//fff
int find_missing(struct information** info){

    char option[5];
    char* token;
  
    if(!info)
        return 0;
 
    


    strcpy(option,"0");
    getchar();
    while (strcmp(option,"5") != 0){
    
        //Validar somente as linhas com NaN
        for (int i = 1; i < (*info)->lines; i++) {
            (*info)->valid_lines[i - 1] = INVALID;

            for (int j = 0; j < (*info)->columns; j++) {

                if ((*info)->types[j] == NUMBER){
                    if (strcmp((*info)->file_data[i][j],"NaN") == 0)
                        (*info)->valid_lines[i - 1] = VALID;
                        
                }
            }   
        } 
        printf("\n1) Listar registros com NaN\n");
        printf("2) Substituir pela média\n");
        printf("3) Substituir pelo próximo valor válido\n");
        printf("4) Remover registros com NaN\n");
        printf("5) Voltar ao menu principal\n");

        fgets(option,sizeof(option),stdin);
        token = strchr(option,'\n');
        *token = '\0';

        if (strcmp(option,"1") == 0){
            print_nan(info);
            
        } else if (strcmp(option,"2") == 0){
            change_average(info);     

        } else if (strcmp(option,"3") == 0){
            change_next(info);                  
        } else if (strcmp(option,"4") == 0){
            remove_nan(info);   
        } else if (strcmp(option,"5") != 0){
            printf("Opção não existe. Tente novamente: ");
        } else {
            for (int i = 0; i < (*info)->lines - 1; i++)
                (*info)->valid_lines[i] = VALID;
        }


    }
    return 1;
}
//------------------------------------------------------------------
//fff
int save_data(struct information** info){

    char save[5] = "S\n";

    if (!*info)
        return 0;

    record(*info,(*info)->name,save);
    press_enter();
    return 1;
}
