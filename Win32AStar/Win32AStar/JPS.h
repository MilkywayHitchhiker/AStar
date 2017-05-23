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
		Node *Parent;		//부모의 노드
		int X, Y;			//좌표
		int F;				//현재 타일까지의 총 이동 비용
		int G;				//출발점부터 현재노드까지의 이동 비용
		int H;				//현재 노드로부터 최종 목적지까지의 예상 이동비용
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

	//multiset 사용 F값 비교를 위한 괄호 연산자 오버로딩
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


	//맵을 동적할당으로 생성
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


	//생성자에서는 인자로 맵의 높이와 너비를 받아서 동적할당해서 맵을 생성한다.
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

	//생성자에서 동적할당 받은 맵을 파괴자에서 파괴한다.
	~JPS (void)
	{
		for ( int cnt = 0; cnt < _MaxY; cnt++ )
		{
			delete[]Map[cnt];
		}
		delete[]Map;
	}


	//길찾기함수
	Node *PathFind (int StartX, int StartY, int EndX, int EndY, bool First);

	//갈수 있는 길과 없는 길 체크
	JPS::Node *Jump (int StartX, int StartY, int EndX, int EndY, bool First);

	//타일에 블록이 있는지 확인후 갈 수 있는 길인지 return 해줌.
	bool TileSearch (int X, int Y);

	void MapTileSet (int X, int Y, Tile Name);
	void MapColorSet (int X,int Y,COLORREF Color);


	//노드 생성
	void MakeNode (Node *Parent, int MovePay, int X, int Y, Move iDir);

	//직선체크
	bool MoveStright (int X, int Y, Move iDir, int *JumpX, int *JumpY);
	//대각선 체크
	bool Movediagonal (int X, int Y, Move iDir, int *JumpX, int *JumpY);


	//OpenList전체삭제
	void OpenList_Delete (void);
	//CloseList전체삭제
	void CloseList_Delete (void);

};