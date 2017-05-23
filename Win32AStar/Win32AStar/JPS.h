#pragma once
#include<set>
using namespace std;
#include<time.h>


enum Tile
{
	ROAD = 0,
	BLOCK = 10
};

enum Move
{
	MOVE_UU=0,
	MOVE_UR,
	MOVE_RR,
	MOVE_RD,
	MOVE_DD,
	MOVE_LD,
	MOVE_LL,
	MOVE_UL,
	MOVE_First
};

class JPS
{
public :
	
	struct Node
	{
		Node *Parent;		//�θ��� ���
		int X, Y;			//��ǥ
		int F;				//���� Ÿ�ϱ����� �� �̵� ���
		int G;				//��������� ����������� �̵� ���
		int H;				//���� ���κ��� ���� ������������ ���� �̵����
		Move iDir;
	};
	
	struct Map_Tile
	{
		Map_Tile (void)
		{
			BLOCK = ROAD;
			rgb = RGB (255, 255, 255);
		}
		Tile BLOCK;
		COLORREF rgb;
	};

	//multiset ��� F�� �񱳸� ���� ��ȣ ������ �����ε�
	template<typename T> 
	struct NODE_COMPARE
	{
		bool operator() (T NodeA, T NodeB) const
		{
			return NodeA->F < NodeB->F;
		}
	};

	struct NODE_FINDER
	{
		NODE_FINDER (int iX, int iY) : iPosX (iX), iPosY (iY)
		{
		}

		bool operator() (Node *pNode) const
		{
			if ( pNode->X == iPosX && pNode->Y == iPosY )
			{
				return true;
			}
			return false;
		}
		int iPosX;
		int iPosY;
	};


	//���� �����Ҵ����� ����
	Map_Tile **Map;
	COLORREF TileColor;
	int _MaxX;
	int _MaxY;

	int _StartX;
	int _StartY;
	int _EndX;
	int _EndY;

	multiset<Node *, NODE_COMPARE<Node *>> OpenList;
	multiset<Node *> CloseList;

	struct location
	{
		int X;
		int Y;
	};


	//�����ڿ����� ���ڷ� ���� ���̿� �ʺ� �޾Ƽ� �����Ҵ��ؼ� ���� �����Ѵ�.
	JPS (int X, int Y)
	{
		_MaxX = X;
		_MaxY = Y;

		Map = new Map_Tile *[Y];
		for ( int cnt = 0; cnt < Y; cnt++ )
		{
			Map[cnt] = new Map_Tile[X];
		}
		srand ((unsigned int)time (NULL));
	}

	//�����ڿ��� �����Ҵ� ���� ���� �ı��ڿ��� �ı��Ѵ�.
	~JPS (void)
	{
		for ( int cnt = 0; cnt < _MaxY; cnt++ )
		{
			delete[]Map[cnt];
		}
		delete[]Map;
	}


	//��ã���Լ�
	Node *PathFind (int StartX, int StartY, int EndX, int EndY, bool First);

	//���� �ִ� ��� ���� �� üũ
	JPS::Node *Jump (int StartX, int StartY, int EndX, int EndY, bool First);

	//Ÿ�Ͽ� ����� �ִ��� Ȯ���� �� �� �ִ� ������ return ����.
	bool TileSearch (int X, int Y);

	void MapTileSet (int X, int Y, Tile Name);
	void MapColorSet (int X,int Y,COLORREF Color);


	//��� ����
	void MakeNode (Node *Parent, int MovePay, int X, int Y, Move iDir);

	//����üũ
	bool MoveStright (int X, int Y, Move iDir, int *JumpX, int *JumpY);
	//�밢�� üũ
	bool Movediagonal (int X, int Y, Move iDir, int *JumpX, int *JumpY);


	//OpenList��ü����
	void OpenList_Delete (void);
	//CloseList��ü����
	void CloseList_Delete (void);

};