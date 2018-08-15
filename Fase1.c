#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

void analizador(char script[200]);
void SrciptExec(char *script);
void ScriptMKdisk(char *script);
void ScriptRMdisk(char *script);
void ScriptFdisk(char *script);
void ScriptMount(char *script);

char *limpiar(char *token);

//------------------------------------------------
void CrearDisco(int size, char *unit, char *path);
void EliminarDisco(char *path);

//-----------------------------------------------

struct Particion{
    char part_status;
    char part_type;
    char part_fit;
    int part_start;
    int part_size;
    char name[16];
};

typedef struct Particion prt;

struct MBR{
    int mbr_tamanio;
    char mbr_fecha_creacion[128];
    int mbr_disk_signature;
    struct Particion prt[4];
};

typedef struct MBR mbr;

typedef struct{
    char part_status;
    char part_fit;
    int part_start;
    int part_size;
    int part_next;
    char name[16];
}EBR;


int main(){

    char comando[200];
    do {

        printf("root@FernandoPaz~ ");

        fgets(comando,200,stdin);

        int i;
        for (i = 0;comando[i]; i++)
            comando[i]= tolower(comando[i]);

        //printf("%s",comando);
        analizador(comando);

    } while (strncmp(comando,"salir",5)!=0);


    return 0;
}


void analizador(char script[200]){

    //printf("%s",script);

    int i;
    for (i = 0;script[i]; i++)
        script[i]= tolower(script[i]);

    char strCOPIA[100];
    char strCOPIA2[100];
    strcpy(strCOPIA, script);

    const char as[2] = "/";
    const char s[2] = " ";

    char *token;
    token = strtok(script, s);

    while (token != NULL) {

        if ((strncmp(token, "mkdisk", 6)) == 0) {
            ScriptMKdisk(strCOPIA);
        } else if ((strncmp(token, "rmdisk", 6)) == 0) {
            ScriptRMdisk(strCOPIA);
        } else if ((strncmp(token, "fdisk", 5)) == 0) {
            ScriptFdisk(strCOPIA);
        } else if ((strncmp(token, "mount", 5)) == 0) {
            ScriptMount(strCOPIA);
        } else if ((strncmp(token, "unmount", 7)) == 0) {
            //INScrearDESMONTAR(strCOPIA);
        } else if ((strncmp(token, "rep", 3)) == 0) {
            //INScrearREP(strCOPIA);
            //prueba();
            //crearReporte1("vda2","disk","/home/fernandopaz/Reportes/R.jpg");
            //ScriptRep(strCOPIA);

        }else {
            if (strCOPIA[0] == '#') {
                printf("::comentario::%s\n", strCOPIA);
            } else {
                strcpy(strCOPIA2, strCOPIA);
                token = strtok(strCOPIA, as);
                if ((strncmp(token, "exec", 4)) == 0) {
                    SrciptExec(strCOPIA2);
                }else{
                    printf("instruccion no reconocida\n");
                }
            }
        }
        break;
    }
}



void SrciptExec(char *script){


    char tokenizer[250];
    strcpy(tokenizer,script);

    int control;
    control=0;

    char *ruta;
    ruta=(char *)malloc(200*sizeof(char));

    int k =strlen(tokenizer);


    for (int var = 0; var < k; var++) {

        if (tokenizer[var] == 32) {

            tokenizer[var] = '_';
            control++;
        }

    }


    if (control==1) {

        char *puntero;
        puntero = strtok(script," -\n");

        char *ruta;
        ruta =(char *)malloc(100*sizeof(char));

        char *token[5];
        int j;

        while (puntero!=NULL) {

            token[j]={puntero};
            j++;
            puntero = strtok(NULL," -\n");
        }

        for (int var = 0; var < j; var++) {

            if (strcmp(token[var],"exec")==0) {
                strcpy(ruta,token[var+1]);
            }

        }

        //printf("%s\n",ruta);



        FILE *archivo;
        archivo = fopen(ruta,"r");
        char linea[2000];

        if (archivo == NULL)
        {
            printf("\nError de apertura del archivo. \n\n");
        }
        else
        {

            while(fgets(linea, 2000, (FILE*) archivo)) {

                char *lineax;
                lineax =(char *)malloc(200*sizeof(char));

                strcpy(lineax,linea);

                if (strcmp(lineax,"\n")==0) {

                }else{

                    //printf("Linea: %s\n", lineax);
                    analizador(lineax);

                }

            }

        }

        fclose(archivo);





    }else{



        strcpy(ruta,"");

        char *puntero;
        puntero = strtok(script,"\"");

        char *arreglo[100];
        int i=0;

        while (puntero!=NULL) {

            arreglo[i]={puntero};
            //printf ("token: %s\n",puntero);
            puntero = strtok(NULL,"\"");
            i++;
        }


        for (int var = 0; var < i; var++) {


            if (strcmp(arreglo[var],"exec ")==0) {

                ruta = arreglo[var+1];

            }

        }

        printf("%s",ruta);




        FILE *archivo;
        archivo = fopen(ruta,"r");
        char linea[2000];

        if (archivo == NULL)
        {
            printf("\nError de apertura del archivo. \n\n");
        }
        else
        {

            while(fgets(linea, 2000, (FILE*) archivo)) {

                char *lineax;
                lineax =(char *)malloc(200*sizeof(char));

                strcpy(lineax,linea);

                if (strcmp(lineax,"\n")==0) {

                }else{

                    //printf("Linea: %s\n", lineax);
                    analizador(lineax);

                }

            }

        }

        fclose(archivo);

    }

}

void ScriptMKdisk(char *script){

    int size;
    char *unit;
    char *path;

    unit=(char *)malloc(100*sizeof(char));
    path=(char *)malloc(100*sizeof(char));

    strcpy(unit,"xxx");


    printf ("%s\n",script);

    char tokenizer[250];
    strcpy(tokenizer,script);

    for (int var = 0; var < 250; var++) {

        if (tokenizer[var] == ' ') {

            tokenizer[var] = '_';
        }

    }

    //printf ("%s\n",tokenizer);

    char *token;
    token=(char *)malloc(200*sizeof(char));

    strcpy(token,tokenizer);


    //printf ("%s\n",token);

    char *puntero;
    puntero = strtok(token,"->-\n");

    char *arreglo[25];
    int i=0;

    while (puntero!=NULL) {

        arreglo[i]={puntero};
        //printf ("token: %s\n",puntero);
        puntero = strtok(NULL,"->-\n");
        i++;
    }

    char *arreglo1[25];
    for (int x = 0; x < i; x++) {

        char *t = limpiar(arreglo[x]);

        arreglo1[x]={t};
        //printf ("-->: %s\n",t);

    }


    for (int var = 0; var < i; var++) {

        if (strcmp(arreglo1[var],"size")==0) {

            size = atoi(arreglo1[var+1]);

        }else if (strcmp(arreglo1[var],"unit")==0) {

            unit = arreglo1[var+1];

        }else if (strcmp(arreglo1[var],"path")==0) {

            path = arreglo1[var+1];

        }

    }

    if (strcmp(unit,"xxx")==0) {
        strcpy(unit,"m");
    }


    printf("--->MKDISK: SIZE:%d UNIT:%s PATH:%s \n",size,unit,path);

    CrearDisco(size,unit,path);



}

void ScriptRMdisk(char *script){

    char tokenizer[250];
    strcpy(tokenizer,script);

    int control;
    control=0;

    char *ruta;
    ruta=(char *)malloc(200*sizeof(char));

    int k =strlen(tokenizer);


    for (int var = 0; var < k; var++) {

        if (tokenizer[var] == 32) {

            tokenizer[var] = '_';
            control++;
        }

    }


    if (control==1) {

        strcpy(ruta,"");


        char *puntero;
        puntero = strtok(script,"-\n->");

        char *token[10];
        int j=0;

        while (puntero!=NULL) {

            token[j]={puntero};
            //printf("%s\n",puntero);
            j++;
            puntero = strtok(NULL,"-\n->");
        }

        for (int var = 0; var < j; var++) {

            if (strcmp(token[var],"path")==0) {
                strcpy(ruta,token[var+1]);
            }

        }

        printf("RMdisk: PATH:%s \n",ruta);
        EliminarDisco(ruta);


    }else{



        strcpy(ruta,"");

        char *puntero;
        puntero = strtok(script,"\"->\n");

        char *arreglo[100];
        int i=0;

        while (puntero!=NULL) {

            arreglo[i]={puntero};
            //printf ("token: %s\n",puntero);
            puntero = strtok(NULL,"\"->\n");
            i++;
        }


        for (int var = 0; var < i; var++) {


            if (strcmp(arreglo[var],"path")==0) {

                ruta = arreglo[var+1];

            }

        }

        printf("RMdisk: PATH:%s \n",ruta);
        EliminarDisco(ruta);

    }

}

void ScriptFdisk(char *script){

    int size;

    char *unit;
    unit=(char *)malloc(100*sizeof(char));

    char *path;
    path=(char *)malloc(100*sizeof(char));

    char *type;
    type=(char *)malloc(100*sizeof(char));

    char *fit;
    fit=(char *)malloc(100*sizeof(char));

    char *delet;
    delet=(char *)malloc(100*sizeof(char));

    char *name;
    name=(char *)malloc(100*sizeof(char));

    int add;

    char *ext;
    ext=(char *)malloc(100*sizeof(char));

    char *extension;
    extension=(char *)malloc(100*sizeof(char));


    char tokenizer[250];
    strcpy(tokenizer,script);

    for (int var = 0; var < 250; var++) {

        if (tokenizer[var] == ' ') {

            tokenizer[var] = '_';
        }

    }

    //printf ("%s\n",tokenizer);

    char *token;
    token=(char *)malloc(200*sizeof(char));

    strcpy(token,tokenizer);


    //printf ("%s\n",token);

    char *puntero;
    puntero = strtok(token,"->-\n");

    char *arreglo[25];
    int i=0;

    while (puntero!=NULL) {

        arreglo[i]={puntero};
        //printf ("token: %s\n",puntero);

        puntero = strtok(NULL,"->-\n");
        i++;
    }



    char *arreglo1[25];
    for (int x = 0; x < i; x++) {


        char *t = limpiar(arreglo[x]);

        arreglo1[x]={t};
        printf ("LLL: %s\n",t);

    }



    for (int var = 0; var < i; var++) {

        if (strcmp(arreglo1[var],"size")==0) {

            size = atoi(arreglo1[var+1]);

        }else if (strcmp(arreglo1[var],"unit")==0) {

            unit = arreglo1[var+1];

        }else if (strcmp(arreglo1[var],"path")==0) {

            path = arreglo1[var+1];

        }else if (strcmp(arreglo1[var],"type")==0) {

            type = arreglo1[var+1];

        }else if (strcmp(arreglo1[var],"fit")==0) {

            fit = arreglo1[var+1];

        }else if (strcmp(arreglo1[var],"name")==0) {

            name = arreglo1[var+1];

        }else if (strcmp(arreglo1[var],"delete")==0) {

            delet = arreglo1[var+1];

        }else if (strcmp(arreglo1[var],"add")==0) {

             add = atoi(arreglo1[var+1]);

        }
    }

    int d=0;
    int a=0;



    for (int var = 0; var < i; ++var) {


        if (strcmp(arreglo[var],"delete")==0) {

            printf("--->FDISK DELETE:%s NAME:%s PATH:%s \n",delet,name,path);
            d=1;

        }else if (strcmp(arreglo[var],"add")==0) {

            printf("--->FDISK ADD:%d UNIT:%s NAME:%s PATH:%s \n",add,unit,name,path);
            a=1;

        }

    }

    if (d==0 && a==00) {
        printf("--->FDISK: SIZE:%d UNIT:%s PATH:%s TYPE:%s FIT:%s NAME:%s \n",size,unit,path,type,fit,name);
        //CrearParticion(size,unit,path,type,fit,name);
    }


}

void ScriptMount(char *script){}

char *limpiar(char *token){


    char tokenizer[100];

    strcpy(tokenizer,token);
    int x=0;


    if (tokenizer[x]== '\"') {

        for (int var = 0; var < 100; var++) {

            if (tokenizer[var] == '_') {

                tokenizer[var] = ' ';
            }

        }

        for (int q = 1; q < 100; q++) {

            if (tokenizer[q]==34) {

                tokenizer[q+1] = '\0';

            }

        }

        char *limpio;
        limpio=(char *)malloc(100*sizeof(char));

        strcpy(limpio,tokenizer);
        return limpio;

    }else{

        for (int var = 0; var < 100; var++) {

            if (tokenizer[var] == '_') {

                tokenizer[var] = ' ';
            }

        }


        //char arreglo1[100];
        for (int q = 0; q < 100; q++) {

            if (tokenizer[q]==32) {

                tokenizer[q] = '\0';

            }

        }


        char *limpio;
        limpio=(char *)malloc(100*sizeof(char));

        strcpy(limpio,tokenizer);
        return limpio;

    }

}

//----------------------------------------------

void CrearDisco(int size, char *unit, char *path){

    int disk_size;

    char unit_char = *unit;

    char* NombreArchivo;
    NombreArchivo=(char *)malloc(100*sizeof(char));

    char *RUTAV;
    RUTAV=(char *)malloc(100*sizeof(char));

    char *RUTAN;
    RUTAN=(char *)malloc(100*sizeof(char));

    int Error;
    Error =0;

    if (size <= 0) {

        printf("ERROR TAMAÑO DISCO DEBE SER MAYOR A 0\n");
        Error = 1;

    }

    if (unit_char=='k') {

        disk_size = size*1024;

    }else if (unit_char=='m') {

        disk_size = size*1024*1024;

    }else {


        printf("ERROR! VALOR PARAMETRO NO VALIDO \n");
        Error =1;

    }

    if (Error !=1) {


        //---------------------------------------------------------


        char token[100];
        strcpy(token,path);

        int TengoQueCrear=0;

        if (token[0]=='\"') {

            TengoQueCrear =1;
            strcpy(RUTAN,path);

        }else{
            TengoQueCrear =0;
            strcpy(RUTAV,path);

        }

        if (TengoQueCrear == 1) {

            char *RutaCarpetas;
            RutaCarpetas=(char *)malloc(100*sizeof(char));

            char *pch;
            pch = strtok (RUTAN,"/\n\"");
            int i=0;

            char *arreglo[25];

            while (pch != NULL)
            {
                //printf ("%s\n",pch);
                arreglo[i]={pch};
                pch = strtok (NULL, "/\n\"");
                i++;
            }

            int t=i-1;

            strcpy(NombreArchivo,arreglo[t]);

            printf ("%s\n",NombreArchivo);


            //-------------------------------------------------
            //-------------- GET RUTA CARPETAS ----------------
            //-------------------------------------------------

            strcpy(RutaCarpetas, "" );

            strcat(RutaCarpetas,"\"");
            strcat(RutaCarpetas,"/");

            for (int z = 0; z < t; z++) {

                strcat(RutaCarpetas,arreglo[z]);
                strcat(RutaCarpetas,"/");

            }
            strcat(RutaCarpetas,"\"");

            printf("--- RutaCarpetas: %s\n",RutaCarpetas);


            //----------------------------------------------
            //-------------- CREAR CARPETAS ----------------
            //----------------------------------------------

            char comando[150];
            strcpy(comando,"sudo mkdir -p ");
            strcat(comando,RutaCarpetas);

            system(comando);

            char comnad[150];
            strcpy(comnad,"sudo chmod 777 ");
            strcat(comnad,RutaCarpetas);
            system(comnad);

            //------------------------------------------------
            //-------------- GET RUTA ARCHIVO ----------------
            //------------------------------------------------

            char *Ruta;
            Ruta =(char *)malloc(100*sizeof(char));

            strcpy(Ruta,path);

            char * puntero;
            puntero = strtok (Ruta,"\"");


            char *RutaChar;
            RutaChar =(char *)malloc(100*sizeof(char));

            while (puntero != NULL) {

                strcpy(RutaChar,puntero);
                puntero = strtok (NULL, "\"");

            }

            printf ("+++ RutaArchivos: %s\n",RutaChar);

            FILE *archivo;

            archivo = fopen(RutaChar,"wb+");

            if (archivo == NULL){
                printf("\nError de escritura del archivo. \n\n");
            }else{

                //printf("%d\n",disk_size);
                //printf("%s\n",output);

                char a= '\0';
                int w;
                struct MBR NuevoDisco;

                if (unit_char=='k') {

                    for (w = 0; w < size*1024; w++) {
                        fwrite(&a, 1, 1, archivo);
                    }

                }else if (unit_char=='m') {

                    for (w = 0; w < size*1024*1024; w++) {
                        fwrite(&a, 1, 1, archivo);
                    }

                }


                NuevoDisco.mbr_tamanio = disk_size;

                time_t tiempo = time(0);
                struct tm *tlocal = localtime(&tiempo);
                char output[128];
                strftime(output,128,"%d/%m/%y %H:%M:%S",tlocal);
                strcpy(NuevoDisco.mbr_fecha_creacion,output);

                NuevoDisco.mbr_disk_signature=10+(rand()%991);

                int x;
                for (x = 0; x < 4; x++) {
                    NuevoDisco.prt[x].part_status = '0';
                    NuevoDisco.prt[x].part_start=0;
                    NuevoDisco.prt[x].part_size=0;
                };


                fseek(archivo, 0, SEEK_SET);
                fwrite(&NuevoDisco, sizeof (MBR), 1, archivo);



            }
            fclose(archivo);


        }else {

            printf("--- RUTA ARCHIVO %s\n",RUTAV);


            FILE *archivo;

            archivo = fopen(RUTAV,"wb+");

            if (archivo == NULL){
                printf("\nError de escritura del archivo. \n\n");
            }else{

                //printf("%d\n",disk_size);
                //printf("%s\n",output);

                char a= '\0';
                int w;
                struct MBR NuevoDisco;

                if (unit_char=='k') {

                    for (w = 0; w < size*1024; w++) {
                        fwrite(&a, 1, 1, archivo);
                    }

                }else if (unit_char=='m') {

                    for (w = 0; w < size*1024*1024; w++) {
                        fwrite(&a, 1, 1, archivo);
                    }

                }


                NuevoDisco.mbr_tamanio = disk_size;

                time_t tiempo = time(0);
                struct tm *tlocal = localtime(&tiempo);
                char output[128];
                strftime(output,128,"%d/%m/%y %H:%M:%S",tlocal);
                strcpy(NuevoDisco.mbr_fecha_creacion,output);

                NuevoDisco.mbr_disk_signature=10+(rand()%991);

                int x;
                for (x = 0; x < 4; x++) {
                    NuevoDisco.prt[x].part_status = '0';
                    NuevoDisco.prt[x].part_start=0;
                    NuevoDisco.prt[x].part_size=0;
                };


                fseek(archivo, 0, SEEK_SET);
                fwrite(&NuevoDisco, sizeof (MBR), 1, archivo);



            }
            fclose(archivo);


        }

    }else {


        printf("NO SE PUDO CREAR DISCO\n");

    }



}

void EliminarDisco(char *path){

    FILE *file;

    file = fopen(path, "rb");
    printf( "fichero: %s\n", path);
    if (file != NULL) {
        fclose(file);
        char des;
        printf("\n______________________________________________________\nSeguro que desea eliminar?[s - aceptar]\n");
        scanf("%c",&des);

        if(des=='s'){
            if (remove(path) == 0)
                printf("\nBorrado %s\n", path);
            else printf("\nNo pudo ser borrado\n");
        }else{
            printf("CANCELADA la eliminacion\n");
        }



    } else printf("DISCO no encontrado\n");



}






