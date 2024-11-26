/* explomem2.c : exploration de la memoire d'un processus
 * utilisation de l'outil de chainage des elements memoire
 * et affichage en HEXA de la zone
 * avec compilation conditionnelle pour la traduction en texte
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>

#define LBUF 50
char buf[LBUF] = "Il fait beau";

/* affichex : affichage en hexadecimal
  le libelle, l'adresse en HEXA, la difference avec l'adresse precedente,
  le contenu de la memoire a cette adresse en HEXA
  Si b vaut NULL elle retourne dans des var. globales les adresses des elts
  afin de comparer en faisant des appels differents !
 */
#define NBO 20
void *Abp, *Ai;
void affichex(char *l, void *b)
{
   unsigned long d;
   static void *bp = NULL;
   int i;
   char *B, signe = '\0', unit;
   if (b == NULL)
   {
      Abp = (void *)&bp;
      Ai = (void *)&i;
      return;
   }
   /* calcul du delta avec le coup d'avant */
   if (b > bp)
      d = b - bp;
   else
      d = bp - b;
   if (bp != NULL)
   {
      if (b < bp)
         signe = '-';
      else
         signe = '+';
   }
   bp = b; /* on memorise l'adresse */
   /* on compte le nb de fois que l'on divide par 1ko */
   i = 0;
   while (d > 1023)
   {
      d = d >> 10;
      i++;
   }
   /* en fct de n on obtient l'unite ! */
   switch (i)
   {
   case 0:
      unit = '\0';
      break;
   case 1:
      unit = 'K';
      break;
   case 2:
      unit = 'M';
      break;
   case 3:
      unit = 'G';
      break;
   case 4:
      unit = 'T';
      break;
   }
   /* affiche libelle delta adresse */
   printf("%-15s: %c%lu%c\t%lx ", l, signe, d, unit, (unsigned long)b);
   /* affiche la zone memoire en hexa + fin de ligne */
   B = (char *)b;
   for (i = 0; i < NBO; i++)
      printf("%.2X", (unsigned char)B[i]);
#ifdef WITH_TEXT
   printf("\n\t\t\t\t");
   for (i = 0; i < NBO * 2; i++)
      if (isprint((int)B[i]))
         printf("%c", B[i]);
      else
         printf("~");
#endif
   printf("\n");
}

/* le chainage des adresses memoire */
#define LNOM 15
struct elt
{                      /* stucture d'un element */
   char nom[LNOM + 1]; /* nom de l'element */
   void *addr;         /* adresse de l'element */
   struct elt *next;   /* adresse du prochain element */
};

#define FIN (struct elt *)NULL
struct elt *First = FIN; /* depart du chainage */

/* La fct ajouteElt retourne l'adresse de la memoire dynamique creee */
void *ajouteElt(char *n, void *a)
{
   void *M;
   struct elt *e, **D;
   /* creation de l'espace memoire de l'element */
   if ((M = malloc(sizeof(struct elt))) == NULL)
   {
      perror("malloc");
      exit(1);
   }
   e = (struct elt *)M;
   /* remplissage de l'element */
   strncpy(e->nom, n, LNOM);
   e->nom[LNOM] = '\0';
   e->addr = a;
   e->next = FIN;
   /* boucle sur l'adresse du suivant */
   D = &First;
   while (*D != FIN)
   {
      if ((*D)->addr > a)
      { /* insertion avant */
         e->next = *D;
         break;
      }
      D = &((*D)->next);
   }
   *D = e;
   return M;
}

/* nf indique le nom de la fct qui appelle la liste */
void listeElts(char *nf)
{
   struct elt **D;
   int n = 0;
   printf("-------- Liste dans %s ------------\n", nf);
   D = &First;
   while (*D != FIN)
   {
      /* printf("Elt %-16s : %lx\n",(*D)->nom,(unsigned long)((*D)->addr)); */
      affichex((*D)->nom, (*D)->addr);
      n++;
      D = &((*D)->next);
   }
   printf("Total %d Elts x %ld = %ld octets.\n", n, sizeof(struct elt),
          n * sizeof(struct elt));
}

int toto(int N)
{
   char buf[100];
   int A = 1, B = 2, C;

   for (C = 0; C < 100; C++)
      buf[C] = 'T';
   C = (A * N) + B;
   ajouteElt("A toto", (void *)&A);
   ajouteElt("B toto", (void *)&B);
   ajouteElt("C toto", (void *)&C);
   ajouteElt("N toto", (void *)&N);
   ajouteElt("buf toto", (void *)buf);
   affichex("", NULL);
   ajouteElt("bp affichex(2)", Abp);
   ajouteElt("i affichex(2)", Ai);
   listeElts("toto");
   return C;
}

int main(int N, char *P[])
{

   if (N < 2)
   {
      printf("Usage: %s <mode> [address]\n", P[0]);
      printf("Modes:\n");
      printf("  1: Affiche l'adresse d'un buffer global contenant 'Il fait beau'.\n");
      printf("  2: Modifie une adresse donnée avec 'La tempête arrive !'.\n");
      exit(1);
   }

   int mode = atoi(P[1]);
   if (mode == 1)
   {
      printf("Mode 1: Adresse du buffer global: %p\n", (void *)buf);
      ajouteElt("Buffer global", (void *)buf);
      affichex("Buffer global", (void *)buf);
      printf("Contenu initial: %s\n", buf);
      printf("Attente de 1 minute...\n");
      sleep(60);
      printf("Contenu après 1 minute: %s\n", buf);
   }
   else if (mode == 2)
   {
      if (N < 3)
      {
         fprintf(stderr, "Erreur: Une adresse doit être spécifiée en mode 2.\n");
         exit(1);
      }
      void *addr = (void *)strtoull(P[2], NULL, 0);
      if (addr == NULL)
      {
         fprintf(stderr, "Erreur : Adresse invalide (NULL).\n");
         exit(1);
      }

      printf("Mode 2: Modification à l'adresse %p\n", addr);
      strncpy((char *)addr, "La tempête arrive !", LBUF - 1);
      ((char *)addr)[LBUF - 1] = '\0';
      ajouteElt("Adresse modifiée", addr);
      affichex("Adresse modifiée", addr);
      printf("Contenu modifié: %s\n", (char *)addr);
   }
   else
   {
      fprintf(stderr, "Erreur: Mode invalide.\n");
      exit(1);
   }

   listeElts("main");

   return 0;
}