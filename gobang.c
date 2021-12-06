#include <stdio.h>

int map[30][30] = {0};
int Last_x, Last_y;
int chess = -1;

int Legal_Chess(int x, int y)
{
	if ((x >= 0)&&(x < 30)&&(y >= 0)&&(y < 30)&&(map[x][y] == 0))
		return 1;
	else
		return 0;
}

void Put_Chess(int chess)
{
	int x, y;
	puts("请输入棋子的坐标：");
	scanf("%d%d", &x, &y);
	x--;
	y--;
	while (Legal_Chess(x, y) == 0)
	{
		puts("输入有误！请再次输入：");
		scanf("%d%d", &x, &y);
		x--;
		y--;
	}
	Last_x = x;
	Last_y = y;
	map[x][y] = chess;
}

//数字化棋盘
void print()
{
	int i, j;
	for (i=0; i<30; i++)
	{
		for (j=0; j<30; j++)
			printf("%-3d",map[i][j]);
		putchar('\n');
	}
}



//图像化棋盘
void Print()
{
	int i, j;
	printf("  ");
	for (i=1; i<=30; i++)
		printf("%2d", i);
	printf("\n%-2d", 1);
	i = 0;
	for (j=0; j<30; j++)
	{
		if (j == 0)
				switch (map[i][j])
				{
					case 0:printf(" ┌");break;
					case 1:printf("○ ");break;
					case -1:printf("● ");break;
				}
		else
			if (j == 29)
				switch (map[i][j])
				{
					case 0:printf("─┐");break;
					case 1:printf("○ ");break;
					case -1:printf("● ");break;
				}
			else
				switch (map[i][j])
				{
					case 0:printf("─┬");break;
					case 1:printf("○ ");break;
					case -1:printf("● ");break;
				}
	}
	putchar('\n');
	for (i=1; i<=28; i++)
	{
		printf("%-2d", i+1);
		for (j=0; j<30; j++)
			if (j == 0)
				switch (map[i][j])
				{
					case 0:printf(" ├");break;
					case 1:printf("○ ");break;
					case -1:printf("● ");break;
				}
			else
				if (j == 29)
					switch (map[i][j])
					{
						case 0:printf("─┤");break;
						case 1:printf("○ ");break;
						case -1:printf("● ");break;
					}
				else
					switch (map[i][j])
					{
						case 0:printf("─┼");break;
						case 1:printf("○ ");break;
						case -1:printf("● ");break;
					}
		putchar('\n');
	}
	printf("%-2d", 30);
	for (j=0; j<30; j++)
	{
		if (j == 0)
				switch (map[i][j])
				{
					case 0:printf(" └");break;
					case 1:printf("○ ");break;
					case -1:printf("● ");break;
				}
		else
			if (j == 29)
				switch (map[i][j])
				{
					case 0:printf("─┘");break;
					case 1:printf("○ ");break;
					case -1:printf("● ");break;
				}
			else
				switch (map[i][j])
				{
					case 0:printf("─┴");break;
					case 1:printf("○ ");break;
					case -1:printf("● ");break;
				}
	}
	putchar('\n');
}

int Game_Over()
{
	int x, y;

	//Direction_1 Up
	int Direction_1 = 0;
	x = Last_x;
	y = Last_y;
	while ((map[x][y] == chess)&&(x-- >= 0))
		Direction_1++;
	//Direction_1 Down
	x = Last_x;
	y = Last_y;
	while ((map[++x][y] == chess)&&(x <= 29))
		Direction_1++;
	if (Direction_1 == 5) return 1;

	//Direction_2 Left
	int Direction_2 = 0;
	x = Last_x;
	y = Last_y;
	while ((map[x][y] == chess)&&(y-- >= 0))
		Direction_2++;
	//Direction_2 Right
	x = Last_x;
	y = Last_y;
	while ((map[x][++y] == chess)&&(y <= 29))
		Direction_2++;
	if (Direction_2 == 5) return 1;

	//Direction_3 Up_Left
	int Direction_3 = 0;
	x = Last_x;
	y = Last_y;
	while ((map[x][y] == chess)&&(y-- >= 0)&&(x-- >= 0))
		Direction_3++;
	//Direction_3 Down_Right
	x = Last_x;
	y = Last_y;
	while ((map[++x][++y] == chess)&&(y <= 29)&&(x <= 29))
		Direction_3++;
	if (Direction_3 == 5) return 1;

	//Direction_4 Up_Right
	int Direction_4 = 0;
	x = Last_x;
	y = Last_y;
	while ((map[x][y] == chess)&&(y++ <= 29)&&(x-- >= 0))
		Direction_4++;
	//Direction_4 Down_Left
	x = Last_x;
	y = Last_y;
	while ((map[++x][--y] == chess)&&(y >= 0)&&(x <= 29))
		Direction_4++;
	if (Direction_4 == 5)
		return 1;
	else
		return 0;
}

void Game()
{
	puts("--------------------------------------------------------------");
	puts("-----------------游戏开始！黑棋先，白棋后。-------------------");
	puts("--------------------------------------------------------------");
	while (Game_Over() == 0)
	{
		Print();
		chess *= -1;
		if (chess)
			puts("请黑棋走");
		else
			puts("请白棋走");
		Put_Chess(chess);
		puts("--------------------------------------------------------------");
		puts("--------------------------------------------------------------");
	}
	puts("--------------------------------------------------------------");
	if (chess == 1)
		puts("--------------------游戏结束！黑棋胜！------------------------");
	else
		puts("--------------------游戏结束！白棋胜！------------------------");
	puts("--------------------------------------------------------------");
	puts("--------------------------------------------------------------");
	puts("--------------------------------------------------------------");
}

int main(int argc, const char *argv[])
{
	Game();
	return 0;
}
