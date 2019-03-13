/**
 * \file level_editor.c
 * \brief Editeur de carte
 * \author Girod Valentin
 * \date 12 mars 2019
 *
 * Contient l'éditeur de carte'
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <fonctions_sdl.h>
#include <map.h>
#include <menu_principal.h>
#include <math.h>

static float VITESSE_PERSO = 0.05; /*!< Vitesse du personnage (plus le jeu est optimisé en mémoire vive, moins il va vite, et plus il faut augmenter cette valeur) */

/**
 * \fn void showEditor(float x, float y)
 * \brief Gère l'édition de la map
 * \param[in] Abscisse du début de l'édition
 * \param[in] Ordonnée du début de l'édition
 */
void showEditor(float x, float y){
  //si on est à des coordonnées trop petites pour l'écran, on adapte
  int nbSpriteX = SCREEN_WIDTH/125;
  int nbSpriteY = SCREEN_HEIGHT/125;
  if(y-nbSpriteY/2<0) y=nbSpriteY/2;
  if(x-nbSpriteX/2<0) x=nbSpriteX/2;
  if(y+nbSpriteY/2+2>1000) y=999-(nbSpriteY/2+2);
  if(x+nbSpriteX/2+2>1000) x=999-(nbSpriteX/2+2);
  showMap( x, y);
  //x_select: les coordonées du seleteur de façon à le centrer sur l'écran
  int x_select = (SCREEN_WIDTH-1200)/2;
  drawImage( x_select, SCREEN_HEIGHT-250, "item_selector.png", 1200, 150);
  SDL_RenderPresent(renderer);
  int running = 1;
  int selected = 1;
  while(running) {
    //Au départ la gestion du clavier se faisait avec un event qui détectait l'appuis d'une touche
    //mais dès que j'ai mis cette partie du code ici sans utiliser l'event, les déplacements sont devenus bien
    //plus fluides, rapides, et surtout la latence au moment de l'appuis sur une touche a disparue
    //(quand on restait appuyé sur une touche, le permonnage restait une seconde immobile, comme quand on écrit
    //un caractère en continu)
    //l'état du clavier à l'instant actuel
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    //si c'est une touche de mouvement
    if(state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_DOWN]){
      //on gère les colisions et la vitesse du perso
      //on a besoin de regarder à quel endroit de la map on est et ce qu'il y a après
      if (state[SDL_SCANCODE_RIGHT] ) x+=VITESSE_PERSO;
      else if (state[SDL_SCANCODE_UP] ) y-=VITESSE_PERSO;
      else if (state[SDL_SCANCODE_DOWN] ) y+=VITESSE_PERSO;
      else if (state[SDL_SCANCODE_LEFT]) x-=VITESSE_PERSO;
      if(x<0) x=0;
      if(y<0) y=0;
      if(x>1000) x=999;
      if(y>1000) y=999;
      showMap( x, y);
      drawImage( x_select, SCREEN_HEIGHT-250, "item_selector.png", 1200, 150);
      SDL_RenderPresent(renderer);
    }

  	SDL_Event e;
  	while(SDL_PollEvent(&e)) {
  		switch(e.type) {
        //la gestion du clavier hors déplacements se fait ici
        case SDL_MOUSEBUTTONDOWN:
        //quand on clique, si on clique au dessus du sélecteur, on va placer une case.
        //Sinon, on regarde si on a cliqué dans une case du sélecteur.
        {
          int mouse_x, mouse_y;
          SDL_GetMouseState(&mouse_x, &mouse_y);
          //si on est dans le sélecteur
          if(mouse_y>SCREEN_HEIGHT-215){
            printf("%d, %d\n", mouse_x, mouse_y);
            //si on est à la hauteur d'une case du sélecteur
            if(mouse_y>SCREEN_HEIGHT-200 && mouse_y<SCREEN_HEIGHT){
              //la position de x sur l'image
              int pos = mouse_x-(x_select+245);
              for(int j=0,i = 1; j<pos+85 ;i++, j+=100){
                if(pos >= j && pos <= j+85){
                  //i corresponds au numéro de la case du sprite sélectionné,
                  //et aussi au numéro du sprite dans la matrice de la map,
                  //et ça c'est pratique
                  printf("%d\n", i);
                  if(i==8){
                    remove("Map.txt");
                    FILE *map_file = fopen("Map.txt", "w");
                    for(int i=0; i<1000; i++){
                      for(int j=0; j<1000; j++){
                        fprintf(map_file, "%d;", map[i][j]);
                      }
                      fprintf(map_file, "\n");
                    }

                    fclose(map_file);
                  }
                  if(i==9){
                    running=0;
                    break;
                  }
                  selected = i;
                }
              }
            }
          }
          //sinon on détecte la bonne position du sprite pour l'afficher sur l'écran, et on l'enregistre
          else{
            int h=y-SCREEN_HEIGHT/125/2;
            for(int j=0; j+125<mouse_y;j+=125, h++);
            int w=x-SCREEN_WIDTH/125/2;
            for(int j=0; j+125<mouse_x;j+=125, w++);

            //une fois qu'on a trouvé les bonnes coordonées on modifie la map pour l'afficher en direct
            map[h][w]= selected;
            showMap( x, y);
            drawImage( x_select, SCREEN_HEIGHT-250, "item_selector.png", 1200, 150);
            SDL_RenderPresent(renderer);
          }
        break;
      }
      }
    }
    SDL_Delay(5);
  }
  SDL_RenderClear(renderer);
  showMenu();
}
