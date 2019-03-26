/**
 * \file game.c
 * \brief Fonction principale du jeu
 * \author Karman Nathalie; Papot Alexandre; Girod Valentin
 * \date 12 mars 2019
 *
 * Contient le loop du jeu
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fonctions_affichage.h>
#include <affichage.h>
#include <map.h>
#include <menu_principal.h>
#include <math.h>
#include <quete.h>
#include <ncurses.h>


/**
 * \fn void startGame(float x, float y)
 * \brief Commence/charge une partie à des coordonnées données
 * \param[in] Abscisse du début de partie
 * \param[in] Ordonnée du début de partie
 */
void startGame(float x, float y){
  //si on est à des coordonnées trop petites pour l'écran, on adapte
  int nbSpriteX = SCREEN_WIDTH/125;
  int nbSpriteY = SCREEN_HEIGHT/125;
  if(y-nbSpriteY/2<0) y=nbSpriteY/2;
  if(x-nbSpriteX/2<0) x=nbSpriteX/2;
  if(y+nbSpriteY/2+2>1000) y=999-(nbSpriteY/2+2);
  if(x+nbSpriteX/2+2>1000) x=999-(nbSpriteX/2+2);
  afficher_Map( x, y);
  //x_select: les coordonées du seleteur de façon à le centrer sur l'écran
  int x_select = (SCREEN_WIDTH-1200)/2;
  faire_rendu();
  int running = 1;
  int selected = 1;

  while(running) {
    //Au départ la gestion du clavier se faisait avec un event qui détectait l'appuis d'une touche
    //mais dès que j'ai mis cette partie du code ici sans utiliser l'event, les déplacements sont devenus bien
    //plus fluides, rapides, et surtout la latence au moment de l'appuis sur une touche a disparue
    //(quand on restait appuyé sur une touche, le personnage restait une seconde immobile, comme quand on écrit
    //un caractère en continu)
    //Donc, si on a un mouvement du joueur, on affiche à nouveau la map et les quêtes
    if(detecter_mouvement(&x, &y)){
      //chargement de la map
      afficher_Map( x, y);
      afficher_quetes();
      faire_rendu();
    }
    char coor[20] = "";
    setcolor(14, 6);
    sprintf(coor, "%f, %f", x, y);
    mvaddstr(23, 20, coor);
    detecter_touches();
  }
  showMenu();
}
