/* Bring in gd library functions */
#include "gd.h"

/* Bring in standard I/O so we can output the PNG to a file */
#include <stdio.h>

#include <sqlite3.h>
#include <stdio.h>
//https://zetcode.com/db/sqlitec/
//https://silanus.fr/sin/?page_id=111

// Pour le string
#include <gdfontl.h>
#include <gdfontg.h>
#include <string.h>
#include <math.h>


int callback(void *, int, char **, char **);



int main() {

    printf("%s\n", sqlite3_libversion());

//*************************

    sqlite3 *db;
        char *err_msg = 0;

        int rc = sqlite3_open("flight.db", &db);

        if (rc != SQLITE_OK) {

            fprintf(stderr, "Cannot open database: %s\n",
                    sqlite3_errmsg(db));
            sqlite3_close(db);

            return 1;
        }

        char *sql = "SELECT * FROM flight";

        rc = sqlite3_exec(db, sql, callback, 0, &err_msg);

        if (rc != SQLITE_OK ) {

            fprintf(stderr, "Failed to select data\n");
            fprintf(stderr, "SQL error: %s\n", err_msg);

            sqlite3_free(err_msg);
            sqlite3_close(db);

            return 1;
        }



        //***************
        //Créez   une variable de type int pour stocker le code de retour pour chaque appel
        // c'est le rc
        char   *query;
        //   Préparer l'accès aux tables
        sqlite3_stmt *stmt;

        // Lire le   contenu de la table
        query = "SELECT * from flight";
        rc =   sqlite3_prepare_v2(db,query,-1,&stmt,0);
        if(rc)
        {
        printf("Impossible de lire la table\n");
        return   -1;
        }

        // Lire le   nombre de lignes récupérées
        int cols = sqlite3_column_count(stmt);
        printf("%d \n", cols);
        // Ecriture de l'entête des colonnes
        int col;
        for(col=0 ; col<cols;col++) printf("%s \t\t |   ",sqlite3_column_name(stmt,col));
        printf("\n");

        //Ecriture   des données
        //Nombre de lignes:  N_lignes
        int N_lignes = 0;
        while(1)
        {
        //   récupérer le status de la ligne (contient de données ou fin de table)
        rc = sqlite3_step(stmt);
        if(rc == SQLITE_ROW)
        {
        //   La ligne contient des données
        int   col;
        for(col=0 ; col<cols;col++)
        {
        // sqlite3_column_text retourne un const   void* => cast en const char*
        const char *val =   (const char*)sqlite3_column_text(stmt,col);
        //printf("%s \t\t | ",val);
        }
        //printf("\n");
        N_lignes = N_lignes +1;
        }
        else if(rc == SQLITE_DONE)
        {
        //   Plus de données
        printf("Fin de la table\n");
        break;
        }
        else
        {
        //   Erreur
        printf("Erreur lors de l'accès aux données\n");
        return -1;
        }
        }

        printf("Nombre de lignes de la base .bd  : %d \n",N_lignes);




        //****************** imprimer les valeurs de la table
        int tab_Time_in_min[N_lignes];
        int tab_velocity[N_lignes];
        int con_ligne = 0;



        //Ecriture   des données
        while(1)
        {
        //   récupérer le status de la ligne (contient de données ou fin de table)
        rc = sqlite3_step(stmt);
        if(rc == SQLITE_ROW)
        {
        //   La ligne contient des données

        // sqlite3_column_double
        tab_Time_in_min[con_ligne]=   sqlite3_column_double(stmt,1);
        tab_velocity[con_ligne]=   sqlite3_column_double(stmt,2);

        //printf("Ligne numéro   : %d  \t\t |", con_ligne);
        //printf("tab_Time_in_min :%d  \t\t | ", tab_Time_in_min[con_ligne]);
        //printf("tab_velocity :%d ", tab_velocity[con_ligne]);

        //printf("\n");
        //printf("con_ligne  : %d \n",con_ligne);
        con_ligne += 1;
        }
        else if(rc == SQLITE_DONE)
        {
        //   Plus de données
        printf("Fin de la table\n");
        break;
        }
        else
        {
        //   Erreur
        printf("Erreur lors de l'accès aux données\n");
        return -1;
        }
        }







        //*****************







        sqlite3_close(db);


//**********************
        //printf("tab_Time_in_min :%d  \t\t | ", tab_Time_in_min[0]);
        //printf("tab_velocity :%d ", tab_velocity[0]);

        //printf("tab_Time_in_min :  \t\t | \n ");



  /* Declare the image */
  gdImagePtr im;
  /* Declare output files */
  FILE *pngout, *jpegout;
  /* Declare color indexes */
  int black, grey, blue_D, blue, red, green;
  int white;
  int box_x= tab_Time_in_min[N_lignes-1]*20;
  int box_y= tab_velocity[N_lignes-1]+50;

  printf("tab_velocity :%d ", tab_Time_in_min[N_lignes-1]);
printf("\n");
printf("\n");
printf("\n");


  /* Allocate the image: 64 pixels across by 64 pixels tall */
  //im = gdImageCreate(64, 64);
  //im = gdImageCreate(tab_Time_in_min[N_lignes], 60);
  im = gdImageCreate(box_x, box_y*0.7);


  /* Allocate the color black (red, green and blue all minimum).
    Since this is the first color in a new image, it will
    be the background color. */

    black = gdImageColorAllocate(im, 0, 0, 0);
    grey = gdImageColorAllocate(im, 215, 215, 215);
    blue_D = gdImageColorAllocate(im, 0, 0, 102);
    blue = gdImageColorAllocate(im, 0, 0, 255);
    red = gdImageColorAllocate(im, 255, 0, 0);
    green = gdImageColorAllocate(im, 0, 255, 0);

  /* Allocate the color white (red, green and blue all maximum). */
  white = gdImageColorAllocate(im, 255, 255, 255);

  gdImageFilledRectangle(im, 0,0, box_x, box_y*0.7, white);

  /* Draw a line from the upper left to the lower right,
    using white color index. */
  //gdImageLine(im, 0, box_y-0, 0, box_y-box_y, white);
  //gdImageLine(im, 0, box_y-1, box_x, box_y-1, white);



  // Legendes y
  int DD = 40;
  int yy = floor(box_y*0.7/DD);
  int axe_y = 0;
  char graduation [10];
  char m = 'm';
  char mm = '/';
  char s = 's';
  for(int i=0; i <= yy; i++)
  {

      gdImageLine(im, 0, axe_y, box_x, axe_y, grey);
      sprintf(graduation, "%d", box_y - axe_y);
      //printf("graduation = %s", graduation);
      strncat(graduation, &m, 1);
      strncat(graduation, &mm, 1);
      strncat(graduation, &s, 1);
      gdImageString (im,gdFontGetLarge(),20,axe_y,graduation,black);
      gdImageSetThickness (im,2);
      gdImageLine(im, 0, axe_y, DD, axe_y, black);
      axe_y += DD;

  }



  // Legendes x
  int LL= 60;
  int xx = floor(box_x/LL);
  int axe_x = 0;
  char graduation_x[10];
  char S = 's';
  for(int i=0; i <= xx ; i++)
  {

      gdImageLine(im, axe_x , 0 , axe_x , box_y*0.7, grey);
      gdImageSetThickness (im,2);
      gdImageLine(im, axe_x, 0, axe_x, 2, black);
      axe_x += LL;

      sprintf(graduation_x, "%d", axe_x/20*10);
      //printf("graduation = %s", graduation_x);
      strncat(graduation_x, &S, 1);
      gdImageString (im,gdFontGetLarge(),axe_x,2,graduation_x,black);

  }



    char xlabel[] = "--Velocity of the flight in m/s--";
    gdImageString (im,gdFontGetLarge(),box_x*0.4,box_y - 0.5*box_y,xlabel,black);












  int cont=0;
  for (cont=0; cont < N_lignes-1 ; cont++)
  {
    gdImageSetThickness (im,3);
    gdImageLine(im, tab_Time_in_min[cont]*20, box_y-tab_velocity[cont], tab_Time_in_min[cont+1]*20, box_y-tab_velocity[cont+1], green);
    /*printf("\n");
    printf("tab_Time_in_min :%d  \t\t | ", tab_Time_in_min[cont]);
    printf("\n");
    printf("tab_velocity :%d ", tab_velocity[cont]);
    printf("\n");
    printf("tab_Time_in_min + 1 :%d  \t\t | ", tab_Time_in_min[cont+1]);
    printf("\n");
    printf("tab_velocity + 1 :%d ", tab_velocity[cont+1]);
    printf("\n");*/
  }



/*


  //gdImageString
      char *h = "Time_in_min";
      char *v = "Number_of_flights";
      int foreground = 0;
      //gdFontPtr fontptr = gdFontGetLarge();
      gdFontPtr fontptr = gdFontGetGiant();

      foreground = gdImageColorAllocate(im, 0, 0, 0);

          gdImageString(im, fontptr,
                  im->sx / 2 - (strlen(h) * fontptr->w ),
                  im->sy  - fontptr->h ,
                  (unsigned char*)h, foreground);


          gdImageStringUp(im, fontptr,
                      im->sx / 200 - fontptr->h / 2,
                      im->sy / 2 + (strlen(v) * fontptr->w / 2),
                      (unsigned char*)v, foreground);


*/

  /* Open a file for writing. "wb" means "write binary", important
    under MSDOS, harmless under Unix. */
  pngout = fopen("test.png", "wb");

  /* Do the same for a JPEG-format file. */
  jpegout = fopen("test.jpg", "wb");

  /* Output the image to the disk file in PNG format. */
  gdImagePng(im, pngout);

  /* Output the same image in JPEG format, using the default
    JPEG quality setting. */
  gdImageJpeg(im, jpegout, -1);

  /* Close the files. */
  fclose(pngout);
  fclose(jpegout);

  /* Destroy the image in memory. */
  gdImageDestroy(im);
}


int callback(void *NotUsed, int argc, char **argv,
                    char **azColName) {

    NotUsed = 0;

    for (int i = 0; i < argc; i++) {

//        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }

    //printf("\n");

    return 0;
}
