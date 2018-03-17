/*
 * AI for knowledge Island
 * The AI currently plants two ARCs and a campus starting from its first spawn, following a left-right pattern.
 * It works checks if it can make a campus, if it can't it checks if it can make an ARC. If not, it checks building ARCs
 * Once the AI has enough resources to start a spinoff, has at least 5 campuses and cannot do anything else apart from retrain,
 * the AI will start a spinoff (almost never happens)
 * otherwise, if no other points are possible and they have > 3 MMONEYs or MTVs, they get converted
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "Game.h"
#include "mechanicalTurk.h"

#define BPS_PER_CAMPUS 1
#define BQN_PER_CAMPUS 1
#define MJ_PER_CAMPUS  1
#define MTV_PER_CAMPUS 1

#define BPS_PER_ARC 1
#define BQN_PER_ARC 1

#define MMONEY_PER_GO8 3
#define MJ_PER_GO8 2

#define MJ_FOR_SPINOFF 1
#define MMONEY_FOR_SPINOFF 1
#define MTV_FOR_SPINOFF 1

static int studentsForARC(Game g);
static int studentsForCampus(Game g);
static void getStartCampus1(Game g, path location);
static void getStartCampus2(Game g, path location);
static void convertDecision(Game g, action *nextAction);
static void exchangeForCampus(Game g, action *nextAction);
static void determineCampusPath1(Game g, action *nextAction);
static void determineCampusPath2(Game g, action *nextAction);
static int getDisciplines2(Game g);
//static int getDisciplines1(Game g);


action decideAction (Game g) {

   int player = getWhoseTurn(g);
   int numARCs = getARCs(g, player);
   int numCampuses = getCampuses(g, player);
   int numMTV = getStudents(g, player, STUDENT_MTV);
   int numMMONEY = getStudents(g, player, STUDENT_MMONEY);
   int numBQN = getStudents(g, player, STUDENT_BQN);
   int numBPS = getStudents(g, player, STUDENT_BPS);
   int numMJ = getStudents(g, player, STUDENT_MJ);

   action nextAction;
   nextAction.actionCode = PASS;
   action *a = &nextAction;


   if (studentsForCampus(g) == TRUE || (numARCs > 0 && numARCs%2 == 0)) {
      nextAction.actionCode = BUILD_CAMPUS;

      int type2 = getDisciplines2(g);

	if (!studentsForCampus(g)) {
	    	exchangeForCampus(g, a);
	} else {

        	if (type2 != STUDENT_BPS) {
		    determineCampusPath1(g, a);
        	} else {
	            determineCampusPath2(g, a);
	    	}
 		if (!isLegalAction(g, nextAction)) {
            	    determineCampusPath1(g, a);
        	}
		if (!isLegalAction(g, nextAction)) {
            	    determineCampusPath2(g, a);
        	}
	}

      if (!isLegalAction(g, nextAction)) {
        nextAction.actionCode = PASS;
      }
   }
   if (studentsForARC(g) == TRUE && nextAction.actionCode == PASS) {
     nextAction.actionCode = OBTAIN_ARC;

     int type2 = getDisciplines2(g);
     if (type2 != STUDENT_BPS) {
            determineCampusPath1(g, a);
     } else {
         determineCampusPath2(g, a);
     }
     if (!isLegalAction(g, nextAction)) {
         determineCampusPath1(g, a);
     }
     if (!isLegalAction(g, nextAction)) {
        determineCampusPath2(g, a);
     }
     if (!isLegalAction(g, nextAction)) {
        nextAction.actionCode = PASS;
     }

   }

   if (numMJ >= MJ_FOR_SPINOFF && numMTV >= MTV_FOR_SPINOFF &&
      numMMONEY >= MMONEY_FOR_SPINOFF && nextAction.actionCode == PASS && numCampuses > 4)  {
      nextAction.actionCode = START_SPINOFF;
   }

   if ((numMTV >= 3 || numMMONEY >= 3 || numBPS > 3 || numMJ > 3 || numBQN > 3) && nextAction.actionCode == PASS) {

      convertDecision(g, a);
   }

   if (!isLegalAction(g, nextAction)) {
      nextAction.actionCode = PASS;
   }
   return nextAction;
}



static void convertDecision(Game g, action *nextAction) {

   int player = getWhoseTurn(g);
   int numARCs = getARCs(g, player);
   int numMTV = getStudents(g, player, STUDENT_MTV);
   int numMMONEY = getStudents(g, player, STUDENT_MMONEY);
   int numBQN = getStudents(g, player, STUDENT_BQN);
   int numBPS = getStudents(g, player, STUDENT_BPS);
   int numMJ = getStudents(g, player, STUDENT_MJ);


      nextAction->actionCode = RETRAIN_STUDENTS;
         nextAction->disciplineTo = STUDENT_BQN;;
      if (numMJ == 0) {
         nextAction->disciplineTo = STUDENT_MJ;
      }

      if (numMTV >= 3) {
         nextAction->disciplineFrom = STUDENT_MTV;
      } else if (numMMONEY >= 3) {
         nextAction->disciplineFrom = STUDENT_MMONEY;
      } else if (numBPS > 3) {
  	 nextAction->disciplineFrom = STUDENT_BPS;
      }
      if (numBPS > 3 && numMJ > 0 && numBQN > 0 && numMTV == 0 && numARCs%2 == 0 && numARCs > 0) {
	 nextAction->disciplineFrom = STUDENT_BPS;
	 nextAction->disciplineTo = STUDENT_MTV;
      }

}

static int studentsForARC(Game g) {

   int player = getWhoseTurn(g);
   int outcome = FALSE;
   int numBPS = getStudents(g, player, STUDENT_BPS);
   int numBQN = getStudents(g, player, STUDENT_BQN);
   if (numBQN >= BQN_PER_ARC && numBPS >= BPS_PER_ARC) {
      outcome = TRUE;
   }
   return outcome;
}

static int studentsForCampus(Game g) {

   int player = getWhoseTurn(g);
   int outcome = FALSE;
   int numBPS = getStudents(g, player, STUDENT_BPS);
   int numBQN = getStudents(g, player, STUDENT_BQN);
   int numMJ = getStudents(g, player, STUDENT_MJ);
   int numMTV = getStudents(g, player, STUDENT_MTV);

   if (numBPS >= BPS_PER_CAMPUS && numBQN >= BQN_PER_CAMPUS &&
   numMJ >= MJ_PER_CAMPUS && numMTV >= MTV_PER_CAMPUS) {
      outcome = TRUE;
   }
   return outcome;
}

static void getStartCampus1(Game g, path location) {

   int player = getWhoseTurn(g);
   strcpy(location, "R");

   if (player == UNI_A) {
      strcpy(location,"R");
   } else if (player == UNI_B) {
      strcpy(location, "RRLRLLBB");
   } else if (player == UNI_C) {
      strcpy(location, "LRLRLB");
   }
}

static void getStartCampus2(Game g, path location) {

   int player = getWhoseTurn(g);
   strcpy(location, "R");

   if (player == UNI_A) {
      strcpy(location, "RLRLRLRLRLLLBB");
   } else if (player == UNI_C) {
      strcpy(location, "RLRLRLRRLR");
   } else if (player == UNI_B) {
      strcpy(location, "LRLRLRRLRLB");
   }
}



static void exchangeForCampus(Game g, action *retrain) {

	int player = getWhoseTurn(g);
	int numBPS = getStudents(g, player, STUDENT_BPS);
	int numBQN = getStudents(g, player, STUDENT_BQN);
	int numMJ = getStudents(g, player, STUDENT_MJ);
	int numMTV = getStudents(g, player, STUDENT_MTV);
	int numMMoney = getStudents(g, player, STUDENT_MMONEY);
    retrain->actionCode = PASS;

    if (numBPS > 3 || numBQN > 3 || numMJ > 3 || numMTV > 3 || numMMoney >= 3) {
    	retrain->actionCode = RETRAIN_STUDENTS;

	if (numMMoney >= 3) {
		retrain->disciplineFrom = STUDENT_MMONEY;
	} else if (numBPS > 3) {
    		retrain->disciplineFrom = STUDENT_BPS;
    	} else if (numMTV > 3) {
    		retrain->disciplineFrom = STUDENT_MTV;
    	} else if (numMJ > 3) {
    		retrain->disciplineFrom = STUDENT_MJ;
    	} else if (numBQN > 3) {
    		retrain->disciplineFrom = STUDENT_BQN;
    	}

    	if (numMTV < 1 && numBQN > 0 && numBPS > 0 && numMJ > 0) {
    		retrain->disciplineTo = STUDENT_MTV;
    	} else if (numMJ < 1 && numMTV > 0 && numBQN > 0 && numBPS > 0) {
    		retrain->disciplineTo = STUDENT_MJ;
    	} else if (numBQN < 1 && numMTV > 0 && numMJ > 0 && numBPS > 0) {
    		retrain->disciplineTo = STUDENT_BQN;
    	} else if (numBPS < 1 && (numMTV >= 3 || numMMoney >= 3)) {
        	retrain->disciplineTo = STUDENT_BPS;
        }


    }
}


static void determineCampusPath1(Game g, action *nextAction) {

//    int player = getWhoseTurn(g);

   getStartCampus1(g, nextAction->destination);
   int length = strlen(nextAction->destination);


   while(isLegalAction(g, *nextAction) != TRUE && length < 150) {
      length = strlen(nextAction->destination);
      if(nextAction->destination[length - 1] == 'L') {
         nextAction->destination[length] = 'R';
      } else {
         nextAction->destination[length] = 'L';
      }
      nextAction->destination[length + 1] = '\0';
   }
}

static void determineCampusPath2(Game g, action *nextAction) {

//   int player = getWhoseTurn(g);

   getStartCampus2(g, nextAction->destination);
   int length = strlen(nextAction->destination);

      while(isLegalAction(g, *nextAction) != TRUE && length < 150) {
         length = strlen(nextAction->destination);
         if(nextAction->destination[length - 1] == 'L') {
            nextAction->destination[length] = 'R';
         } else {
            nextAction->destination[length] = 'L';
         }
         nextAction->destination[length + 1] = '\0';
      }

}

/*
static int getDisciplines1(Game g,int player) {

    int type = 0;
    if (player == UNI_A) {
        type = getDiscipline(g, 7);
    } else if (player == UNI_B) {
        type = getDiscipline(g, 0);
    } else {
        type = getDiscipline(g, 16);
    }
    return type;
}
*/
static int getDisciplines2(Game g) {
    int player = getWhoseTurn(g);
    int type = 0;
    if (player == UNI_A) {
        type = getDiscipline(g, 11);
    } else if (player == UNI_B) {
        type = getDiscipline(g, 18);
    } else {
        type = getDiscipline(g, 2);
    }
    return type;
}
