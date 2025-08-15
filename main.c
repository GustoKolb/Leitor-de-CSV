#include "io.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    
    (void)argc;
    char* name;
    int option;
    FILE* file;
    struct information* info;


    if (argv[1]) 
        name = argv[1];
    
    file = fopen(name,"r");
    if (!file)
        return 0;
    fclose(file);

    info = get_info(name);
 
    if (info){
        option = 0;
        while(option != 9){
            printf("1) Sumário do Arquivo\n");
            printf("2) Mostrar\n");
            printf("3) Filtros\n");
            printf("4) Descrição dos Dados\n");
            printf("5) Ordenação\n");
            printf("6) Seleção\n");
            printf("7) Dados Faltantes\n");
            printf("8) Salvar Dados\n");
            printf("9) Fim\n");

            scanf("%d",&option);

            switch (option) {
                case(1):
                    summary(&info);
                    break;
                case(2):
                    show(&info);
                    break;
                case(3): 
                    filter(&info);
                    break;
                case(4):
                    describe(&info);
                    break;
                case(5): 
                    sorting(&info);
                    break;
                case(6): 
                    selecting(&info);
                    break;
                case(7): 
                    find_missing(&info);
                    break;
                case(8):
                    save_data(&info);
                    break;
                case(9): 
                    break;
                default:
                    printf("Número Inválido. Tente novamente:\n");
                    scanf("%d",&option);
            }
        }
    }
    destroy_info(&info);
    return 0;
}
