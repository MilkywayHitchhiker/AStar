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
	//ó�� ������ ��� ���¸���Ʈ�� ������ �ϳ� �ְ� ����.
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
		NewNode->H = 10 * (abs (StartX - EndX) + abs (StartY - EndY));	//����ź ������
		NewNode->F = NewNode->G + NewNode->H;

		NewNode->Parent = NULL;
		NewNode->X = StartX;
		NewNode->Y = StartY;

		OpenList[0].F = NewNode->F;
		OpenList[0].p = NewNode;

	}

	//���ѹݺ��� ���鼭 �� ã�� ����.
//	while ( 1 )
	{
		//OpenList�� ������� �ٷ� ����.
		if (true == empty() )
		{
			return nullptr;
		}

		//OpenList���� F�� �ּҰ��� ��� ����.
		Node *p = Fmin();
		erase (p);
		CloseList[p->Y][p->X] = p;


		//�������� ���� �ߴ��� �ƴ��� üũ�ؼ� ���������� ����.
		if ( p->X == EndX && p->Y == EndY )
		{
			return p;
		}

		//���� �������� �������� �ʾ����� 8������ üũ�ؼ� �ٽ� ���¸���Ʈ�� ����.

		int X = p->X;
		int Y = p->Y;
		//12��
		MakeNode (p, 10, X, Y - 1);
		//3��
		MakeNode (p, 10, X + 1, Y);
		//6��
		MakeNode (p, 10, X, Y + 1);
		//9��
		MakeNode (p, 10, X - 1, Y);
	
		//2��
		MakeNode (p, 14, X + 1, Y - 1);
		//5��
		MakeNode (p, 14, X + 1, Y + 1);
		//8��
		MakeNode (p, 14, X - 1, Y + 1);
		//11��
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

//�θ����ڸ� �޾Ƽ� ��� ����
void AStar::MakeNode (Node *Parent,int MovePay, int X, int Y)
{
	//�� �ȹ������ üũ
	if ( X <0 || X >MaxMapX )
	{
		return;
	}
	if ( Y <0 || Y >MaxMapY )
	{
		return;
	}

	//���� �� �� �ִ� ������ �ƴ��� �Ǵ�.
	if ( Map[Y][X] == BLOCK )
	{
		return;
	}

	//��� ���� �� ����

	Node *NewNode = new Node;
	
	NewNode->Parent = Parent;
	NewNode->X = X;
	NewNode->Y = Y;

	NewNode->H = 10 * (abs (X - End_X) + abs (Y - End_Y));
	NewNode->G = Parent->G + MovePay;
	NewNode->F = NewNode->H + NewNode->G;

	//CLOSELIST�˻�
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

	//OpenList �˻�
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


	//���¸���Ʈ�� ����ϰ� return;
	Insert_OpenList (NewNode);
	return;
}


//Flag true�� OpenList false�� CloseList
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

//ù��°�� �ι�° ���ڷ� x,y��ǥ ����° ���ڷ� false�� close, true�� OpenList
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