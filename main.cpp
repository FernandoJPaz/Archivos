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
void CrearParticion(int size, char *unit , char*path, char *type, char *fit, char*name);
int MontarDisco(char path[50], char name[16]);

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

int numerosDiscosmontados;
int numerosPartismontadas;

int main(){

    numerosDiscosmontados = 0;
    numerosPartismontadas = 0;

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
        //printf ("LLL: %s\n",t);

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


    char *ext;
    ext=(char *)malloc(100*sizeof(char));



    char * punteroPath;
    punteroPath = strtok (path,"\"\n");

    while (punteroPath != NULL)
    {

        strcpy(ext,punteroPath);
        punteroPath = strtok (NULL, "\"\n");
    }

    strcpy(path,ext);


    char *extension;
    extension=(char *)malloc(100*sizeof(char));

    char * punteroName;
    punteroName = strtok (name,"\"\n");

    while (punteroName != NULL)
    {

        strcpy(extension,punteroName);
        punteroName = strtok (NULL, "\"\n");
    }

    strcpy(name,extension);


    for (int var = 0; var < i; ++var) {


        if (strcmp(arreglo[var],"delete")==0) {

            printf("--->FDISK DELETE:%s NAME:%s PATH:%s \n",delet,name,path);
            d=1;

        }else if (strcmp(arreglo[var],"add")==0) {

            printf("--->FDISK ADD:%d UNIT:%s NAME:%s PATH:%s \n",add,unit,name,path);
            a=1;

        }

    }

    if (d==0 && a==0) {
        printf("--->FDISK: SIZE:%d UNIT:%s PATH:%s TYPE:%s FIT:%s NAME:%s \n",size,unit,path,type,fit,name);
        CrearParticion(size,unit,path,type,fit,name);
    }


}

void ScriptMount(char *script){

    char *name;
    char *path;

    name=(char *)malloc(100*sizeof(char));
    path=(char *)malloc(100*sizeof(char));



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
    puntero = strtok(token,"->-\n#");

    char *arreglo[25];
    int i=0;

    while (puntero!=NULL) {

        arreglo[i]={puntero};
        //        printf ("token: %s\n",puntero);
        puntero = strtok(NULL,"->-\n#");
        i++;
    }


    char *arreglo1[25];
    for (int x = 0; x < i; x++) {

        char *t = limpiar(arreglo[x]);

        arreglo1[x]={t};
        //printf ("LLL: %s\n",t);

    }





    for (int var = 0; var < i; var++) {

        if (strcmp(arreglo1[var],"path")==0) {

            path = arreglo1[var+1];

        }else if (strcmp(arreglo1[var],"name")==0) {

            name = arreglo1[var+1];

        }
    }

    char * punteroPath;
    punteroPath = strtok (path,"\"\n");

    char *ext;
    ext=(char *)malloc(100*sizeof(char));

    while (punteroPath != NULL)
    {

        strcpy(ext,punteroPath);
        punteroPath = strtok (NULL, "\"\n");
    }

    strcpy(path,ext);

    printf("--->MOUNT: PATH:%s NAME:%s \n",path,name);
    MontarDisco(path,name);





}

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

void CrearParticion(int size,char *unit ,char*path,char *type,char *fit, char*name){

    char type_char = *type;
    char fit_char = *fit;
    char unit_char = *unit;

    int supera = 0;

    int bitini;
    int bitMAX;
    bitini = sizeof (MBR);
    // printf("%c\n", unit);
    int tamPartiREAL;

    if (unit_char == 'b') {
        tamPartiREAL = size;
    } else if (unit_char == 'k') {
        tamPartiREAL = size * 1024;
    } else if (unit_char == 'm') {
        tamPartiREAL = size * 1024 * 1024;
    } else {
        unit_char = 'k';
        tamPartiREAL = size * 1024;
    }

    printf("TAMANO PARTICION%d\n",tamPartiREAL);


    FILE *fichero;
    fichero = fopen(path, "rb");
    if (fichero) {
        // printf("ABERTO\n");
        //...........................................................recuperar mbr [ ] { }
        int numeropartis;
        numeropartis = 0;
        int numeroP;
        numeroP = 0;
        int numeroE;
        numeroE = 0;
        int numeroL;
        numeroL = 0;

        MBR mbrerecuperado;
        fseek(fichero, 0, SEEK_SET);


        fread(&mbrerecuperado, sizeof (MBR), 1, fichero);
        int x;

        for (x = 0; x < 4; x++) {
            //printf( "STATUS %c\n",mbrerecuperado.prt[x].status );
            if (mbrerecuperado.prt[x].part_status == '1') {
                numeropartis++;
                if (mbrerecuperado.prt[x].part_type == 'p') {
                    numeroP++;
                } else if (mbrerecuperado.prt[x].part_type == 'e') {
                    numeroE++;
                } else if (mbrerecuperado.prt[x].part_type == 'l') {
                    numeroL++;
                }
            }
        };
        fclose(fichero);
        //************************************************************************************************
        //  printf( "DISCO:%i\tTAMANIO DISCO:%i\n", mbrerecuperado.disk_signature,mbrerecuperado.tamanio);
        //printf( "numPartis %i,Bit inicial:%i\n",numeropartis,bitini);
        //************************************************************************************************  [ ] { }

        if (tamPartiREAL < mbrerecuperado.mbr_tamanio) {
            if ((numeroP + numeroE) <= 3 || type_char == 'l') {
                if (type_char == 'p') {

                    printf("PRIMARIA\n");
                    int i;
                    int iguales;
                    iguales = 0;
                    int tama;
                    tama = (unsigned) strlen(name);
                    // printf("tam %i\n",tama);
                    for (i = 0; i < 4; i++) {

                        if (strncmp(name, mbrerecuperado.prt[i].name, tama)) {
                            if (mbrerecuperado.prt[i].part_status == '0') {
                                if (mbrerecuperado.prt[i].part_size == 0) {
                                    if (tamPartiREAL + bitini < mbrerecuperado.mbr_tamanio) {
                                        mbrerecuperado.prt[i].part_fit = fit_char;
                                        strcpy(mbrerecuperado.prt[i].name, name);
                                        mbrerecuperado.prt[i].part_size = tamPartiREAL;
                                        mbrerecuperado.prt[i].part_start = bitini;
                                        mbrerecuperado.prt[i].part_status = '1';
                                        mbrerecuperado.prt[i].part_type = 'p';

                                        //  crearRaiz(path,name);
                                        break;
                                    } else {
                                        printf("SUPERA EL TAMAÑO DEL DISCO, NO SE PUEDE CREAR\n");
                                        supera = 1;
                                        break;
                                    }
                                } else {
                                    if (tamPartiREAL <= mbrerecuperado.prt[i].part_size) {
                                        if (tamPartiREAL + bitini < mbrerecuperado.mbr_tamanio) {
                                            mbrerecuperado.prt[i].part_fit = fit_char;
                                            strcpy(mbrerecuperado.prt[i].name, name);
                                            mbrerecuperado.prt[i].part_size = tamPartiREAL;
                                            mbrerecuperado.prt[i].part_start = bitini;
                                            mbrerecuperado.prt[i].part_status = '1';
                                            mbrerecuperado.prt[i].part_type = 'p';
                                            break;
                                        } else {
                                            printf("SUPERA EL TAMAÑO DEL DISCO, NO SE PUEDE CREAR\n");
                                            supera = 1;
                                            break;
                                        }

                                    } else {
                                        printf("CONTINUE AL SIGUIENTE ESPACIO\n");
                                        bitini = bitini + (mbrerecuperado.prt[i].part_size);
                                    }
                                }


                            } else {
                                bitini = bitini + (mbrerecuperado.prt[i].part_size);
                            }
                        } else {
                            if (mbrerecuperado.prt[i].part_status == '1') {
                                iguales = 1;
                            }

                            break;
                        }
                    }

                    if (supera == 0) {
                        if (iguales == 0) {
                            // ACTUALIZAR MBR

                            printf("\n PARTICION Creada::name:%s::\tsize:%d::\tunit:%c::\ntype:%c::\t\tFit:%c::\tpathDisc:%s::\tBitInicial:%i::\n",
                                   mbrerecuperado.prt[i].name,
                                   mbrerecuperado.prt[i].part_size,
                                   unit_char,
                                   mbrerecuperado.prt[i].part_type, mbrerecuperado.prt[i].part_fit, path, mbrerecuperado.prt[i].part_start);
                            if (mbrerecuperado.prt[i].part_status == '1') {
                                FILE *fichero2;
                                fichero2 = fopen(path, "rb+");
                                if (fichero2) {
                                    printf("ABERTO2\n");
                                    fseek(fichero, 0, SEEK_SET);
                                    fwrite(&mbrerecuperado, sizeof (MBR), 1, fichero2);
                                    fclose(fichero2);



                                }
                            }
                        } else {
                            printf("PARTICION YA EXISTE CON ESE NOMBRE   nombre: %s    tamaño: %i\n", mbrerecuperado.prt[i].name, mbrerecuperado.prt[i].part_size);
                        }
                    }




                } else if (type_char == 'e') {
                    if (numeroE == 0) {
                        printf("EXTENDIDA\n");
                        //-------------------------------------------------------------------------------------------------EXTENDIDA[ ] { }
                        int i;
                        int iguales;
                        iguales = 0;
                        int tama;
                        tama = (unsigned) strlen(name);
                        printf("tam %i\n", tama);
                        for (i = 0; i < 4; i++) {
                            if (strncmp(name, mbrerecuperado.prt[i].name, tama)) {
                                if (mbrerecuperado.prt[i].part_status == '0') {
                                    if (mbrerecuperado.prt[i].part_size == 0) {
                                        if (tamPartiREAL + bitini < mbrerecuperado.mbr_tamanio) {
                                            mbrerecuperado.prt[i].part_fit = fit_char;
                                            strcpy(mbrerecuperado.prt[i].name, name);
                                            mbrerecuperado.prt[i].part_size = tamPartiREAL;
                                            mbrerecuperado.prt[i].part_start = bitini;
                                            mbrerecuperado.prt[i].part_status = '1';
                                            mbrerecuperado.prt[i].part_type = 'e';
                                            break;
                                        } else {
                                            printf("SUPERA EL TAMAÑO DEL DISCO, NO SE PUEDE CREAR\n");
                                            supera = 1;
                                            break;
                                        }
                                    } else {
                                        if (tamPartiREAL <= mbrerecuperado.prt[i].part_size) {
                                            mbrerecuperado.prt[i].part_fit = fit_char;
                                            strcpy(mbrerecuperado.prt[i].name, name);
                                            mbrerecuperado.prt[i].part_size = tamPartiREAL;
                                            mbrerecuperado.prt[i].part_start = bitini;
                                            mbrerecuperado.prt[i].part_status = '1';
                                            mbrerecuperado.prt[i].part_type = 'e';
                                            break;
                                        } else {
                                            printf("CONTINUE AL SIGUIENTE ESPACIO\n");
                                            bitini = bitini + (mbrerecuperado.prt[i].part_size);
                                        }
                                    }
                                } else {
                                    bitini = bitini + (mbrerecuperado.prt[i].part_size);
                                }
                            } else {
                                if (mbrerecuperado.prt[i].part_status == '1') {
                                    iguales = 1;
                                }
                                break;
                            }
                        }

                        if (supera == 0) {
                            if (iguales == 0) {

                                printf("\n PARTICION Creada::name:%s::\tsize:%i::\tunit:%c::\ntype:%c::\t\tFit:%c::\tpathDisc:%s::\tBitInicial:%i::\n",
                                       mbrerecuperado.prt[i].name,
                                       mbrerecuperado.prt[i].part_size,
                                       unit_char,
                                       mbrerecuperado.prt[i].part_type, mbrerecuperado.prt[i].part_fit, path, mbrerecuperado.prt[i].part_start);
                                if (mbrerecuperado.prt[i].part_status == '1') {
                                    FILE *fichero2;
                                    fichero2 = fopen(path, "rb+");
                                    if (fichero2) {

                                        fseek(fichero, 0, SEEK_SET);
                                        fwrite(&mbrerecuperado, sizeof (MBR), 1, fichero2);

                                        EBR ebrLOGIC;
                                        ebrLOGIC.part_status = '0';
                                        ebrLOGIC.part_start = bitini;
                                        ebrLOGIC.part_next = -1;
                                        fseek(fichero, bitini, SEEK_SET);
                                        fwrite(&ebrLOGIC, sizeof ( EBR), 1, fichero2);
                                        fclose(fichero2);
                                        printf("Logico Creado en el bit: %i\n", ebrLOGIC.part_start);

                                    }
                                }
                            } else {
                                printf("PARTICION YA EXISTE CON ESE NOMBRE\n");
                            }
                        } else {
                            printf("YA EXISTE UNA PARTICION EXTENDIDA\n");
                        }
                    }
                } else if (type_char == 'l') {










                    //--------------------------------------------------------------------------------------------LOGICA [ ] { }
                    printf("LOGICA\n");
                    if (numeroE != 0) {
                        int i;
                        int iguales;
                        iguales = 0;
                        int tama;
                        tama = (unsigned) strlen(name);
                        // printf("tam %i\n",tama);
                        for (i = 0; i < 4; i++) {
                            if (strncmp(name, mbrerecuperado.prt[i].name, tama)) {
                                if (mbrerecuperado.prt[i].part_type == 'e') {
                                    bitini = mbrerecuperado.prt[i].part_start; // AQUI EMPIEAZ LA EXTENDIDA  [ ] { }
                                    bitMAX = mbrerecuperado.prt[i].part_start + mbrerecuperado.prt[i].part_size;
                                    // printf("INI logicas: %i Bit Max: %i \n", bitini, bitMAX);
                                    break;
                                }
                            } else {
                                iguales = 1;
                                break;
                            }
                        }

                        if (iguales == 0) {

                            //VAMOS  A ENCONTRAR EL BIT INICIAL; [ ] { }
                            FILE *fichero3;
                            fichero3 = fopen(path, "rb");
                            EBR ebrLOGIC;

                            fseek(fichero3, bitini, SEEK_SET);
                            fread(&ebrLOGIC, sizeof (EBR), 1, fichero3);
                            // printf("Logica::%s:: tamaño %i status %c\n\n\n", ebrLOGIC.name, ebrLOGIC.size, ebrLOGIC.status);
                            if (ebrLOGIC.part_status == '0') {
                                // ESTA EL EBR PERO NO HAY PARTI


                                if (bitini + tamPartiREAL > bitMAX) {
                                    printf("Logica ya no cabe \n");
                                    iguales = 1;
                                } else {
                                    ebrLOGIC.part_fit = fit_char;
                                    strcpy(ebrLOGIC.name, name);
                                    ebrLOGIC.part_next = -1;
                                    ebrLOGIC.part_size = tamPartiREAL;
                                    ebrLOGIC.part_start = bitini;
                                    ebrLOGIC.part_status = '1';
                                }
                            } else {
                                // YA ESTA LA PRIMERA PARTI Y  BUSCA EL SIG ESPACIO
                                bitini = bitini + sizeof (EBR) + ebrLOGIC.part_size;

                                while (ebrLOGIC.part_next != -1) {
                                    // printf("%i\n", bitini);
                                    fseek(fichero3, bitini, SEEK_SET);
                                    fread(&ebrLOGIC, sizeof (EBR), 1, fichero);
                                    bitini = bitini + sizeof (EBR) + ebrLOGIC.part_size;
                                    //  printf("%i\n", bitini);
                                }
                                fclose(fichero3);
                                //  printf("%i\n", bitini);



                                if (bitini + tamPartiREAL > bitMAX) {
                                    printf("Logica ya no cabe \n");
                                    iguales = 1;
                                } else {

                                    int bitAnterior;
                                    bitAnterior = bitini - sizeof (EBR) - ebrLOGIC.part_size;
                                    // printf("%i\n", bitAnterior);
                                    //ACTUALIZAR

                                    FILE *fichero31;
                                    fichero31 = fopen(path, "rb+");
                                    EBR ebrLOGIC2;
                                    fseek(fichero31, bitAnterior, SEEK_SET);
                                    fread(&ebrLOGIC2, sizeof (EBR), 1, fichero);
                                    //printf("Logica::%s:: tamaño %i status %c\n\n\n", ebrLOGIC2.name, ebrLOGIC2.size, ebrLOGIC2.status);
                                    ebrLOGIC2.part_next = bitini;
                                    fseek(fichero31, bitAnterior, SEEK_SET);
                                    fwrite(&ebrLOGIC2, sizeof (EBR), 1, fichero31);
                                    fclose(fichero31);

                                    ebrLOGIC.part_fit = fit_char;


                                    strcpy(ebrLOGIC.name, name);
                                    printf("name %s\n", ebrLOGIC.name);
                                    ebrLOGIC.part_next = -1;
                                    ebrLOGIC.part_size = tamPartiREAL;
                                    ebrLOGIC.part_status = '1';
                                    ebrLOGIC.part_start = bitini;
                                    //printf("INICIAL %i\n", bitini);
                                }
                            }
                            if (iguales == 0) {
                                FILE *fichero32;
                                fichero32 = fopen(path, "rb+");
                                fseek(fichero32, bitini, SEEK_SET);
                                fwrite(&ebrLOGIC, sizeof (EBR), 1, fichero32);
                                fclose(fichero32);
                                int cantidad;
                                cantidad = sizeof (EBR);

                                printf("Logica creada %s Tamanio: %i Tamnio EBR: %i Bit inicio: %i\n", ebrLOGIC.name, ebrLOGIC.part_size, cantidad, bitini);
                            }


                        } else {
                            printf("PARTICION YA EXISTE CON ESE NOMBRE\n");
                        }



                    } else {
                        printf("DEBE CREAR UNA PARTICION EXTENDIDA\n");
                    }
                } else {
                    printf("TIPO DESCONOCIDO\n");
                }


            } else {
                printf("YA TIENE 4 PARTICIONES PRIMARIAS Y EXTENDIDAS\n");
            }
        } else {
            printf("LA PARTICION NO PUEDE SER MAYOR QUE EL TAMANIO DE DISCO\n");
        }


    } else {
        printf("DISCO NO SE ENCONTRO\n");
    }


}

//----------------------------------------------

typedef struct {
    char nombre[100];
    char letra;
    int cantidad;
} ARRAYDISCO;

typedef struct {
    char nombre[100];
    char disco[100];
    char id[10];
    int estado;
} ARRAYPARTI;

ARRAYDISCO discos[28];
ARRAYPARTI partis[28];

char letraqToca(int numeroPARTIS) {

    if (numeroPARTIS == 0) {
        return 'a';
    } else if (numeroPARTIS == 1) {
        return 'b';
    } else if (numeroPARTIS == 2) {
        return 'c';
    } else if (numeroPARTIS == 3) {
        return 'd';
    } else if (numeroPARTIS == 4) {
        return 'e';
    } else if (numeroPARTIS == 5) {
        return 'f';
    } else if (numeroPARTIS == 6) {
        return 'g';
    } else if (numeroPARTIS == 7) {
        return 'h';
    } else if (numeroPARTIS == 8) {
        return 'i';
    } else if (numeroPARTIS == 9) {
        return 'j';
    } else if (numeroPARTIS == 10) {
        return 'k';
    } else if (numeroPARTIS == 11) {
        return 'l';
    } else if (numeroPARTIS == 12) {
        return 'm';
    } else if (numeroPARTIS == 13) {
        return 'n';
    } else if (numeroPARTIS == 14) {
        return 'o';
    } else if (numeroPARTIS == 15) {
        return 'p';
    } else if (numeroPARTIS == 16) {
        return 'q';
    } else if (numeroPARTIS == 17) {
        return 'r';
    } else if (numeroPARTIS == 18) {
        return 's';
    } else if (numeroPARTIS == 19) {
        return 't';
    } else if (numeroPARTIS == 20) {
        return 'u';
    } else if (numeroPARTIS == 21) {
        return 'v';
    } else if (numeroPARTIS == 22) {
        return 'w';
    } else if (numeroPARTIS == 23) {
        return 'x';
    } else if (numeroPARTIS == 24) {
        return 'y';
    } else if (numeroPARTIS == 25) {
        return 'z';
    } else {
        return 'z';
    }

}

int MontarDisco(char path[50], char name[16]) {
    int hayExtendida;
    int bitInicioExtendida;
    hayExtendida = 0;
    bitInicioExtendida = 0;
    FILE *fichero;
    fichero = fopen(path, "rb");

    if (fichero) {
        MBR mbrerecuperado;
        fseek(fichero, 0, SEEK_SET);
        fread(&mbrerecuperado, sizeof (MBR), 1, fichero);
        fclose(fichero);
        int i;
        int iguales;
        for (i = 0; i < 4; i++) {
            if (mbrerecuperado.prt[i].part_type == 'e') {
                hayExtendida = 1;
                bitInicioExtendida = mbrerecuperado.prt[i].part_size;
            }
            if (mbrerecuperado.prt[i].part_status == '1') {
                iguales = 0;
                int tama;
                tama = (unsigned) strlen(name);
                int tama2;
                tama2 = (unsigned) strlen(mbrerecuperado.prt[i].name);
                // printf("tam %i\n",tama);
                int i2;
                //COM CHAR CON CHAR
                for (i2 = 0; i2 < tama; i2++) {
                    if (mbrerecuperado.prt[i].name[i2] != name[i2]) {
                        iguales = 1;
                    }
                }
                for (i2 = 0; i2 < tama2; i2++) {
                    if (mbrerecuperado.prt[i].name[i2] != name[i2]) {
                        iguales = 1;
                    }
                } // 1 CUANDO NO SON IGUALES
                if (iguales == 0) {
                    break;
                }
            }
        }
        if (iguales == 0) { // EL 1 ES Q NO SON IGUALES
            //   printf("Particion Montada::%s::\tPath:%s::\n",namePARTI,pathDISC);
            int ii;
            if (numerosDiscosmontados == 0) {
                for (ii = 0; ii < 28; ii++) {
                    discos[ii].cantidad == 1;
                }
                //  printf("cantidad %i\n",   discos[0].cantidad);
                //AGREGO EL PRIMER DISCO EN EL ARRAY DE DISCOS
                discos[0].letra = letraqToca(discos[0].cantidad);
                // printf("%c\n" ,discos[0].letra);
                discos[0].cantidad++;
                strcpy(discos[0].nombre, path);
                //   printf("cantidad %i\n",   discos[0].cantidad);
                //AGREGO EL PRIMER PARTICION
                strcpy(partis[0].nombre, name);
                strcpy(partis[0].disco, path);
                partis[0].id[0] = 'v';
                partis[0].id[1] = 'd';
                partis
                        [0].id[2] = discos[0].letra;
                char b[2];
                sprintf(b, "%d", discos[0].cantidad);
                partis[0].id[3] = b[0];
                partis[0].estado = 1;
                printf("numero montura::%i:: MONTADA PARTI::::%s:: con el ID::%s::\n", numerosPartismontadas, partis[numerosPartismontadas].nombre, partis[numerosPartismontadas].id);
                numerosDiscosmontados++;
                numerosPartismontadas++;
            } else {
                // QUE LA PARTICION NO ESTE MONTADA
                for (i = 0; i < numerosPartismontadas; i++) {
                    iguales = 0;
                    int tama; // PARTICION
                    tama = (unsigned) strlen(name);
                    int tama2;
                    tama2 = (unsigned) strlen(partis[i].nombre);
                    // printf("tam %i\n",tama);
                    int i2;
                    //COM CHAR CON CHAR
                    for (i2 = 0; i2 < tama; i2++) {
                        if (partis[i].nombre[i2] != name[i2]) {
                            iguales = 1;
                        }
                    }
                    for (i2 = 0; i2 < tama2; i2++) {
                        if (partis[i].nombre[i2] != name[i2]) {
                            iguales = 1;
                        }
                    }
                    // 1 CUANDO NO SON IGUALES
                    //                 DISCO
                    tama = (unsigned) strlen(path);
                    // int tama2;
                    tama2 = (unsigned) strlen(partis[i].disco);
                    // printf("tam %i\n",tama);
                    //      int i2;
                    //COM CHAR CON CHAR
                    for (i2 = 0; i2 < tama; i2++) {
                        if (partis[i].disco[i2] != path[i2]) {
                            iguales = 1;
                        }
                    }
                    for (i2 = 0; i2 < tama2; i2++) {
                        if (partis[i].disco[i2] != path[i2]) {
                            iguales = 1;
                        }
                    } // 1 CUANDO NO SON IGUALES
                    if (iguales == 0) {
                        if (partis[i].estado == 1) {
                            break;
                        } else {
                            iguales = 1;
                        }
                    }
                }
                if (iguales == 0) {
                    printf("PARTICION YA SE ENCUANTRA MONADA:: %s:: CON EL ID:: %s\n", name, partis[i].id);

                } else {
                    //  BUSCAR QUE EL DISCO YA ESTE MONTADO Y Q TENGA LETRA
                    int idondeqda;
                    for (i = 0; i < numerosDiscosmontados; i++) {
                        iguales = 0;
                        int tama;
                        tama = (unsigned) strlen(path);
                        int tama2;
                        tama2 = (unsigned) strlen(discos[i].nombre);
                        // printf("tam %i\n",tama);
                        int i2;
                        //COM CHAR CON CHAR

                        for (i2 = 0; i2 < tama; i2++) {
                            if (discos[i].nombre[i2] != path[i2]) {
                                iguales = 1;
                            }
                        }
                        for (i2 = 0; i2 < tama2; i2++) {
                            if (discos[i].nombre[i2] != path[i2]) {
                                iguales = 1;
                            }
                        } // 1 CUANDO NO SON IGUALES
                        if (iguales == 0) {
                            break;
                        }
                    }
                    if (iguales == 0) {
                        idondeqda = i;
                        printf("%i:::i:%i\n", idondeqda, i);
                        char letra;
                        letra = discos[i].letra;
                        //  int idondeqda;
                        int numero;
                        discos[idondeqda].cantidad++;
                        numero = discos[idondeqda].cantidad;
                        //GUARDO EN EL ARRAY
                        strcpy(partis[numerosPartismontadas].nombre, name);
                        strcpy(partis[numerosPartismontadas].disco, path);
                        partis[numerosPartismontadas].id[0] = 'v';
                        partis[numerosPartismontadas].id[1] = 'd';
                        partis[numerosPartismontadas].id[2] = letra;
                        char b[2];
                        sprintf(b, "%d", numero);
                        partis[numerosPartismontadas].id[3] = b[0];
                        partis[numerosPartismontadas].estado = 1;
                        printf("numero montura::%i:: MONTADA PARTI::::%s:: con el ID::%s::\n", numerosPartismontadas, partis[numerosPartismontadas].nombre, partis[numerosPartismontadas].id);
                        numerosPartismontadas++;
                    } else {
                        // VOY A MONTAR UN NUEVO DISCO
                        discos[numerosDiscosmontados].cantidad++;
                        discos[numerosDiscosmontados].letra = letraqToca(discos[numerosDiscosmontados].cantidad);
                        strcpy(discos[numerosDiscosmontados].nombre, path);
                        //GUARDO EN EL ARRAY
                        strcpy(partis[numerosPartismontadas].nombre, name);
                        strcpy(partis[numerosPartismontadas].disco, path);
                        partis[numerosPartismontadas].id[0] = 'v';
                        partis[numerosPartismontadas].id[1] = 'd';
                        partis[numerosPartismontadas].id[2] = discos[numerosDiscosmontados].letra;
                        char b[2];
                        sprintf(b, "%d", discos[numerosDiscosmontados].cantidad);
                        partis[numerosPartismontadas].id[3] = b[0];
                        partis[numerosPartismontadas].estado = 1;
                        // partis[numerosPartismontadas].id[3]=;
                        printf("numero montura::%i:: MONTADA PARTI::::%s:: con el ID::%s::\n", numerosPartismontadas, partis[numerosPartismontadas].nombre, partis[numerosPartismontadas].id);
                        numerosPartismontadas++;
                        numerosDiscosmontados++;
                    }
                }
            }
        } else {
            printf("ERROR, NO SE ENCUENTRA PARTICION A MONTAR\n ");
        }
    } else {
        printf("NO SE ENCUENTRA DISCO\n");
    }

};


