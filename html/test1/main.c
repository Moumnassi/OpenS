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
  int box_x= 920;
  int box_y=200;

  /* Allocate the image: 64 pixels across by 64 pixels tall */
  //im = gdImageCreate(64, 64);
  //im = gdImageCreate(tab_Time_in_min[N_lignes], 60);
  im = gdImageCreate(box_x, box_y);


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

  gdImageFilledRectangle(im, 0,0, box_x, box_y, white);

  /* Draw a line from the upper left to the lower right,
    using white color index. */
  //gdImageLine(im, 0, box_y-0, 0, box_y-box_y, white);
  //gdImageLine(im, 0, box_y-1, box_x, box_y-1, white);

  // Legendes y
  int yy = floor(box_y/50);
  int axe_y = 0;
  char graduation [10];
  for(int i=0; i <= yy; i++)
  {

      gdImageLine(im, 0, axe_y, box_x, axe_y, grey);
      /*sprintf(graduation, "%d", axe_y);
      printf("graduation = %s", graduation);
      gdImageString (im,gdFontGetLarge(),20,axe_y,graduation,black);*/
      gdImageSetThickness (im,2);
      gdImageLine(im, 0, axe_y, 50, axe_y, black);
      axe_y += 50;

  }
       //gdImageLine(im, 0, 100, 450, 100, grey);
       //gdImageLine(im, 0, 200, 450, 200, grey);
       //gdImageLine(im, 0, 300, 450, 300, grey);
       //gdImageLine(im, 0, 400, 450, 400, grey);
       //gdImageLine(im, 0, 450, 450, 450, grey);

       // Legendes x
  int xx = floor(box_x/60);
  int axe_x = 0;
  char graduation_x[10];
  char graduation_x_h[10];
  char H = 'H';
  for(int i=0; i <= xx ; i++)
  {

      gdImageLine(im, axe_x , 0 , axe_x , box_y, grey);
      gdImageSetThickness (im,2);
      gdImageLine(im, axe_x, 0, axe_x, 20, black);
      axe_x += 60;

      sprintf(graduation_x, "%d", axe_x);
      printf("graduation = %s", graduation_x);
      gdImageString (im,gdFontGetLarge(),axe_x,20,graduation_x,black);

      int heure = i+18;
      if (heure < 24)
      {
      sprintf(graduation_x_h, "%d", heure);
      printf("graduation = %s", graduation_x_h);
      // append H to graduation_x_h
          strncat(graduation_x_h, &H, 1);
      gdImageString (im,gdFontGetLarge(),axe_x,40,graduation_x_h,black);
      }
      else {
          sprintf(graduation_x_h, "%d", heure-24);
          printf("graduation = %s", graduation_x_h);
          // append H to graduation_x_h
              strncat(graduation_x_h, &H, 1);
          gdImageString (im,gdFontGetLarge(),axe_x,40,graduation_x_h,black);
      }

  }
       /*gdImageLine(im, 0, 0, 0, 450, grey);
       gdImageLine(im, 50, 0 ,50, 450, grey);
       gdImageLine(im, 100, 0,100, 450, grey);
       gdImageLine(im, 150, 0, 150, 450, grey);
       gdImageLine(im, 200, 0, 200, 450, grey);
       gdImageLine(im, 250, 0, 250, 450, grey);
       gdImageLine(im, 300, 0, 300, 450, grey);
       gdImageLine(im, 350, 0, 350, 450, grey);
       gdImageLine(im, 400, 0, 400, 450, grey);
       gdImageLine(im, 450, 0, 450, 450, grey);*/

/*
  //Discretisation des axes x
    int x=0;
    for (x=0; x < 9 ; x++)
    {
    gdImageSetThickness (im,2);
    gdImageLine(im, x*50, 0, x*50, 10, black);
    }

    //Discretisation des axes y
      int y=0;
      char graduation1 [3000];
      for (y=1; y < 8 ;y++)
      {
      //graduation1 = y100;
      sprintf(graduation1, "%d", y*100);
      printf("graduation = %s", graduation1);
      gdImageString (im,gdFontGetLarge(),20,y*100,graduation1,black);
      gdImageSetThickness (im,2);
      gdImageLine(im, 0, y*100, 10, y*100, black);
      }

*/


    char xlabel[] = "--Hour--";
    char max_flights[] = "Max_flights = 65";
    char min_flights[] = "Min_flights = 1";
    gdImageString (im,gdFontGetLarge(),358,65,xlabel,black);
    gdImageString (im,gdFontGetLarge(),600,75,max_flights,black);
    gdImageString (im,gdFontGetLarge(),600,150,min_flights,black);

       //valeurs seuil ok NOK
       gdImageDashedLine(im, 0, 0.7*(box_y-65), box_x, 0.7*(box_y-65), red);
       gdImageDashedLine(im, 0, 0.7*(box_y-1), box_x, 0.7*(box_y-1), blue);









  int cont=0;
  for (cont=0; cont < N_lignes-1 ; cont++)
  {
    gdImageSetThickness (im,5);
    gdImageLine(im, tab_Time_in_min[cont], 0.7*(box_y-tab_velocity[cont]), tab_Time_in_min[cont+1], 0.7*(box_y-tab_velocity[cont+1]), green);
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
