#pragma once

#include<algorithm>
using namespace std;

#define MaxMapX 64
#define MaxMapY 40
#define TileSize 16
#define OpenListNum 1000
enum Tile
{
	ROAD = 0,
	BLOCK = 200
};

extern int Map[MaxMapY][MaxMapX];



class AStar
{
public :
	struct Node
	{
		Node *Parent;		//�θ��� ���
		int X, Y;			//��ǥ
		int F;				//���� Ÿ�ϱ����� �� �̵� ���
		int G;				//��������� ����������� �̵� ���
		int H;				//���� ���κ��� ���� ������������ ���� �̵����
	};
	struct SearchNode
	{
		int F;
		Node *p;
	};

	AStar (void)
	{
		memset (Map, 0, sizeof (Map));
	}
	~AStar (void)
	{

	}

	void GetMapTileSet (int PosX, int PosY, Tile Name = BLOCK);

	Node *PathFind (int StartX, int StartY, int EndX, int EndY,bool First);

private:
	//��� ���� �˻��Լ���

	int End_X;
	int End_Y;
	int Start_X;
	int Start_Y;
	
	//OpenList���� �ּ� F���� ã�Ƽ� ��带 ��ȯ���ش�.
	Node *Fmin (void);
	
	//OpenList�� ������� Ȯ��
	bool empty (void);
	
	//OpenList���� �����.
	void erase (Node *search);

	//Flag true�� OpenList false�� CloseList
	bool Insert_OpenList (Node *p);
	
	//ù��°�� �ι�° ���ڷ� x,y��ǥ ����° ���ڷ� false�� close, true�� OpenList
	Node *Search (int X,int Y, bool List);

	//�θ����ڸ� �޾Ƽ� ��� ����
	void MakeNode (Node *Parent, int MovePay ,int X,int Y);
};

extern AStar::SearchNode OpenList[OpenListNum];

extern AStar::Node *CloseList[MaxMapY][MaxMapX];