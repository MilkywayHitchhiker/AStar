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
		Node *Parent;		//부모의 노드
		int X, Y;			//좌표
		int F;				//현재 타일까지의 총 이동 비용
		int G;				//출발점부터 현재노드까지의 이동 비용
		int H;				//현재 노드로부터 최종 목적지까지의 예상 이동비용
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
	//노드 관련 검색함수들

	int End_X;
	int End_Y;
	int Start_X;
	int Start_Y;
	
	//OpenList에서 최소 F값을 찾아서 노드를 반환해준다.
	Node *Fmin (void);
	
	//OpenList가 비었는지 확인
	bool empty (void);
	
	//OpenList에서 지운다.
	void erase (Node *search);

	//Flag true는 OpenList false는 CloseList
	bool Insert_OpenList (Node *p);
	
	//첫번째와 두번째 인자로 x,y좌표 세번째 인자로 false는 close, true는 OpenList
	Node *Search (int X,int Y, bool List);

	//부모인자를 받아서 노드 생성
	void MakeNode (Node *Parent, int MovePay ,int X,int Y);
};

extern AStar::SearchNode OpenList[OpenListNum];

extern AStar::Node *CloseList[MaxMapY][MaxMapX];