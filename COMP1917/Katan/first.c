

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



static int getDisciplines1(Game g);
action firstTurnStrat(Game g);
action buildCampusStrat(Game g);
static action exchangeForCampus(Game g);


action buildCampusStrat(Game g) {
   action a;

   if ()
   a = exchangeForCampus(g);
   if (a.actionCode == PASS || (isLegalAction(g, a) == FALSE)) {
      a.actionCode = BUILD_CAMPUS;

      strcpy(a.destination, startPoint(g));
      int length = strlen(a.destination);
      a.destination[length] = 'L';
      a.destination[length + 1] = 'L';
      int counter = 0;
      while(!isLegalAction && counter < 6) {
         if (a.destination[length + counter + 1] == 'L')
            a.destination[length + counter + 2] = 'R';
         } else {
            a.destination[length + counter + 2] = 'L';
         }
            a.destination[length + counter + 3] = '\0';

      }
      if (!isLegal)


   }





action firstTurnStrat(Game g) {
   action a;
   int player = getWhoseTurn(g);
   strcpy(a.destination, startPoint(g));
   int length = strlen(a.destination);
   if (getARCs(g, player) == 0) {
      a.actionCode = OBTAIN_ARC;
      a.destination[length] = 'L';
      a.destination[length + 1] = '\0';
   } else if (getARCs(g, player) == 1) {
      a.actionCode = OBTAIN_ARC;
      a.destination[length] = 'L';
      a.destination[length + 1] = 'L';
      a.destination[legnth + 2] = '\0';
   } else {
      a.actionCode = BUILD_CAMPUS;
      a.destination[length] = 'L';
      a.destination[length + 1] = 'L';
      a.destination[legnth + 2] = '\0';
   }
   return a;

}

static int getDisciplines1(Game g) {

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

path startPoint(g) {
   path destination;
   int type = getDiscpilines1(g);
   if (type != STUDENT_BPS) {
      if (player == UNI_A) {
         strcpy(destination, "RLRLRLRLRLL");
      } else if (player == UNI_B) {
         strcpy(destination, "RLLRRLRLLRLL");
      } else if (player == UNI_C) {
         strcpy(destination, "RRLRLLRLRL");
      }
   } else {
      if (player == UNI_A) {
         strcpy(destination, "LB");
      } else if (player == UNI_B) {
         strcpy(destination, "RRLRL");
      } else if (player == UNI_C) {
         strcpy(destination, "LRLRLRB");
      }
   }
   return destination;
}

static action exchangeForCampus(Game g) {

	int player = getWhoseTurn(g);
	int numBPS = getStudents(g, player, STUDENT_BPS);
	int numBQN = getStudents(g, player, STUDENT_BQN);
	int numMJ = getStudents(g, player, STUDENT_MJ);
	int numMTV = getStudents(g, player, STUDENT_MTV);
	int numMMoney = getStudents(g, player, STUDENT_MMONEY);
    action retrain;
    retrain.actionCode = PASS;

    if (numBPS > 3 || numBQN > 3 || numMJ > 3 || numMTV > 3 || numMMoney >= 3) {
    	retrain.actionCode = RETRAIN_STUDENTS;

	if (numMMoney >= 3) {
		retrain.disciplineFrom = STUDENT_MMONEY;
	} else if (numBPS > 3) {
    		retrain.disciplineFrom = STUDENT_BPS;
    	} else if (numMTV > 3) {
    		retrain.disciplineFrom = STUDENT_MTV;
    	} else if (numMJ > 3) {
    		retrain.disciplineFrom = STUDENT_MJ;
    	} else if (numBQN > 3) {
    		retrain.disciplineFrom = STUDENT_BQN;
    	}

    	if (numMTV < 1 && numBQN > 0 && numBPS > 0 && numMJ > 0) {
    		retrain.disciplineTo = STUDENT_MTV;
    	} else if (numMJ < 1 && numMTV > 0 && numBQN > 0 && numBPS > 0) {
    		retrain.disciplineTo = STUDENT_MJ;
    	} else if (numBQN < 1 && numMTV > 0 && numMJ > 0 && numBPS > 0) {
    		retrain.disciplineTo = STUDENT_BQN;
    	} else if (numBPS < 1 && (numMTV >= 3 || numMMoney >= 3)) {
        		retrain.disciplineTo = STUDENT_BPS;
        }


    }
   	return retrain;
}


static int studentsForCampus(Game g, int player) {
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
