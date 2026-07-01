//RAUL ENRIQUE MARTINEZ CRUZ 4CM2
//PROGRAMA DE COMBINACIONES BINARIAS 

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h> //para sleep

//-------------------Prototipos de funcion-------------------
void generarCadenas(int n, const char* filename);
void generarCadenasAuto(int n);

//-------------------F U N C I O N E S-------------------------

// Genera combinaciones binarias de un n específico (consola + archivo)
void generarCadenas(int n, const char* filename){
    long long total = 1LL<<n; //<<N nos servira para recorrer a la izquierda n posiciones y asi definir 2^n 
                              //sin necesidad de hacer calculos y agregar mas procesos al codigo o cargar la memoria
                              //tambien tendremos como beneficio que todos estos calculos no se guardaran en memoria 
                              //por lo cual podremos manejar N's muy grandes sin comprometer nuestro equipo

    FILE *f = fopen(filename, "w");
    if (!f) {
        printf("Error al abrir el archivo %s.\n", filename);
        return;
    }

    // Memoria dinámica para la cadena
    char *cadena = (char*) malloc((n+1) * sizeof(char));
    if (!cadena) {
        printf("Error de memoria.\n");
        fclose(f);
        return;
    }
    cadena[n] = '\0'; 

    fprintf(f, "\nn=%d\n\n", n);
    //printf("\nn=%d\n\n", n);

    // Abrimos el conjunto
    fprintf(f, "{");
    //  printf("{");

    //en este ciclo for generamos las combinaciones binarias
    //tambien es la parte mas importante del codigo, y la mas pesada en cuanto a logica y complejidad
    //este ciclo anidado tiene una complejidad de O(n*2^n) ya que el ciclo externo se ejecuta 2^n veces
    //y el ciclo interno se ejecuta n veces por cada iteracion del ciclo externo
    for (long long i = 0; i < total; i++) { //recorremos desde 0 hasta 2^n - 1
        for (int j = n-1; j >= 0; j--) {    //recorremos desde n-1 hasta 0 de forma decreciente
            //usamos el operador de desplazamiento a la derecha (>>) y el operador AND (&) 
            //para determinar si el bit en la posicion j es 1 o 0
            cadena[n-1-j] = ((i >> j) & 1) ? '1' : '0'; //n-1-j para llenar la cadena de izquierda a derecha
            //i>>j desplaza los bits de i j posiciones a la derecha
            //&1 obtiene el bit menos significativo (el de la derecha)
        }
        // i< total - 1 para evitar la coma al final
        //guardamos la cadena en el archivo y la imprimimos en consola
        if (i < total - 1) {
            fprintf(f, "%s, ", cadena);
           // printf("%s, ", cadena);

           //else para la ultima cadena sin coma
        } else {
            fprintf(f, "%s", cadena);
            //printf("%s", cadena);
        }
    }

    // Cerramos el conjunto en el archivo y en consola
    fprintf(f, "}\n");
    printf("}\n");

    // Liberamos memoria
    free(cadena);
    printf("\n\nCadenas generadas correctamente, gracias por usar nuestro sistema:)))\n");
    //cerramos el archivo
    fclose(f);
    //mensaje de confirmacion
    printf("\nCadenas guardadas en %s\n\n\n", filename);
    for(int k=0;k<8;k++){
        printf(".");
        sleep(1);
    }
    }

// Genera combinaciones desde n=0 hasta el n dado (solo archivo)
//aqui generamos todas las cadenas de forma automatica desde n=0 hasta n dado
void generarCadenasAuto(int n){
    FILE *f = fopen("combinaciones.txt", "w");
    if (!f) {
        printf("Error al abrir el archivo.\n");
        return;
    }

    for (int k = 0; k <= n; k++) {
        long long total = 1LL << k;
        char *cadena = (char*) malloc((k+1) * sizeof(char));
        if (!cadena) {
            printf("Error de memoria.\n");
            fclose(f);
            return;
        }
        cadena[k] = '\0';
        
        fprintf(f, "\n\nN=%d\n\n", k);

        // Abrimos conjunto
        fprintf(f, "{");

        //utilizamos el mismo ciclo anidado explicado anteriormente
        //este ciclo nos da la complejidad de O(n*2^n) para cada n en cualquiera de las opciones 
        //del menu, asi que podemos conluir que la complejidad total del programa es O(n*2^n)
        for (long long i = 0; i < total; i++) {
            for (int j = k-1; j >= 0; j--) {
                cadena[k-1-j] = ((i >> j) & 1) ? '1' : '0';
            }
            if (i < total - 1) {
                fprintf(f, "%s, ", cadena);
            } else {
                fprintf(f, "%s", cadena);
            }
        }

        // Cerramos conjunto
        fprintf(f, "}\n");
        
        free(cadena);
    }
    printf("\nCadenas generadas correctamente, gracias por usar nuestro sistema:)))");
    fclose(f);
    printf("\nCadenas guardadas en combinaciones.txt\n\n");
    for(int k=0;k<5;k++){
        printf(".");
        sleep(1);
    }
}




//-------------------M A I N-------------------------
int main(){
    int opc;

    do {  
        system("cls");
        printf("\n===========================================================");
        printf("\nBienvenido a su programa de generacion de cadenas binarias");
        printf("\n===========================================================\n");
        sleep(1);
        printf("\n---------------Menu---------------");
        printf("\n1) Arranque automatico dado 'N' limite");
        printf("\n2) Manual dado 'N' especifico");
        printf("\n3) Salir\n");
        printf("Opcion: ");
        scanf("%d",&opc);

        switch (opc) {
        case 1: { // automatico
            int max;
            system("cls");
            printf("\nIngresa un 'N' limite: ");
            scanf("%d",&max);

            if(max < 0 || max > 34){ // límite razonable para evitar archivos gigantes

                printf("Ingrese un numero valido (0-25).\n");
                for(int k=0;k<3;k++){
                    printf(".");
                    sleep(1);
                }            
            }
            else {
                generarCadenasAuto(max);
            }
            break;
        }

        case 2: { // manual
            int n;
            system("cls");
            printf("Ingrese un numero de n combinaciones que desea ver: ");
            scanf("%d",&n);

            if(n < 0 || n > 34){
                printf("Ingrese un numero valido (0-25).\n");
                for(int k=0;k<3;k++){
                    printf(".");
                    sleep(1);
                }            
            }
            else{
                const char* filename = "combinaciones.txt";
                generarCadenas(n, filename);
            }
            break;
        }

        case 3:
            printf("\nSaliendo");
            for(int k=0;k<3;k++){
                printf(".");
                sleep(1);
            }            
            return 0;

        default:
            printf("Opcion no valida.\n");
        }
    } while (1);    

    return 0;
}
