#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Game.h"
#include <string.h>


#define SAMPLE_MAP_DISCIPLINES {STUDENT_MMONEY, STUDENT_MJ, STUDENT_MJ, \
            	STUDENT_BQN, STUDENT_MTV, STUDENT_MJ, STUDENT_BPS,\
            	STUDENT_BPS, STUDENT_MTV, STUDENT_THD, STUDENT_BQN,\
            	STUDENT_BPS, STUDENT_MMONEY, STUDENT_MJ, STUDENT_MMONEY,\
            	STUDENT_MTV, STUDENT_BQN, STUDENT_BQN, STUDENT_MTV}
#define SAMPLE_MAP_DICE {6,10,8,3,4,9,6,11,11,7,12,3,5,2,5,10,9,4,8}

#define BADLY_DRAWN_MAP_DISCIPLINES {STUDENT_BQN, STUDENT_MMONEY, STUDENT_MJ, \
            	STUDENT_MMONEY, STUDENT_MJ, STUDENT_BPS, STUDENT_MTV, \
            	STUDENT_MTV, STUDENT_BPS,STUDENT_MTV, STUDENT_BQN, \
            	STUDENT_MJ, STUDENT_BQN, STUDENT_THD, STUDENT_MJ, \
            	STUDENT_MMONEY, STUDENT_MTV, STUDENT_BQN, STUDENT_BPS}
#define BADLY_DRAWN_MAP_DICE {9,10,8,12,6,5,3,11,3,11,4,6,4,7,9,2,8,10,5}

void testNewGame1(Game g1);
void testNewGame2(Game g2);
void testPreGameLegalActions(Game g);
void testGeneralIsLegalActions(Game g);
void testMakeActions(Game g);
void testIsLegalActionGame(Game g);
void testGO8Board(Game g);

int main (int argc, char *argv[]) {

   int sampleDisciplines[NUM_REGIONS] = SAMPLE_MAP_DISCIPLINES;
   int sampleDice[NUM_REGIONS] = SAMPLE_MAP_DICE;
   int badDisciplines[NUM_REGIONS] = BADLY_DRAWN_MAP_DISCIPLINES;
   int badDice[NUM_REGIONS] = BADLY_DRAWN_MAP_DICE;

   Game g1 = newGame(sampleDisciplines, sampleDice);
   printf("Testing initial boardstate for specfic game.\n");
   testNewGame1(g1);
   printf("Tests Passed!\n");
   printf("Testing pregame legal actions for same game.\n");
   testPreGameLegalActions(g1);
   printf("Tests Passed!\n");

   Game g2 = newGame(badDisciplines, badDice);
   printf("Testing initial boardstate for another game.\n");
   testNewGame2(g2);
   printf("Tests Passed!\n");
   printf("Testing pregame legal actions for same game.\n");
   testPreGameLegalActions(g2);
   printf("Tests Passed!\n");
   throwDice(g2, 3);
   printf("Testing legal actions for turn 1. \n");
   testGeneralIsLegalActions(g2);
   printf("Tests Passed!\n");

   disposeGame(g2);
   printf("Testing your game can create a complex boardstate.\n");
   testMakeActions(g1);
   printf("Tests Passed!\n");
   printf("Testing legal actions for complex gamestate.\n");
   testIsLegalActionGame(g1);
   printf("Testing GO8 Board\n");
   testGO8Board(g1);
   printf("Tests Passed!\n");
   disposeGame(g1);

   printf("All tests passed...You are Awesome!\n");
   return EXIT_SUCCESS;
}



void testNewGame2(Game g1) {
   assert(g1 != NULL);
   assert(getMostPublications(g1) == NO_ONE);
   assert(getMostARCs(g1) == NO_ONE);
   assert(getTurnNumber(g1) == -1);
   assert(getWhoseTurn(g1) == NO_ONE);

   assert(getIPs(g1, UNI_A) == 0);
   assert(getIPs(g1, UNI_B) == 0);
   assert(getIPs(g1, UNI_C) == 0);

   assert(getGO8s(g1, UNI_A) == 0);
   assert(getGO8s(g1, UNI_B) == 0);
   assert(getGO8s(g1, UNI_C) == 0);

   assert(getPublications(g1, UNI_A) == 0);
   assert(getPublications(g1, UNI_B) == 0);
   assert(getPublications(g1, UNI_C) == 0);

   assert(getKPIpoints(g1, UNI_A) == 20);
   assert(getKPIpoints(g1, UNI_B) == 20);
   assert(getKPIpoints(g1, UNI_C) == 20);

   assert(getCampuses(g1, UNI_A) == 2);
   assert(getCampuses(g1, UNI_B) == 2);
   assert(getCampuses(g1, UNI_C) == 2);

   path location;
   strcpy(location, "LB");
   assert(getCampus(g1, location) == CAMPUS_A);
   strcpy(location,"RRLRL");
   assert(getCampus(g1, location) == CAMPUS_B);
   strcpy(location,"LRLRL");
   assert(getCampus(g1, location) == CAMPUS_C);
   strcpy(location,"RR");
   assert(getCampus(g1, location) == VACANT_VERTEX);
   strcpy(location,"RRLRLLL");
   assert(getCampus(g1, location) == VACANT_VERTEX);
   strcpy(location,"LRLRB");
   assert(getCampus(g1, location) == VACANT_VERTEX);
   strcpy(location,"RLRLRLRLB");
   assert(getCampus(g1, location) == VACANT_VERTEX);


   assert(getCampus(g1, location) == VACANT_ARC);
   assert(getStudents(g1, UNI_A, STUDENT_THD) == 0);
   assert(getStudents(g1, UNI_A, STUDENT_BPS) == 3);
   assert(getStudents(g1, UNI_A, STUDENT_BQN) == 3);
   assert(getStudents(g1, UNI_A, STUDENT_MJ) == 1);
   assert(getStudents(g1, UNI_A, STUDENT_MTV) == 1);
   assert(getStudents(g1, UNI_A, STUDENT_MMONEY) == 1);

   assert(getStudents(g1, UNI_B, STUDENT_THD) == 0);
   assert(getStudents(g1, UNI_B, STUDENT_BPS) == 3);
   assert(getStudents(g1, UNI_B, STUDENT_BQN) == 3);
   assert(getStudents(g1, UNI_B, STUDENT_MJ) == 1);
   assert(getStudents(g1, UNI_B, STUDENT_MTV) == 1);
   assert(getStudents(g1, UNI_B, STUDENT_MMONEY) == 1);

   assert(getStudents(g1, UNI_C, STUDENT_THD) == 0);
   assert(getStudents(g1, UNI_C, STUDENT_BPS) == 3);
   assert(getStudents(g1, UNI_C, STUDENT_BQN) == 3);
   assert(getStudents(g1, UNI_C, STUDENT_MJ) == 1);
   assert(getStudents(g1, UNI_C, STUDENT_MTV) == 1);
   assert(getStudents(g1, UNI_C, STUDENT_MMONEY) == 1);

   strcpy(location,"LB");
   assert(getARC(g1, location) == VACANT_ARC);
   strcpy(location,"RRLRL");
   assert(getARC(g1, location) == VACANT_ARC);
   strcpy(location,"LRLRL");
   assert(getARC(g1, location) == VACANT_ARC);
   strcpy(location,"RR");
   assert(getARC(g1, location) == VACANT_ARC);
   strcpy(location,"RRLRLLL");
   assert(getARC(g1, location) == VACANT_ARC);
   strcpy(location,"LRLRLRLRBLBLR");
   assert(getARC(g1, location) == VACANT_ARC);
   strcpy(location,"RLRLRLRLB");
   assert(getARC(g1, location) == VACANT_ARC);

   assert(getKPIpoints(g1, UNI_A) == 20);
   assert(getKPIpoints(g1, UNI_B) == 20);
   assert(getKPIpoints(g1, UNI_C) == 20);

   assert(getARCs(g1, UNI_A) == 0);
   assert(getARCs(g1, UNI_B) == 0);
   assert(getARCs(g1, UNI_C) == 0);

   assert(getDiscipline(g1, 0) == STUDENT_BQN);
   assert(getDiscipline(g1, 1) == STUDENT_MMONEY);
   assert(getDiscipline(g1, 2) == STUDENT_MJ);
   assert(getDiscipline(g1, 3) == STUDENT_MMONEY);
   assert(getDiscipline(g1, 4) == STUDENT_MJ);
   assert(getDiscipline(g1, 5) == STUDENT_BPS);
   assert(getDiscipline(g1, 6) == STUDENT_MTV);
   assert(getDiscipline(g1, 7) == STUDENT_MTV);
   assert(getDiscipline(g1, 8) == STUDENT_BPS);
   assert(getDiscipline(g1, 9) == STUDENT_MTV);
   assert(getDiscipline(g1, 10) == STUDENT_BQN);
   assert(getDiscipline(g1, 11) == STUDENT_MJ);
   assert(getDiscipline(g1, 12) == STUDENT_BQN);
   assert(getDiscipline(g1, 13) == STUDENT_THD);
   assert(getDiscipline(g1, 14) == STUDENT_MJ);
   assert(getDiscipline(g1, 15) == STUDENT_MMONEY);
   assert(getDiscipline(g1, 16) == STUDENT_MTV);
   assert(getDiscipline(g1, 17) == STUDENT_BQN);
   assert(getDiscipline(g1, 18) == STUDENT_BPS);

   assert(getDiceValue(g1, 0) == 9);
   assert(getDiceValue(g1, 1) == 10);
   assert(getDiceValue(g1, 2) == 8);
   assert(getDiceValue(g1, 3) == 12);
   assert(getDiceValue(g1, 4) == 6);
   assert(getDiceValue(g1, 5) == 5);
   assert(getDiceValue(g1, 6) == 3);
   assert(getDiceValue(g1, 7) == 11);
   assert(getDiceValue(g1, 8) == 3);
   assert(getDiceValue(g1, 9) == 11);
   assert(getDiceValue(g1, 10) == 4);
   assert(getDiceValue(g1, 11) == 6);
   assert(getDiceValue(g1, 12) == 4);
   assert(getDiceValue(g1, 13) == 7);
   assert(getDiceValue(g1, 14) == 9);
   assert(getDiceValue(g1, 15) == 2);
   assert(getDiceValue(g1, 16) == 8);
   assert(getDiceValue(g1, 17) == 10);
   assert(getDiceValue(g1, 18) == 5);


   int counter1 = 1;
   int counter2 = 0;
   int counter3 = 0;

   while(counter1 <= NUM_UNIS){
  	while(counter2 < 5) {
     	while(counter3 < 5) {
    	assert(getExchangeRate(g1,counter1,counter2,counter3) == 3);
    	counter3++;
     	}
     	counter2++;
  	}
  	counter1++;
   }
}

void testNewGame1(Game g2) {

   assert(g2 != NULL);
   assert(getMostPublications(g2) == NO_ONE);
   assert(getMostARCs(g2) == NO_ONE);
   assert(getTurnNumber(g2) == -1);
   assert(getWhoseTurn(g2) == NO_ONE);

   assert(getIPs(g2, UNI_A) == 0);
   assert(getIPs(g2, UNI_B) == 0);
   assert(getIPs(g2, UNI_C) == 0);

   assert(getGO8s(g2, UNI_A) == 0);
   assert(getGO8s(g2, UNI_B) == 0);
   assert(getGO8s(g2, UNI_C) == 0);

   assert(getPublications(g2, UNI_A) == 0);
   assert(getPublications(g2, UNI_B) == 0);
   assert(getPublications(g2, UNI_C) == 0);

   assert(getKPIpoints(g2, UNI_A) == 20);
   assert(getKPIpoints(g2, UNI_B) == 20);
   assert(getKPIpoints(g2, UNI_C) == 20);

   assert(getCampuses(g2, UNI_A) == 2);
   assert(getCampuses(g2, UNI_B) == 2);
   assert(getCampuses(g2, UNI_C) == 2);

   path location;
   strcpy(location,"LB");
   assert(getCampus(g2, location) == CAMPUS_A);
   strcpy(location,"RRLRL");
   assert(getCampus(g2, location) == CAMPUS_B);
   strcpy(location,"LRLRL");
   assert(getCampus(g2, location) == CAMPUS_C);
   strcpy(location,"RR");
   assert(getCampus(g2, location) == VACANT_VERTEX);
   strcpy(location,"RRLRLLL");
   assert(getCampus(g2, location) == VACANT_VERTEX);
   strcpy(location,"LRLRB");
   assert(getCampus(g2, location) == VACANT_VERTEX);
   strcpy(location,"RLRLRLRLB");
   assert(getCampus(g2, location) == VACANT_VERTEX);


   assert(getCampus(g2, location) == VACANT_ARC);
   assert(getStudents(g2, UNI_A, STUDENT_THD) == 0);
   assert(getStudents(g2, UNI_A, STUDENT_BPS) == 3);
   assert(getStudents(g2, UNI_A, STUDENT_BQN) == 3);
   assert(getStudents(g2, UNI_A, STUDENT_MJ) == 1);
   assert(getStudents(g2, UNI_A, STUDENT_MTV) == 1);
   assert(getStudents(g2, UNI_A, STUDENT_MMONEY) == 1);

   assert(getStudents(g2, UNI_B, STUDENT_THD) == 0);
   assert(getStudents(g2, UNI_B, STUDENT_BPS) == 3);
   assert(getStudents(g2, UNI_B, STUDENT_BQN) ==3);
   assert(getStudents(g2, UNI_B, STUDENT_MJ) == 1);
   assert(getStudents(g2, UNI_B, STUDENT_MTV) == 1);
   assert(getStudents(g2, UNI_B, STUDENT_MMONEY) == 1);

   assert(getStudents(g2, UNI_C, STUDENT_THD) == 0);
   assert(getStudents(g2, UNI_C, STUDENT_BPS) == 3);
   assert(getStudents(g2, UNI_C, STUDENT_BQN) == 3);
   assert(getStudents(g2, UNI_C, STUDENT_MJ) == 1);
   assert(getStudents(g2, UNI_C, STUDENT_MTV) == 1);
   assert(getStudents(g2, UNI_C, STUDENT_MMONEY) == 1);

   strcpy(location,"LB");
   assert(getARC(g2, location) == VACANT_ARC);
   strcpy(location,"RRLRL");
   assert(getARC(g2, location) == VACANT_ARC);
   strcpy(location,"LRLRL");
   assert(getARC(g2, location) == VACANT_ARC);
   strcpy(location,"RR");
   assert(getARC(g2, location) == VACANT_ARC);
   strcpy(location,"RRLRLLL");
   assert(getARC(g2, location) == VACANT_ARC);
   strcpy(location,"LRLRB");
   assert(getARC(g2, location) == VACANT_ARC);
   strcpy(location,"RLRLRLRLB");
   assert(getARC(g2, location) == VACANT_ARC);

   assert(getKPIpoints(g2, UNI_A) == 20);
   assert(getKPIpoints(g2, UNI_B) == 20);
   assert(getKPIpoints(g2, UNI_C) == 20);

   assert(getARCs(g2, UNI_A) == 0);
   assert(getARCs(g2, UNI_B) == 0);
   assert(getARCs(g2, UNI_C) == 0);

   assert(getDiscipline(g2, 0) == STUDENT_MMONEY);
   assert(getDiscipline(g2, 1) == STUDENT_MJ);
   assert(getDiscipline(g2, 2) == STUDENT_MJ);
   assert(getDiscipline(g2, 3) == STUDENT_BQN);
   assert(getDiscipline(g2, 4) == STUDENT_MTV);
   assert(getDiscipline(g2, 5) == STUDENT_MJ);
   assert(getDiscipline(g2, 6) == STUDENT_BPS);
   assert(getDiscipline(g2, 7) == STUDENT_BPS);
   assert(getDiscipline(g2, 8) == STUDENT_MTV);
   assert(getDiscipline(g2, 9) == STUDENT_THD);
   assert(getDiscipline(g2, 10) == STUDENT_BQN);
   assert(getDiscipline(g2, 11) == STUDENT_BPS);
   assert(getDiscipline(g2, 12) == STUDENT_MMONEY);
   assert(getDiscipline(g2, 13) == STUDENT_MJ);
   assert(getDiscipline(g2, 14) == STUDENT_MMONEY);
   assert(getDiscipline(g2, 15) == STUDENT_MTV);
   assert(getDiscipline(g2, 16) == STUDENT_BQN);
   assert(getDiscipline(g2, 17) == STUDENT_BQN);
   assert(getDiscipline(g2, 18) == STUDENT_MTV);

   assert(getDiceValue(g2, 0) == 6);
   assert(getDiceValue(g2, 1) == 10);
   assert(getDiceValue(g2, 2) == 8);
   assert(getDiceValue(g2, 3) == 3);
   assert(getDiceValue(g2, 4) == 4);
   assert(getDiceValue(g2, 5) == 9);
   assert(getDiceValue(g2, 6) == 6);
   assert(getDiceValue(g2, 7) == 11);
   assert(getDiceValue(g2, 8) == 11);
   assert(getDiceValue(g2, 9) == 7);
   assert(getDiceValue(g2, 10) == 12);
   assert(getDiceValue(g2, 11) == 3);
   assert(getDiceValue(g2, 12) == 5);
   assert(getDiceValue(g2, 13) == 2);
   assert(getDiceValue(g2, 14) == 5);
   assert(getDiceValue(g2, 15) == 10);
   assert(getDiceValue(g2, 16) == 9);
   assert(getDiceValue(g2, 17) == 4);
   assert(getDiceValue(g2, 18) == 8);


   int counter1 = 1;
   int counter2 = 0;
   int counter3 = 0;

   while(counter1 <= NUM_UNIS){
  	while(counter2 < 5) {
     	while(counter3 < 5) {
        	assert(getExchangeRate(g2,counter1,counter2,counter3) == 3);
    	counter3++;
     	}
     	counter2++;
  	}
  	counter1++;
   }

}




void testMakeActions(Game g) {

   action pass;
   pass.actionCode = PASS;
   throwDice(g, 4);
   int counter = 0;
//printf("%d\n",getTurnNumber(g));
   while (counter < 95) {
  	throwDice(g, 11);
  	makeAction(g, pass);
  	counter++;
   }
//   printf("%d\n", counter);
//   printf("%d\n", getStudents(g,UNI_A, STUDENT_BPS));
   assert(getTurnNumber(g) == counter);
   assert(getWhoseTurn(g) == UNI_C);

   assert(getStudents(g, UNI_A, STUDENT_THD) == 0);
   assert(getStudents(g, UNI_B, STUDENT_THD) == 0);
   assert(getStudents(g, UNI_C, STUDENT_THD) == 0);
   assert(getStudents(g, UNI_A, STUDENT_BPS) == 98);
   assert(getStudents(g, UNI_B, STUDENT_BPS) == 3);
   assert(getStudents(g, UNI_C, STUDENT_BPS) == 3);
   assert(getStudents(g, UNI_A, STUDENT_BQN) == 3);
   assert(getStudents(g, UNI_B, STUDENT_BQN) == 3);
   assert(getStudents(g, UNI_C, STUDENT_BQN) == 3);
   assert(getStudents(g, UNI_A, STUDENT_MJ) == 1);
   assert(getStudents(g, UNI_B, STUDENT_MJ) == 1);
   assert(getStudents(g, UNI_C, STUDENT_MJ) == 1);
   assert(getStudents(g, UNI_A, STUDENT_MTV) == 1);
   assert(getStudents(g, UNI_B, STUDENT_MTV) == 1);
   assert(getStudents(g, UNI_C, STUDENT_MTV) == 1);
   assert(getStudents(g, UNI_A, STUDENT_MMONEY) == 1);
   assert(getStudents(g, UNI_B, STUDENT_MMONEY) == 1);
   assert(getStudents(g, UNI_C, STUDENT_MMONEY) == 1);

//printf("sup\n");
   action makePublication;
   makePublication.actionCode = 5;
   makeAction(g, makePublication);
//printf("Hi\n");
//printf("%d\n", getWhoseTurn(g));
   assert(getPublications (g, UNI_C) == 1);
   assert(getMostPublications(g) == UNI_C);
//printf("%d\n", getKPIpoints(g, UNI_C));
   assert(getKPIpoints(g, UNI_C) == 30);
   assert(getStudents(g, UNI_C, STUDENT_THD) == 0);
   assert(getStudents(g, UNI_C, STUDENT_BPS) == 3);
   assert(getStudents(g, UNI_C, STUDENT_BQN) == 3);
   assert(getStudents(g, UNI_C, STUDENT_MJ) == 0);
   assert(getStudents(g, UNI_C, STUDENT_MTV) == 0);
   assert(getStudents(g, UNI_C, STUDENT_MMONEY) == 0);

   action makeARC;
   makeARC.actionCode = OBTAIN_ARC;
   strcpy(makeARC.destination, "LRLRL");
//printf("lol\n");
   makeAction(g, makeARC);
//printf("success\n");
   assert(getARCs(g, UNI_C) == 1);
   assert(getMostARCs(g) == UNI_C);
//printf("%d\n", getKPIpoints(g, UNI_C));

   assert(getKPIpoints(g, UNI_C) == 42);
   assert(getARC(g, "LRLRL") == UNI_C);
   assert(getStudents(g, UNI_C, STUDENT_THD) == 0);
   assert(getStudents(g, UNI_C, STUDENT_BPS) == 2);
   assert(getStudents(g, UNI_C, STUDENT_BQN) == 2);
   assert(getStudents(g, UNI_C, STUDENT_MJ) == 0);
   assert(getStudents(g, UNI_C, STUDENT_MTV) == 0);
   assert(getStudents(g, UNI_C, STUDENT_MMONEY) == 0);

   makeAction(g, pass);
   throwDice(g, 11);
   counter++;
//   printf("sup\n");

   assert(getStudents(g, UNI_A, STUDENT_THD) == 0);
   assert(getStudents(g, UNI_B, STUDENT_THD) == 0);
   assert(getStudents(g, UNI_C, STUDENT_THD) == 0);
   assert(getStudents(g, UNI_A, STUDENT_BPS) == 99);
   assert(getStudents(g, UNI_B, STUDENT_BPS) == 3);
   assert(getStudents(g, UNI_C, STUDENT_BPS) == 2);
   assert(getStudents(g, UNI_A, STUDENT_BQN) == 3);
   assert(getStudents(g, UNI_B, STUDENT_BQN) == 3);
   assert(getStudents(g, UNI_C, STUDENT_BQN) == 2);
   assert(getStudents(g, UNI_A, STUDENT_MJ) == 1);
   assert(getStudents(g, UNI_B, STUDENT_MJ) == 1);
   assert(getStudents(g, UNI_C, STUDENT_MJ) == 0);
   assert(getStudents(g, UNI_A, STUDENT_MTV) == 1);
   assert(getStudents(g, UNI_B, STUDENT_MTV) == 1);
   assert(getStudents(g, UNI_C, STUDENT_MTV) == 0);
   assert(getStudents(g, UNI_A, STUDENT_MMONEY) == 1);
   assert(getStudents(g, UNI_B, STUDENT_MMONEY) == 1);
   assert(getStudents(g, UNI_C, STUDENT_MMONEY) == 0);

   action BPStoBQN;
   BPStoBQN.actionCode = 7;
   BPStoBQN.disciplineFrom = STUDENT_BPS;
   BPStoBQN.disciplineTo = STUDENT_BQN;
   counter = 0;
   while (counter < 10) {
  	makeAction(g, BPStoBQN);
  	counter++;
   }
   action BPStoMJ;
   BPStoMJ.actionCode = 7;
   BPStoMJ.disciplineFrom = STUDENT_BPS;
   BPStoMJ.disciplineTo = STUDENT_MJ;
   counter = 0;
   while (counter < 7) {
  	makeAction(g, BPStoMJ);
  	counter++;
   }
   action BPStoMTV;
   BPStoMTV.actionCode = 7;
   BPStoMTV.disciplineFrom = STUDENT_BPS;
   BPStoMTV.disciplineTo = STUDENT_MTV;
   counter = 0;
   while (counter < 5) {
  	makeAction(g, BPStoMTV);
  	counter++;
   }
   action BPStoMMONEY;
   BPStoMMONEY.actionCode = 7;
   BPStoMMONEY.disciplineFrom = STUDENT_BPS;
   BPStoMMONEY.disciplineTo = STUDENT_MMONEY;
   counter = 0;
   while (counter < 7) {
  	makeAction(g, BPStoMMONEY);
  	counter++;
   }

   assert(getStudents(g, UNI_A, STUDENT_THD) == 0);
   assert(getStudents(g, UNI_B, STUDENT_THD) == 0);
   assert(getStudents(g, UNI_C, STUDENT_THD) == 0);
   assert(getStudents(g, UNI_A, STUDENT_BPS) == 12);
   assert(getStudents(g, UNI_B, STUDENT_BPS) == 3);
   assert(getStudents(g, UNI_C, STUDENT_BPS) == 2);
   assert(getStudents(g, UNI_A, STUDENT_BQN) == 13);
   assert(getStudents(g, UNI_B, STUDENT_BQN) == 3);
   assert(getStudents(g, UNI_C, STUDENT_BQN) == 2);
   assert(getStudents(g, UNI_A, STUDENT_MJ) == 8);
   assert(getStudents(g, UNI_B, STUDENT_MJ) == 1);
   assert(getStudents(g, UNI_C, STUDENT_MJ) == 0);
   assert(getStudents(g, UNI_A, STUDENT_MTV) == 6);
   assert(getStudents(g, UNI_B, STUDENT_MTV) == 1);
   assert(getStudents(g, UNI_C, STUDENT_MTV) == 0);
   assert(getStudents(g, UNI_A, STUDENT_MMONEY) == 8);
   assert(getStudents(g, UNI_B, STUDENT_MMONEY) == 1);
   assert(getStudents(g, UNI_C, STUDENT_MMONEY) == 0);

   makeAction(g, makePublication);
//printf("%d %d %d\n", getPublications(g, 1), getPublications(g, 3), getMostPublications(g));
   assert(getMostPublications(g) == UNI_C);
   makeAction(g, makePublication);
   assert(getMostPublications(g) == UNI_A);
   assert(getKPIpoints(g, UNI_C) == 32);
   assert(getKPIpoints(g, UNI_A) == 30);

   action makeIP;
   makeIP.actionCode = 6;
   makeAction(g, makeIP);
   assert(getIPs(g, UNI_A) == 1);
   assert(getKPIpoints(g, UNI_A) == 40);
   makeAction(g, makeIP);
   assert(getIPs(g, UNI_A) == 2);
   assert(getKPIpoints(g, UNI_A) == 50);
   assert(getStudents(g, UNI_A, STUDENT_THD) == 0);
   assert(getStudents(g, UNI_A, STUDENT_BPS) == 12);
   assert(getStudents(g, UNI_A, STUDENT_BQN) == 13);
   assert(getStudents(g, UNI_A, STUDENT_MJ) == 4);
   assert(getStudents(g, UNI_A, STUDENT_MTV) == 2);
   assert(getStudents(g, UNI_A, STUDENT_MMONEY) == 4);


   strcpy(makeARC.destination, "RLRLRLRLLRRR");
//printf("enteringasasd\n");
   makeAction(g, makeARC);
//printf("leaving\n");
   assert(getMostARCs(g) == UNI_C);
   assert(getKPIpoints(g, UNI_C) == 32);
//printf("%d\n", getKPIpoints(g, 1));
   assert(getKPIpoints(g, UNI_A) == 52);
   strcpy(makeARC.destination, "RLRLRLRLLRRRR");
   makeAction(g, makeARC);
   assert(getMostARCs(g) == UNI_A);
   assert(getKPIpoints(g, UNI_C) == 22);
   assert(getKPIpoints(g, UNI_A) == 64);
//printf("lol\n");
   strcpy(makeARC.destination, "RLRLRLRLLRRRRL");
   makeAction(g, makeARC);
//printf("rip\n");
   strcpy(makeARC.destination, "RLRLRLRLLRRRRLR");
   makeAction(g, makeARC);
//printf("plz\n");

   strcpy(makeARC.destination, "R");
   makeAction(g, makeARC);
   strcpy(makeARC.destination, "RR");
   makeAction(g, makeARC);
   strcpy(makeARC.destination, "RRL");
   makeAction(g, makeARC);
   strcpy(makeARC.destination, "RRLR");
   makeAction(g, makeARC);
   strcpy(makeARC.destination, "RRLRL");
   makeAction(g, makeARC);

   action makeCampus;
   makeCampus.actionCode = 1;
   strcpy(makeCampus.destination,"RLRLRLRLLRRRRL");
   makeAction(g, makeCampus);
//printf("asdliuawevokuhewrvliuhawervwaer;lkvjwaelrv\n");
   assert(getCampuses(g, UNI_A) == 3);
   assert(getCampus(g, "RLRLRLRLLRRRRL") == CAMPUS_A);
   assert(getKPIpoints(g, UNI_A) == 88);
   assert(getStudents(g, UNI_A, STUDENT_THD) == 0);
   assert(getStudents(g, UNI_A, STUDENT_BPS) == 2);
   assert(getStudents(g, UNI_A, STUDENT_BQN) == 3);
   assert(getStudents(g, UNI_A, STUDENT_MJ) == 3);
   assert(getStudents(g, UNI_A, STUDENT_MTV) == 1);
   assert(getStudents(g, UNI_A, STUDENT_MMONEY) == 4);
   throwDice(g, 6);
   throwDice(g, 6);
   throwDice(g, 6);
   assert(getStudents(g, UNI_A, STUDENT_BPS) == 5);
   assert(getStudents(g, UNI_B, STUDENT_MMONEY) == 4);
//printf("oijnergfliearflkhjbaeflkhjaeflkhjbaweflkjbawe\n");

   counter = 0;
   while (counter < 6) {
   assert(getExchangeRate(g, UNI_A, STUDENT_BPS, counter) == 2);
   counter++;
   }
   counter = 0;
   while (counter < 6) {
  	assert(getExchangeRate(g, UNI_A, STUDENT_BQN, counter) == 3);
   counter++;
   }
   counter = 0;
   while (counter < 6) {
  	assert(getExchangeRate(g, UNI_B, STUDENT_BQN, counter) == 3);
   counter++;
   }


   action makeGO8;
   makeGO8.actionCode = 2;
   strcpy(makeGO8.destination,"RLRLRLRLLRRRRL");
   makeAction(g, makeGO8);
//printf("GO8 exiting aSLOEiuhrbvawlekuhlbwrikujhv\n");
   assert(getGO8s(g, UNI_A) == 1);
   assert(getCampuses(g, UNI_A) == 2);
   assert(getKPIpoints(g, UNI_A) == 98);
   assert(getCampus(g, "RLRLRLRLLRRRRL") == GO8_A);
   assert(getStudents(g, UNI_A, STUDENT_THD) == 0);
   assert(getStudents(g, UNI_A, STUDENT_BPS) == 5);
   assert(getStudents(g, UNI_A, STUDENT_BQN) == 3);
   assert(getStudents(g, UNI_A, STUDENT_MJ) == 1);
   assert(getStudents(g, UNI_A, STUDENT_MTV) == 1);
   assert(getStudents(g, UNI_A, STUDENT_MMONEY) == 1);


   counter = 0;
   while (counter < 6) {
   assert(getExchangeRate(g, UNI_A, STUDENT_BPS, counter) == 2);
   counter++;
   }
   counter = 0;
   while (counter < 6) {
  	assert(getExchangeRate(g, UNI_A, STUDENT_BQN, counter) == 3);
   counter++;
   }
   counter = 0;
   while (counter < 6) {
  	assert(getExchangeRate(g, UNI_B, STUDENT_BQN, counter) == 3);
   counter++;
   }



 //UNI_B's turn

   makeAction(g, BPStoMTV);
   assert(getStudents(g, UNI_A, STUDENT_BPS) == 3);
   assert(getStudents(g, UNI_A, STUDENT_MTV) == 2);
   makeAction(g, pass);
   throwDice(g, 6);
   assert(getStudents(g, UNI_A, STUDENT_BPS) == 5);
   strcpy(makeARC.destination, "RLLRLRLRRL");
   makeAction(g, makeARC);
   strcpy(makeARC.destination, "RLLRLRLRRLR");
   makeAction(g, makeARC);
   strcpy(makeARC.destination, "RLLRLRLRRLRR");
   makeAction(g, makeARC);

   throwDice(g, 7);
   assert(getStudents(g, UNI_A, STUDENT_THD) == 3);
   assert(getStudents(g, UNI_B, STUDENT_THD) == 6);
   assert(getStudents(g, UNI_C, STUDENT_THD) == 0);
   assert(getStudents(g, UNI_A, STUDENT_BPS) == 5);
   assert(getStudents(g, UNI_B, STUDENT_BPS) == 0);
   assert(getStudents(g, UNI_C, STUDENT_BPS) == 2);
   assert(getStudents(g, UNI_A, STUDENT_BQN) == 3);
   assert(getStudents(g, UNI_B, STUDENT_BQN) == 0);
   assert(getStudents(g, UNI_C, STUDENT_BQN) == 2);
   assert(getStudents(g, UNI_A, STUDENT_MJ) == 1);
   assert(getStudents(g, UNI_B, STUDENT_MJ) == 1);
   assert(getStudents(g, UNI_C, STUDENT_MJ) == 0);
   assert(getStudents(g, UNI_A, STUDENT_MTV) == 0);
   assert(getStudents(g, UNI_B, STUDENT_MTV) == 0);
   assert(getStudents(g, UNI_C, STUDENT_MTV) == 0);
   assert(getStudents(g, UNI_A, STUDENT_MMONEY) == 0);
   assert(getStudents(g, UNI_B, STUDENT_MMONEY) == 0);
   assert(getStudents(g, UNI_C, STUDENT_MMONEY) == 0);

   counter = 0;
   while (counter < 10) {
  	throwDice(g, 3);
  	counter++;
   }


   makeAction(g, BPStoMJ);
   makeAction(g, BPStoMTV);
   makeAction(g, BPStoMMONEY);
   makeAction(g, BPStoMMONEY);
   makeAction(g, BPStoMMONEY);

   assert(getStudents(g, UNI_A, STUDENT_BPS) == 5);
   assert(getStudents(g, UNI_A, STUDENT_THD) == 3);
   assert(getStudents(g, UNI_A, STUDENT_BQN) == 3);
   assert(getStudents(g, UNI_A, STUDENT_MJ) == 2);
   assert(getStudents(g, UNI_A, STUDENT_MTV) == 1);
   assert(getStudents(g, UNI_A, STUDENT_MMONEY) == 3);

   assert(getKPIpoints(g,UNI_B) == 26);
   assert(getKPIpoints(g,UNI_C) == 22);


}





void testPreGameLegalActions(Game g) {
   action passAction;
   passAction.actionCode = PASS;
   assert(isLegalAction(g, passAction) == FALSE);
   action ARCaction;
   ARCaction.actionCode = 3;
   strcpy(ARCaction.destination,"R");
   assert(isLegalAction(g, ARCaction) == FALSE);
}

void testGeneralIsLegalActions(Game g) {

   action buildARC;
   buildARC.actionCode = OBTAIN_ARC;
   strcpy(buildARC.destination, "LL");
   assert(isLegalAction(g, buildARC) == FALSE);
   strcpy(buildARC.destination, "B");
   assert(isLegalAction(g, buildARC) == FALSE);
   strcpy(buildARC.destination, "RL");
   assert(isLegalAction(g, buildARC) == FALSE);
   strcpy(buildARC.destination, "R");
   assert(isLegalAction(g, buildARC) == TRUE);
   strcpy(buildARC.destination, "L");
   assert(isLegalAction(g, buildARC) == TRUE);

   action buildGO8;
   buildGO8.actionCode = BUILD_GO8;

   strcpy(buildGO8.destination, "");
   assert(isLegalAction(g, buildGO8) == FALSE);
   strcpy(buildGO8.destination, "LRL");
   assert(isLegalAction(g, buildGO8) == FALSE);
   strcpy(buildGO8.destination, "RRLRL");
   assert(isLegalAction(g, buildGO8) == FALSE);
   strcpy(buildGO8.destination, "RLRLRLRLRLR");
   assert(isLegalAction(g, buildGO8) == FALSE);
   strcpy(buildGO8.destination, "RLRLRLRLRLR");
   assert(isLegalAction(g, buildGO8) == FALSE);

   action retrain;
   retrain.actionCode = RETRAIN_STUDENTS;
   retrain.disciplineFrom = STUDENT_BPS;
   retrain.disciplineTo = STUDENT_BQN;
   assert(isLegalAction(g, retrain) == TRUE);
   retrain.disciplineTo = STUDENT_THD;
   assert(isLegalAction(g, retrain) == TRUE);
   retrain.disciplineTo = STUDENT_MJ;
   assert(isLegalAction(g, retrain) == TRUE);
   retrain.disciplineTo = STUDENT_MTV;
   assert(isLegalAction(g, retrain) == TRUE);
   retrain.disciplineTo = STUDENT_MMONEY;
   assert(isLegalAction(g, retrain) == TRUE);

   retrain.disciplineFrom = STUDENT_BQN;
   retrain.disciplineTo = STUDENT_BPS;
   assert(isLegalAction(g, retrain) == TRUE);
   retrain.disciplineTo = STUDENT_THD;
   assert(isLegalAction(g, retrain) == TRUE);
   retrain.disciplineTo = STUDENT_MJ;
   assert(isLegalAction(g, retrain) == TRUE);
   retrain.disciplineTo = STUDENT_MTV;
   assert(isLegalAction(g, retrain) == TRUE);
   retrain.disciplineTo = STUDENT_MMONEY;
   assert(isLegalAction(g, retrain) == TRUE);

   retrain.disciplineFrom = STUDENT_THD;
   retrain.disciplineTo = STUDENT_BPS;
   assert(isLegalAction(g, retrain) == FALSE);
   retrain.disciplineTo = STUDENT_BQN;
   assert(isLegalAction(g, retrain) == FALSE);
   retrain.disciplineTo = STUDENT_MJ;
   assert(isLegalAction(g, retrain) == FALSE);
   retrain.disciplineTo = STUDENT_MTV;
   assert(isLegalAction(g, retrain) == FALSE);
   retrain.disciplineTo = STUDENT_MMONEY;
   assert(isLegalAction(g, retrain) == FALSE);

   retrain.disciplineFrom = STUDENT_MMONEY;
   retrain.disciplineTo = STUDENT_BPS;
   assert(isLegalAction(g, retrain) == FALSE);
   retrain.disciplineTo = STUDENT_BQN;
   assert(isLegalAction(g, retrain) == FALSE);
   retrain.disciplineTo = STUDENT_MJ;
   assert(isLegalAction(g, retrain) == FALSE);
   retrain.disciplineTo = STUDENT_MTV;
   assert(isLegalAction(g, retrain) == FALSE);
   retrain.disciplineTo = STUDENT_THD;
   assert(isLegalAction(g, retrain) == FALSE);

   retrain.disciplineFrom = STUDENT_MTV;
   retrain.disciplineTo = STUDENT_BPS;
   assert(isLegalAction(g, retrain) == FALSE);
   retrain.disciplineTo = STUDENT_BQN;
   assert(isLegalAction(g, retrain) == FALSE);
   retrain.disciplineTo = STUDENT_MJ;
   assert(isLegalAction(g, retrain) == FALSE);
   retrain.disciplineTo = STUDENT_THD;
   assert(isLegalAction(g, retrain) == FALSE);
   retrain.disciplineTo = STUDENT_MMONEY;
   assert(isLegalAction(g, retrain) == FALSE);

   retrain.disciplineFrom = STUDENT_MJ;
   retrain.disciplineTo = STUDENT_BPS;
   assert(isLegalAction(g, retrain) == FALSE);
   retrain.disciplineTo = STUDENT_BQN;
   assert(isLegalAction(g, retrain) == FALSE);
   retrain.disciplineTo = STUDENT_THD;
   assert(isLegalAction(g, retrain) == FALSE);
   retrain.disciplineTo = STUDENT_MTV;
   assert(isLegalAction(g, retrain) == FALSE);
   retrain.disciplineTo = STUDENT_MMONEY;
   assert(isLegalAction(g, retrain) == FALSE);
//   printf("sup\n");
//   printf("nooo\n");
}


void testIsLegalActionGame(Game g) {
  action retrain;
   retrain.actionCode = RETRAIN_STUDENTS;
   retrain.disciplineFrom = STUDENT_BPS;
   retrain.disciplineTo = STUDENT_MTV;
   assert(isLegalAction(g, retrain) == TRUE);
   assert(isLegalAction(g, retrain) == TRUE);
   retrain.disciplineTo = STUDENT_MJ;
   assert(isLegalAction(g, retrain) == TRUE);
   retrain.disciplineTo = STUDENT_BQN;
   assert(isLegalAction(g, retrain) == TRUE);
   retrain.disciplineTo = STUDENT_MJ;
   assert(isLegalAction(g, retrain) == TRUE);
   retrain.disciplineTo = STUDENT_MMONEY;
   assert(isLegalAction(g, retrain) == TRUE);

   throwDice(g, 10); //B's turn now

   assert(isLegalAction(g, retrain) == FALSE);
   retrain.disciplineTo = STUDENT_MJ;
   assert(isLegalAction(g, retrain) == FALSE);
   retrain.disciplineTo = STUDENT_BQN;
   assert(isLegalAction(g, retrain) == FALSE);
   retrain.disciplineTo = STUDENT_MTV;
   assert(isLegalAction(g, retrain) == FALSE);
   retrain.disciplineTo = STUDENT_MMONEY;
   assert(isLegalAction(g, retrain) == FALSE);

   action buildCampus;
   buildCampus.actionCode = BUILD_CAMPUS;
   strcpy(buildCampus.destination, "RLRLRLRLRRR");
   assert(isLegalAction(g, buildCampus) == FALSE);
   strcpy(buildCampus.destination, "RLRLRLRL");
   assert(isLegalAction(g, buildCampus) == FALSE);
   strcpy(buildCampus.destination, "RLRLRLRLR");
   assert(isLegalAction(g, buildCampus) == FALSE);

   action buildARC;
   buildARC.actionCode = OBTAIN_ARC;
   strcpy(buildARC.destination, "RLRLRLRLRRR");
   assert(isLegalAction(g, buildARC) == FALSE);
   strcpy(buildARC.destination,"RLRLRLRLRRRL");
   assert(isLegalAction(g, buildARC) == FALSE);
   strcpy(buildARC.destination, "RLRLRLLL");
   assert(isLegalAction(g, buildARC) == FALSE);
   strcpy(buildARC.destination, "RLRLRLRLR");
   assert(isLegalAction(g, buildARC) == FALSE);

   action buildGO8;
   buildGO8.actionCode = BUILD_GO8;
   strcpy(buildGO8.destination, "RRLRL");
   assert(isLegalAction(g, buildGO8) == FALSE);
   strcpy(buildGO8.destination, "RRR");
   assert(isLegalAction(g, buildGO8) == FALSE);
   strcpy(buildGO8.destination, "RRL");
   assert(isLegalAction(g, buildGO8) == FALSE);

   action spinoff;
   spinoff.actionCode = START_SPINOFF;
   assert(isLegalAction(g, buildARC) == FALSE);
   spinoff.actionCode = OBTAIN_PUBLICATION;
   assert(isLegalAction(g, spinoff) == FALSE); //Can't obtain publication
   spinoff.actionCode = OBTAIN_IP_PATENT;
   assert(isLegalAction(g, spinoff) == FALSE); //Can't obtain patent

   throwDice(g, 10);
   throwDice(g, 10);// Back to player A


   buildCampus.actionCode = BUILD_CAMPUS;
   strcpy(buildCampus.destination, "RLRLRLRLRRR");
   assert(isLegalAction(g, buildCampus) == FALSE);
   strcpy(buildCampus.destination, "RLRLRLRLR");
   assert(isLegalAction(g, buildCampus) == FALSE);
   strcpy(buildCampus.destination, "RLRLRLRLR");
   assert(isLegalAction(g, buildCampus) == FALSE);
   strcpy(buildCampus.destination, "LLBRLR");
   assert(isLegalAction(g, buildCampus) == FALSE);
   strcpy(buildCampus.destination, "RR");
//   printf("Campus: %d\n", getCampus(g, "RR"));
//   printf("Students: MTV: %d BPS: %d MJ: %d BQN: %d\n",
//getStudents(g,UNI_A,STUDENT_MTV),getStudents(g,UNI_A,STUDENT_BPS),
//getStudents(g,UNI_A,STUDENT_MJ), getStudents(g,UNI_A,STUDENT_BQN));
   assert(isLegalAction(g, buildCampus) == TRUE);
   strcpy(buildCampus.destination, "RL");
   assert(isLegalAction(g, buildCampus) == FALSE);
   strcpy(buildCampus.destination, "RRLR");
   assert(isLegalAction(g, buildCampus) == FALSE);


   buildARC.actionCode = OBTAIN_ARC;
   strcpy(buildARC.destination, "RLRLRLRLRRR");
   assert(isLegalAction(g, buildARC) == FALSE);
   strcpy(buildARC.destination, "RLRLRLRLRRRL");
   assert(isLegalAction(g, buildARC) == TRUE);
   strcpy(buildARC.destination, "B");
   assert(isLegalAction(g, buildARC) == FALSE);
   strcpy(buildARC.destination, "RLRLRLRLRRRLR");
   assert(isLegalAction(g, buildARC) == FALSE);
   strcpy(buildARC.destination, "RLRLRLLL");
   assert(isLegalAction(g, buildARC) == FALSE);
   strcpy(buildARC.destination, "RR");
// printf("get ARC RR %d\n",getARC(g,"RR"));
   assert(isLegalAction(g, buildARC) == FALSE);
   strcpy(buildARC.destination, "RL");
   assert(isLegalAction(g, buildARC) == TRUE);
   strcpy(buildARC.destination, "RRLR");
   assert(isLegalAction(g, buildARC) == FALSE);



   buildGO8.actionCode = BUILD_GO8;
   strcpy(buildGO8.destination, "RB");
   assert(isLegalAction(g, buildGO8) == TRUE);
   strcpy(buildGO8.destination, "RLRLRLRLRLL");
   assert(isLegalAction(g, buildGO8) == TRUE);
   strcpy(buildGO8.destination, "B");
   assert(isLegalAction(g, buildGO8) == FALSE);
   strcpy(buildGO8.destination, "LLLL");
   assert(isLegalAction(g, buildGO8) == FALSE);
   strcpy(buildGO8.destination, "RLRLLRRR");
   assert(isLegalAction(g, buildGO8) == FALSE);
   strcpy(buildGO8.destination, "LRLRL");
   assert(isLegalAction(g, buildGO8) == FALSE);


   spinoff.actionCode = OBTAIN_PUBLICATION;
   assert(isLegalAction(g, spinoff) == FALSE); //Can't obtain publication
   spinoff.actionCode = OBTAIN_IP_PATENT;
   assert(isLegalAction(g, spinoff) == FALSE); //Can't obtain patent
   spinoff.actionCode = START_SPINOFF;
   assert(isLegalAction(g, spinoff) == TRUE); //Can start spinoff



}

void testGO8Board(Game g) {

  int counter = 0;
  while (counter < 97) {
     throwDice(g, 8);
     counter++;
  }

   assert(getWhoseTurn(g) == UNI_B);

   assert(getKPIpoints(g, UNI_A) == 98);
   assert(getKPIpoints(g, UNI_B) == 26);
   assert(getKPIpoints(g, UNI_C) == 22);

   assert(getCampuses(g, UNI_B) == 2);
   assert(getCampuses(g, UNI_A) == 2);
   assert(getCampuses(g, UNI_C) == 2);

   assert(getStudents(g, UNI_A, STUDENT_THD) == 3);
   assert(getStudents(g, UNI_A, STUDENT_BPS) == 5);
   assert(getStudents(g, UNI_A, STUDENT_BQN) == 3);
   assert(getStudents(g, UNI_A, STUDENT_MJ) == 2);
   assert(getStudents(g, UNI_A, STUDENT_MTV) == 1);
   assert(getStudents(g, UNI_A, STUDENT_MMONEY) == 3);

   assert(getStudents(g, UNI_B, STUDENT_THD) == 6);
   assert(getStudents(g, UNI_B, STUDENT_BPS) == 0);
   assert(getStudents(g, UNI_B, STUDENT_BQN) ==0);
   assert(getStudents(g, UNI_B, STUDENT_MJ) == 1);
   assert(getStudents(g, UNI_B, STUDENT_MTV) == 97);
   assert(getStudents(g, UNI_B, STUDENT_MMONEY) == 0);

   assert(getStudents(g, UNI_C, STUDENT_THD) == 0);
   assert(getStudents(g, UNI_C, STUDENT_BPS) == 2);
   assert(getStudents(g, UNI_C, STUDENT_BQN) == 2);
   assert(getStudents(g, UNI_C, STUDENT_MJ) == 97);
   assert(getStudents(g, UNI_C, STUDENT_MTV) == 0);
   assert(getStudents(g, UNI_C, STUDENT_MMONEY) == 0);

   counter = 0;
   action exchange;
   exchange.actionCode = RETRAIN_STUDENTS;
   exchange.disciplineFrom = STUDENT_MTV;
   exchange.disciplineTo = STUDENT_MMONEY;

   while (counter < 12) {
      assert(isLegalAction(g, exchange));
      makeAction(g, exchange);
      counter++;
   }

   exchange.disciplineTo = STUDENT_MJ;
   counter = 0;
   while (counter < 11) {
      assert(isLegalAction(g, exchange));
      makeAction(g, exchange);
      counter++;
   }

   counter = 0;
   exchange.disciplineTo = STUDENT_BPS;
   while (counter < 4) {
      assert(isLegalAction(g, exchange));
      makeAction(g, exchange);
      counter++;
   }

   counter = 0;
   exchange.disciplineTo = STUDENT_BQN;
   while (counter < 4) {
      assert(isLegalAction(g, exchange));
      makeAction(g, exchange);
      counter++;
   }

   assert(getStudents(g, UNI_B, STUDENT_THD) == 6);
   assert(getStudents(g, UNI_B, STUDENT_BPS) == 4);
   assert(getStudents(g, UNI_B, STUDENT_BQN) == 4);
   assert(getStudents(g, UNI_B, STUDENT_MJ) == 12);
   assert(getStudents(g, UNI_B, STUDENT_MTV) == 4);
   assert(getStudents(g, UNI_B, STUDENT_MMONEY) == 12);

   action buildCampus;
   buildCampus.actionCode = BUILD_CAMPUS;
   strcpy(buildCampus.destination, "RLRLRLLRLR");
   assert(isLegalAction(g,buildCampus));
   makeAction(g, buildCampus);

   action buildGO8;
   buildGO8.actionCode = BUILD_GO8;
   strcpy(buildGO8.destination, buildCampus.destination);
   assert(isLegalAction(g,buildGO8));
   makeAction(g, buildGO8);

   action buildARC;
   buildARC.actionCode = OBTAIN_ARC;
   strcpy(buildARC.destination, "RLRLRLLRLR");
   assert(isLegalAction(g,buildARC));
   makeAction(g, buildARC);

   buildARC.actionCode = OBTAIN_ARC;
   strcpy(buildARC.destination, "RLRLRLLRL");
   assert(isLegalAction(g,buildARC));
   makeAction(g, buildARC);

   strcpy(buildCampus.destination, "RLRLRLLR");
   assert(isLegalAction(g,buildCampus));
   makeAction(g, buildCampus);

   strcpy(buildGO8.destination, buildCampus.destination);
   assert(isLegalAction(g,buildGO8));
   makeAction(g, buildGO8);

   strcpy(buildGO8.destination, "RLRLRLLRLRLL");
   assert(isLegalAction(g,buildGO8));
   makeAction(g, buildGO8);

   strcpy(buildGO8.destination, "RRLRL");
   assert(isLegalAction(g,buildGO8));
   makeAction(g, buildGO8);

   assert(getStudents(g, UNI_B, STUDENT_THD) == 6);
   assert(getStudents(g, UNI_B, STUDENT_BPS) == 0);
   assert(getStudents(g, UNI_B, STUDENT_BQN) == 0);
   assert(getStudents(g, UNI_B, STUDENT_MJ) == 2);
   assert(getStudents(g, UNI_B, STUDENT_MTV) == 2);
   assert(getStudents(g, UNI_B, STUDENT_MMONEY) == 0);

   assert(getCampuses(g, UNI_B) == 0);
   assert(getGO8s(g, UNI_B) == 4);
   assert(getKPIpoints(g, UNI_B) == 90);
   assert(getARCs(g, UNI_B) == 5);


   throwDice(g, 3);
   assert(getWhoseTurn(g) == UNI_C);

   counter = 0;
   exchange.disciplineFrom = STUDENT_MJ;
   exchange.disciplineTo = STUDENT_MMONEY;

   while (counter < 9) {
      assert(isLegalAction(g, exchange));
      makeAction(g, exchange);
      counter++;
   }

   counter = 0;
   exchange.disciplineTo = STUDENT_MTV;
   while (counter < 1) {
      assert(isLegalAction(g, exchange));
      makeAction(g, exchange);
      counter++;
   }

   counter = 0;
   exchange.disciplineTo = STUDENT_BQN;
   while (counter < 2) {
      assert(isLegalAction(g, exchange));
      makeAction(g, exchange);
      counter++;
   }

   counter = 0;
   exchange.disciplineTo = STUDENT_BPS;
   while (counter < 2) {
      assert(isLegalAction(g, exchange));
      makeAction(g, exchange);
      counter++;
   }

   assert(getStudents(g, UNI_C, STUDENT_THD) == 0);
   assert(getStudents(g, UNI_C, STUDENT_BPS) == 4);
   assert(getStudents(g, UNI_C, STUDENT_BQN) == 4);
   assert(getStudents(g, UNI_C, STUDENT_MJ) == 55);
   assert(getStudents(g, UNI_C, STUDENT_MTV) == 1);
   assert(getStudents(g, UNI_C, STUDENT_MMONEY) == 9);


   strcpy(buildARC.destination, "RRLRLLRLRL");
   assert(isLegalAction(g, buildARC));
   makeAction(g, buildARC);

   strcpy(buildGO8.destination, buildARC.destination);
   assert(isLegalAction(g, buildGO8));
   makeAction(g, buildGO8);

   strcpy(buildARC.destination, "RRLRLLRLR");
   assert(isLegalAction(g,buildARC));
   makeAction(g, buildARC);

   strcpy(buildCampus.destination, "RRLRLLRL");
   assert(isLegalAction(g,buildCampus));
   makeAction(g, buildCampus);

   strcpy(buildGO8.destination, buildCampus.destination);
   assert(isLegalAction(g, buildGO8));
   makeAction(g, buildGO8);

   strcpy(buildGO8.destination, "LRLRL");
   assert(isLegalAction(g, buildGO8));
   makeAction(g, buildGO8);

   assert(getStudents(g, UNI_C, STUDENT_THD) == 0);
   assert(getStudents(g, UNI_C, STUDENT_BPS) == 1);
   assert(getStudents(g, UNI_C, STUDENT_BQN) == 1);
   assert(getStudents(g, UNI_C, STUDENT_MJ) == 48);
   assert(getStudents(g, UNI_C, STUDENT_MTV) == 0);
   assert(getStudents(g, UNI_C, STUDENT_MMONEY) == 0);

   assert(getCampuses(g, UNI_C) == 0);
   assert(getGO8s(g, UNI_C) == 3);
   assert(getKPIpoints(g, UNI_C) == 66);
   assert(getARCs(g, UNI_C) == 3);

   throwDice(g,3);

   assert(getWhoseTurn(g) == UNI_A);
   strcpy(buildGO8.destination,"");
   assert(!isLegalAction(g, buildGO8));

   strcpy(buildARC.destination, "RRLRLL");
   assert(isLegalAction(g, buildARC));

}
