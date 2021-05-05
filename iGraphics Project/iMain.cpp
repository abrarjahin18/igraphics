#include "iGraphics.h"
#include "bitmap_loader.h"
#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>

#define screenWidth 1410
#define screenHight 690
#define ENEMYNUMBER 3
#define g 9.8/100
#define G 9.8
#define PI 3.14159265
int xCircle = 120;
int yCircle = 150;
int radius = 80;
double qeta = 0;
int qeta2 = 180;
int xLine = xCircle + (radius + 30)*cos(qeta * PI / 180);
int yLine = yCircle + (radius + 30)*sin(qeta * PI / 180);


int xGranade = xCircle;
int yGranade = yCircle;

int v = 10;
int vx = v*cos(qeta*PI / 180);
int vy = v*sin(qeta*PI / 180);

int p;
int e;
int mposx, mposy;
int check = 0;
int curveCenterX = 250 + 100;
int curveCenterY = 100;
int diameter;
int screenRadius = 250;
int i = 0;
int iCheck = 0;
int timeChange = 1;
int timeMax;
int rMax;

int ar[100];
int ai = 0, si = 0; int ni;
char st[100][100];
char nhigh[100];

typedef struct dotLine{
	int x;
	int y;
}line[100];

int x = 300, y = 300, r = 80;
int win_b = 400, win_l = 400;
float delay = 0;
int speed = 0;

void drawRoadLines(int offset)
{
	int len = 10;
	int width = 5;
	int gap = 10;
	int midx = 0, midy = win_l / 2;
	for (int i = 0; i < win_b / (len + gap); i++)
		iFilledRectangle((i * (len + gap) + offset) % win_b, midy, len, width);
}

char background_pic[6][20] = { "Gerila\\1.bmp", "Gerila\\2.bmp", "Gerila\\3.bmp", "Gerila\\4.bmp", "Gerila\\5.bmp", "Gerila\\1.bmp" };
struct background{
	int x;
	int y;
}Gamebackground[6];

struct bcord{
	int x;
	int y;
}button1[5];

char back[20] = { "back.bmp" };
char menu[20] = { "Menu.bmp" };
char buttons[10][23] = { "Hmenu\\play.bmp", "Hmenu\\score.bmp", "Hmenu\\instructions.bmp", "Hmenu\\credit.bmp","Hmenu\\Exit.bmp" };
char inst[10] = { "ins.bmp" };
int gamestate = -1;

char pakistan_army[ENEMYNUMBER][20] = { "enemy\\enemy.bmp", "enemy\\enemy.bmp", "enemy\\enemy.bmp" };// , "enemy\\enemy1.bmp", "enemy\\enemy1.bmp" };// , "enemy\\enemy1.bmp", "enemy\\enemy1.bmp", "enemy\\enemy1.bmp", "enemy\\enemy1.bmp", "enemy\\enemy1.bmp" };

struct ENEMY
{
	int enemy_x;
	int enemy_y;
	int bullet_X;
	int bullet_Y;
	int enemyIndex;
	bool enemyShow;
	bool bulletShow;
}pakistani_enemy[ENEMYNUMBER];

struct ENEMY_officer
{
	int enemy_x = 800;
	int enemy_y = 30;
	int bullet_X = 800;
	int bullet_Y = 123;
	int officer_hp = 50;
	bool enemyShow = true;
	bool bulletShow = true;
}pakistani_enemy_officer;


int bulletTimer = 0;
int bulletCount = 0;
struct Bullet
{
	int x, y;
	bool hasShot;             // controls if bullet is visible or not
}bullet[1000];


int flag;
struct tank{

	int dx;
	int tankX = 1100;
	int tankY = 100;
	int tankbulletX = tankX;
	int tankbulletY = tankY + 92;
	int tank_hp = 200;
	bool tank_show = true;
}pak_tank;

struct helicopter{
	int helicopterX = 1000;
	int helicopterY = 600;
	int dx = 1, dy = 1;
	int heli_bulletX = 1000;
	int heli_bulletY = 650;
	int heli_hp = 250;
	bool heli_bulletshow;
	bool helicopter_show = true;
}pak_helicopter;

char secondsString[10];
char minutesString[10];
int startime;
int timeElapsed;
int Second;
int minute;
int startTime;
//FILE* fp;
void timeShow()//showing the time Elapsed Call  iDraw
{
	int endTime = clock();

	timeElapsed = (endTime - startTime) / CLOCKS_PER_SEC;
	Second = timeElapsed % 60;
	minute = timeElapsed / 60;
	itoa(minute, minutesString, 10);
	itoa(Second, secondsString, 10);
	iSetColor(65, 105, 225);
	iFilledRectangle(805, 640, 140, 40);
	iSetColor(255, 255, 255);

	iText(895, 650, ": ", GLUT_BITMAP_TIMES_ROMAN_24);

	iText(910, 650, secondsString, GLUT_BITMAP_TIMES_ROMAN_24);
	iText(880, 650, minutesString, GLUT_BITMAP_TIMES_ROMAN_24);
}

void enemyMovement()
{

	for (int i = 0; i < ENEMYNUMBER; i++)
	{
		if (pakistani_enemy[i].enemyShow == true)
		{
			iShowBMP2(pakistani_enemy[i].enemy_x, pakistani_enemy[i].enemy_y, pakistan_army[pakistani_enemy[i].enemyIndex],0);
			//fprintf(fp,"Enemy showing %d x %d enemyindex %d\n",i,pakistani_enemy[i].enemy_x,pakistani_enemy[i].enemyIndex);
			//fprintf(fp,"%s\n", secondsString);
		}
	}
}
struct gerilla{
	int gerillaXcord = 100;
	int gerillaYcord = 100;
	int health = 150;
	bool gerillaShow = true;
}montu_mia;
void BulletShot()              // Called from isettimer
{
	for (int i = 0; i < bulletCount; i++)
	{
		if (bullet[i].hasShot)
			bullet[i].x += 20;

		if (bullet[i].x > 1380)
			bullet[i].hasShot = false;
	}
}

void enemyMovement_officer()
{

	if (pakistani_enemy_officer.enemyShow == true)
	{
		iShowBMP2(pakistani_enemy_officer.enemy_x, pakistani_enemy_officer.enemy_y, "enemy2.bmp", 0);
	}

}

void kill_enemy_officer()
{
	for (int i = 0; i < bulletCount; i++)
	{
		if (bullet[i].hasShot == true && pakistani_enemy_officer.enemyShow == true)
		{
			if (bullet[i].x >= pakistani_enemy_officer.enemy_x && bullet[i].x <= pakistani_enemy_officer.enemy_x + 117)
			{
				if (bullet[i].y >= pakistani_enemy_officer.enemy_y&&bullet[i].y <= pakistani_enemy_officer.enemy_y + 150)
				{
					pakistani_enemy_officer.officer_hp -= 1;
					if (pakistani_enemy_officer.officer_hp <= 0)
					{
						pakistani_enemy_officer.enemyShow = false;
						bullet[i].hasShot = false;
						pakistani_enemy_officer.bulletShow = false;
					}
				}
			}
		}
	}
}

void bullet_show()
{
	for (int i = 0; i < bulletCount; i++)
	{
		if (bullet[i].hasShot == true)
			iShowBMP2(bullet[i].x, bullet[i].y, "bullet1.bmp",0);
	}

}
int grenadecounter = 1;
int enemyCounter = 1;

void kill_enemy()
{
	for (int i = 0; i < bulletCount; i++)
	{
		for (int j = 0; j < ENEMYNUMBER; j++)
		{
			if (bullet[i].hasShot == true && pakistani_enemy[j].enemyShow == true)
			{
				if (bullet[i].x >= pakistani_enemy[j].enemy_x)// && bullet[i].x <= pakistani_enemy[j].enemy_x + 117)
				{
					if (bullet[i].y >= pakistani_enemy[j].enemy_y&&bullet[i].y <= pakistani_enemy[j].enemy_y + 150)
					{
					pakistani_enemy[j].enemyShow = false;
					bullet[i].hasShot = false;
					pakistani_enemy[j].bulletShow = false;
					}
				}
			}
		}
	}
}
int battle_counter = 0;

void writeHighScore(char name[], int n)
{
	FILE *fp = fopen("F:\\hhh.txt", "a");

	if (fp != NULL)
	{
		fprintf(fp, "%s\n", name);
		fprintf(fp, "%d\n", n);
	}

	fclose(fp);

}

void readHighScore()
{
	FILE *fp = fopen("F:\\hhh.txt", "r");
	char str[100];

	if (fp != NULL)
	{
		while (fgets(str, 15, fp) != NULL)
		{
			//puts(str);
			strcpy(st[si++], str);
			puts(st[ai - 1]);
			fgets(str, 15, fp);
			ar[ai++] = atoi(str);
			printf("score = %d\n", ar[ai - 1]);
			iText(100, 200, str);
		}
	}

	fclose(fp);
}
void showHighScore()
{
	int temp;
	char t[100];

	for (int i = 0; i < ai - 1; i++)
	{
		for (int j = 0; j < ai - 1; j++)
		{
			if (ar[j] > ar[j + 1])
			{
				temp = ar[j];
				ar[j] = ar[j + 1];
				ar[j + 1] = temp;
				strcpy(t, st[j]);
				strcpy(st[j], st[j + 1]);
				strcpy(st[j + 1], t);
			}
		}
	}
	//printf("Shorted\n");
	//iClear();
	for (int k = 0, x = 0; k < 5; k++, x += 200)
	{
		//printf("%d %s\n", ar[k], st[k]);
		char str[100];
		itoa(ar[k], str, 10);
		iSetColor(255,0, 0);
		iText(50 + x, 100, str, GLUT_BITMAP_TIMES_ROMAN_24);
		iSetColor(255, 255, 0);
		iText(50+x, 0 + y, st[k], GLUT_BITMAP_TIMES_ROMAN_24);
	}
}

void showName()
{
	iText(100, 180, "Enter Your Name: ", GLUT_BITMAP_TIMES_ROMAN_24);
	iText(100, 150, nhigh);
}


void vanish_tank()
{
	for (int i = 0; i < bulletCount; i++)
	{
		if (bullet[i].hasShot == true && pak_tank.tank_show == true)
		{
			if (bullet[i].x >= pak_tank.tankX&&bullet[i].x <= pak_tank.tankX + 80)
			{
				pak_tank.tank_hp -= 1;
				if (pak_tank.tank_hp <= 0)
				{
					pak_tank.tank_show = false;
					bullet[i].hasShot = false;
				}
			}
		}
	}
}

void vanish_helicopter()
{
	for (int i = 0; i < bulletCount; i++)
	{
		if (bullet[i].hasShot == true && pak_helicopter.helicopter_show == true)
		{
			if (bullet[i].x >= pak_helicopter.helicopterX&&bullet[i].x <= pak_helicopter.helicopterX + 80)
			{
				if (bullet[i].y >= pak_helicopter.helicopterY&&bullet[i].y <= pak_helicopter.helicopterY + 80)
				{
					pak_helicopter.heli_hp -= 1;
					if (pak_helicopter.heli_hp <= 0)
					{
						pak_helicopter.helicopter_show = false;
						bullet[i].hasShot = false;
					}
				}
			}
		}
	}
}

void killEnemy_bomb()
{
	for (int j = 0; j < ENEMYNUMBER; j++)
	{
		if (4 - grenadecounter != 0)
		{
			if (yGranade + 27 >= pakistani_enemy[j].enemy_y)
			{
				if (xGranade + 27 >= pakistani_enemy[j].enemy_x)
				{
					pakistani_enemy[j].enemyShow = false;
					pakistani_enemy[j].bulletShow = false;
				}
			}
		}
	}
	if (4 - grenadecounter != 0)
	{
		if (xGranade + 27 >= pakistani_enemy_officer.enemy_x)
		{
			if (yGranade + 27 >= pakistani_enemy_officer.enemy_y)
			{
				pakistani_enemy_officer.officer_hp -= 2;
				if (pakistani_enemy_officer.officer_hp <= 0)
				{
					pakistani_enemy_officer.enemyShow = false;
					pakistani_enemy_officer.bulletShow = false;
				}
			}
		}
	}
}

void vanish_player()
{
	for (int i = 0; i < ENEMYNUMBER; i++)
	{
		if (pakistani_enemy[i].bulletShow && montu_mia.gerillaShow)
		{
			if (pakistani_enemy[i].bullet_X <= montu_mia.gerillaXcord)//&&pakistani_enemy[i].bullet_X>=montu_mia.gerillaXcord-40)
			{
				if (pakistani_enemy[i].bullet_Y >= montu_mia.gerillaYcord && 
					pakistani_enemy[i].bullet_Y <= montu_mia.gerillaYcord + 150) //if the bullet of the enemy is between the gerilla image height which is 150, then health will decrease.
				{
					montu_mia.health = montu_mia.health - 1;
					if (montu_mia.health <= 0)
					{
						montu_mia.gerillaShow = false;
						pakistani_enemy[i].bulletShow = false;
					}
				}
			}
		}
	}
	if (pakistani_enemy_officer.bulletShow && montu_mia.gerillaShow)
	{
		if (pakistani_enemy_officer.bullet_X <= montu_mia.gerillaXcord)//&&pakistani_enemy[i].bullet_X>=montu_mia.gerillaXcord-40)
		{
			if (pakistani_enemy_officer.bullet_Y >= montu_mia.gerillaYcord &&
				pakistani_enemy_officer.bullet_Y <= montu_mia.gerillaYcord + 150) //if the bullet of the enemy is between the gerilla image height which is 150, then health will decrease.
			{
				montu_mia.health = montu_mia.health - 1;
				if (montu_mia.health <= 0)
				{
					montu_mia.gerillaShow = false;
					pakistani_enemy_officer.bulletShow = false;
				}
			}
		}
	}
	if (battle_counter >= 15)
	{
		if (pak_tank.tankbulletX <= montu_mia.gerillaXcord)
		{
			if (pak_tank.tankbulletY >= montu_mia.gerillaYcord &&
				pak_tank.tankbulletY<= montu_mia.gerillaYcord + 150) //if the bullet of the enemy is between the gerilla image height which is 150, then health will decrease.
			{
				montu_mia.health = montu_mia.health - 3;
				if (montu_mia.health <= 0)
				{
					montu_mia.gerillaShow = false;
				}
			}
		}
	}
	if (pak_tank.tank_hp<=0)
	{
		if (pak_helicopter.heli_bulletX <= montu_mia.gerillaXcord)
		{
			if (pak_helicopter.heli_bulletY >= montu_mia.gerillaYcord &&
				pak_helicopter.heli_bulletY <= montu_mia.gerillaYcord + 150) //if the bullet of the enemy is between the gerilla image height which is 150, then health will decrease.
			{
				montu_mia.health = montu_mia.health - 5;
				if (montu_mia.health <= 0)
				{
					montu_mia.gerillaShow = false;
					pak_helicopter.heli_bulletshow = false;
				}
			}
		}
	}
}

char remaining_granade[10];
void remainingGranade_show()
{
	iSetColor(255, 255, 255);
	iText(100, 560, "Remaining Grenade:");
	itoa(4-grenadecounter, remaining_granade, 10);
	iSetColor(255, 0, 0);
	iText(300, 550, remaining_granade, GLUT_BITMAP_TIMES_ROMAN_24);
}
 
void iDraw()
{
	iClear();
	if (gamestate == -1){
		//menu
		iShowBMP(0, 0, menu);
		for (int i = 0; i<5; i++){
			iShowBMP2(button1[i].x, button1[i].y, buttons[i],0);
		}
		iShowBMP2(700, 100, "gerilla_name.bmp", 0);
	}
	else if (gamestate == 0)
	{
		for (int i = 0; i < 6; i++){
			iShowBMP(Gamebackground[i].x, Gamebackground[i].y, background_pic[i]);

		}
		timeShow();
		bullet_show();
		killEnemy_bomb();
		for (int i = 0; i < ENEMYNUMBER; i++)
		{
			if (pakistani_enemy[i].bulletShow)
				iShowBMP2(pakistani_enemy[i].bullet_X, pakistani_enemy[i].bullet_Y, "bullet.bmp",0);
		}

		enemyMovement_officer();

		if (pakistani_enemy_officer.bulletShow)
			iShowBMP2(pakistani_enemy_officer.bullet_X, pakistani_enemy_officer.bullet_Y, "bullet.bmp", 0);
		kill_enemy_officer();


		iSetColor(0, 0, 255);
		iText(100, 640, "player health");
		iSetColor(255, 0, 0);
		iFilledRectangle(100, 600, montu_mia.health * 2, 30);
		remainingGranade_show();

		enemyMovement();
		kill_enemy();
		if (battle_counter >= 15){
			vanish_tank();
		}
		if (pak_tank.tank_hp <= 0)
		{
			vanish_helicopter();
		}
		if (montu_mia.gerillaShow)
		{
			iShowBMP2(montu_mia.gerillaXcord, montu_mia.gerillaYcord, "gerilla.bmp",0);
			iSetColor(0, 0, 255);
			iLine(xCircle, yCircle, xLine, yLine);
			if (grenadecounter < 4)
			{
				iShowBMP2(xGranade, yGranade, "grenade.bmp",0);
			}
		}
		vanish_player();
		
		if (battle_counter == 15)
		{
			iSetColor(255, 255, 0);
			iText(600, 600, "level 2", GLUT_BITMAP_TIMES_ROMAN_24);
		}
		else
		{
			iSetColor(255, 255, 0);
			iText(600, 600, "level 1", GLUT_BITMAP_TIMES_ROMAN_24);
		}

		if (battle_counter == 15)
		{
			if (pak_tank.tank_show)
			{
				iShowBMP2(pak_tank.tankbulletX, pak_tank.tankbulletY, "tank shell.bmp", 0);
				iShowBMP2(pak_tank.tankX, pak_tank.tankY, "tank1.bmp", 0);
			}
		}
		if (pak_tank.tank_hp <= 0)
		{
			if (pak_helicopter.helicopter_show)
			{
				iShowBMP2(pak_helicopter.helicopterX, pak_helicopter.helicopterY, "helicopter.bmp", 0);
				iShowBMP2(pak_helicopter.heli_bulletX, pak_helicopter.heli_bulletY, "bullet.bmp", 0);
			}
		}
		if (montu_mia.health <= 0)
		{
			iShowBMP(0, 0, "You_lose.bmp");
		}
		if (pak_tank.tank_show == false && pak_helicopter.helicopter_show == false)
		{
			iShowBMP(0, 0, "You win.bmp");
			iShowBMP(500, 500, "congratulations.bmp");
			//showHighScore();
			showName();
		}
	}
	if (gamestate == 1){
		iShowBMP(0, 0, menu);
		showHighScore();
		//iShowBMP2(1200, 100, back, 0);
	}
	if (gamestate == 2){
		iShowBMP(0, 0, inst);
		//iShowBMP2(1200, 100, back, 0);
	}
	if (gamestate == 3){
		iShowBMP(0, 0, "credit.bmp");
		//iShowBMP2(1200, 100, back, 0);
	}
}

void reset()
{
	gamestate = -1;
	ai = 0; ni = 0; si = 0;
	timeElapsed = 0; nhigh[0] = '\0';
}
void tank_movement()
{
	if (pak_tank.tankX == 1100)
		flag = 1;
	else if (pak_tank.tankX == 400)
		flag = 0;
	if (pak_tank.tankX <= 1100 && pak_tank.tankX >= 400)
	{
		if (flag == 1)
		{
			pak_tank.dx = -10;
			pak_tank.tankX = pak_tank.tankX + pak_tank.dx;
		}
		else if (flag == 0){
			pak_tank.dx = 10;
			pak_tank.tankX = pak_tank.tankX + pak_tank.dx;
		}
	}
}

void tank_fire()
{
	pak_tank.tankbulletX -= 20;
	if (pak_tank.tankbulletX <= 20)
	{
		pak_tank.tankbulletX = pak_tank.tankX;
	}
}

void helicopter_move()
{
	if (pak_helicopter.helicopterY == 600)
		flag = 1;
	else if (pak_helicopter.helicopterY == 100)
		flag = 0;

	if (pak_helicopter.helicopterY <= 600 && pak_helicopter.helicopterY >= 100)
	{
		if (flag == 1)
		{
			pak_helicopter.dx = -10;
			pak_helicopter.dy = -10;
			pak_helicopter.helicopterX = pak_helicopter.helicopterX + pak_helicopter.dx;
			pak_helicopter.helicopterY = pak_helicopter.helicopterY + pak_helicopter.dy;
		}
		else if (flag == 0){
			pak_helicopter.dx = 10;
			pak_helicopter.dy = 10;
			pak_helicopter.helicopterX = pak_helicopter.helicopterX + pak_helicopter.dx;
			pak_helicopter.helicopterY = pak_helicopter.helicopterY + pak_helicopter.dy;
		}
	}
}

void helicopter_fire()
{
	pak_helicopter.heli_bulletX -= 50;
	if (pak_helicopter.heli_bulletX <= 0)
	{
		pak_helicopter.heli_bulletX = pak_helicopter.helicopterX;
		pak_helicopter.heli_bulletY = pak_helicopter.helicopterY;
	}
}

void iMouseMove(int mx, int my)
{

}

void iMouse(int button, int state, int mx, int my)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		for (int i = 0; i<5; i++){

			if (mx >= button1[i].x&&mx <= button1[i].x + 320 && my >= button1[i].y&&my <= button1[i].y + 100){
				gamestate = i;
			}
			if (i == 1){
				readHighScore();
			}
		}
		if (gamestate == 4)
		{
			exit(0);
		}
		/*if (mx >= 1200 && mx <= 1380 && my >= 100 && my <= 180)
		{
			gamestate = -1;
		}*/
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		double a = mx - 100;
		double b = my - 100;

		qeta = atan2(b, a) * 180 / PI;

		xLine = xCircle + (radius + 30)*cos(qeta * PI / 180);
		yLine = yCircle + (radius + 30)*sin(qeta * PI / 180);
		vx = v*cos(qeta*PI / 180);
		vy = v*sin(qeta*PI / 180);

		diameter = curveCenterX + (screenRadius)*cos(qeta * PI / 180);
		diameter = diameter / 2;

		x -= 10;
		y -= 10;
	}
}

void iPassiveMouseMove(int mx, int my)
{
	mposx = mx;
	mposy = my;
	if (mx == 2){}
	else if (my == 2){}
}


void iKeyboard(unsigned char key)
{
	if (key == 'l')
	{
		if (bulletCount < 1000)
			bulletCount++;
		else if (bulletCount == 1000)
			bulletCount = 0;

		bullet[bulletCount - 1].hasShot = true;
		bullet[bulletCount - 1].y = montu_mia.gerillaYcord + 86;
		bullet[bulletCount - 1].x = montu_mia.gerillaXcord + 20;
	}
	if (key == ' ')
	{
		if (grenadecounter <= 3)
		{
			check = 1;
			grenadecounter++;
		}
	}
	else if (key == 'd')
	{
		if (montu_mia.gerillaXcord <= screenWidth - 100)
		{

			montu_mia.gerillaXcord += 10;
			xGranade += 10;
			xLine += 10;
			xCircle += 10;
		}
	}
	else if (key == 'a')
	{
		if (montu_mia.gerillaXcord >= 20)
		{

			montu_mia.gerillaXcord -= 10;
			xGranade -= 10;
			xLine -= 10;
			xCircle -= 10;
		}
	}
	else if (key == 'w')
	{
		if (montu_mia.gerillaYcord <= screenHight- 150)
		{
			montu_mia.gerillaYcord += 10;
			yGranade += 10;
			yLine += 10;
			yCircle += 10;
		}
	}
	else if (key == 's')
	{
		if (montu_mia.gerillaYcord >= 20)
		{
			montu_mia.gerillaYcord -= 10;
			yGranade -= 10;
			yLine -= 10;
			yCircle -= 10;
		}
	}
	if (pak_tank.tank_show == false && pak_helicopter.helicopter_show == false)
	{
		char chr;
		if ((chr = key) != '\r')
		{
		nhigh[ni++] = chr;
		nhigh[ni] = '\0';
		if (chr == '\b' && ni >= 2)
		{
		ni--;
		nhigh[--ni] = '\0';
		}
		}
		if (key == '\r')
		{
		writeHighScore(nhigh, timeElapsed);
		showHighScore();
		reset();
		//gamestate = 0;
		}
	}
	else if (key == 'w' && speed < 5)
		speed += 1;
	else if (key == 's' && speed > 0)
		speed -= 1;
}

void iSpecialKeyboard(unsigned char key)
{
	if (key == GLUT_KEY_UP)
	{
		if (qeta <= 80 && check == 0)
		{
			qeta += 5;
			xLine = xCircle + (radius + 30)*cos(qeta * PI / 180);
			yLine = yCircle + (radius + 30)*sin(qeta * PI / 180);
			vx = v*cos(qeta*PI / 180);
			vy = v*sin(qeta*PI / 180);
			timeMax = v*sin(qeta*PI / 180) / g;
			rMax = v*v*sin(2 * qeta*PI / 180) / g;
		}
	}
	if (key == GLUT_KEY_DOWN)
	{
		if (qeta >= 0 && check == 0)
		{
			qeta -= 5;
			xLine = xCircle + (radius + 30)*cos(qeta * PI / 180);
			yLine = yCircle + (radius + 30)*sin(qeta * PI / 180);
			vx = v*cos(qeta*PI / 180);
			vy = v*sin(qeta*PI / 180);
			timeMax = v*sin(qeta*PI / 180) / g;
			rMax = v*v*sin(2 * qeta*PI / 180) / g;
		}
	}
	if (key == GLUT_KEY_RIGHT)
	{
		if (qeta > 0 && check == 0)
		{
			qeta -= 5;
			xLine = xCircle + (radius + 30)*cos(qeta * PI / 180);
			yLine = yCircle + (radius + 30)*sin(qeta * PI / 180);
			vx = v*cos(qeta*PI / 180);
			vy = v*sin(qeta*PI / 180);
			timeMax = v*sin(qeta*PI / 180) / g;
			rMax = v*v*sin(2 * qeta*PI / 180) / g;
		}
	}
	if (key == GLUT_KEY_LEFT)
	{
		if (qeta <= 80 && check == 0)
		{
			qeta += 5;
			xLine = xCircle + (radius + 30)*cos(qeta * PI / 180);
			yLine = yCircle + (radius + 30)*sin(qeta * PI / 180);
			vx = v*cos(qeta*PI / 180);
			vy = v*sin(qeta*PI / 180);
			timeMax = v*sin(qeta*PI / 180) / g;
			rMax = v*v*sin(2 * qeta*PI / 180) / g;
		}
	}
}

void change_background(){
	for (int i = 0; i < 6; i++){
		Gamebackground[i].x = Gamebackground[i].x - 307;

		if (Gamebackground[i].x < 0){

			Gamebackground[i].x = 1535;
		}
	}
}

void EnemyBulletFiring_officer()
{

	pakistani_enemy_officer.bullet_X -= 15;
	if (pakistani_enemy_officer.bullet_X <= 0)
	{
		pakistani_enemy_officer.bullet_X = pakistani_enemy_officer.enemy_x;
	}
	if (battle_counter == 15)
	{
		pakistani_enemy_officer.bulletShow = false;
		pakistani_enemy_officer.enemyShow = false;
	}
}

void change_grenade()
{
	if (check == 1)
	{
		xGranade = (v*cos(qeta*PI / 180)*timeChange) + xCircle;
		yGranade = (xGranade*tan(qeta*PI / 180) - 0.5*g*xGranade*xGranade / (v*cos(qeta*PI / 180)) / (v*cos(qeta*PI / 180))) + yCircle;

		if (timeChange != timeMax)
			timeChange++;

		if (yGranade < 50)
		{
			timeChange = 1;
			timeMax = 0;
			check = 0;
			xGranade = xCircle;
			yGranade = yCircle;
		}
	}
}

void change_enemy()
{
	for (int i = 0; i < ENEMYNUMBER; i++)
	{
		pakistani_enemy[i].enemy_x -= 10;
		if (pakistani_enemy[i].enemy_x <= 0)
		{
			battle_counter++;
			if (i == 0) //first enemy
			{
				pakistani_enemy[i].enemy_x = screenWidth + (150 + rand() % 150);
				if (battle_counter == 13) //3n-2
				{
					//1st enemy reaches to left
					pakistani_enemy[0].enemy_x = 2500;
					pakistani_enemy[0].enemyShow = true;
					pakistani_enemy[0].bulletShow = true;
				}
			}
			else //second & 3rd enemy
			{
				pakistani_enemy[i].enemy_x = pakistani_enemy[i - 1].enemy_x + (150 + rand() % 150);

				if (battle_counter == 15) //3n
				{
					//second enemy reaches to left
					pakistani_enemy[1].enemy_x = 2500;
					pakistani_enemy[1].enemyShow = true;
					pakistani_enemy[1].bulletShow = true;

					if (i == ENEMYNUMBER - 1) //third enemy reaches to left side
					{
						iPauseTimer(2);
						pakistani_enemy[2].enemy_x = 2500;
						pakistani_enemy[2].enemyShow = true;
						pakistani_enemy[2].bulletShow = true;
					}
				}
				else
				{
					pakistani_enemy[0].enemyShow = true;
					pakistani_enemy[0].bulletShow = true;
					pakistani_enemy[1].enemyShow = true;
					pakistani_enemy[1].bulletShow = true;
					pakistani_enemy[2].enemyShow = true;
					pakistani_enemy[2].bulletShow = true;
				}
			}
		}
		pakistani_enemy[i].enemyIndex++;
		if (pakistani_enemy[i].enemyIndex >= ENEMYNUMBER)
		{
			pakistani_enemy[i].enemyIndex = 0;
		}
	}
}

void EnemyBulletFiring()
{
	for (int i = 0; i < ENEMYNUMBER; i++)
	{
		pakistani_enemy[i].bullet_X -= 15;
		if (pakistani_enemy[i].bullet_X <= 0)
		{
			pakistani_enemy[i].bullet_X = pakistani_enemy[i].enemy_x;
			if (battle_counter == 15)
			{
				iPauseTimer(3);
				pakistani_enemy[i].bullet_X = 10000;
			}
		}

	}
}

void setEnemyVariables()
{
	for (int i = 0; i < ENEMYNUMBER; i++)
	{
		if (i == 0)
		{
			pakistani_enemy[i].enemy_x = screenWidth + (150 + rand() % 150);
			pakistani_enemy[i].bullet_X = pakistani_enemy[i].enemy_x;
			pakistani_enemy[i].enemy_y = (150 + rand() % 150);
			pakistani_enemy[i].bullet_Y = pakistani_enemy[i].enemy_y + 115;
			pakistani_enemy[i].enemyIndex = 0;
			pakistani_enemy[i].enemyShow = true;
			pakistani_enemy[i].bulletShow = true;
		}
		else
		{
			pakistani_enemy[i].enemy_x = pakistani_enemy[i - 1].enemy_x + (150 + rand() % 150);
			pakistani_enemy[i].bullet_X = pakistani_enemy[i].enemy_x;
			pakistani_enemy[i].enemy_y = (150 + rand() % 150);
			pakistani_enemy[i].bullet_Y = pakistani_enemy[i].enemy_y + 115;
			pakistani_enemy[i].enemyIndex = 0;
			pakistani_enemy[i].enemyShow = true;
			pakistani_enemy[i].bulletShow = true;
		}
	}
}

void setvariables_background(){
	int sum = 1;
	for (int i = 0; i < 6; i++)
	{
		Gamebackground[i].y = 0;
		Gamebackground[i].x = sum;
		sum += 307;
	}
}

void setvariables_homemenu()
{
	int sum = 100;
	for (int i = 4; i >= 0; i--){
		button1[i].x = 100;
		button1[i].y = sum;
		sum += 100;
	}
}

int main()
{
	//fp = fopen("debug.txt","w");
	setvariables_homemenu();
	srand(time(NULL));
	startTime = clock();
	bulletTimer = iSetTimer(20, BulletShot);
	iResumeTimer(bulletTimer);

	p = iSetTimer(1, change_grenade);

	setEnemyVariables();
	e = iSetTimer(5, change_enemy);
	iSetTimer(3, EnemyBulletFiring);

	setvariables_background();
	iSetTimer(800, change_background);

	iSetTimer(50, tank_movement);
	iSetTimer(10, tank_fire);

	iSetTimer(25, helicopter_move);
	iSetTimer(15, helicopter_fire);

	iSetTimer(25, EnemyBulletFiring_officer);

	iInitialize(screenWidth, screenHight, "Gerilla 71");
	//fclose(fp);
	return 0;
}