#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<Windows.h>
#include<conio.h>

#define Wide 40
#define Long 20
//���õ�ͼ�ķ�Χ������
//��ʼ���ߣ���ʼ��ʳ��
//���ߺ�ʳ����ʾ����Ļ��
//�ߵ��ƶ���wasd
/*
	������ǽ
	�������ߵ�����
	�������ϰ���
	������ʳ��
*/
//�ߵ�����
typedef struct body
{
	int x;
	int y;
}Body;
typedef struct snake
{
	Body list[Wide * Long]; //����
	int size;	//�����С
	Body food;	//ʳ��
	COORD coord;//���ù��λ��
	int dx;	//��x����ƶ�����
	int dy;	//��y����ƶ�����
	int sorce;	//����
	//��һ��β��λ��
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
	//���������
	srand(time(NULL));//�������������
	snake->food.x = rand() % Wide;
	snake->food.y = rand() % Long;
}

void init_snake(Snake* snake)
{
	//��ʼ����ͷ����
	snake->list[0].x = Wide / 2;
	snake->list[0].y = Long / 2;
	//��ʼ����β����
	snake->list[1].x = Wide / 2 + 1;
	snake->list[1].y = Long / 2;
	//��ʼ���ߵ������С
	snake->size = 2;
	//��ʼ��ʳ������
	init_food(snake);
	//��ʼ���ߵ��ƶ�����
	snake->dx = 1;
	snake->dy = 0;
	//��ʼ������
	snake->sorce = 0;
	//snake->tail.x = 0;
	//snake->tail.y = 0;
}

void show_ui(Snake* snake)
{
	//SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	//��ʾ�ߣ�����λ��
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
	//��ʾʳ��
	snake->coord.X = snake->food.x;
	snake->coord.Y = snake->food.y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);
	printf("&");
	//��ԭ��β�͵�λ���ÿ�
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
	//������ͷ
	snake->list[0].x += snake->dx;
	snake->list[0].y += snake->dy;
}

void game_end(Snake* snake)
{
	snake->coord.X = 25;
	snake->coord.Y = 25;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);
	printf("��Ϸ���� �÷�Ϊ��%d\n", snake->sorce);
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
	//��ͷ����һ����������ͬ
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
		//ԭʳ����ʧ,�������������������ӣ�������ʳ��
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
		//��ȡ���������ֵ
		control_snake(snake);
		//�����ߵ�����
		move_snake(snake);
		//���ƶ�
		//system("cls");
		show_ui(snake);
		//���Ƿ������Լ���˫��forѭ����������ͷҪȥ����������������
		snake_eat_body(snake);
		//���Ƿ�����ʳ��������ʳ������ʧ��������ʳ��ж���ͷ�����Ƿ��ʳ�������ص�����
		snake_eat_food(snake);
		Sleep(200);
	}
	game_end(snake);
}

void hide_cur()
{
	//���ؿ���̨���
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
	//��ʼ����
	init_snake(snake);
	//��ʾ��
	show_ui(snake);
	//��ʼ��Ϸ
	start_game(snake);
	system("pause");
	return 0;
}