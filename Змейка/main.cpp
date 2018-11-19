#include <windows.h>
#include <glut.h>
#include <vector>
#include <ctime>
#include <stdio.h>
using namespace std;

FILE *dataFile;


#define START_SIZE 6
#define MAX_MENU_SELECT 4
#define MAX_IN_GAME_MENU_SELECT 4
#define ANGLE_RADIAN 0.01745

#define DIRECTION_RIGHT 0
#define DIRECTION_UP 1
#define DIRECTION_LEFT 2
#define DIRECTION_DOWN 3

#define MENU_DELTA_COLOR_RED 0
#define MENU_DELTA_COLOR_GREEN 0.125
#define MENU_DELTA_COLOR_BLUE 0.35

#define MODE_NO_SELECT 1
#define MODE_SELECTED 0
#define MODE_NO_SELECT_SMALL 2
#define MODE_SELECTED_SMALL 3
#define MODE_HIGHTLIGHT 4
#define MODE_HIGHTLIGHT_SMALL 5

#define PIC_PLAY 0
#define PIC_SPEED_LOW 1
#define PIC_SPEED_MIDDLE 2
#define PIC_SPEED_HIGHT 3
#define PIC_FIELD_SMALL 4
#define PIC_FIELD_MIDDLE 5
#define PIC_FIELD_BIG 6
#define PIC_EXIT 7
#define PIC_BORDERS_OFF 8
#define PIC_BORDERS_ON 9
#define PIC_CONTINUE_GAME 10
#define PIC_MAIN_MENU 11
#define PIC_SNAKE_COLOR 12
#define PIC_BACKGROUND_COLOR_LIGHT 13
#define PIC_BACKGROUND_COLOR_DARK 14




vector<short> xbody;
vector<short> ybody;

short stMenu = 0;
short stInGameMenu = 0;
short enterNameMenuSelect = 1;
short stSnakeColor = 3;
float colors[7][3] =
{ { 0.95, 0.5, 0.5 }, { 0.95, 0.65, 0.30 }, { 0.95, 0.8, 0.2 },
{ 0.5, 0.95, 0.4 }, { 0.55, 0.8, 0.95 }, { 0.35, 0.55, 0.95 }, { 0.75, 0.5, 0.95 } };


short stSpeed = 1;
short stFieldSize = 1;
float multSpeed = 1, multField = 1, multBorders = 1, multScore = 1, multScoreNoSize = 1;



int maxScore[8];
char** maxScoreNames = new char*[5];
static char** maxScoreString = new char*[5];
char namePlayer[7] = "Õ» »“¿";


float fieldSizeFactor;
float score = 0;
float textSize;
float animationCompleteness;
short dir;
short pos = 6;
short animSmoothness = 5;
short animationPhase = -1;
short dirBuffer;
short xfood;
short yfood;
short textEnterPointer = strlen(namePlayer);
float foodNutritionalValue = 0;
float size = START_SIZE;
int previousSize = START_SIZE;
short delay = 100;
short fieldSize = 16;
short fieldSizeX = 16;
short fieldSizeY = 16;
short snakeEndingDirection;

short removedSnakeX, removedSnakeY;

int megaFoodCheck = 1;
short megaFoodEatedX, megaFoodEatedY;
char multScoreString[15];
char resultTable[15];
char enterNameString[20];

int windowSizeX, windowSizeY;


boolean redisplay = 1;
boolean gameOver = 0;
boolean border = 0;
boolean directionChanged = 0;
boolean megaFood = 0;
boolean ensbleDirBuffer = 0;
boolean gamePaused = 0;
boolean darkBackground = 0;
boolean hideScoreBackground = 0;
boolean foodSeeded = 0;
boolean megaFoodSpoiled = 0;
boolean megaFoodEated = 0;
boolean enterNameDialog = 0;
boolean inGame = 0;
boolean endGame = 0;
boolean snakeSizeChanged = 0;
boolean snakeSizeChanged2 = 0;
boolean extraBin = 0;
// ‰Îˇ ‡ÌËÏ‡ˆËË ‰‚ËÊÂÌËˇ ÁÏÂÈÍË


float darkColorFactor = (darkBackground ? 0.75 : 1);
float snakeRed = 0.5*darkColorFactor, snakeGreen = 1 * darkColorFactor, snakeBlue = 0.5*darkColorFactor;
float snakeRedCurr = 0.5, snakeGreenCurr = 1, snakeBlueCurr = 0.5;
float backgroundRed = 1, backgroundGreen = 1, backgroundBlue = 1;
float textrgb = 0.6;
float aspect;


void drawButton(float x, float y, short type, short mode);
void myInit(void);
void seedFood();
void game(void);
void initGame(void);
void write(float x, float y, char* string, float scale);
void menu(void);
void closeGame();
void gameControl(int key, int x, int y);
void menuControl(int key, int x, int y);
void menuControlStand(unsigned char key, int x, int y);
void gameControlStand(unsigned char key, int x, int y);
void circle(float x, float y, float r, boolean fill, double steep);
void aligWriteCenter(float y, char* string, float scale, float xcenter);
void changeBackgroundColor(boolean dark, float redBack, float greenBack, float blueBack);
void drawInGameMenuBackground(float x, float y, float roundRadius, float xcenter, float ycenter);
void menuAccentString(float y, char* string, float scale, float xcenter, boolean accented);
short getDirection(float x1, float y1, float x2, float y2);

void changeWindowSize(int x, int y){
	aspect = (float)x / y;
	windowSizeX = x;
	windowSizeY = y;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, x, y);
	if (!inGame){
		if (aspect>1)  gluOrtho2D(-32 * aspect, 32 * aspect, -32, 32);
		if (aspect <= 1)  gluOrtho2D(-32, 32, -32 / aspect, 32 / aspect);
	}
	else{

		if (aspect>1)  { fieldSizeX = fieldSize*aspect;  fieldSizeY = fieldSize; }
		if (aspect <= 1)  { fieldSizeY = fieldSize / aspect;  fieldSizeX = fieldSize; }

		gluOrtho2D(0, fieldSizeX, 0, fieldSizeY);

		multScore = multScoreNoSize*(float)(fieldSize*fieldSize) / (fieldSizeX*fieldSizeY);
		fieldSizeFactor = (float)pow((float)(24 * 24) / (fieldSizeX*fieldSizeY), (float)1 / 2);

		seedFood();
	}

	glMatrixMode(GL_MODELVIEW);

}
void main(int argc, char **argv){
	for (int i = 0; i<5; i++) {
		maxScoreNames[i] = new char[8];
		maxScoreString[i] = new char[15];
	}
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(640, 640);
	glutInitWindowPosition(350, 50);
	glutCreateWindow("«ÏÂÈÍ‡");
	myInit();
	glutReshapeFunc(changeWindowSize);
	glutMainLoop();
}
void sq(float x, float y){
	glRectf(x, y, x + 1, y + 1);
}
void squareScaled(float xcenter, float ycenter, float scale){
	glRectf(xcenter - 0.5*scale, ycenter - 0.5*scale, xcenter + 0.5*scale, ycenter + 0.5*scale);
}
void next(){
	switch (dir){
	case 0:
		xbody.push_back(xbody.back() + 1);
		ybody.push_back(ybody.back()); break;
	case 1:
		xbody.push_back(xbody.back());
		ybody.push_back(ybody.back() + 1); break;
	case 2:
		xbody.push_back(xbody.back() - 1);
		ybody.push_back(ybody.back()); break;
	case 3:
		xbody.push_back(xbody.back());
		ybody.push_back(ybody.back() - 1); break;
	}
}
void remove(){


	if (xbody.size()>size){
		removedSnakeX = xbody[0];
		removedSnakeY = ybody[0];

		xbody.erase(xbody.begin());
		ybody.erase(ybody.begin());

		if (snakeSizeChanged) if (extraBin) extraBin = 0; else extraBin = 1;

	}


}
void seedFood(){

	if ((int)size / 15 >= megaFoodCheck) megaFood = 1;
	if (!megaFood){
		xfood = rand() % fieldSizeX;
		yfood = rand() % fieldSizeY;
		foodNutritionalValue = rand() % 10 + 1;
	}
	else {
		xfood = rand() % (fieldSizeX - 2) + 1;
		yfood = rand() % (fieldSizeY - 2) + 1;
		foodNutritionalValue = (rand() % 51 + 50);
		megaFoodCheck++;
	}
	if (extraBin) extraBin = 0; else extraBin = 1;
	foodSeeded = 1;
}
void myInit(void){


	dataFile = fopen("snake.txt", "rb+");
	if (dataFile == 0){
		dataFile = fopen("snake.txt", "w");
		for (int i = 0; i<5; i++)

			fprintf(dataFile, "œ”—“Œ 0\n");
		fprintf(dataFile, "%d %d %d %d %d %s\n", 3, 0, 1, 1, 0, "»Ãﬂ");
		fclose(dataFile);
		dataFile = fopen("snake.txt", "rb+");
	}


	for (int i = 0; i<5; i++){
		fscanf(dataFile, "%s %d", maxScoreNames[i], &maxScore[i]);
		sprintf(maxScoreString[i], "%d", maxScore[i]);
	}
	fscanf(dataFile, "%d %d %d %d %d %s", &stSnakeColor, &darkBackground, &stSpeed, &stFieldSize, &border, &namePlayer);
	fclose(dataFile);

	strcpy(enterNameString, namePlayer);
	textEnterPointer = strlen(enterNameString);
	switch (stSpeed){
	case 0:
		multSpeed = 0.75;

		break;
	case 1:
		multSpeed = 1;

		break;
	case 2:
		multSpeed = 1.35;

		break;
	}
	switch (stFieldSize){
	case 0:
		multField = 1.5;
		break;
	case 1:
		multField = 1;
		break;
	case 2:
		multField = 0.75;
		break;
	}
	if (border) multBorders = 1.5; else multBorders = 1.0;



	if (darkBackground)
		changeBackgroundColor(1, 0.3, 0.3, 0.3);
	else changeBackgroundColor(0, 1, 1, 1);

	snakeRedCurr = colors[stSnakeColor][0];
	snakeGreenCurr = colors[stSnakeColor][1];
	snakeBlueCurr = colors[stSnakeColor][2];

	snakeRed = snakeRedCurr*darkColorFactor;
	snakeGreen = snakeGreenCurr*darkColorFactor;
	snakeBlue = snakeBlueCurr*darkColorFactor;

	glutSpecialFunc(menuControl);
	glutKeyboardFunc(menuControlStand);
	glClearColor(backgroundRed, backgroundGreen, backgroundBlue, 0);
	glColor3f(0.5, 1, 0.5);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-32, 32, -32, 32);
	glMatrixMode(GL_MODELVIEW);
	inGame = 0;
	changeWindowSize(windowSizeX, windowSizeY);
	glutDisplayFunc(menu);
	glLineWidth(3);
	glPointSize(3);


}
char title[25];


void initGame(){
	switch (stSpeed){
	case 0: delay = 150;
		multSpeed = 0.75;
		animSmoothness = 10;
		break;
	case 1: delay = 100;
		multSpeed = 1;
		animSmoothness = 7;
		break;
	case 2: delay = 50;
		multSpeed = 1.35;
		animSmoothness = 5;
		break;
	}
	switch (stFieldSize){
	case 0: fieldSize = 12;
		multField = 1.5;
		break;
	case 1: fieldSize = 16;
		multField = 1;
		break;
	case 2: fieldSize = 20;
		multField = 0.75;
		break;
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, fieldSize, 0, fieldSize);
	glMatrixMode(GL_MODELVIEW);


	fieldSizeFactor = (24 * 24) / (fieldSizeX*fieldSizeY);

	xbody.push_back(rand() % fieldSizeX);
	ybody.push_back(rand() % fieldSizeY);
	if (xbody.back()<(fieldSizeX / 2)) if (ybody.back()<(fieldSizeY / 2)) dir = rand() % 2; else dir = 3 * (rand() % 2);
	else if (ybody.back()<(fieldSizeY / 2)) dir = rand() % 2 + 1; else dir = rand() % 2 + 2;

	seedFood();
	snakeEndingDirection = dir;
	textSize = 0.01*fieldSize;
	inGame = 1;
	changeWindowSize(windowSizeX, windowSizeY);
	enterNameDialog = 0;
	glutKeyboardFunc(gameControlStand);
	glutSpecialFunc(gameControl);
	glutDisplayFunc(game);

}
void menuControl(int key, int x, int y)
{



	switch (key){
	case GLUT_KEY_UP: if (stMenu != 0) stMenu--;  break;
	case GLUT_KEY_DOWN: if (stMenu != MAX_MENU_SELECT) stMenu++; break;


	case GLUT_KEY_LEFT: switch (stMenu){
	case 1: if (stSpeed != 0) stSpeed--; break;
	case 2: if (stFieldSize != 0) stFieldSize--; break;
	case 3: if (border) border = 0; break;




	}	break;




	case GLUT_KEY_RIGHT: switch (stMenu){
	case 1: if (stSpeed != 2) stSpeed++; break;
	case 2: if (stFieldSize != 2) stFieldSize++; break;
	case 3: if (!border) border = 1; break;



		break;
	}
	}

	switch (stSpeed){
	case 0: delay = 100;
		multSpeed = 0.75;
		break;
	case 1: delay = 66;
		multSpeed = 1;
		break;
	case 2: delay = 33;
		multSpeed = 1.35;
		break;
	}
	switch (stFieldSize){
	case 0: fieldSize = 16;
		multField = 1.5;
		break;
	case 1: fieldSize = 24;
		multField = 1;
		break;
	case 2: fieldSize = 32;
		multField = 0.75;
		break;
	}
	if (border) multBorders = 1.5; else multBorders = 1;
	glutPostRedisplay();
	multScore = multScoreNoSize = multSpeed*multField*multBorders;
}
void inGameMenuControl(int key, int x, int y)
{

	switch (key){
	case GLUT_KEY_LEFT: if (stInGameMenu != 0) stInGameMenu--;  break;
	case GLUT_KEY_RIGHT: if (stInGameMenu != MAX_IN_GAME_MENU_SELECT) stInGameMenu++; break;


	case GLUT_KEY_UP: switch (stInGameMenu){
	case 2: if (stSnakeColor != 0) stSnakeColor--;
		snakeRedCurr = colors[stSnakeColor][0];
		snakeGreenCurr = colors[stSnakeColor][1];
		snakeBlueCurr = colors[stSnakeColor][2];

		snakeRed = snakeRedCurr*darkColorFactor;
		snakeGreen = snakeGreenCurr*darkColorFactor;
		snakeBlue = snakeBlueCurr*darkColorFactor;
		break;
	case 3: if (darkBackground)
		changeBackgroundColor(0, 1, 1, 1); break;
	}	break;




	case GLUT_KEY_DOWN: switch (stInGameMenu){
	case 2: if (stSnakeColor != 6) stSnakeColor++;
		snakeRedCurr = colors[stSnakeColor][0];
		snakeGreenCurr = colors[stSnakeColor][1];
		snakeBlueCurr = colors[stSnakeColor][2];

		snakeRed = snakeRedCurr*darkColorFactor;
		snakeGreen = snakeGreenCurr*darkColorFactor;
		snakeBlue = snakeBlueCurr*darkColorFactor;
		break;
	case 3: if (!darkBackground)
		changeBackgroundColor(1, 0.3, 0.3, 0.3); break;
		break;
	}break;
	}
	glutPostRedisplay();
}
void inGameMenuControlStand(unsigned char key, int x, int y)
{

	switch (key){
	case 13: switch (stInGameMenu){
	case 0: gamePaused = 0;
		glClearColor(backgroundRed, backgroundGreen, backgroundBlue, 0);
		stInGameMenu = 0;
		glutKeyboardFunc(gameControlStand);
		glutSpecialFunc(gameControl);
		break;
	case 1: stInGameMenu = 0; closeGame(); break;
	case 2: if (stSnakeColor != 6) {
				stSnakeColor++;
				snakeRedCurr = colors[stSnakeColor][0];
				snakeGreenCurr = colors[stSnakeColor][1];
				snakeBlueCurr = colors[stSnakeColor][2];

				snakeRed = snakeRedCurr*darkColorFactor;
				snakeGreen = snakeGreenCurr*darkColorFactor;
				snakeBlue = snakeBlueCurr*darkColorFactor;
	}
			else stSnakeColor = 0; break;
	case 3: if (!darkBackground)
		changeBackgroundColor(1, 0.3, 0.3, 0.3);
			else changeBackgroundColor(0, 1, 1, 1);
			glClearColor(backgroundRed - 0.1, backgroundGreen - 0.1, backgroundBlue - 0.1, 0);
			glutPostRedisplay();
			break;
	case 4:
		dataFile = fopen("snake.txt", "w");
		for (int i = 0; i<5; i++) fprintf(dataFile, "%s %d\n", maxScoreNames[i], maxScore[i]);
		fprintf(dataFile, "%d %d %d %d %d %s\n", stSnakeColor, darkBackground, stSpeed, stFieldSize, border, namePlayer);
		exit(0);
	}break;
	case 27: {gamePaused = 0;
		glClearColor(backgroundRed, backgroundGreen, backgroundBlue, 0);
		stInGameMenu = 0;
		glutKeyboardFunc(gameControlStand);
		glutSpecialFunc(gameControl); } break;


	}


	glutPostRedisplay();
}
void menuControlStand(unsigned char key, int x, int y){
	if (key == 13) switch (stMenu){
	case 0:initGame(); break;
	case 1:if (stSpeed != 2) stSpeed++; else stSpeed = 0; break;
	case 2:if (stFieldSize != 2) stFieldSize++; else stFieldSize = 0; break;
	case 3:if (!border) border = 1; else border = 0; break;
	case 4:
		dataFile = fopen("snake.txt", "w");
		for (int i = 0; i<5; i++) fprintf(dataFile, "%s %d\n", maxScoreNames[i], maxScore[i]);
		fprintf(dataFile, "%d %d %d %d %d %s\n", stSnakeColor, darkBackground, stSpeed, stFieldSize, border, namePlayer);
		exit(0);
	}

	glutPostRedisplay();
}
void enterNameMenuControl(int key, int x, int y)
{

	switch (key){
	case GLUT_KEY_LEFT: if (enterNameMenuSelect != 0) enterNameMenuSelect--;  break;
	case GLUT_KEY_RIGHT: if (enterNameMenuSelect != 1) enterNameMenuSelect++; break;


	}

	glutPostRedisplay();

}
void enterNameDialogControlStand(unsigned char key, int x, int y){
	switch (key){
	case 27:
		strcpy(namePlayer, enterNameString);
		enterNameDialog = 0;
		closeGame();
		break;

	case 13:
		switch (enterNameMenuSelect){
		case 0:
			strcpy(namePlayer, enterNameString);
			enterNameDialog = 0;
			closeGame();
			exit(0);
			break;
		case 1:
			strcpy(namePlayer, enterNameString);
			enterNameDialog = 0;
			closeGame();
			break;
		}
		break;


	}
	if (textEnterPointer<8)
	if (key >= 224 && key <= 255)
	{
		enterNameString[textEnterPointer] = key - 32; textEnterPointer++;
	}
	else if ((key >= 192 && key <= 223) || (key >= '0'&&key <= '9')) { enterNameString[textEnterPointer] = key; textEnterPointer++; }
	if (textEnterPointer>0)
	if (key == 8) { if (textEnterPointer>1) enterNameString[textEnterPointer - 1] = '\0'; else enterNameString[textEnterPointer - 1] = 32; textEnterPointer--; }

	glutPostRedisplay();
}
void gameControl(int key, int x, int y)
{
	if (!directionChanged)
		switch (key){
		case GLUT_KEY_UP: if (dir != 3) dir = 1; directionChanged = 1; break;
		case GLUT_KEY_LEFT: if (dir != 0) dir = 2; directionChanged = 1; break;
		case GLUT_KEY_DOWN: if (dir != 1) dir = 3; directionChanged = 1; break;
		case GLUT_KEY_RIGHT: if (dir != 2) dir = 0; directionChanged = 1; break;


	}
	else {
		switch (key){
		case GLUT_KEY_UP: if (dir != 3) dirBuffer = 1; break;
		case GLUT_KEY_LEFT: if (dir != 0) dirBuffer = 2; break;
		case GLUT_KEY_DOWN: if (dir != 1) dirBuffer = 3; break;
		case GLUT_KEY_RIGHT: if (dir != 2) dirBuffer = 0; break;
		}
		ensbleDirBuffer = 1;
	}

}
void shiftRecords(int pos){
	for (int i = 4; i >= pos; i--) {
		strcpy(maxScoreNames[i], maxScoreNames[i - 1]);
		maxScore[i] = maxScore[i - 1];
	}
}
void closeGame(){
	gamePaused = 0;

	dataFile = fopen("snake.txt", "w");
	if (pos != 6){
		strcpy(maxScoreNames[pos - 1], namePlayer);
		maxScore[pos - 1] = score;
		pos = 6;
	}
	for (int i = 0; i<5; i++) fprintf(dataFile, "%s %d\n", maxScoreNames[i], maxScore[i]);
	fprintf(dataFile, "%d %d %d %d %d %s\n", stSnakeColor, darkBackground, stSpeed, stFieldSize, border, namePlayer);

	endGame = 0;
	xbody.clear();
	ybody.clear();
	gameOver = 0;
	size = START_SIZE;
	score = 0;
	animationPhase = -1;
	megaFoodCheck = 1;
	megaFood = 0;
	myInit();
}
void eatFood(){
	score += foodNutritionalValue*multScore;

	previousSize = size;
	size += foodNutritionalValue / 10;
	if ((xbody.size() - previousSize) % 2) snakeSizeChanged = 1;
	if (megaFood){
		megaFood = 0; megaFoodEated = 1; megaFoodEatedX = xfood; megaFoodEatedY = yfood;
	}
	seedFood();
}
void gameControlStand(unsigned char key, int x, int y){
	switch (key){


	case 13: if (gameOver){
				 for (int i = 0; i<5; i++)
				 if (score>maxScore[i]) {
					 pos = i + 1;
					 shiftRecords(pos);

					 switch (pos){
					 case 1: sprintf(resultTable, "%s", "ÕŒ¬€… –≈ Œ–ƒ!"); break;
					 default: sprintf(resultTable, "%d %s", pos, " Ã≈—“Œ"); break;
					 }
					 enterNameDialog = 1;
					 glutKeyboardFunc(enterNameDialogControlStand);
					 glutSpecialFunc(enterNameMenuControl);
					 glClearColor(backgroundRed - 0.1, backgroundGreen - 0.1, backgroundBlue - 0.1, 0);
					 glutPostRedisplay();
					 break;
				 }
				 else if (i == 4) endGame = 1;
				 if (endGame) { pos = 6; closeGame(); }break;
	}
			 else;
	case 27: if (!gamePaused) {
				 gamePaused = 1;
				 glClearColor(backgroundRed - 0.15, backgroundGreen - 0.15, backgroundBlue - 0.15, 0);
				 glutPostRedisplay();
				 glutKeyboardFunc(inGameMenuControlStand);
				 glutSpecialFunc(inGameMenuControl);
	}
			 else gamePaused = 0; break;


	}

}
short getDirection(float x1, float y1, float x2, float y2){


	if (x2 != x1)
		return x2 - x1>0 ? DIRECTION_RIGHT : DIRECTION_LEFT;
	if (y2 != y1)
		return y2 - y1>0 ? DIRECTION_UP : DIRECTION_DOWN;

	return -1;
}
float dlColor(float delta){
	return (darkBackground ? -delta : delta);
}
void changeBackgroundColor(boolean dark, float redBack, float greenBack, float blueBack){
	darkBackground = dark;

	backgroundRed = redBack, backgroundGreen = greenBack, backgroundBlue = blueBack;
	if (gamePaused) glClearColor(backgroundRed - 0.15, backgroundGreen - 0.15, backgroundBlue - 0.15, 0);
	else glClearColor(backgroundRed, backgroundGreen, backgroundBlue, 0);

	darkColorFactor = (darkBackground ? 0.75 : 1);
	snakeRed = snakeRedCurr*darkColorFactor, snakeGreen = snakeGreenCurr*darkColorFactor, snakeBlue = snakeBlueCurr*darkColorFactor;

	textrgb = 0.6;

}
boolean foodWillBeEated(){
	if ((xbody[xbody.size() - 1] == xfood - 1 && dir == DIRECTION_RIGHT&&ybody[ybody.size() - 1] == yfood) || (xbody[xbody.size() - 1] == xfood + 1 && dir == DIRECTION_LEFT&&ybody[ybody.size() - 1] == yfood)
		|| (ybody[ybody.size() - 1] == yfood - 1 && dir == DIRECTION_UP&&xbody[xbody.size() - 1] == xfood) || (ybody[ybody.size() - 1] == yfood + 1 && dir == DIRECTION_DOWN&&xbody[xbody.size() - 1] == xfood))
	if ((int)(size + foodNutritionalValue)>(int)(size))
		return 1;
	return 0;
}

void game(void){
	animationCompleteness = (float)animationPhase / animSmoothness;
	glClear(GL_COLOR_BUFFER_BIT);

	if (animationPhase == 0)

	if (!gamePaused){


		if (!gameOver) next();

		if (foodSeeded) foodSeeded = 0;
		if (megaFoodSpoiled) megaFoodSpoiled = 0;
		if (megaFoodEated) megaFoodEated = 0;

		if (border) if (xbody.back() <= -1 || xbody.back() >= fieldSizeX || ybody.back() <= -1 || ybody.back() >= fieldSizeY) gameOver = 1;
		if (!border) {
			if (xbody.back() <= -1) { xbody.push_back(fieldSizeX - 1); ybody.push_back(ybody.back()); if (extraBin) extraBin = 0; else extraBin = 1; }
			if (ybody.back() <= -1) { ybody.push_back(fieldSizeY - 1); xbody.push_back(xbody.back()); if (extraBin) extraBin = 0; else extraBin = 1; }
			if (ybody.back() >= fieldSizeY) { ybody.push_back(0); xbody.push_back(xbody.back()); if (extraBin) extraBin = 0; else extraBin = 1; }
			if (xbody.back() >= fieldSizeX) { xbody.push_back(0); ybody.push_back(ybody.back()); if (extraBin) extraBin = 0; else extraBin = 1; }


			if (xbody[0] <= -1 || ybody[0] >= fieldSizeY || ybody[0] <= -1 || xbody[0] >= fieldSizeX) { remove(); if (extraBin) extraBin = 0; else extraBin = 1; }

		}

		if (megaFood){
			for (int i = -1; i <= 1; i++)
			for (int j = -1; j <= 1; j++)
			if (xbody.back() == xfood + i)
			if (ybody.back() == yfood + j)
				eatFood();
		}

		if (xbody.back() == xfood&&ybody.back() == yfood)

			eatFood(); else
		for (int i = 1; i<xbody.size() - 1; i++){
			if (xbody.back() == xbody[i]) if (ybody.back() == ybody[i]) gameOver = 1;
		}
		snakeEndingDirection = getDirection(xbody[0], ybody[0], xbody[1], ybody[1]);
		if (xbody.size()>1)


			remove();


	}



	glColor3f(snakeRed, snakeGreen, snakeBlue);


	for (int i = 0; i<xbody.size(); i++){


		if (i != 0) sq(xbody[i], ybody[i]);

		if (!gamePaused&&!enterNameDialog){
			if (!gameOver)  if (i % 2 == (extraBin ? 1 : 0)) glColor3f(snakeRed, snakeGreen, snakeBlue); else glColor3f(snakeRed + 0.02, snakeGreen + 0.02, snakeBlue + 0.02);
			else if (i % 2 == (extraBin ? 1 : 0)) glColor3f(0.5*darkColorFactor, 0.25*darkColorFactor, 0.5*darkColorFactor); else glColor3f(0.55*darkColorFactor, 0.3*darkColorFactor, 0.55*darkColorFactor);
		}
		else if (i % 2 == (extraBin ? 1 : 0)) glColor3f(0.375*darkColorFactor, 0.375*darkColorFactor, 0.375*darkColorFactor); else glColor3f(0.425*darkColorFactor, 0.425*darkColorFactor, 0.425*darkColorFactor);





		if (i != xbody.size() - 1)
			switch (getDirection(xbody[i], ybody[i], xbody[i + 1], ybody[i + 1])){
			case DIRECTION_RIGHT:sq(xbody[i] + 1 * animationCompleteness, ybody[i]);	break;
			case DIRECTION_UP: sq(xbody[i], ybody[i] + 1 * animationCompleteness); break;
			case DIRECTION_LEFT: sq(xbody[i] - 1 * animationCompleteness, ybody[i]); break;
			case DIRECTION_DOWN: sq(xbody[i], ybody[i] - 1 * animationCompleteness); break;
		}
		else switch (dir){
		case DIRECTION_RIGHT:sq(xbody[i] + 1 * animationCompleteness, ybody[i]);	break;
		case DIRECTION_UP: sq(xbody[i], ybody[i] + 1 * animationCompleteness); break;
		case DIRECTION_LEFT: sq(xbody[i] - 1 * animationCompleteness, ybody[i]); break;
		case DIRECTION_DOWN: sq(xbody[i], ybody[i] - 1 * animationCompleteness); break;
		}






	}




	if (!megaFood){
		if (!gamePaused&&!enterNameDialog)
			glColor3f((((1.2 - snakeRedCurr) + 0.3*((float)foodNutritionalValue / 10)))*darkColorFactor,
			(((1.2 - snakeGreenCurr) + 0.3*((float)foodNutritionalValue / 10)))*darkColorFactor,
			((1.2 - snakeBlueCurr) + 0.3*((float)foodNutritionalValue / 10))*darkColorFactor);
		else glColor3f(0.625*darkColorFactor, 0.625*darkColorFactor, 0.625*darkColorFactor);



		if (!megaFoodSpoiled)
		if (foodSeeded)
			squareScaled(xfood + 0.5, yfood + 0.5, animationCompleteness);
		else
			sq(xfood, yfood);
		else
			squareScaled(xfood + 0.5, yfood + 0.5, 4 - 3 * animationCompleteness);


	}
	else{

		if (!gamePaused&&!enterNameDialog)
			glColor3f(((((1.2 - snakeRedCurr) + 0.03*((float)foodNutritionalValue / 10)))*darkColorFactor + 0.05*animationCompleteness)*darkColorFactor,
			((((1.2 - snakeGreenCurr) + 0.03*((float)foodNutritionalValue / 10)))*darkColorFactor + 0.05*animationCompleteness)*darkColorFactor,
			((((1.2 - snakeBlueCurr) + 0.03*((float)foodNutritionalValue / 10)))*darkColorFactor + 0.05*animationCompleteness)*darkColorFactor);
		else glColor3f(0.775*darkColorFactor, 0.775*darkColorFactor, 0.775*darkColorFactor);

		if (foodSeeded)
			squareScaled(xfood + 0.5, yfood + 0.5, 3 * animationCompleteness);
		else
		for (int i = -1; i <= 1; i++)
		for (int j = -1; j <= 1; j++)
			sq(xfood + i, yfood + j);



	}

	if (megaFoodEated)
	for (int i = -1; i <= 1; i++)
	for (int j = -1; j <= 1; j++)
		squareScaled(megaFoodEatedX + 0.5 + i, megaFoodEatedY + 0.5 + j, 1 - 1 * animationCompleteness);


	if (!gamePaused&&!enterNameDialog)
	if (!gameOver) glColor3f(snakeRed - 0.1, snakeGreen - 0.1, snakeBlue - 0.1); else glColor3f(1 * darkColorFactor, 0.3*darkColorFactor, 0.3*darkColorFactor);
	else glColor3f(0.15*darkColorFactor, 0.15*darkColorFactor, 0.15*darkColorFactor);
	switch (dir){
	case DIRECTION_RIGHT: sq(xbody[xbody.size() - 1] + 1 * animationCompleteness, ybody[xbody.size() - 1]); break;
	case DIRECTION_UP: sq(xbody[xbody.size() - 1], ybody[xbody.size() - 1] + 1 * animationCompleteness); break;
	case DIRECTION_LEFT: sq(xbody[xbody.size() - 1] - 1 * animationCompleteness, ybody[xbody.size() - 1]); break;
	case DIRECTION_DOWN: sq(xbody[xbody.size() - 1], ybody[xbody.size() - 1] - 1 * animationCompleteness); break;
	}


	if (!gamePaused&&!enterNameDialog)
		glColor3f(0.8, 0.8, 0.8);
	char qwerr[5];
	sprintf(qwerr, "%d", (int)score);
	glLineWidth(2);
	menuAccentString(fieldSizeY*0.94, qwerr, textSize, fieldSizeX / 2, gamePaused || enterNameDialog);



	if (animationPhase == 0)
	if (!gamePaused){

		if (megaFood) if (foodNutritionalValue <= 10) { megaFood = 0; megaFoodSpoiled = 1; }
		if (megaFood) foodNutritionalValue -= 2 * fieldSizeFactor;
		if (megaFood) if (foodNutritionalValue <= 10) { megaFood = 0; megaFoodSpoiled = 1; }
		directionChanged = 0;
		if (ensbleDirBuffer) { dir = dirBuffer; ensbleDirBuffer = 0; }
		if (snakeSizeChanged) snakeSizeChanged = 0;

		if ((int)size>previousSize) snakeSizeChanged2 = 1; else snakeSizeChanged2 = 0;
		previousSize = size;
	}
	if (megaFood) if (foodNutritionalValue <= 10) { megaFood = 0; megaFoodSpoiled = 1; }

	if (gamePaused) {
		glClearColor(backgroundRed - 0.15, backgroundGreen - 0.15, backgroundBlue - 0.15, 0);


		glColor3f(backgroundRed - dlColor(0.05), backgroundGreen - dlColor(0.05), backgroundBlue - dlColor(0.05));
		switch (stInGameMenu){
		case 2:drawInGameMenuBackground((float)fieldSizeX*0.97 / aspect, (float)fieldSizeY*0.6, 4 * (float)fieldSize / 64, (float)fieldSizeX / 2, (float)fieldSizeY / 2 - 0.115*fieldSizeY); break;

		case 3:drawInGameMenuBackground((float)fieldSizeX*0.97 / aspect, (float)fieldSizeY*0.45, 4 * (float)fieldSize / 64, (float)fieldSizeX / 2, (float)fieldSizeY / 2 - 0.04*fieldSizeY); break;
		}
		glColor3f(backgroundRed + 0.1, backgroundGreen + 0.1, backgroundBlue + 0.1);
		drawInGameMenuBackground((float)fieldSizeX*0.97 / aspect, (float)fieldSizeY*0.33, 4 * (float)fieldSize / 64, (float)fieldSizeX / 2, (float)fieldSizeY / 2 + fieldSizeY*0.03);

		glColor3f(0.7 - dlColor(0.1), 0.7 - dlColor(0.1), 0.7 - dlColor(0.1));
		glLineWidth(4);
		aligWriteCenter(fieldSizeY*0.595, "œ¿”«¿", 0.017*fieldSize, fieldSizeX / 2);
		glLineWidth(3);

		drawButton(fieldSizeX*(0.5 - 0.38 / aspect), fieldSizeY*(0.465 + (stInGameMenu == 0 ? 0.04 : 0)), PIC_CONTINUE_GAME, (stInGameMenu == 0) ? MODE_SELECTED_SMALL : MODE_NO_SELECT_SMALL);
		drawButton(fieldSizeX*(0.5 - 0.19 / aspect), fieldSizeY*(0.465 + (stInGameMenu == 1 ? 0.04 : 0)), PIC_MAIN_MENU, (stInGameMenu == 1) ? MODE_SELECTED_SMALL : MODE_NO_SELECT_SMALL);
		drawButton(fieldSizeX*(0.5), fieldSizeY*(0.465 + (stInGameMenu == 2 ? 0.02 : 0)), PIC_SNAKE_COLOR, (stInGameMenu == 2) ? MODE_HIGHTLIGHT_SMALL : MODE_NO_SELECT_SMALL);
		drawButton(fieldSizeX*(0.5 + 0.19 / aspect), fieldSizeY*(0.465 + (stInGameMenu == 3 ? 0.02 : 0)), darkBackground ? PIC_BACKGROUND_COLOR_DARK : PIC_BACKGROUND_COLOR_LIGHT, (stInGameMenu == 3) ? MODE_HIGHTLIGHT_SMALL : MODE_NO_SELECT_SMALL);
		drawButton(fieldSizeX*(0.5 + 0.38 / aspect), fieldSizeY*(0.465 + (stInGameMenu == 4 ? 0.04 : 0)), PIC_EXIT, (stInGameMenu == 4) ? MODE_SELECTED_SMALL : MODE_NO_SELECT_SMALL);

		glColor3f(snakeRed*(1 + MENU_DELTA_COLOR_RED) - 0.1, snakeGreen*(1 + MENU_DELTA_COLOR_GREEN) - 0.1, snakeBlue*(1 + MENU_DELTA_COLOR_BLUE) - 0.1);
		glLineWidth(2);
		switch (stInGameMenu){
		case 0: aligWriteCenter(fieldSizeY*0.392, "œ–ŒƒŒÀ∆»“‹", 0.35*(float)fieldSize / 64, fieldSizeX*(0.5 - 0.38 / aspect)); break;
		case 1: aligWriteCenter(fieldSizeY*0.392, "¬€…“» ¬ Ã≈Õﬁ", 0.3*(float)fieldSize / 64, fieldSizeX*(0.5 - 0.19 / aspect)); break;
		case 2: aligWriteCenter(fieldSizeY*0.372, "÷¬≈“ «Ã≈… »:", 0.3*(float)fieldSize / 64, fieldSizeX*0.5);
			glColor3f(textrgb, textrgb, textrgb);
			menuAccentString(fieldSizeY*0.335, " –¿—Õ€…", 0.35*(float)fieldSize / 64, fieldSizeX*0.5, stSnakeColor == 0);
			menuAccentString(fieldSizeY*0.295, "Œ–¿Õ∆≈¬€…", 0.35*(float)fieldSize / 64, fieldSizeX*0.5, stSnakeColor == 1);
			menuAccentString(fieldSizeY*0.255, "∆®À“€…", 0.35*(float)fieldSize / 64, fieldSizeX*0.5, stSnakeColor == 2);
			menuAccentString(fieldSizeY*0.215, "«≈À®Õ€…", 0.35*(float)fieldSize / 64, fieldSizeX*0.5, stSnakeColor == 3);
			menuAccentString(fieldSizeY*0.175, "√ŒÀ”¡Œ…", 0.35*(float)fieldSize / 64, fieldSizeX*0.5, stSnakeColor == 4);
			menuAccentString(fieldSizeY*0.135, "—»Õ»…", 0.35*(float)fieldSize / 64, fieldSizeX*0.5, stSnakeColor == 5);
			menuAccentString(fieldSizeY*0.095, "‘»ŒÀ≈“Œ¬€…", 0.35*(float)fieldSize / 64, fieldSizeX*0.5, stSnakeColor == 6);
			break;
		case 3: aligWriteCenter(fieldSizeY*0.372, "‘ŒÕ:", 0.35*(float)fieldSize / 64, fieldSizeX*(0.5 + 0.19 / aspect));
			glColor3f(textrgb, textrgb, textrgb);
			menuAccentString(fieldSizeY*0.315, "—¬≈“À€…", 0.35*(float)fieldSize / 64, fieldSizeX*(0.5 + 0.19 / aspect), !darkBackground);
			menuAccentString(fieldSizeY*0.265, "“®ÃÕ€…", 0.35*(float)fieldSize / 64, fieldSizeX*(0.5 + 0.19 / aspect), darkBackground);
			break;
		case 4: aligWriteCenter(fieldSizeY*0.392, "¬€…“» »« »√–€", 0.3*(float)fieldSize / 64, fieldSizeX*(0.5 + 0.38 / aspect)); break;
		}

		glLineWidth(3);
	}

	if (enterNameDialog){
		glClearColor(backgroundRed - 0.1, backgroundGreen - 0.1, backgroundBlue - 0.1, 0);
		glColor3f(backgroundRed + 0.1, backgroundGreen + 0.1, backgroundBlue + 0.1);


		drawInGameMenuBackground((float)fieldSizeX*0.97 / aspect, (float)fieldSizeY*0.4, 5.5*(float)fieldSize / 64, (float)fieldSizeX / 2, (float)fieldSizeY / 2);

		glColor3f(backgroundRed - dlColor(0.05), backgroundGreen - dlColor(0.05), backgroundBlue - dlColor(0.05));
		drawInGameMenuBackground((float)fieldSizeX*0.48 / aspect, (float)fieldSizeY*0.15, 3 * (float)fieldSize / 64, (float)fieldSizeX / 2, (float)fieldSizeY*0.422);

		glColor3f(0.7 - dlColor(0.1), 0.7 - dlColor(0.1), 0.7 - dlColor(0.1));
		glLineWidth(4);
		aligWriteCenter(fieldSizeY*0.6, resultTable, 0.012*fieldSize, fieldSizeX*0.5);
		glLineWidth(2);
		aligWriteCenter(fieldSizeY*0.525, "¬¬≈ƒ»“≈ —¬Œ® »Ãﬂ:", 0.006*fieldSize, fieldSizeX / 2);

		aligWriteCenter(fieldSizeY*0.38, enterNameString, 0.015*fieldSize, fieldSizeX / 2);
		drawButton(fieldSizeX*(0.5 + 0.365 / aspect), fieldSizeY*(0.425 + (enterNameMenuSelect == 1 ? 0.02 : 0)), PIC_MAIN_MENU, enterNameMenuSelect == 1 ? MODE_SELECTED_SMALL : MODE_NO_SELECT_SMALL);
		drawButton(fieldSizeX*(0.5 - 0.365 / aspect), fieldSizeY*(0.425 + (enterNameMenuSelect == 0 ? 0.02 : 0)), PIC_EXIT, enterNameMenuSelect == 0 ? MODE_SELECTED_SMALL : MODE_NO_SELECT_SMALL);
		glColor3f(snakeRed*(1 + MENU_DELTA_COLOR_RED) - 0.1, snakeGreen*(1 + MENU_DELTA_COLOR_GREEN) - 0.1, snakeBlue*(1 + MENU_DELTA_COLOR_BLUE) - 0.1);
		if (enterNameMenuSelect == 0) aligWriteCenter(fieldSizeY*0.335, "¬€…“» »« »√–€", 0.3*(float)fieldSize / 64, fieldSizeX*(0.5 - 0.365 / aspect));
		if (enterNameMenuSelect == 1) aligWriteCenter(fieldSizeY*0.335, "¬€…“» ¬ Ã≈Õﬁ", 0.3*(float)fieldSize / 64, fieldSizeX*(0.5 + 0.365 / aspect));
		glLineWidth(3);
		glColor3f(textrgb, textrgb, textrgb);
	}

	if (!gamePaused&&!enterNameDialog) glutPostRedisplay();
	glFlush();

	Sleep((float)delay / animSmoothness);

	if (!gamePaused)
	if (!gameOver) animationPhase++;
	if (animationPhase == animSmoothness) animationPhase = 0;


}
void circle(float x, float y, float r, boolean fill, double steep){
	double angle = 0;

	if (fill)
		glBegin(GL_TRIANGLE_FAN); else glBegin(GL_LINE_LOOP);
	for (angle = 0; angle <= 360; angle += steep){
		glVertex2d(x + r*cos(angle*ANGLE_RADIAN), y + r*sin(angle*ANGLE_RADIAN));
	}
	glEnd();

}

void drawInGameMenuBackground(float x, float y,
	float roundRadius, float xcenter, float ycenter){
	float x1 = xcenter - x / 2, x2 = xcenter + x / 2,
		y1 = ycenter - y / 2, y2 = ycenter + y / 2;

	circle(x1 + roundRadius,
		y1 + roundRadius, roundRadius, 1, 10);
	circle(x2 - roundRadius,
		y1 + roundRadius, roundRadius, 1, 10);
	circle(x2 - roundRadius,
		y2 - roundRadius, roundRadius, 1, 10);
	circle(x1 + roundRadius,
		y2 - roundRadius, roundRadius, 1, 10);

	glRectf(x1, y1 + roundRadius,
		x1 + roundRadius, y2 - roundRadius);
	glRectf(x2, y1 + roundRadius,
		x2 - roundRadius, y2 - roundRadius);
	glRectf(x1 + roundRadius, y1,
		x2 - roundRadius, y1 + roundRadius);
	glRectf(x1 + roundRadius, y2,
		x2 - roundRadius, y2 - roundRadius);

	glRectf(x1 + roundRadius, y1 + roundRadius,
		x2 - roundRadius, y2 - roundRadius);
}

void drawButton(float x, float y, short type, short mode){
	float s = 1;

	float radius = 6;

	double picRed, picGreen, picBlue, backRed, backGreen, backBlue;
	switch (mode){
	case MODE_NO_SELECT_SMALL:
		radius = 5.5;
		s = 1 * ((float)fieldSizeY)/64;
	case MODE_NO_SELECT:
		picRed = picGreen = picBlue = 0.6;
		backRed = backGreen = backBlue = (backgroundRed + backgroundGreen + backgroundBlue) / 3 - dlColor(0.07);
		glColor3f(backRed, backGreen, backBlue);
		circle(x, y, radius*s, 1, 10);
		break;
	case MODE_SELECTED_SMALL:
		radius = 5.5;
		s = 1 * ((float)fieldSizeY) / 64;
	case MODE_SELECTED:
		picRed = picGreen = picBlue = 1;
		backRed = snakeRed*(1 + MENU_DELTA_COLOR_RED); backGreen = snakeGreen*(1 + MENU_DELTA_COLOR_GREEN); backBlue = snakeBlue*(1 + MENU_DELTA_COLOR_BLUE);
		glColor3f(backRed, backGreen, backBlue);
		circle(x, y, radius*s, 1, 10);
		break;
	case MODE_HIGHTLIGHT_SMALL:
		radius = 5.5;
		s = 1 * ((float)fieldSizeY) / 64;
	case MODE_HIGHTLIGHT:
		backRed = backGreen = backBlue = (backgroundRed + backgroundGreen + backgroundBlue) / 3 - dlColor(0.07);
		picRed = snakeRed*(1 + MENU_DELTA_COLOR_RED); picGreen = snakeGreen*(1 + MENU_DELTA_COLOR_GREEN); picBlue = snakeBlue*(1 + MENU_DELTA_COLOR_BLUE);
		glColor3f(backRed + dlColor(0.04), backGreen + dlColor(0.04), backBlue + dlColor(0.04));
		circle(x, y, radius*s, 1, 10);
		glColor3f(picRed, picGreen, picBlue);
		circle(x, y, radius*s, 0, 5);
		break;
	}



	glColor3f(picRed, picGreen, picBlue);
	switch (type){
	case PIC_PLAY:

		glBegin(GL_TRIANGLES);
		glVertex2d(x - 2 * s, y + 2.5*s);
		glVertex2d(x + 2.5*s, y + 0);
		glVertex2d(x - 2 * s, y - 2.5*s);
		glEnd(); break;

	case PIC_SPEED_LOW:
		glBegin(GL_LINE_STRIP);
		glVertex2d(x - 0.75*s, y + 2.5*s);
		glVertex2d(x + 0.75*s, y + 0);
		glVertex2d(x - 0.75*s, y - 2.5*s);
		glEnd(); break;

	case PIC_SPEED_MIDDLE:
		glBegin(GL_LINE_STRIP);
		glVertex2d(x - 1.25*s, y + 2.5*s);
		glVertex2d(x + 0.25*s, y + 0);
		glVertex2d(x - 1.25*s, y - 2.5*s);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glVertex2d(x - 0.25*s, y + 2.5*s);
		glVertex2d(x + 1.25*s, y + 0);
		glVertex2d(x - 0.25*s, y - 2.5*s);
		glEnd(); break;


	case PIC_SPEED_HIGHT:
		glBegin(GL_LINE_STRIP);
		glVertex2d(x - 1.5*s, y + 2.5*s);
		glVertex2d(x + 0, y + 0);
		glVertex2d(x - 1.5*s, y - 2.5*s);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glVertex2d(x - 0.5*s, y + 2.5*s);
		glVertex2d(x + 1 * s, y + 0);
		glVertex2d(x - 0.5*s, y - 2.5*s);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glVertex2d(x + 0.5*s, y + 2.5*s);
		glVertex2d(x + 2 * s, y + 0);
		glVertex2d(x + 0.5*s, y - 2.5*s);
		glEnd();
		break;

	case PIC_FIELD_SMALL:
		glBegin(GL_LINE_LOOP);
		glVertex2d(x - 1 * s, y + 1 * s);
		glVertex2d(x + 1 * s, y + 1 * s);
		glVertex2d(x + 1 * s, y - 1 * s);
		glVertex2d(x - 1 * s, y - 1 * s);
		glEnd(); break;

	case PIC_FIELD_MIDDLE:
		glBegin(GL_LINE_LOOP);
		glVertex2d(x - 2 * s, y + 2 * s);
		glVertex2d(x + 2 * s, y + 2 * s);
		glVertex2d(x + 2 * s, y - 2 * s);
		glVertex2d(x - 2 * s, y - 2 * s);
		glEnd(); break;

	case PIC_FIELD_BIG:
		glBegin(GL_LINE_LOOP);
		glVertex2d(x - 3 * s, y + 3 * s);
		glVertex2d(x + 3 * s, y + 3 * s);
		glVertex2d(x + 3 * s, y - 3 * s);
		glVertex2d(x - 3 * s, y - 3 * s);
		glEnd(); break;

	case PIC_EXIT:
		glBegin(GL_LINE_LOOP);
		glVertex2d(x - 1.5*s, y + 2.25*s);
		glVertex2d(x + 1.5*s, y + 2.25*s);
		glVertex2d(x + 1.5*s, y - 2.25*s);
		glVertex2d(x - 1.5*s, y - 2.25*s);
		glEnd();
		glBegin(GL_QUADS);
		glVertex2d(x - 1.5*s, y + 2.25*s);
		glVertex2d(x + 0.5*s, y + 1.25*s);
		glVertex2d(x + 0.5*s, y - 3.25*s);
		glVertex2d(x - 1.5*s, y - 2.25*s);
		glEnd();
		break;

	case PIC_BORDERS_OFF:
		glBegin(GL_LINES);
		glVertex2d(x - 1.5*s, y - 2.25*s);
		glVertex2d(x + 1.5*s, y - 2.25*s);

		glVertex2d(x + 0, y - 2.25*s);
		glVertex2d(x + 0, y - 0.5*s);

		glVertex2d(x + 0, y - 2.25*s);
		glVertex2d(x - 0.75*s, y - 1.5*s);
		glVertex2d(x + 0, y - 2.25*s);
		glVertex2d(x + 0.75*s, y - 1.5*s);

		glVertex2d(x - 1.5*s, y + 2.25*s);
		glVertex2d(x + 1.5*s, y + 2.25*s);

		glVertex2d(x + 0, y + 2.25*s);
		glVertex2d(x + 0, y + 0.75*s);

		glVertex2d(x + 0, y + 0.75*s);
		glVertex2d(x - 0.75*s, y + 1.5*s);
		glVertex2d(x + 0, y + 0.75*s);
		glVertex2d(x + 0.75*s, y + 1.5*s);
		glEnd();
		break;

	case PIC_BORDERS_ON:
		glBegin(GL_LINES);
		glVertex2d(x - 1.5*s, y - 2.25*s);
		glVertex2d(x + 1.5*s, y - 2.25*s);

		glVertex2d(x + 0 * s, y - 2.25*s);
		glVertex2d(x + 0 * s, y - 0.5*s);

		glVertex2d(x + 0.75*s, y - 3 * s);
		glVertex2d(x - 0.75*s, y - 1.5*s);
		glVertex2d(x - 0.75*s, y - 3 * s);
		glVertex2d(x + 0.75*s, y - 1.5*s);

		glVertex2d(x - 1.5*s, y + 2.25*s);
		glVertex2d(x + 1.5*s, y + 2.25*s);


		glEnd();

		break;

	case PIC_CONTINUE_GAME:
		glBegin(GL_LINES);
		glVertex2d(x - 2.5*s, y);
		glVertex2d(x + 1.5*s, y);

		glVertex2d(x + 1.5*s, y);
		glVertex2d(x, y + 1.5*s);

		glVertex2d(x + 1.5*s, y);
		glVertex2d(x, y - 1.5*s);
		glEnd();

		break;
	case PIC_MAIN_MENU:
		glPointSize(6);
		glBegin(GL_POINTS);
		glVertex2d(x - 2 * s, y + 2 * s);
		glVertex2d(x - 2 * s, y);
		glVertex2d(x - 2 * s, y - 2 * s);
		glEnd();
		glPointSize(3);

		glBegin(GL_LINES);
		glVertex2d(x - 1.5*s, y + 2 * s);
		glVertex2d(x + 2 * s, y + 2 * s);

		glVertex2d(x - 1.5*s, y);
		glVertex2d(x + 2 * s, y);

		glVertex2d(x - 1.5*s, y - 2 * s);
		glVertex2d(x + 2 * s, y - 2 * s);

		glEnd();

		break;

	case PIC_SNAKE_COLOR:
		glLineWidth(6);
		glBegin(GL_LINE_STRIP);
		glVertex2d(x - 2 * s, y + 2 * s);
		glVertex2d(x + 2 * s, y + 2 * s);
		glVertex2d(x + 2 * s, y);
		glVertex2d(x - 2 * s, y);
		glVertex2d(x - 2 * s, y - 2 * s);
		glVertex2d(x + 2 * s, y - 2 * s);
		glEnd();
		glLineWidth(3);
		break;

	case PIC_BACKGROUND_COLOR_LIGHT:
		circle(x, y, 2 * s, 1, 15);
		glBegin(GL_LINES);
		glVertex2d(x - 2.25*s, y);
		glVertex2d(x - 3 * s, y);

		glVertex2d(x + 2.25*s, y);
		glVertex2d(x + 3 * s, y);

		glVertex2d(x, y + 2.25*s);
		glVertex2d(x, y + 3 * s);

		glVertex2d(x, y - 2.25*s);
		glVertex2d(x, y - 3 * s);

		glVertex2d(x + 1.65*s, y + 1.65*s);
		glVertex2d(x + 2.25*s, y + 2.25*s);

		glVertex2d(x - 1.65*s, y - 1.65*s);
		glVertex2d(x - 2.25*s, y - 2.25*s);

		glVertex2d(x - 1.65*s, y + 1.65*s);
		glVertex2d(x - 2.25*s, y + 2.25*s);

		glVertex2d(x + 1.65*s, y - 1.65*s);
		glVertex2d(x + 2.25*s, y - 2.25*s);
		glEnd(); break;

	case PIC_BACKGROUND_COLOR_DARK:
		circle(x + 0.1, y, 3 * s, 1, 15);
		if (mode == MODE_HIGHTLIGHT_SMALL) glColor3f(backRed + dlColor(0.04), backGreen + dlColor(0.04), backBlue + dlColor(0.04));
		else glColor3f(backRed, backGreen, backBlue);
		circle(x + 0.6, y, 3 * s, 1, 15);
		glColor3f(picRed, picGreen, picBlue);
		break;
	}

}
void linebz(GLfloat* p){
	glMap1f(GL_MAP1_VERTEX_3, 0, 1, 3, 4, p);
	glBegin(GL_LINE_STRIP);
	for (float i = 0; i <= 20; i++) glEvalCoord1f(i / 20);
	glEnd();
}
void drawChar(float x, float y, char c, double s){
	switch (c){
	case '¿': glBegin(GL_LINES);
		glVertex2d(x, y);
		glVertex2d(x + 1 * s, y + 4 * s);

		glVertex2d(x + 1 * s, y + 4 * s);
		glVertex2d(x + 2 * s, y);

		glVertex2d(x + 0.5*s, y + 2 * s);
		glVertex2d(x + 1.5*s, y + 2 * s);
		glEnd(); break;

	case '¡':
		glBegin(GL_LINE_STRIP);
		glVertex2d(x, y + 2 * s);
		glVertex2d(x + 2 * s, y + 1 * s);
		glVertex2d(x, y);
		glVertex2d(x, y + 4 * s);
		glVertex2d(x + 2 * s, y + 4 * s);
		glEnd(); break;

	case '¬': glBegin(GL_LINE_STRIP);
		glVertex2d(x, y + 2 * s);
		glVertex2d(x + 2 * s, y + 1 * s);
		glVertex2d(x, y);
		glVertex2d(x, y + 4 * s);
		glVertex2d(x + 2 * s, y + 3 * s);
		glVertex2d(x, y + 2 * s);
		glEnd(); break;

	case '√': glBegin(GL_LINE_STRIP);
		glVertex2d(x, y);
		glVertex2d(x, y + 4 * s);
		glVertex2d(x + 2 * s, y + 4 * s);
		glEnd(); break;

	case 'ƒ': glBegin(GL_LINE_STRIP);
		glVertex2d(x, y);
		glVertex2d(x, y + 1 * s);
		glVertex2d(x + 2 * s, y + 1 * s);
		glVertex2d(x + 2 * s, y);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glVertex2d(x + 0.5*s, y + 1 * s);
		glVertex2d(x + 1 * s, y + 4 * s);
		glVertex2d(x + 1.5*s, y + 1 * s);
		glEnd();
		break;

	case '≈': glBegin(GL_LINE_STRIP);
		glVertex2d(x, y);
		glVertex2d(x, y + 4 * s);
		glVertex2d(x + 2 * s, y + 4 * s);
		glEnd();
		glBegin(GL_LINES);
		glVertex2d(x, y + 2 * s);
		glVertex2d(x + 2 * s, y + 2 * s);

		glVertex2d(x, y);
		glVertex2d(x + 2 * s, y);
		glEnd();
		break;
	case '®': glBegin(GL_LINE_STRIP);
		glVertex2d(x, y);
		glVertex2d(x, y + 4 * s);
		glVertex2d(x + 2 * s, y + 4 * s);
		glEnd();
		glBegin(GL_LINES);
		glVertex2d(x, y + 2 * s);
		glVertex2d(x + 2 * s, y + 2 * s);

		glVertex2d(x, y);
		glVertex2d(x + 2 * s, y);
		glEnd();

		glBegin(GL_POINTS);
		glVertex2d(x + 0.5*s, y + 4.5*s);
		glVertex2d(x + 1.5*s, y + 4.5*s);
		glEnd();
		break;
	case '∆':
		glBegin(GL_LINES);
		glVertex2d(x, y);
		glVertex2d(x + 2 * s, y + 4 * s);

		glVertex2d(x, y + 4 * s);
		glVertex2d(x + 2 * s, y);

		glVertex2d(x + 1 * s, y + 4 * s);
		glVertex2d(x + 1 * s, y);
		glEnd();

		break;

	case '«':
		glBegin(GL_LINE_STRIP);
		glVertex2d(x, y + 4 * s);
		glVertex2d(x + 2 * s, y + 3 * s);
		glVertex2d(x, y + 2 * s);
		glVertex2d(x + 2 * s, y + 1 * s);
		glVertex2d(x, y);
		glEnd();

		break;

	case '»':
		glBegin(GL_LINE_STRIP);
		glVertex2d(x, y + 4 * s);
		glVertex2d(x, y);
		glVertex2d(x + 2 * s, y + 4 * s);
		glVertex2d(x + 2 * s, y);
		glEnd();
		break;

	case '…':
		glBegin(GL_LINE_STRIP);
		glVertex2d(x, y + 4 * s);
		glVertex2d(x, y);
		glVertex2d(x + 2 * s, y + 4 * s);
		glVertex2d(x + 2 * s, y);
		glEnd();

		glBegin(GL_LINES);
		glVertex2d(x + 0.5*s, y + 4.5*s);
		glVertex2d(x + 1.5*s, y + 4.5*s);
		glEnd();
		break;

	case ' ':
		glBegin(GL_LINES);
		glVertex2d(x, y + 4 * s);
		glVertex2d(x, y);
		glVertex2d(x, y + 2 * s);
		glVertex2d(x + 2 * s, y + 4 * s);
		glVertex2d(x, y + 2 * s);
		glVertex2d(x + 2 * s, y);
		glEnd();
		break;

	case 'À':
		glBegin(GL_LINE_STRIP);
		glVertex2d(x, y);
		glVertex2d(x + 1 * s, y + 4 * s);
		glVertex2d(x + 2 * s, y);
		glEnd();
		break;

	case 'Ã':
		glBegin(GL_LINE_STRIP);
		glVertex2d(x, y);
		glVertex2d(x, y + 4 * s);
		glVertex2d(x + 1 * s, y);
		glVertex2d(x + 2 * s, y + 4 * s);
		glVertex2d(x + 2 * s, y);
		glEnd();
		break;
	case 'Õ': glBegin(GL_LINES);
		glVertex2d(x, y);
		glVertex2d(x, y + 4 * s);
		glVertex2d(x, y + 2 * s);
		glVertex2d(x + 2 * s, y + 2 * s);
		glVertex2d(x + 2 * s, y);
		glVertex2d(x + 2 * s, y + 4 * s);
		glEnd();
		break;

	case 'Œ': glBegin(GL_LINE_LOOP);
		glVertex2d(x + 1 * s, y);
		glVertex2d(x + 2 * s, y + 2 * s);
		glVertex2d(x + 1 * s, y + 4 * s);
		glVertex2d(x, y + 2 * s);
		glEnd();
		break;

	case 'œ': glBegin(GL_LINE_STRIP);
		glVertex2d(x, y);
		glVertex2d(x, y + 4 * s);
		glVertex2d(x + 2 * s, y + 4 * s);
		glVertex2d(x + 2 * s, y);
		glEnd(); break;

	case '–': glBegin(GL_LINE_STRIP);
		glVertex2d(x, y);
		glVertex2d(x, y + 4 * s);
		glVertex2d(x + 2 * s, y + 3 * s);
		glVertex2d(x, y + 2 * s);
		glEnd(); break;

	case '—': glBegin(GL_LINE_STRIP);
		glVertex2d(x + 2 * s, y + 4 * s);
		glVertex2d(x, y + 2 * s);
		glVertex2d(x + 2 * s, y);
		glEnd(); break;

	case '“': glBegin(GL_LINES);
		glVertex2d(x + 1 * s, y);
		glVertex2d(x + 1 * s, y + 4 * s);
		glVertex2d(x, y + 4 * s);
		glVertex2d(x + 2 * s, y + 4 * s);
		glEnd();
		break;

	case '”': glBegin(GL_LINES);
		glVertex2d(x, y);
		glVertex2d(x + 2 * s, y + 4 * s);
		glVertex2d(x, y + 4 * s);
		glVertex2d(x + 1 * s, y + 2 * s);

		glEnd();
		break;

	case '‘': glBegin(GL_LINES);
		glVertex2d(x + 1 * s, y);
		glVertex2d(x + 1 * s, y + 4 * s);
		glEnd();
		glBegin(GL_LINE_LOOP);
		glVertex2d(x, y + 2 * s);
		glVertex2d(x + 1 * s, y + 3.25*s);
		glVertex2d(x + 2 * s, y + 2 * s);
		glVertex2d(x + 1 * s, y + 0.75*s);
		glEnd();
		break;

	case '’':glBegin(GL_LINES);
		glVertex2d(x, y);
		glVertex2d(x + 2 * s, y + 4 * s);
		glVertex2d(x, y + 4 * s);
		glVertex2d(x + 2 * s, y);
		glEnd();
		break;

	case '÷':
		glBegin(GL_LINE_STRIP);
		glVertex2d(x, y + 4 * s);
		glVertex2d(x, y);
		glVertex2d(x + 2 * s, y);
		glVertex2d(x + 2 * s, y + 4 * s);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glVertex2d(x + 2 * s, y);
		glVertex2d(x + 2.5*s, y);
		glVertex2d(x + 2.5*s, y - 0.75*s);
		glEnd();
		break;

	case '◊':
		glBegin(GL_LINE_STRIP);
		glVertex2d(x, y + 4 * s);
		glVertex2d(x, y + 2 * s);
		glVertex2d(x + 2 * s, y + 2 * s);
		glVertex2d(x + 2 * s, y + 4 * s);
		glVertex2d(x + 2 * s, y);
		glEnd();
		break;

	case 'ÿ':
		glBegin(GL_LINE_STRIP);
		glVertex2d(x, y + 4 * s);
		glVertex2d(x, y);
		glVertex2d(x + 1 * s, y);
		glVertex2d(x + 1 * s, y + 4 * s);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glVertex2d(x + 1 * s, y);
		glVertex2d(x + 2 * s, y);
		glVertex2d(x + 2 * s, y + 4 * s);
		glEnd();
		break;

	case 'Ÿ':
		glBegin(GL_LINE_STRIP);
		glVertex2d(x, y + 4 * s);
		glVertex2d(x, y);
		glVertex2d(x + 1 * s, y);
		glVertex2d(x + 1 * s, y + 4 * s);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glVertex2d(x + 1 * s, y);
		glVertex2d(x + 2 * s, y);
		glVertex2d(x + 2 * s, y + 4 * s);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glVertex2d(x + 2 * s, y);
		glVertex2d(x + 2.5*s, y);
		glVertex2d(x + 2.5*s, y - 0.75*s);
		glEnd();
		break;

	case '⁄':
		glBegin(GL_LINE_STRIP);
		glVertex2d(x, y + 2 * s);
		glVertex2d(x + 2 * s, y + 1 * s);
		glVertex2d(x, y);
		glVertex2d(x, y + 4 * s);
		glVertex2d(x - 0.75*s, y + 4 * s);
		glEnd(); break;

	case '€':
		glBegin(GL_LINE_STRIP);
		glVertex2d(x, y + 2 * s);
		glVertex2d(x + 1.75*s, y + 1 * s);
		glVertex2d(x, y);
		glVertex2d(x, y + 4 * s);
		glEnd();
		glBegin(GL_LINES);
		glVertex2d(x + 2 * s, y);
		glVertex2d(x + 2 * s, y + 4 * s);
		glEnd();
		break;

	case '‹':
		glBegin(GL_LINE_STRIP);
		glVertex2d(x, y + 2 * s);
		glVertex2d(x + 2 * s, y + 1 * s);
		glVertex2d(x, y);
		glVertex2d(x, y + 4 * s);
		glEnd(); break;

	case '›':
		glBegin(GL_LINE_STRIP);
		glVertex2d(x, y + 4 * s);
		glVertex2d(x + 2 * s, y + 2 * s);
		glVertex2d(x, y);
		glEnd();
		glBegin(GL_LINES);
		glVertex2d(x + 2 * s, y + 2 * s);
		glVertex2d(x, y + 2 * s);
		glEnd();
		break;

	case 'ﬁ':
		glBegin(GL_LINES);
		glVertex2d(x, y);
		glVertex2d(x, y + 4 * s);
		glVertex2d(x, y + 2 * s);
		glVertex2d(x + 0.5*s, y + 2 * s);
		glEnd();
		glBegin(GL_LINE_LOOP);
		glVertex2d(x + 0.5*s, y + 2 * s);
		glVertex2d(x + 1.25*s, y + 4 * s);
		glVertex2d(x + 2 * s, y + 2 * s);
		glVertex2d(x + 1.25*s, y);
		glEnd();
		break;

	case 'ﬂ':
		glBegin(GL_LINE_STRIP);
		glVertex2d(x + 2 * s, y);
		glVertex2d(x + 2 * s, y + 4 * s);
		glVertex2d(x, y + 3 * s);
		glVertex2d(x + 2 * s, y + 2 * s);
		glVertex2d(x, y);
		glEnd();

		break;

	case '(':
		glBegin(GL_LINE_STRIP);
		glVertex2d(x + 2 * s, y + 4 * s);
		glVertex2d(x + 1 * s, y + 2 * s);
		glVertex2d(x + 2 * s, y);
		glEnd(); break;
	case ')':
		glBegin(GL_LINE_STRIP);
		glVertex2d(x, y + 4 * s);
		glVertex2d(x + 1 * s, y + 2 * s);
		glVertex2d(x, y);
		glEnd(); break;
	case ',':
		glBegin(GL_LINES);
		glVertex2d(x + 0.75*s, y);
		glVertex2d(x, y - 1.25*s);
		glVertex2d(x, y);
		glEnd(); break;
	case '.':
		glBegin(GL_POINTS);
		glVertex2d(x, y);
		glEnd(); break;
	case '-':
		glBegin(GL_LINES);
		glVertex2d(x, y + 2 * s);
		glVertex2d(x + 2 * s, y + 2 * s);
		glEnd(); break;
	case '1':
		glBegin(GL_LINE_STRIP);
		glVertex2d(x, y + 2 * s);
		glVertex2d(x + 2 * s, y + 4 * s);
		glVertex2d(x + 2 * s, y);
		glEnd(); break;
	case '2':
		glBegin(GL_LINE_STRIP);
		glVertex2d(x, y + 4 * s);
		glVertex2d(x + 2 * s, y + 2.5*s);
		glVertex2d(x, y + 1 * s);
		glVertex2d(x, y);
		glVertex2d(x + 2 * s, y);
		glEnd(); break;
	case '3':
		glBegin(GL_LINE_STRIP);
		glVertex2d(x, y + 4 * s);
		glVertex2d(x + 2 * s, y + 3 * s);
		glVertex2d(x, y + 2 * s);
		glVertex2d(x + 2 * s, y + 1 * s);
		glVertex2d(x, y);
		glEnd();

		break;
	case '4':
		glBegin(GL_LINE_STRIP);
		glVertex2d(x + 2 * s, y);
		glVertex2d(x + 2 * s, y + 4 * s);
		glVertex2d(x, y + 2 * s);
		glVertex2d(x + 2.5*s, y + 2 * s);
		glEnd();
		break;
	case '5':
		glBegin(GL_LINE_STRIP);
		glVertex2d(x + 2 * s, y + 4 * s);
		glVertex2d(x, y + 4 * s);
		glVertex2d(x, y + 3 * s);
		glVertex2d(x + 2 * s, y + 1.5*s);
		glVertex2d(x, y);

		glEnd(); break;

	case '6':
		glBegin(GL_LINE_STRIP);
		glVertex2d(x + 1 * s, y + 4 * s);
		glVertex2d(x, y + 2 * s);
		glVertex2d(x + 1 * s, y);
		glVertex2d(x + 2 * s, y + 2 * s);
		glVertex2d(x, y + 2 * s);

		glEnd(); break;

	case '7':
		glBegin(GL_LINE_STRIP);
		glVertex2d(x, y + 4 * s);
		glVertex2d(x + 2 * s, y + 4 * s);
		glVertex2d(x, y);
		glEnd();
		glBegin(GL_LINES);
		glVertex2d(x + 0.25*s, y + 2 * s);
		glVertex2d(x + 1.75*s, y + 2 * s);
		glEnd();
		break;

	case '8':
		glBegin(GL_LINE_LOOP);
		glVertex2d(x + 1 * s, y + 4 * s);
		glVertex2d(x, y + 3 * s);
		glVertex2d(x + 2 * s, y + 1 * s);
		glVertex2d(x + 1 * s, y);
		glVertex2d(x, y + 1 * s);
		glVertex2d(x + 2 * s, y + 3 * s);
		glEnd(); break;

	case '9':
		glBegin(GL_LINE_STRIP);
		glVertex2d(x + 1 * s, y);
		glVertex2d(x + 2 * s, y + 2 * s);
		glVertex2d(x + 1 * s, y + 4 * s);
		glVertex2d(x, y + 2 * s);
		glVertex2d(x + 2 * s, y + 2 * s);

		glEnd(); break;

	case '0':
		glBegin(GL_LINE_LOOP);
		glVertex2d(x + 1 * s, y);
		glVertex2d(x + 2 * s, y + 2 * s);
		glVertex2d(x + 1 * s, y + 4 * s);
		glVertex2d(x, y + 2 * s);
		glEnd();

		glBegin(GL_LINES);
		glVertex2d(x + 1.5*s, y + 3 * s);
		glVertex2d(x + 0.5*s, y + 1 * s);
		glEnd();

		break;

	case ':':
		glBegin(GL_POINTS);
		glVertex2d(x, y + 0.75*s);
		glVertex2d(x, y + 3.25*s);
		glEnd();
		break;
	case '!':
		glPointSize(5);
		glBegin(GL_POINTS);
		glVertex2d(x + 0.5, y);
		glEnd();
		glPointSize(3);

		glBegin(GL_LINES);
		glVertex2d(x + 0.5, y + 0.75*s);
		glVertex2d(x + 0.5, y + 4 * s);
		glEnd();
		break;
	}
}

void write(float x, float y,
	char* string, float scale){
	int l = strlen(string);
	int m = 0, ns = 0;
	for (int i = 0; i<l; i++){
		if (string[i] == '\n') { m = -1; ns++; }
		drawChar(x + 3 * m*scale,
			y - 5 * ns*scale, string[i], scale);
		m++;
	}
}
void aligWriteCenter(float y, char* string,
	float scale, float xcenter){
	float xs;
	int length = strlen(string);
	xs = xcenter - (3 * length*scale) / 2;
	write(xs, y, string, scale);
}


void menuAccentString(float y, char* string,
	float scale, float xcenter, boolean accented){
	float xsBegin, xsEnd;
	int length = strlen(string);
	xsBegin = xcenter - (3 * length*scale) / 2;
	xsEnd = xcenter + (3 * length*scale) / 2;
	if (accented){
		glColor3f(snakeRed*(1 + MENU_DELTA_COLOR_RED), snakeGreen*(1 + MENU_DELTA_COLOR_GREEN), snakeBlue*(1 + MENU_DELTA_COLOR_BLUE));

		circle(xsBegin - 1.5*scale, y + 2 * scale, 3.5*scale, 1, 15);
		circle(xsEnd + 1.5*scale, y + 2 * scale, 3.5*scale, 1, 15);

		glRectf(xsBegin - 1.5*scale, y - 1.5*scale, xsEnd + 1.5*scale, y + 5.5*scale);

		glColor3f(1, 1, 1);


	}
	else glColor3f(textrgb, textrgb, textrgb);


	write(xsBegin, y, string, scale);
	glColor3f(textrgb, textrgb, textrgb);

}

void menuColoredString(float y, char* string, float scale, float xcenter, boolean accented){
	float xsBegin, xsEnd;
	int length = strlen(string);
	xsBegin = xcenter - (3 * length*scale) / 2;
	xsEnd = xcenter + (3 * length*scale) / 2;

	if (accented)
		glColor3f(snakeRed*(1 + MENU_DELTA_COLOR_RED) - 0.1, snakeGreen*(1 + MENU_DELTA_COLOR_GREEN) - 0.1, snakeBlue*(1 + MENU_DELTA_COLOR_BLUE) - 0.1);
	write(xsBegin, y, string, scale);
	glColor3f(textrgb, textrgb, textrgb);
}


float textYposition;
float multTestScore;
void getTestScore(float multSpeed, float multField, float multBorders){
	multTestScore = multSpeed*multField*multBorders;
	sprintf(multScoreString, "%.2f’", multTestScore);
}
void menu(void){
#define MENU_STRING_SIZE 0.7
#define MENU_HEADER_Y 18
#define MENU_STRING_1_Y 13-5*MENU_STRING_SIZE*1
#define MENU_STRING_2_Y 13-5*MENU_STRING_SIZE*2
#define MENU_STRING_3_Y 13-5*MENU_STRING_SIZE*3
#define MENU_STRING_4_Y 13-5*MENU_STRING_SIZE*4
#define MENU_STRING_5_Y 13-5*MENU_STRING_SIZE*5
#define MENU_STRING_6_Y 13-5*MENU_STRING_SIZE*6
#define MENU_STRING_7_Y 13-5*MENU_STRING_SIZE*7
#define MENU_STRING_8_Y 13-5*MENU_STRING_SIZE*8
#define MENU_STRING_9_Y 13-5*MENU_STRING_SIZE*9
#define MENU_STRING_10_Y 13-5*MENU_STRING_SIZE*10
#define MENU_STRING_11_Y 13-5*MENU_STRING_SIZE*11
#define MENU_STRING_12_Y 13-5*MENU_STRING_SIZE*12


	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(backgroundRed - dlColor(0.03), backgroundGreen - dlColor(0.03), backgroundBlue - dlColor(0.03));
	circle(-32 * aspect - 21, -3, 43, 1, 5);

	drawButton(-32 * aspect + 7, 19, PIC_PLAY, (stMenu == 0) ? MODE_SELECTED : MODE_NO_SELECT);
	drawButton(-32 * aspect + 13, 8, stSpeed == 0 ? PIC_SPEED_LOW : (stSpeed == 1 ? PIC_SPEED_MIDDLE : PIC_SPEED_HIGHT), (stMenu == 1) ? MODE_SELECTED : MODE_NO_SELECT);
	drawButton(-32 * aspect + 7, -3, stFieldSize == 0 ? PIC_FIELD_SMALL : (stFieldSize == 1 ? PIC_FIELD_MIDDLE : PIC_FIELD_BIG), (stMenu == 2) ? MODE_SELECTED : MODE_NO_SELECT);
	drawButton(-32 * aspect + 13, -14, border ? PIC_BORDERS_ON : PIC_BORDERS_OFF, (stMenu == 3) ? MODE_SELECTED : MODE_NO_SELECT);
	drawButton(-32 * aspect + 7, -25, PIC_EXIT, (stMenu == 4) ? MODE_SELECTED : MODE_NO_SELECT);

	glColor3f(snakeRed*(1 + MENU_DELTA_COLOR_RED), snakeGreen*(1 + MENU_DELTA_COLOR_GREEN), snakeBlue*(1 + MENU_DELTA_COLOR_BLUE));
	glRectf(-25.75, 32 / (aspect<1 ? aspect : 1) - 6.25, 25.75, 32 / (aspect<1 ? aspect : 1));

	circle(-25.75, 32 / (aspect<1 ? aspect : 1) - 3, 3.125, 1, 15);
	circle(25.75, 32 / (aspect<1 ? aspect : 1) - 3, 3.125, 1, 15);
	glColor3f(1, 1, 1);
	aligWriteCenter(32 / (aspect<1 ? aspect : 1) - 5.5, "«Ã≈… ¿", 1, 0);


	glColor3f(textrgb, textrgb, textrgb);

	textYposition = -32 * aspect + 32>-10 * aspect ? -32 * aspect + 32 + 10 * aspect : 0;

	switch (stMenu){
	case 0:

		aligWriteCenter(MENU_HEADER_Y, "Õ¿◊¿“‹ »√–”", MENU_STRING_SIZE, textYposition);
		aligWriteCenter(MENU_STRING_1_Y, "Œ◊ »:", 0.6, textYposition);
		getTestScore(multSpeed, multField, multBorders);
		if (multTestScore<0.9) glColor3f(0.9, 0.65, 0.4);
		else if (multTestScore<1.51) glColor3f(0.65, 0.9, 0.4);
		else if (multTestScore<2.25) glColor3f(0.4, 0.65, 0.9);
		else glColor3f(0.65, 0.4, 1);
		glLineWidth(2);
		aligWriteCenter(MENU_STRING_2_Y, multScoreString, 0.5, textYposition);
		glLineWidth(3);
		glColor3f(textrgb + dlColor(0.2), textrgb + dlColor(0.2), textrgb + dlColor(0.2));
		aligWriteCenter(MENU_STRING_5_Y, "–≈ Œ–ƒ€:", 0.6, textYposition);
		glLineWidth(2);
		glColor3f(textrgb, textrgb + dlColor(0.05), textrgb + dlColor(0.2));
		aligWriteCenter(MENU_STRING_6_Y, "1", 0.5, textYposition - 11);
		aligWriteCenter(MENU_STRING_6_Y, maxScoreNames[0], 0.5, textYposition - 2);
		aligWriteCenter(MENU_STRING_6_Y, maxScoreString[0], 0.5, textYposition + 10);

		glColor3f(textrgb + dlColor(0.06), textrgb + dlColor(0.1), textrgb + dlColor(0.2));
		aligWriteCenter(MENU_STRING_7_Y, "2", 0.5, textYposition - 11);
		aligWriteCenter(MENU_STRING_7_Y, maxScoreNames[1], 0.5, textYposition - 2);
		aligWriteCenter(MENU_STRING_7_Y, maxScoreString[1], 0.5, textYposition + 10);

		glColor3f(textrgb + dlColor(0.13), textrgb + dlColor(0.15), textrgb + dlColor(0.2));
		aligWriteCenter(MENU_STRING_8_Y, "3", 0.5, textYposition - 11);
		aligWriteCenter(MENU_STRING_8_Y, maxScoreNames[2], 0.5, textYposition - 2);
		aligWriteCenter(MENU_STRING_8_Y, maxScoreString[2], 0.5, textYposition + 10);

		glColor3f(textrgb + dlColor(0.2), textrgb + dlColor(0.2), textrgb + dlColor(0.2));
		aligWriteCenter(MENU_STRING_9_Y, "4", 0.5, textYposition - 11);
		aligWriteCenter(MENU_STRING_9_Y, maxScoreNames[3], 0.5, textYposition - 2);
		aligWriteCenter(MENU_STRING_9_Y, maxScoreString[3], 0.5, textYposition + 10);

		aligWriteCenter(MENU_STRING_10_Y, "5", 0.5, textYposition - 11);
		aligWriteCenter(MENU_STRING_10_Y, maxScoreNames[4], 0.5, textYposition - 2);
		aligWriteCenter(MENU_STRING_10_Y, maxScoreString[4], 0.5, textYposition + 10);
		glLineWidth(3);
		glColor3f(textrgb, textrgb, textrgb);
		break;
	case 1:
		aligWriteCenter(MENU_STRING_1_Y, "— Œ–Œ—“‹ «Ã≈… »:", MENU_STRING_SIZE, textYposition);
		glLineWidth(2);
		menuAccentString(MENU_STRING_3_Y, "Ã≈ƒÀ≈ÕÕ¿ﬂ", 0.4, textYposition - 12, stSpeed == 0 ? 1 : 0);
		menuAccentString(MENU_STRING_3_Y, "—–≈ƒÕﬂﬂ", 0.4, textYposition, stSpeed == 1 ? 1 : 0);
		menuAccentString(MENU_STRING_3_Y, "¡€—“–¿ﬂ", 0.4, textYposition + 12, stSpeed == 2 ? 1 : 0);

		getTestScore(0.75, multField, multBorders);
		menuColoredString(MENU_STRING_4_Y, multScoreString, 0.4, textYposition - 12, stSpeed == 0 ? 1 : 0);
		getTestScore(1, multField, multBorders);
		menuColoredString(MENU_STRING_4_Y, multScoreString, 0.4, textYposition, stSpeed == 1 ? 1 : 0);
		getTestScore(1.35, multField, multBorders);
		menuColoredString(MENU_STRING_4_Y, multScoreString, 0.4, textYposition + 12, stSpeed == 2 ? 1 : 0);
		glLineWidth(3);
		break;
	case 2:
		aligWriteCenter(MENU_STRING_3_Y, "–¿«Ã≈– œŒÀﬂ:", MENU_STRING_SIZE, textYposition);
		glLineWidth(2);
		menuAccentString(MENU_STRING_5_Y, "Ã¿À≈Õ‹ »…", 0.4, textYposition - 12, stFieldSize == 0 ? 1 : 0);
		menuAccentString(MENU_STRING_5_Y, "—–≈ƒÕ»…", 0.4, textYposition, stFieldSize == 1 ? 1 : 0);
		menuAccentString(MENU_STRING_5_Y, "¡ŒÀ‹ÿŒ…", 0.4, textYposition + 12, stFieldSize == 2 ? 1 : 0);

		getTestScore(multSpeed, 1.5, multBorders);
		menuColoredString(MENU_STRING_6_Y, multScoreString, 0.4, textYposition - 12, stFieldSize == 0 ? 1 : 0);
		getTestScore(multSpeed, 1, multBorders);
		menuColoredString(MENU_STRING_6_Y, multScoreString, 0.4, textYposition, stFieldSize == 1 ? 1 : 0);
		getTestScore(multSpeed, 0.75, multBorders);
		menuColoredString(MENU_STRING_6_Y, multScoreString, 0.4, textYposition + 12, stFieldSize == 2 ? 1 : 0);
		glLineWidth(3);
		break;
	case 3:
		aligWriteCenter(MENU_STRING_5_Y, "√–¿Õ»÷€:", MENU_STRING_SIZE, textYposition);
		glLineWidth(2);
		menuAccentString(MENU_STRING_7_Y, "¡≈« √–¿Õ»÷", 0.4, textYposition - 9, !border);
		menuAccentString(MENU_STRING_7_Y, "— √–¿Õ»÷¿Ã»", 0.4, textYposition + 9, border);

		getTestScore(multSpeed, multField, 1);
		menuColoredString(MENU_STRING_8_Y, multScoreString, 0.4, textYposition - 9, !border);
		getTestScore(multSpeed, multField, 1.5);
		menuColoredString(MENU_STRING_8_Y, multScoreString, 0.4, textYposition + 9, border);
		glLineWidth(3);
		break;
	case 4:


		glColor3f(backgroundRed - dlColor(0.1), backgroundGreen - dlColor(0.1), backgroundBlue - dlColor(0.1));
		glLineWidth(2);
		aligWriteCenter(MENU_STRING_1_Y, "»√–” –¿«–¿¡Œ“¿À", 0.5, textYposition);
		aligWriteCenter(MENU_STRING_2_Y, "—“”ƒ≈Õ“ ŒÃ√“”", 0.5, textYposition);
		aligWriteCenter(MENU_STRING_3_Y, "‘¿ ”À‹“≈“¿ ‘»“» —", 0.5, textYposition);
		aligWriteCenter(MENU_STRING_4_Y, "√–”œœ€ ÃŒ-171", 0.5, textYposition);
		glColor3f(backgroundRed - dlColor(0.15), backgroundGreen - dlColor(0.05), backgroundBlue);
		aligWriteCenter(MENU_STRING_6_Y, "Œ√”–ÕŒ… Õ» »“¿", MENU_STRING_SIZE, textYposition);
		glColor3f(backgroundRed - dlColor(0.1), backgroundGreen - dlColor(0.1), backgroundBlue - dlColor(0.1));
		aligWriteCenter(MENU_STRING_8_Y, "ŒÃ— , 2018 √.", 0.5, textYposition);
		glLineWidth(3);
		glColor3f(textrgb, textrgb, textrgb);
		aligWriteCenter(MENU_STRING_12_Y, "¬€’Œƒ", MENU_STRING_SIZE, textYposition);
		break;
	}



	glFlush();
}

