// Game.c
// Aman Sanghvi & Jake Edwards
// 15.5.16

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "Game.h"


#define BPS_PER_CAMPUS 1
#define BQN_PER_CAMPUS 1
#define MJ_PER_CAMPUS  1
#define MTV_PER_CAMPUS 1

#define BPS_PER_ARC 1
#define BQN_PER_ARC 1

#define MMONEY_PER_GO8 3
#define MJ_PER_GO8 2

#define MAX_GO8S 8

#define MJ_PER_SPINOFF 1
#define MMONEY_PER_SPINOFF 1
#define MTV_PER_SPINOFF 1
#define NUM_STUDENTS 6
#define VERTEX_HEIGHT 11
#define VERTEX_WIDTH 6
#define EDGE_HEIGHT 11
#define EDGE_WIDTH 11
#define HEX_SIZE 6

#define INITIAL_KPIS 20
#define INITIAL_ARCS 0
#define INITIAL_CAMPUSES 2
#define INITIAL_GO8S 0
#define INITIAL_PUBLICATIONS 0
#define INITIAL_IPS 0

typedef struct _coord {
 int row;
 int col;
 int lastRow;
 int lastCol;
} coord;

typedef struct _game {
 int turnNumber;
 int disciplines[NUM_REGIONS];
 int dice[NUM_REGIONS];

 int vertices[VERTEX_HEIGHT + 2][VERTEX_WIDTH + 2];
 int edges[EDGE_HEIGHT + 2][EDGE_WIDTH + 2];

 int students[NUM_UNIS + 1][NUM_STUDENTS];
 int KPIs[NUM_UNIS + 1];
 int ARCs[NUM_UNIS + 1];
 int campuses[NUM_UNIS + 1];
 int GO8s[NUM_UNIS + 1];
 int publications[NUM_UNIS + 1];
 int IPs[NUM_UNIS + 1];

 int previousMostARCs;
 int previousMostPublications;
} game;


static void initialiseResources(int resource[], int value);

static void makeBoard(Game g, int discipline[], int dice[]);
static coord stringToVertex(path location);
static coord subtractVector(coord point, coord vector);
static coord addVector(coord point, coord vector);
static coord stringToEdge(path location);
static coord findNewRow(coord point);

static coord getTileLocation(Game g, int tilePosition);
static int validCampusPath(path location, Game g, int player);
static int validARCpath(path location, Game g, int player);
static int validGO8path(path location, Game g, int player);
static int isLegalPath(path location);





Game newGame (int discipline[], int dice[]) {
 Game g = malloc(sizeof(game));

 g->turnNumber = -1;

 int counter = 0;
 while (counter < NUM_REGIONS) {
     g->disciplines[counter] = discipline[counter];
     counter++;
 }
 counter = 0;
 while (counter < NUM_REGIONS) {
     g->dice[counter] = dice[counter];
     counter++;
 }

 makeBoard(g, discipline, dice);

 counter = UNI_A;
 while (counter <= UNI_C) {
     g->students[counter][STUDENT_THD] = 0;
     g->students[counter][STUDENT_BPS] = 3;
     g->students[counter][STUDENT_BQN] = 3;
     g->students[counter][STUDENT_MJ] = 1;
     g->students[counter][STUDENT_MTV] = 1;
     g->students[counter][STUDENT_MMONEY] = 1;
     counter++;
 }

 initialiseResources(g->KPIs, INITIAL_KPIS);
 initialiseResources(g->ARCs, INITIAL_ARCS);
 initialiseResources(g->campuses, INITIAL_CAMPUSES);
 initialiseResources(g->GO8s, INITIAL_GO8S);
 initialiseResources(g->publications, INITIAL_PUBLICATIONS);
 initialiseResources(g->IPs, INITIAL_IPS);
 g->previousMostARCs = NO_ONE;
 g->previousMostPublications = NO_ONE;

 return g;
}



void disposeGame (Game g) {
 free(g);
}



void makeAction (Game g, action a) {

 if (a.actionCode != OBTAIN_PUBLICATION && a.actionCode != OBTAIN_IP_PATENT) {
     assert(isLegalAction(g,a));
 } else {
     assert(getStudents(g,getWhoseTurn(g),STUDENT_MMONEY) > 0);
     assert(getStudents(g,getWhoseTurn(g),STUDENT_MMONEY) > 0);
     assert(getStudents(g,getWhoseTurn(g),STUDENT_MJ) > 0);
 }
 int player = getWhoseTurn(g);

 if (a.actionCode == PASS) {

 } else if (a.actionCode == BUILD_CAMPUS) {
     coord point = stringToVertex(a.destination);
     g->vertices[point.row][point.col] = player;
     g->students[player][STUDENT_BQN] -= 1;
     g->students[player][STUDENT_BPS] -= 1;
     g->students[player][STUDENT_MJ] -= 1;
     g->students[player][STUDENT_MTV] -= 1;
     g->KPIs[player] += 10;
     g->campuses[player]++;

 } else if (a.actionCode == BUILD_GO8) {
     coord point = stringToVertex(a.destination);
     g->vertices[point.row][point.col] = player + NUM_UNIS;
     g->students[player][STUDENT_MJ] -= 2;
     g->students[player][STUDENT_MMONEY] -= 3;
     g->KPIs[player] += 10;
     g->GO8s[player]++;
 g->campuses[player]--;

 } else if (a.actionCode == OBTAIN_ARC) {
     coord point = stringToEdge(a.destination);
//printf("The edge this leads to is %d %d\n", point.row, point.col);
     g->edges[point.row][point.col] = player;
     g->students[player][STUDENT_BQN] -= 1;
     g->students[player][STUDENT_BPS] -= 1;
     g->KPIs[player] += 2;
     g->ARCs[player]++;


         if (g->previousMostARCs != getMostARCs(g)) {
         g->KPIs[g->previousMostARCs] -= 10;
         g->KPIs[getMostARCs(g)] += 10;
         }
     g->previousMostARCs = getMostARCs(g);


 } else if (a.actionCode == OBTAIN_PUBLICATION || a.actionCode == OBTAIN_IP_PATENT) {

 g->students[player][STUDENT_MJ]--;
 g->students[player][STUDENT_MTV]--;
 g->students[player][STUDENT_MMONEY]--;

 if (a.actionCode == OBTAIN_IP_PATENT) {
     g->KPIs[player] += 10;
     g->IPs[player]++;
 } else {
     g->publications[player]++;
     if (getMostPublications(g) == player) {
         g->KPIs[player] += 10;

         if (g->previousMostPublications != getMostPublications(g)) {
         g->KPIs[g->previousMostPublications] -= 10;
         }
     g->previousMostPublications = getMostPublications(g);
     }
 }



//  } else if (a.actionCode == OBTAIN_PUBLICATION) {

//  } else if (a.actionCode == OBTAIN_IP_PATENT) {

 } else if (a.actionCode == RETRAIN_STUDENTS) {

     int disciplineFrom = a.disciplineFrom;
     int disciplineTo = a.disciplineTo;

     g->students[player][disciplineFrom] -= getExchangeRate(g, player, disciplineFrom, disciplineTo);
     g->students[player][disciplineTo] += 1;

 }

}



void throwDice (Game g, int diceScore) {
 int studentType = 0;
 int tilePosition = 0;
 g->turnNumber++;

if (diceScore == 7) {
 int players = 1;
 while (players <= UNI_C) {
     g->students[players][STUDENT_THD] += g->students[players][STUDENT_MMONEY];
     g->students[players][STUDENT_THD] += g->students[players][STUDENT_MTV];
     g->students[players][STUDENT_MMONEY] = 0;
     g->students[players][STUDENT_MTV] = 0;
     players++;
 }
}


 while (tilePosition < 19) {
     if (g->dice[tilePosition] == diceScore) {
         studentType = g->disciplines[tilePosition];
         int campusType1 = 0;
         int campusType2 = 0;
         int counter = 0;

         coord tile = getTileLocation(g, tilePosition);
         while(counter < 3) {
         if (tile.col < 2) {
             campusType1 = g->vertices[tile.row + counter][tile.col];
             campusType2 = g->vertices[tile.row + counter + 1][tile.col + 1];
         } else if (tile.col == 2) {
             campusType1 = g->vertices[tile.row + counter][tile.col];
             campusType2 = g->vertices[tile.row + counter][tile.col + 1];
         } else if (tile.col > 2) {
             campusType1 = g->vertices[tile.row + counter][tile.col];
             campusType2 = g->vertices[tile.row + counter - 1][tile.col + 1];
         }
         if (campusType1 <= NUM_UNIS) {
             g->students[campusType1][studentType]++;

         } else {
             g->students[campusType1 - NUM_UNIS][studentType] += 2;

         }
         if (campusType2 <= NUM_UNIS) {
             g->students[campusType2][studentType]++;
         } else {
             g->students[campusType2 - NUM_UNIS][studentType] += 2;
         }
         counter++;
         }
     }
 tilePosition++;

 }


}
int getDiceValue (Game g, int regionID) {
 return g->dice[regionID];
}



int getMostARCs (Game g) {
 int uniA = g->ARCs[UNI_A];
 int uniB = g->ARCs[UNI_B];
 int uniC = g->ARCs[UNI_C];
 int mostARCs;
 if (uniA > uniB && uniA > uniC) {
     mostARCs = UNI_A;
 }
 else if (uniB > uniC && uniB > uniA) {
     mostARCs = UNI_B;
 }
 else if (uniC > uniB && uniC > uniA) {
     mostARCs = UNI_C;
 }
 else {
     mostARCs = g->previousMostARCs;
 }
 return mostARCs;
}



int getMostPublications (Game g) {
 int uniA = g->publications[UNI_A];
 int uniB = g->publications[UNI_B];
 int uniC = g->publications[UNI_C];
 int mostPublications;
 if (uniA > uniB && uniA > uniC) {
     mostPublications = UNI_A;
 }
 else if (uniB > uniC && uniB > uniA) {
     mostPublications = UNI_B;
 }
 else if (uniC > uniB && uniC > uniA) {
     mostPublications = UNI_C;
 }
 else {
     mostPublications = g->previousMostPublications;
 }
 return mostPublications;
}

int getTurnNumber (Game g) {
 return g->turnNumber;
}


int getWhoseTurn (Game g) {
 return (g->turnNumber%3) + 1;

}


int getCampus(Game g, path pathToVertex) {
 coord point = stringToVertex(pathToVertex);
 return g->vertices[point.row][point.col];
}


int getARC(Game g, path pathToEdge) {
 coord edge = stringToEdge(pathToEdge);
 return g->edges[edge.row][edge.col];
}



int isLegalAction (Game g, action a) {
 int outcome = TRUE;
 int player = getWhoseTurn(g);
 if (getTurnNumber(g) == -1) {
     outcome = FALSE;
 }

 int numBPS = getStudents(g, player, STUDENT_BPS);
 int numBQN = getStudents(g, player, STUDENT_BQN);
 int numMJ = getStudents(g, player, STUDENT_MJ);
 int numMTV = getStudents(g, player, STUDENT_MTV);
 int numMMONEY = getStudents(g, player, STUDENT_MMONEY);
 if (getTurnNumber(g) == -1) {
     outcome = FALSE;
 }


if (a.actionCode > 7 || a.actionCode < 0) {
 outcome = FALSE;
}

 if (a.actionCode == BUILD_CAMPUS) {

     if (numBPS < BPS_PER_CAMPUS || numBQN < BQN_PER_CAMPUS || //If any statements are true, its illegal
     numMTV < MTV_PER_CAMPUS || numMJ < MJ_PER_CAMPUS) {
         outcome = FALSE;
//      	printf("Failed validARCpath)\n");
     } else if (!validCampusPath(a.destination, g, player)) {
         outcome = FALSE;
//      	printf("Failed validCampusPath\n");
     }

 } else if(a.actionCode == BUILD_GO8) {

     if(numMJ < MJ_PER_GO8 || numMMONEY < MMONEY_PER_GO8) {
         outcome = FALSE;
 //    	printf("Failing students for GO8\n");
     } else if (!validGO8path(a.destination, g, player)) {
         outcome = FALSE;
   //  	printf("Failing valid GO8path\n");
     } else if (getGO8s(g, UNI_A) + getGO8s(g, UNI_B) + getGO8s(g, UNI_C) >= MAX_GO8S) {
       outcome = FALSE;
     }

 } else if(a.actionCode == OBTAIN_ARC) {

     if(numBQN < BQN_PER_ARC || numBPS < BPS_PER_ARC) {
         outcome = FALSE;
     } else if (!validARCpath(a.destination, g, player)) {
         outcome = FALSE;
     }
 } else if(a.actionCode == START_SPINOFF) {

     if (numMJ <= MJ_PER_SPINOFF || numMTV < MTV_PER_SPINOFF ||
         numMMONEY <= MMONEY_PER_SPINOFF) {
         outcome = FALSE;
     }

 } else if(a.actionCode == OBTAIN_PUBLICATION) {
     outcome = FALSE;
 } else if(a.actionCode == OBTAIN_IP_PATENT) {
     outcome = FALSE;
 } else if(a.actionCode == RETRAIN_STUDENTS) {
     int exchangeRate = getExchangeRate(g, player, a.disciplineFrom, a.disciplineTo);
     if (getStudents(g, player, a.disciplineFrom) < exchangeRate) {
         outcome = FALSE;
     }
 }

return outcome;
}

int getKPIpoints (Game g, int player) {
 return g->KPIs[player];
}

int getARCs (Game g, int player) {
 return g->ARCs[player];
}

int getGO8s (Game g, int player) {
 return g->GO8s[player];
}


int getCampuses (Game g, int player) {
 return g->campuses[player];
}


int getIPs (Game g, int player) {
 return g->IPs[player];

}


int getPublications (Game g, int player) {
 return g->publications[player];
}


int getStudents (Game g, int player, int discipline) {
 return g->students[player][discipline];
}



int getExchangeRate (Game g, int player,
                     int disciplineFrom, int disciplineTo) {

 int exchangeRate = 3;
 if (disciplineFrom == STUDENT_BPS) {
     if (g->vertices[7][1] == player || g->vertices[8][1] == player
         || g->vertices[7][1] == player + NUM_UNIS || g->vertices[8][1] == player +NUM_UNIS) {
     exchangeRate = 2;
     }
 } else if (disciplineFrom == STUDENT_BQN) {
     if (g->vertices[3][5] == player || g->vertices[4][5] == player
         || g->vertices[3][5] == player + NUM_UNIS || g->vertices[4][5] == player + NUM_UNIS) {
     exchangeRate = 2;
     }
 } else if (disciplineFrom ==  STUDENT_MJ) {
     if (g->vertices[7][4] == player || g->vertices[8][4] == player
         || g->vertices[7][4] == player +NUM_UNIS || g->vertices[8][4] == player + NUM_UNIS) {
     exchangeRate = 2;
     }
 }  else if (disciplineFrom == STUDENT_MTV) {
     if (g->vertices[0][1] == player || g->vertices[1][2] == player
         || g->vertices[0][1] == player + NUM_UNIS || g->vertices[1][2] == player + NUM_UNIS) {
     exchangeRate = 2;
     }
 } else if (disciplineFrom == STUDENT_MMONEY) {
     if (g->vertices[2][3] == player || g->vertices[0][4] == player
         || g->vertices[2][3] == player + NUM_UNIS || g->vertices[0][4] == player + NUM_UNIS) {
     exchangeRate = 2;
     }
 }
 return exchangeRate;
}


static void initialiseResources(int resource[], int value) {
 int counter = UNI_A;
 while (counter <= UNI_C) {
     resource[counter] = value;
     counter++;
 }
}


static void makeBoard(Game g, int discipline[], int dice[]) {

 int counter1 = 0;
 int counter2 = 0;
 while (counter1 < VERTEX_HEIGHT + 2) {
    counter2 = 0;
    while (counter2 < VERTEX_WIDTH + 2) {
       g->vertices[counter1][counter2] = 0;
       counter2++;
    }
    counter1++;
 }
 counter1 = 0;
 counter2 = 0;
 while (counter1 < EDGE_HEIGHT + 2) {
    counter2 = 0;
    while (counter2 < EDGE_WIDTH + 2) {
       g->edges[counter1][counter2] = 0;
       counter2++;
    }
    counter1++;
 }
 counter1 = 0;
 counter2 = 0;

 g->vertices[0][0] = NO_ONE;
 g->vertices[1][0] = UNI_B;
 g->vertices[2][0] = NO_ONE;
 g->vertices[3][0] = NO_ONE;
 g->vertices[4][0] = NO_ONE;
 g->vertices[5][0] = NO_ONE;
 g->vertices[6][0] = UNI_C;

 int counter = 0;
 while (counter < 9) {
     g->vertices[counter][1] = NO_ONE;
     counter++;
 }

 g->vertices[0][2] = UNI_A;
 counter = 1;
 while (counter < 11) {
     g->vertices[counter][2] = NO_ONE;
     counter++;
 }

 counter = 0;
 while (counter < 10) {
     g->vertices[counter][3] = NO_ONE;
     counter++;
 }
 g->vertices[10][3] = UNI_A;

 counter = 0;
 while (counter < 9) {
     g->vertices[counter][4] = NO_ONE;
     counter++;
 }

 g->vertices[0][5] = UNI_C;
 g->vertices[1][5] = NO_ONE;
 g->vertices[2][5] = NO_ONE;
 g->vertices[3][5] = NO_ONE;
 g->vertices[4][5] = NO_ONE;
 g->vertices[5][5] = UNI_B;
 g->vertices[6][5] = NO_ONE;


 int row = 0;
 int col = 0;
 while (row < EDGE_HEIGHT) {
     while (col < EDGE_WIDTH) {
     g->edges[row][col] = 0;
     col++;
     }
 row++;
 col = 0;
 }

}
static int validCampusPath(path location, Game g, int player){
 int outcome = TRUE;
 int length = strlen(location);
 coord vertex = stringToVertex(location);
//Check its on the map

 if (length > 150) {
     outcome = FALSE;
//printf("1!!\n");
 } else if (vertex.col < 0 || vertex.col > 5) {
     outcome = FALSE;
//printf("2!!\n");
 } else if (vertex.col % 5 == 0 && (vertex.row < 0 || vertex.row > 6)) {
     outcome = FALSE;
//printf("3!!\n");
 } else if (vertex.col % 3 == 1 && (vertex.row < 0 || vertex.row > 8)) {
     outcome = FALSE;
//printf("4!!\n");
 } else if (vertex.row < 0 || vertex.row > 10) {
     outcome = FALSE;
//printf("5!!\n");
 } else if (!isLegalPath(location)) {
     outcome = FALSE;
//printf("6!!\n");
 }
// printf("outcome after on map stuff: %d\n",outcome);


 if (getCampus(g, location) != VACANT_VERTEX) {
        outcome = FALSE;
 }
//  printf("outcome after VACANT: %d\n",outcome);
 int leftARC = NO_ONE;
 int rightARC = NO_ONE;
 int backARC = NO_ONE;
 int leftCampus = NO_ONE;
 int rightCampus = NO_ONE;
 int backCampus = NO_ONE;

 location[length] = 'L';
 location[length + 1] = '\0';

 if (isLegalPath(location)) {
    leftARC = getARC(g, location);
    leftCampus = getCampus(g, location);
 }
 location[length] = 'R';
 rightARC = getARC(g, location);
 if (isLegalPath(location)) {
    rightCampus = getCampus(g, location);
 }

 location[length] = 'B';
 backARC = getARC(g, location);
 if (isLegalPath(location)) {
    backCampus = getCampus(g, location);
 }

//  printf("BARC: %d LARC: %d RARC: %d\n",backARC,leftARC,rightARC);
//  printf("Bcamp: %d Lcamp: %d Rcamp: %d\n",backCampus,leftCampus,rightCampus);

 if (leftARC != player && rightARC != player && backARC != player) {
 //if there aren't any nearby ARCs, it fails
     outcome = FALSE;
//   printf("failing no nearby ARCs\n");
 } else if (leftCampus != NO_ONE || rightCampus != NO_ONE || backCampus != NO_ONE) {
     //If there are any nearby campuses, it fails
     outcome = FALSE;
//   printf("failing nearby campuses\n");
 }
//	printf("out at end: %d\n",outcome);
 return outcome;
}
static int validARCpath(path location, Game g, int player) {

 int outcome = TRUE;
 int length = strlen(location);
 coord edge = stringToEdge(location);
//  	printf("The edge we are checking is %d, %d\n", edge.row, edge.col);
 //11 col and 11 rows
 if (length > 150) {
     outcome = FALSE;

 } else if (edge.col > 11 || edge.col < 0 || edge.row > 11 || edge.row < 0) {
     outcome = FALSE;

 } else if (edge.col % 2 == 1 && edge.row % 2 == 1) {
     outcome = FALSE;

 } else if (!isLegalPath(location)) {
     outcome = FALSE;
 }

//   printf("out: %d\n",outcome);
//   printf("ARC at location: %d\n",getARC(g, location));

 if (getARC(g, location) != NO_ONE) {
     outcome = FALSE;
 }

//	printf("outcome Arc %d\n",outcome);
int leftARC = NO_ONE;
int rightARC = NO_ONE;
int backLARC = NO_ONE;
int backRARC = NO_ONE;
int leftCampus = NO_ONE;
int rightCampus = NO_ONE;
int backCampus = NO_ONE;

int currCampus = getCampus(g, location);
//   printf("curr: %d\n",currCampus);
//   coord p = stringToEdge(location);
//   printf("%d %d\n", p.row, p.col);

location[length] = 'L';
location[length + 1] = '\0';

if (isLegalPath(location)) {
   leftARC = getARC(g, location);
   leftCampus = getCampus(g, location);
}

location[length] = 'R';
if (isLegalPath(location)) {
   rightARC = getARC(g, location);
   rightCampus = getCampus(g, location);
}
location[length] = 'B';
if (isLegalPath(location)) {
   backCampus = getCampus(g, location);
}
location[length + 1] = 'L';
location[length + 2] = '\0';
if (isLegalPath(location)) {
   backLARC = getARC(g, location);
}
//coord point = stringToVertex(location);
//printf("the point is %d %d prev is %d %d\n", point.row, point.col, point.lastRow, point.lastCol);
//coord p1 = stringToEdge(location);
//printf("edge location is %d %d\n", p1.row, p1.col);

location[length+1] = 'R';
if (isLegalPath(location)) {
   backRARC = getARC(g, location);
}
//  printf("BLARC: %d LARC: %d RARC: %d\n",backLARC, leftARC, rightARC);
//  printf("Bcamp: %d Lcamp :%d Rcamp: %d currCamp: %d\n",backCampus, leftCampus, rightCampus, currCampus);


if ((leftCampus != player && rightCampus != player && backCampus != player && currCampus != player) &&
 (leftARC != player && rightARC != player && backRARC != player && backLARC != player)) {//If there are no campuses or ARCS nearby
 outcome = FALSE;
}
// 	printf("outcome Arc %d\n",outcome);
return outcome;
}




static int validGO8path(path location, Game g, int player) {
 int outcome = FALSE;
 if (getCampus(g, location) == player) {
        outcome = TRUE;
 }
//  	printf("GO8 path outcome: %d \n",outcome);
 return outcome;
}

static coord getTileLocation(Game g, int tilePosition) {
 coord start;
 start.row = 0;
 start.col = 2;
 if (tilePosition < 3) {
     start.col = 0;
     start.row = tilePosition*2;
 } else if (tilePosition < 7) {
     start.col = 1;
     start.row = (tilePosition-3)*2;
 } else if (tilePosition < 12) {
     start.col = 2;
     start.row = (tilePosition - 7)*2;
 } else if (tilePosition < 16) {
     start.col = 3;
     start.row = (tilePosition - 12)*2 + 1 ;
 } else if (tilePosition < 19) {
     start.col = 4;
     start.row = (tilePosition - 16)*2 + 1;
 }
 return start;
}


static coord stringToVertex(path location) {
 coord point;
 point.row = 0;
 point.lastRow = 0;
 point.col = 2;
 point.lastCol = 2;

 coord vectorA;
 vectorA.row = 1;
 vectorA.col = 0;

 coord vectorB;
 vectorB.row = 0;
 vectorB.col = 1;

 coord directionVector;

 if (location[0] != '\0') {
     if (location[0] == 'R') {
     point.row++;
     }
     else {
         point.col++;
     }
     int counter = 1;
     while (location[counter] != '\0') {
         directionVector.row = point.row - point.lastRow;
         directionVector.col = point.col - point.lastCol;

         if (location[counter] == 'B') {
         point = subtractVector(point, directionVector);
         }
         else {
         if (directionVector.col == 0 && directionVector.row == 1) {
             if (location[counter] == 'R') {
                 if ((point.row%2 == 0 && point.col <= 2) || (point.row%2 != 0 && point.col >= 3)) {
                     point = addVector(point, vectorA);
                 }
                 else {
                     point = subtractVector(point, vectorB);
                     point = findNewRow(point);
                 }

             }
             else {
                 if ((point.row%2 == 0 && point.col <= 2) || (point.row%2 != 0 && point.col >= 3)) {
                     point = addVector(point, vectorB);
                     point = findNewRow(point);
                 }
                 else {
                     point = addVector(point, vectorA);
                 }
             }
         }
         else if (directionVector.col == 0 && directionVector.row == -1) {
             if (location[counter] == 'R') {
                 if ((point.row%2 == 0 && point.col <= 2) || (point.row%2 != 0 && point.col >= 3)) {
                     point = addVector(point, vectorB);
                     point = findNewRow(point);
                 }
                 else {
                     point = subtractVector(point, vectorA);
                 }

             }
             else {
                 if ((point.row%2 == 0 && point.col <= 2) || (point.row%2 != 0 && point.col >= 3)) {
                     point = subtractVector(point, vectorA);
                 }
                 else {
                     point = subtractVector(point, vectorB);
                     point = findNewRow(point);
                 }
             }
         }
         else {
             if (point.col - point.lastCol == 1) {
                 if (location[counter] == 'R') {
                     point = addVector(point, vectorA);
                 }
                 else {
                     point = subtractVector(point, vectorA);
                 }
             }
             else {
                 if (location[counter] == 'R') {
                     point = subtractVector(point, vectorA);
                 }
                 else {
                     point = addVector(point, vectorA);
                 }
             }
         }
         }
     counter++;
     }
 }
 return point;
}

int getDiscipline(Game g, int regionID) {
return g->disciplines[regionID];
}
static coord findNewRow(coord point) {
 coord vectorA;
 vectorA.row = 1;
 vectorA.col = 0;
 if (point.col - point.lastCol == 1) {
     if (point.lastCol < 2) {
         point = addVector(point, vectorA);
         point.lastCol--;
     }
     else if (point.lastCol >= 3) {
         point = subtractVector(point, vectorA);
         point.lastCol--;
     }
 }
 else {
     if (point.lastCol <= 2) {
         point = subtractVector(point, vectorA);
         point.lastCol++;
     }
     else if (point.lastCol > 3) {
         point = addVector(point, vectorA);
         point.lastCol++;
     }
 }
 return point;
}

static coord subtractVector(coord point, coord vector) {
 point.row -= vector.row;
 point.col -= vector.col;
 point.lastRow = point.row + vector.row;
 point.lastCol = point.col + vector.col;
 return point;
}

coord addVector(coord point, coord vector) {
 point.row += vector.row;
 point.col += vector.col;
 point.lastRow = point.row - vector.row;
 point.lastCol = point.col - vector.col;

 return point;
}

static coord stringToEdge(path location) {
 coord point = stringToVertex(location);
//printf("info passed in was %d %d with last point %d %d\n", point.row, point.col, point.lastRow, point.lastCol);
 coord edge = point;
 edge.col*=2;
 if (point.col - point.lastCol == 0) {
     if (point.row - point.lastRow == 1) {
         edge.row--;
     }
     else {
     }
 }
 else {
     if (point.col - point.lastCol == 1) {
         edge.col--;
         if (edge.col < 5) {
         edge.row = edge.lastRow;
         }
     }
     else {
         edge.col++;
         if (edge.col > 5) {
         edge.row = edge.lastRow;
         }
     }
 }
//printf("edge is at %d %d\n", edge.row, edge.col);
 return edge;
}

static int isLegalPath(path location) {

int outcome = TRUE;
int counter = 0;
if (location[counter] == 'B') {
 outcome = FALSE;
}
while(location[counter] != '\0' && counter < 150) {
 path newLoc;
 newLoc[counter] = location[counter];
 newLoc[counter + 1] = '\0';
 coord vertex = stringToVertex(newLoc);

 if (location[counter] != 'L' && location[counter] != 'R' &&
     location[counter] != 'B') {
     outcome = FALSE;
 } else if (vertex.col < 0 || vertex.col > 5) {
     outcome = FALSE;
 } else if (vertex.col % 5 == 0 && (vertex.row < 0 || vertex.row > 6)) {
     outcome = FALSE;
//  printf("vert: %d\n",outcome);
 } else if (vertex.col % 3 == 1 && (vertex.row < 0 || vertex.row > 8)) {
     outcome = FALSE;

 } else if (vertex.row < 0 || vertex.row > 10) {

     outcome = FALSE;
 }

 counter++;
}
return outcome;
}
