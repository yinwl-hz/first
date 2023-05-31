#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<Windows.h>
#include<conio.h>

#define Wide 40
#define Long 20
//设置地图的范围：宽，高
//初始化蛇，初始化食物
//将蛇和食物显示在屏幕上
//蛇的移动，wasd
/*
	蛇碰到墙
	蛇碰到蛇的身体
	蛇碰到障碍物
	蛇碰到食物
*/
//蛇的死亡
typedef struct body
{
	int x;
	int y;
}Body;
typedef struct snake
{
	Body list[Wide * Long]; //身体
	int size;	//身体大小
	Body food;	//食物
	COORD coord;//设置光标位置
	int dx;	//蛇x轴的移动方向
	int dy;	//蛇y轴的移动方向
	int sorce;	//分数
	//上一次尾巴位置
	Body tail;
}Snake;

void init_wall()
{
	for (int i = 0; i <= Long; i++)
	{
		for (int j = 0; j <= Wide; j++)
		{
			if (i == Long || j == Wide)
			{
				printf("+");
			}
			else
			{
				printf(" ");
			}
		}
		printf("\n");
	}
}

void init_food(Snake* snake)
{
	//设置随机数
	srand(time(NULL));//设置随机数种子
	snake->food.x = rand() % Wide;
	snake->food.y = rand() % Long;
}

void init_snake(Snake* snake)
{
	//初始化蛇头坐标
	snake->list[0].x = Wide / 2;
	snake->list[0].y = Long / 2;
	//初始化蛇尾坐标
	snake->list[1].x = Wide / 2 + 1;
	snake->list[1].y = Long / 2;
	//初始化蛇的身体大小
	snake->size = 2;
	//初始化食物坐标
	init_food(snake);
	//初始化蛇的移动方向
	snake->dx = 1;
	snake->dy = 0;
	//初始化分数
	snake->sorce = 0;
	//snake->tail.x = 0;
	//snake->tail.y = 0;
}

void show_ui(Snake* snake)
{
	//SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	//显示蛇，设置位置
	for (int i = 0; i < snake->size; i++)
	{
		snake->coord.X = snake->list[i].x;
		snake->coord.Y = snake->list[i].y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);
		if (0 == i)
		{
			printf("@");
		}
		else
		{
			printf("*");
		}
	}
	//显示食物
	snake->coord.X = snake->food.x;
	snake->coord.Y = snake->food.y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);
	printf("&");
	//将原来尾巴的位置置空
	snake->coord.X = snake->tail.x;
	snake->coord.Y = snake->tail.y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);
	printf(" ");
}

void move_snake(Snake* snake)
{
	snake->tail.x = snake->list[snake->size - 1].x;
	snake->tail.y = snake->list[snake->size - 1].y;
	for (int i = snake->size - 1; i > 0; i--)
	{
		snake->list[i].x = snake->list[i - 1].x;
		snake->list[i].y = snake->list[i - 1].y;
	}
	//更新蛇头
	snake->list[0].x += snake->dx;
	snake->list[0].y += snake->dy;
}

void game_end(Snake* snake)
{
	snake->coord.X = 25;
	snake->coord.Y = 25;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);
	printf("游戏结束 得分为：%d\n", snake->sorce);
	Sleep(300);
	exit(0);
}

void control_snake(Snake* snake)
{
	char key = 0;
	while (_kbhit())
	{
		key = _getch();
	}
	switch (key)
	{
	case 'a':
		snake->dx = -1;
		snake->dy = 0;
		break;
	case 'd':
		snake->dx = 1;
		snake->dy = 0;
		break;
	case 'w':
		snake->dx = 0;
		snake->dy = -1;
		break;
	case 's':
		snake->dx = 0;
		snake->dy = 1;
		break;
	}
}

void snake_eat_body(Snake* snake)
{
	//蛇头和任一身体坐标相同
	for (int i = 1; i < snake->size; i++)
	{
		if (snake->list[0].x == snake->list[i].x && snake->list[0].y == snake->list[i].y)
		{
			game_end(snake);
		}
	}
}

void snake_eat_food(Snake* snake)
{
	if (snake->list[0].x == snake->food.x && snake->list[0].y == snake->food.y)
	{
		//原食物消失,蛇身体增长，分数增加，生成新食物
		init_food(snake);
		snake->size++;
		snake->sorce += 10;
	}
}

void start_game(Snake* snake)
{
	while (snake->list[0].x < 40 && snake->list[0].x > 0
		&& snake->list[0].y < 20 && snake->list[0].y > 0)
	{
		//获取到鼠标输入值
		control_snake(snake);
		//更新蛇的坐标
		move_snake(snake);
		//蛇移动
		//system("cls");
		show_ui(snake);
		//蛇是否碰到自己：双重for循环；除了蛇头要去覆盖蛇身，其他不管
		snake_eat_body(snake);
		//蛇是否碰到食物，如果碰到食物则消失，产生新食物：判断蛇头坐标是否和食物坐标重叠即可
		snake_eat_food(snake);
		Sleep(200);
	}
	game_end(snake);
}

void hide_cur()
{
	//隐藏控制台光标
	CONSOLE_CURSOR_INFO  cci;
	cci.dwSize = sizeof(cci);
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
}

int main()
{
	hide_cur();
	init_wall();
	Snake* snake = (Snake*)malloc(sizeof(Snake));
	//初始化蛇
	init_snake(snake);
	//显示蛇
	show_ui(snake);
	//开始游戏
	start_game(snake);
	system("pause");
	return 0;
}