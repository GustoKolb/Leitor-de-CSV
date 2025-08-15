#ifndef IO_H
#define IO_H

#define MAX 1024
#define NUMBER 1
#define STRING 0
#define DIGTLINES 6 
#define COLUMNS 8
#define VALID 1
#define INVALID 0


struct information {
    char name[MAX+1];
    int lines;
    int columns;
    int *types;
    int word;
    char*** file_data;
    int* bwords;
    int* valid_lines;
    int* valid_cols;
};

struct string {
        int line;
        char value[MAX + 1];
};

struct mode {
    char* value;
    int times;
    struct mode* next;
};


typedef int (*func_t)(char*,char*);

//---------- Funções Principais --------------
int summary( struct information** info);

int show(struct information** info);

int describe(struct information** info);

int filter(struct information** info);

int sorting(struct information** info);

int selecting(struct information** info);

int find_missing(struct information** info); 
    
int save_data(struct information** info);

//---------- Funções de Informações Gerais --------------
int* biggest_words(char*** file_data, int lines, int columns);

char*** allocate_file_data( int lines, int columns);

char*** fill_data(char* name, char*** file_data, int lines);

struct information* get_info(char* name);

void destroy_file_data(char*** file_data, int lines, int columns);

void destroy_info(struct information** info);

int filtro(struct information* info, int column, char* value, int (*func)(char*, char*));

//---------- Funções de Estatística --------------
float calt_desvio(struct information* info, int col, float average);

int compara_med(const void* a, const void* b);

//---------- Funções de Auxiliares --------------

int print_line(struct information* info, int line);

int print_dots(struct information* info);

int print_vabs(struct information* info);

int compara_num(const void* a, const void* b);

int compara_num_dec(const void* a, const void* b);

int record_sorted(struct information* info, char* name, struct string* v, char* save);

void press_enter();

struct mode* count_times(struct information* info, int line, int col, struct mode* head);

int smaller(char* s1, char* s2);

int bigger(char* s1, char* s2);

int equal(char* s1, char* s2);

int different(char* s1, char* s2);

int small_equal(char* s1, char* s2);

int bigg_equal(char* s1, char* s2);

int record(struct information* info, char* name, char* save);
   
int discard_orgn(struct information** info);

int find_col(struct information* info, char* word);
//---------- Funções de NaN --------------
int print_nan(struct information** info);

int change_average(struct information** info);

int change_next(struct information** info);

int remove_nan(struct information** info);

#endif
