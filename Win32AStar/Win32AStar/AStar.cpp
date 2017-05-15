#include"stdafx.h"
#include"AStar.h"



int Map[MaxMapY][MaxMapX];


AStar::SearchNode OpenList[OpenListNum];
AStar::Node *CloseList[MaxMapY][MaxMapX];

void AStar::GetMapTileSet (int PosX, int PosY, Tile Name)
{
	int TileNumX;
	int TileNumY;


	TileNumX = PosX / TileSize;
	TileNumY = PosY / TileSize;

	if ( TileNumX < 0 || TileNumX > MaxMapX )
	{
		return;
	}
	if ( TileNumY < 0 || TileNumY > MaxMapY )
	{
		return;
	}


	switch ( Name )
	{
	case ROAD:
		Map[TileNumY][TileNumX] = ROAD;
		break;
	case BLOCK:
		Map[TileNumY][TileNumX] = BLOCK;
		break;
	}
	return;
}

AStar::Node *AStar::PathFind (int StartX, int StartY, int EndX, int EndY, bool First)
{
	//처음 시작할 경우 오픈리스트에 시작점 하나 넣고 시작.
	if (true == First )
	{

		memset (OpenList, NULL, sizeof (OpenList));
		memset (CloseList, NULL, sizeof (CloseList));

		Start_X = StartX;
		Start_Y = StartY;
		End_X = EndX;
		End_Y = EndY;


		Node *NewNode = new Node;

		NewNode->G = 0;
		NewNode->H = 10 * (abs (StartX - EndX) + abs (StartY - EndY));	//멘하탄 방정식
		NewNode->F = NewNode->G + NewNode->H;

		NewNode->Parent = NULL;
		NewNode->X = StartX;
		NewNode->Y = StartY;

		OpenList[0].F = NewNode->F;
		OpenList[0].p = NewNode;

	}

	//무한반복을 돌면서 길 찾기 시작.
//	while ( 1 )
	{
		//OpenList가 비었으면 바로 종료.
		if (true == empty() )
		{
			return nullptr;
		}

		//OpenList에서 F가 최소값인 노드 뽑음.
		Node *p = Fmin();
		erase (p);
		CloseList[p->Y][p->X] = p;


		//목적지에 도달 했는지 아닌지 체크해서 도달했으면 종료.
		if ( p->X == EndX && p->Y == EndY )
		{
			return p;
		}

		//아직 목적지에 도달하지 않았으면 8방향을 체크해서 다시 오픈리스트에 저장.

		int X = p->X;
		int Y = p->Y;
		//12시
		MakeNode (p, 10, X, Y - 1);
		//3시
		MakeNode (p, 10, X + 1, Y);
		//6시
		MakeNode (p, 10, X, Y + 1);
		//9시
		MakeNode (p, 10, X - 1, Y);
	
		//2시
		MakeNode (p, 14, X + 1, Y - 1);
		//5시
		MakeNode (p, 14, X + 1, Y + 1);
		//8시
		MakeNode (p, 14, X - 1, Y + 1);
		//11시
		MakeNode (p, 14, X - 1, Y - 1);


	}

	return nullptr;

}

AStar::Node *AStar::Fmin (void)
{
	int Num = 999999;
	int chk;

	for ( int cnt = 0; cnt < OpenListNum; cnt++ )
	{
		if ( (OpenList[cnt].F < Num) && (OpenList[cnt].F != NULL) )
		{
			Num = OpenList[cnt].F;
			chk = cnt;
		}
	}
	return OpenList[chk].p;
}

bool AStar::empty (void)
{
	for ( int cnt = 0; cnt < OpenListNum; cnt++ )
	{
		if ( OpenList[cnt].F != NULL )
		{
			return false;
		}
	}
	return true;
}
void AStar::erase (Node *search)
{
	for ( int cnt = 0; cnt < OpenListNum; cnt++ )
	{
		if ( OpenList[cnt].p == search )
		{
			OpenList[cnt].F = NULL;
			OpenList[cnt].p = NULL;
			return;
		}
	}
	return;
}

//부모인자를 받아서 노드 생성
void AStar::MakeNode (Node *Parent,int MovePay, int X, int Y)
{
	//맵 안벗어나도록 체크
	if ( X <0 || X >MaxMapX )
	{
		return;
	}
	if ( Y <0 || Y >MaxMapY )
	{
		return;
	}

	//맵의 갈 수 있는 곳인지 아닌지 판단.
	if ( Map[Y][X] == BLOCK )
	{
		return;
	}

	//노드 생성 및 셋팅

	Node *NewNode = new Node;
	
	NewNode->Parent = Parent;
	NewNode->X = X;
	NewNode->Y = Y;

	NewNode->H = 10 * (abs (X - End_X) + abs (Y - End_Y));
	NewNode->G = Parent->G + MovePay;
	NewNode->F = NewNode->H + NewNode->G;

	//CLOSELIST검색
	Node *p = Search (X, Y, false);
	if ( p != NULL )
	{
		if ( p->F > NewNode->F )
		{
			p->F = NewNode->F;
			p->G = NewNode->G;
			p->H = NewNode->H;
			p->Parent = NewNode->Parent;
		}
		delete NewNode;
		return;
	}

	//OpenList 검색
	p = Search (X, Y, true);
	if ( p != NULL )
	{
		if ( p->F > NewNode->F )
		{
			p->F = NewNode->F;
			p->G = NewNode->G;
			p->H = NewNode->H;
			p->Parent = NewNode->Parent;
		}
		delete NewNode;
		return;
	}


	//오픈리스트에 등록하고 return;
	Insert_OpenList (NewNode);
	return;
}


//Flag true는 OpenList false는 CloseList
bool AStar::Insert_OpenList (Node *p)
{
	for ( int cnt = 0; cnt < OpenListNum; cnt++ )
	{
		if ( OpenList[cnt].F == NULL )
		{
			OpenList[cnt].F = p->F;
			OpenList[cnt].p = p;
			return true;
		}
	}
	return true;
}

//첫번째와 두번째 인자로 x,y좌표 세번째 인자로 false는 close, true는 OpenList
AStar::Node *AStar::Search (int X, int Y, bool List)
{
	if ( List == false )
	{
		if ( CloseList[Y][X] != NULL )
		{
			return CloseList[Y][X];
		}
		return NULL;
	}

	if ( List == true )
	{
		for ( int cnt = 0; cnt < OpenListNum; cnt++ )
		{
			if ( OpenList[cnt].F != NULL )
			{
				if ( OpenList[cnt].p->X == X && OpenList[cnt].p->Y == Y )
				{
					return OpenList[cnt].p;
				}
			}

		}
		return NULL;
	}
	return NULL;
}