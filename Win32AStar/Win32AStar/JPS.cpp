#include"stdafx.h"
#include"JPS.h"
#include<algorithm>
#include<time.h>



bool JPS::TileSearch (int X, int Y)
{
	if ( Map[Y][X].BLOCK == ROAD )
	{
		return true;
	}
	return false;
}


JPS::Node *JPS::PathFind (int StartX, int StartY, int EndX, int EndY, bool First)
{
	TileColor = RGB (rand () % 254, rand () % 254, rand () % 254);
	if ( First )
	{
		OpenList_Delete ();
		CloseList_Delete ();

		_StartX = StartX;
		_StartY = StartY;
		_EndX = EndX;
		_EndY = EndY;
		
		Node *NewNode = new Node;

		NewNode->G = 0;
		NewNode->H = 10 * (abs (StartX - EndX) + abs (StartY - EndY));	//멘하탄 방정식
		NewNode->F = NewNode->G + NewNode->H;

		NewNode->Parent = NULL;
		NewNode->X = StartX;
		NewNode->Y = StartY;
		OpenList.insert (NewNode);
	}

	//길찾기 시작
	//while(1)
	{
		multiset<Node *>::iterator Iter;
			
		//오픈리스트에서 하나 뽑고 시작.
		Iter = OpenList.begin ();

		//오픈리스트가 비었다면 길이 없는 것이므로 바로 종료.
		if ( Iter == OpenList.end() )
		{
			return NULL;
		}

		Node *p = *Iter;

		int pX = p->X;
		int pY = p->Y;

		//오픈리스트에서 뽑은 노드 삭제 및 CloseList에 저장.
		OpenList.erase (Iter);
		CloseList.insert (p);

		//목적지에 도달했다면 노드를 반환하고 종료
		if ( pX == EndX && pY == EndY )
		{
			return p;
		}

		/*

		//12시
		MakeNode (p, 10, pX, pY - 1);
		//3시
		MakeNode (p, 10, pX + 1, pY);
		//6시
		MakeNode (p, 10, pX, pY + 1);
		//9시
		MakeNode (p, 10, pX - 1, pY);

		//2시
		MakeNode (p, 14, pX + 1, pY - 1);
		//5시
		MakeNode (p, 14, pX + 1, pY + 1);
		//8시
		MakeNode (p, 14, pX - 1, pY + 1);
		//11시
		MakeNode (p, 14, pX - 1, pY - 1);
		*/
		return NULL;
	}

}

JPS::Node *JPS::Jump (int StartX, int StartY, int EndX, int EndY, bool First)
{
	TileColor = RGB (rand () % 254, rand () % 254, rand () % 254);

	int JumpX;
	int JumpY;
	bool New = false;

	if ( First )
	{
		OpenList_Delete ();
		CloseList_Delete ();

		_StartX = StartX;
		_StartY = StartY;
		_EndX = EndX;
		_EndY = EndY;

		Node *NewNode = new Node;

		NewNode->G = 0;
		NewNode->H = 10 * (abs (StartX - EndX) + abs (StartY - EndY));	//멘하탄 방정식
		NewNode->F = NewNode->G + NewNode->H;

		NewNode->Parent = NULL;
		NewNode->X = StartX;
		NewNode->Y = StartY;
		NewNode->iDir = MOVE_First;
		OpenList.insert (NewNode);

	}


	//while ( 1 )
	{
		multiset<Node *>::iterator Iter;

		//오픈리스트에서 하나 뽑고 시작.
		Iter = OpenList.begin ();

		//오픈리스트가 비었다면 길이 없는 것이므로 바로 종료.
		if ( Iter == OpenList.end () )
		{
			return NULL;
		}

		Node *p = *Iter;

		int pX = p->X;
		int pY = p->Y;

		//오픈리스트에서 뽑은 노드 삭제 및 CloseList에 저장.
		OpenList.erase (Iter);
		CloseList.insert (p);

		//목적지에 도달했다면 노드를 반환하고 종료
		if ( pX == EndX && pY == EndY )
		{
			return p;
		}





		switch ( p->iDir )
		{
		case MOVE_UU:
			//기본
			if ( MoveStright (pX, pY - 1, MOVE_UU, &JumpX, &JumpY) )
			{
				MakeNode (p, 10, JumpX, JumpY, MOVE_UU);
			}

			//왼쪽 타일과 오른쪽타일 조사 블럭일경우 true
			if ( !TileSearch (pX - 1, pY) )
			{
				//왼쪽타일이 블럭일 경우 바로 그 윗타일 조사 블럭이 아닐경우 
				if ( Movediagonal (pX - 1, pY - 1, MOVE_UL, &JumpX, &JumpY) )
				{
					MakeNode (p, 14, JumpX, JumpY, MOVE_UL);
				}
			}
			if ( !TileSearch (pX + 1, pY) )
			{
				//오른쪽타일이 블럭일 경우 바로 그 윗타일 조사 블럭이 아닐경우 현재위치 반환
				if ( Movediagonal (pX + 1, pY - 1, MOVE_UR, &JumpX, &JumpY) )
				{
					MakeNode (p, 14, JumpX, JumpY, MOVE_UR);
				}
			}

			break;

		case MOVE_UR :
			//기본
			if ( Movediagonal (pX + 1, pY - 1,MOVE_UR, &JumpX, &JumpY) )
			{
				MakeNode (p, 14, JumpX, JumpY, MOVE_UR);
			}
			if ( MoveStright (pX, pY - 1, MOVE_UU, &JumpX, &JumpY) )
			{
				MakeNode (p, 10, JumpX, JumpY, MOVE_UU);
			}
			if ( MoveStright (pX + 1, pY, MOVE_RR, &JumpX, &JumpY) )
			{
				MakeNode (p, 10, JumpX, JumpY, MOVE_RR);
			}
			//옵션
			//왼쪽 타일과 아래타일 조사 블럭일경우 true
			if ( !TileSearch (pX - 1, pY) )
			{
				//왼쪽타일이 블럭일 경우 바로 그 윗타일 조사 블럭이 아닐경우 
				if ( Movediagonal (pX - 1, pY - 1, MOVE_UL, &JumpX, &JumpY) )
				{
					MakeNode (p, 14, JumpX, JumpY, MOVE_UL);
				}
				
			}
			if( !TileSearch (pX, pY + 1))
			{
				//아래타일이 블럭일 경우 바로 그 다음타일 조사 블럭이 아닐경우 현재위치 반환
				if ( Movediagonal (pX + 1, pY + 1, MOVE_RD, &JumpX, &JumpY) )
				{
					MakeNode (p, 14, JumpX, JumpY, MOVE_RD);
				}
			}
			break;

		case MOVE_RR :
			//기본
			if ( MoveStright (pX + 1, pY , MOVE_RR, &JumpX, &JumpY) )
			{
				MakeNode (p, 10, JumpX, JumpY, MOVE_RR);
			}

			//윗타일과 아래타일 조사 블럭일경우 true
			if ( !TileSearch (pX, pY - 1) )
			{
				//위타일이 블럭일 경우 바로 그 윗타일 조사 블럭이 아닐경우 
					if ( Movediagonal (pX + 1, pY - 1, MOVE_UR, &JumpX, &JumpY) )
					{
						MakeNode (p, 14, JumpX, JumpY, MOVE_UR);
					}
			}
			if( !TileSearch (pX, pY + 1) )
			{
				//아래타일이 블럭일 경우 바로 그 윗타일 조사 블럭이 아닐경우 현재위치 반환
					if ( Movediagonal (pX + 1, pY + 1, MOVE_RD, &JumpX, &JumpY) )
					{
						MakeNode (p, 14, JumpX, JumpY, MOVE_RD);
					}
			}

			break;

		case MOVE_RD :
			//기본
			if ( Movediagonal (pX + 1, pY + 1, MOVE_RD, &JumpX, &JumpY) )
			{
				MakeNode (p, 14, JumpX, JumpY, MOVE_RD);
			}
			if ( MoveStright (pX + 1, pY, MOVE_RR, &JumpX, &JumpY) )
			{
				MakeNode (p, 10, JumpX, JumpY, MOVE_RR);
			}
			if ( MoveStright (pX, pY + 1, MOVE_DD, &JumpX, &JumpY) )
			{
				MakeNode (p, 10, JumpX, JumpY, MOVE_DD);
			}
			//옵션
			//위쪽 타일과 왼쪽타일 조사 블럭일경우 true
			if ( !TileSearch (pX, pY - 1) )
			{
				//위쪽타일이 블럭일 경우 바로 그 다음타일 조사 블럭이 아닐경우 
					if ( Movediagonal (pX + 1, pY - 1, MOVE_UR, &JumpX, &JumpY) )
					{
						MakeNode (p, 14, JumpX, JumpY, MOVE_UR);
					}
			}
			if ( !TileSearch (pX - 1, pY) )
			{
				//아래타일이 블럭일 경우 바로 그 아래타일 조사 블럭이 아닐경우 현재위치 반환
					if ( Movediagonal (pX -1, pY + 1, MOVE_LD, &JumpX, &JumpY) )
					{
						MakeNode (p, 14, JumpX, JumpY, MOVE_LD);
					}
			}
			break;

		case MOVE_DD :
			//기본
			if ( MoveStright (pX, pY + 1, MOVE_DD, &JumpX, &JumpY) )
			{
				MakeNode (p, 10, JumpX, JumpY, MOVE_DD);
			}

			//왼쪽 타일과 오른쪽타일 조사 블럭일경우 true
			if ( !TileSearch (pX - 1, pY) )
			{
				//왼쪽타일이 블럭일 경우 바로 그 아래타일 조사 블럭이 아닐경우 
					if ( Movediagonal (pX - 1, pY + 1, MOVE_LD, &JumpX, &JumpY) )
					{
						MakeNode (p, 14, JumpX, JumpY, MOVE_LD);
					}
			}
			if( !TileSearch (pX + 1, pY) )
			{
				//오른쪽타일이 블럭일 경우 바로 그 아래타일 조사 블럭이 아닐경우 현재위치 반환
					if ( Movediagonal (pX + 1, pY + 1, MOVE_RD, &JumpX, &JumpY) )
					{
						MakeNode (p, 14, JumpX, JumpY, MOVE_RD);
					}
			}
			break;

		case MOVE_LD :
			//기본
			if ( Movediagonal (pX - 1, pY + 1, MOVE_LD, &JumpX, &JumpY) )
			{
				MakeNode (p, 14, JumpX, JumpY, MOVE_LD);
			}
			if ( MoveStright (pX - 1, pY, MOVE_LL, &JumpX, &JumpY) )
			{
				MakeNode (p, 10, JumpX, JumpY, MOVE_LL);
			}
			if ( MoveStright (pX, pY + 1, MOVE_DD, &JumpX, &JumpY) )
			{
				MakeNode (p, 10, JumpX, JumpY, MOVE_DD);
			}
			//옵션
			//위쪽 타일과 오른쪽타일 조사 블럭일경우 true
			if ( !TileSearch (pX, pY - 1) )
			{
				//위쪽타일이 블럭일 경우 바로 그 이전타일 조사 블럭이 아닐경우 
					if ( Movediagonal (pX - 1, pY - 1, MOVE_UL, &JumpX, &JumpY) )
					{
						MakeNode (p, 14, JumpX, JumpY, MOVE_UL);
					}
			}
			if ( !TileSearch (pX + 1, pY) )
			{
				//아래타일이 블럭일 경우 바로 그 아래타일 조사 블럭이 아닐경우 현재위치 반환
					if ( Movediagonal (pX + 1, pY + 1, MOVE_RD, &JumpX, &JumpY) )
					{
						MakeNode (p, 14, JumpX, JumpY, MOVE_RD);
					}
			}
			break;

		case MOVE_LL :
			//기본
			if ( MoveStright (pX - 1, pY, MOVE_LL, &JumpX, &JumpY) )
			{
				MakeNode (p, 10, JumpX, JumpY, MOVE_LL);
			}

			//윗타일과 아래타일 조사 블럭일경우 true
			if ( !TileSearch (pX, pY - 1) )
			{
				//위타일이 블럭일 경우 바로 그 이전타일 조사 블럭이 아닐경우 
					if ( Movediagonal (pX - 1, pY - 1, MOVE_UL, &JumpX, &JumpY) )
					{
						MakeNode (p, 14, JumpX, JumpY, MOVE_UL);
					}
			}
			if( !TileSearch (pX, pY + 1) )
			{
				//아래타일이 블럭일 경우 바로 그 이전타일 조사 블럭이 아닐경우 현재위치 반환
				if ( TileSearch (pX - 1, pY + 1) )
				{
					if ( Movediagonal (pX - 1, pY + 1, MOVE_LD, &JumpX, &JumpY) )
					{
						MakeNode (p, 14, JumpX, JumpY, MOVE_LD);
					}
				}
			}
			break;

		case MOVE_UL :
			//기본
			if ( Movediagonal (pX - 1, pY - 1, MOVE_UL, &JumpX, &JumpY) )
			{
				MakeNode (p, 14, JumpX, JumpY, MOVE_UL);
			}
			if ( MoveStright (pX, pY - 1, MOVE_UU, &JumpX, &JumpY) )
			{
				MakeNode (p, 10, JumpX, JumpY, MOVE_UU);
			}
			if ( MoveStright (pX - 1, pY, MOVE_LL, &JumpX, &JumpY) )
			{
				MakeNode (p, 10, JumpX, JumpY, MOVE_LL);
			}
			//옵션
			//오른쪽타일과 아래쪽타일 조사 블럭일경우 true
			if ( !TileSearch (pX + 1, pY) )
			{
				//위쪽타일이 블럭일 경우 바로 그 다음타일 조사 블럭이 아닐경우 
					if ( Movediagonal (pX + 1, pY - 1, MOVE_UR, &JumpX, &JumpY) )
					{
						MakeNode (p, 14, JumpX, JumpY, MOVE_UR);
					}
			}
			if ( !TileSearch (pX, pY + 1) )
			{
				//아래타일이 블럭일 경우 바로 그 이전타일 조사 블럭이 아닐경우 현재위치 반환
					if ( Movediagonal (pX - 1, pY + 1, MOVE_LD, &JumpX, &JumpY) )
					{
						MakeNode (p, 14, JumpX, JumpY, MOVE_LD);
					}
			}
			break;

		case MOVE_First :

			//직선 4방향 체크
			if ( MoveStright (pX + 1, pY, MOVE_RR, &JumpX, &JumpY) )
			{
				MakeNode (p, 10, JumpX, JumpY,MOVE_RR);
			}
			if ( MoveStright (pX, pY + 1, MOVE_DD, &JumpX, &JumpY) )
			{
				MakeNode (p, 10, JumpX, JumpY,MOVE_DD);
			}
			if ( MoveStright (pX - 1, pY, MOVE_LL, &JumpX, &JumpY) )
			{
				MakeNode (p, 10, JumpX, JumpY,MOVE_LL);
			}
			if ( MoveStright (pX, pY - 1, MOVE_UU, &JumpX, &JumpY) )
			{
				MakeNode (p, 10, JumpX, JumpY,MOVE_UU);
			}
			//대각선 4방향 체크
			if ( Movediagonal (pX + 1, pY - 1, MOVE_UR, &JumpX, &JumpY) )
			{
				MakeNode (p, 14, JumpX, JumpY,MOVE_UR);
			}
			if ( Movediagonal (pX + 1, pY + 1, MOVE_RD, &JumpX, &JumpY) )
			{
				MakeNode (p, 14, JumpX, JumpY,MOVE_RD);
			}
			if ( Movediagonal (pX - 1, pY + 1, MOVE_LD, &JumpX, &JumpY) )
			{
				MakeNode (p, 14, JumpX, JumpY,MOVE_LD);
			}
			if ( Movediagonal (pX - 1, pY - 1, MOVE_UL, &JumpX, &JumpY) )
			{
				MakeNode (p, 14, JumpX, JumpY,MOVE_UL);
			}
			break;

		}


	}
	
	return NULL;
}


//직선체크
bool JPS::MoveStright (int X, int Y, Move iDir, int *JumpX, int *JumpY)
{
	while ( 1 )
	{
		//현재 맵의 X,Y좌표가 블럭이라면 그대로 종료.
		if ( !TileSearch (X, Y) )
		{
			return false;
		}
		//현재 위치가 맵 좌표를 벗어나면 그대로 종료
		if ( X < 0 || X >= _MaxX )
		{
			return false;
		}
		if ( Y < 0 || Y >= _MaxY )
		{
			return false;
		}

		if ( X == _EndX && Y == _EndY )
		{
			*JumpX = X;
			*JumpY = Y;
			return true;
		}


		//현재 체크한 타일에 컬러 설정
		MapColorSet (X, Y, TileColor);

		switch ( iDir )
		{
		case MOVE_UU :
			//왼쪽 타일 조사 블럭일경우 true
			if ( !TileSearch (X - 1, Y) )
			{
				//왼쪽타일이 블럭일 경우 바로 그 윗타일 조사 블럭이 아닐경우 현재위치 반환
				if ( TileSearch (X - 1, Y - 1) )
				{
					*JumpX = X;
					*JumpY = Y;
					return true;
				}
			}
			//오른쪽 타일 조사 블럭일경우 true
			if ( !TileSearch (X + 1, Y) )
			{
				//오른쪽타일이 블럭일 경우 바로 그 윗타일 조사 블럭이 아닐경우 현재위치 반환
				if ( TileSearch (X + 1, Y - 1) )
				{
					*JumpX = X;
					*JumpY = Y;
					return true;
				}
			}
			//둘다 아닐 경우 위로 한칸 가서 다시 반복
			Y = Y - 1;

			break;

		case MOVE_RR :
			//윗 타일 조사 블럭일경우 true
			if ( !TileSearch (X, Y - 1) )
			{
				//윗 타일이 블럭일 경우 바로 그 다음타일 조사 블럭이 아닐경우 현재위치 반환
				if ( TileSearch (X + 1, Y - 1) )
				{
					*JumpX = X;
					*JumpY = Y;
					return true;
				}
			}
			//아래 타일 조사 블럭일경우 true
			if ( !TileSearch (X , Y + 1) )
			{
				//아래쪽타일이 블럭일 경우 바로 그 다음타일 조사 블럭이 아닐경우 현재위치 반환
				if ( TileSearch (X + 1, Y + 1) )
				{
					*JumpX = X;
					*JumpY = Y;
					return true;
				}
			}
			//둘다 아닐 경우 오른쪽으로 한칸 가서 다시 반복
			X = X + 1;

			break;

		case MOVE_DD :
			//왼쪽 타일 조사 블럭일경우 true
			if ( !TileSearch (X - 1, Y) )
			{
				//왼쪽타일이 블럭일 경우 바로 그 아래타일 조사 블럭이 아닐경우 현재위치 반환
				if ( TileSearch (X - 1, Y + 1) )
				{
					*JumpX = X;
					*JumpY = Y;
					return true;
				}
			}
			//오른쪽 타일 조사 블럭일경우 true
			if ( !TileSearch (X + 1, Y) )
			{
				//오른쪽타일이 블럭일 경우 바로 그 아래타일 조사 블럭이 아닐경우 현재위치 반환
				if ( TileSearch (X + 1, Y + 1) )
				{
					*JumpX = X;
					*JumpY = Y;
					return true;
				}
			}
			//둘다 아닐 경우 위로 한칸 가서 다시 반복
			Y = Y + 1;

			break;

		case MOVE_LL :

			//윗 타일 조사 블럭일경우 true
			if ( !TileSearch (X, Y - 1) )
			{
				//윗 타일이 블럭일 경우 바로 그 앞타일 조사 블럭이 아닐경우 현재위치 반환
				if ( TileSearch (X - 1, Y - 1) )
				{
					*JumpX = X;
					*JumpY = Y;
					return true;
				}
			}
			//아래 타일 조사 블럭일경우 true
			if ( !TileSearch (X, Y + 1) )
			{
				//아래쪽타일이 블럭일 경우 바로 그 앞타일 조사 블럭이 아닐경우 현재위치 반환
				if ( TileSearch (X - 1, Y + 1) )
				{
					*JumpX = X;
					*JumpY = Y;
					return true;
				}
			}
			//둘다 아닐 경우 왼쪽으로 한칸 가서 다시 반복
			X = X - 1;

			break;

		}

	}
	return false;

}
//대각선 체크
bool JPS::Movediagonal (int X, int Y, Move iDir, int *JumpX, int *JumpY)
{
	int checkX;
	int checkY;
	while ( 1 )
	{
		//현재 맵의 X,Y좌표가 블럭이라면 그대로 종료.
		if ( !TileSearch (X, Y) )
		{
			return false;
		}
		//현재 위치가 맵 좌표를 벗어나면 그대로 종료
		if ( X < 0 || X >= _MaxX )
		{
			return false;
		}
		if ( Y < 0 || Y >= _MaxY )
		{
			return false;
		}

		//현재 체크한 타일에 컬러 설정
		MapColorSet (X, Y, TileColor);
		if ( X == _EndX && Y == _EndY )
		{
			*JumpX = X;
			*JumpY = Y;
			return true;
		}

		switch ( iDir )
		{
		case MOVE_UR:
			//왼쪽 타일 조사 블럭일경우 true
			if ( !TileSearch (X - 1, Y) )
			{
				//왼쪽타일이 블럭일 경우 바로 그 윗타일 조사 블럭이 아닐경우 현재위치 반환
				if ( TileSearch (X - 1, Y - 1) )
				{
					*JumpX = X;
					*JumpY = Y;
					return true;
				}
			}
			//왼쪽 타일 블럭이 아니므로 왼쪽 타일로 위로 직선 검사
			if ( MoveStright (X - 1, Y, MOVE_UU, &checkX, &checkY) )
			{
				*JumpX = X;
				*JumpY = Y;
				return true;
			}
			//아래 타일 조사 블럭일경우 true
			if ( !TileSearch (X , Y + 1) )
			{
				//아래 타일이 블럭일 경우 바로 그 다음타일 조사 블럭이 아닐경우 현재위치 반환
				if ( TileSearch (X + 1, Y + 1) )
				{
					*JumpX = X;
					*JumpY = Y;
					return true;
				}
			}
			//아래쪽 타일 블럭이 아니므로 오른쪽 타일로 위로 직선 검사
			if ( MoveStright (X, Y + 1, MOVE_RR, &checkX, &checkY) )
			{
				*JumpX = X;
				*JumpY = Y;
				return true;
			}

			//둘다 아닐 경우 위 오른쪽 으로 한칸 가서 다시 반복
			X = X + 1;
			Y = Y - 1;


			break;

		case MOVE_RD :
			//위 타일 조사 블럭일경우 true
			if ( !TileSearch (X, Y - 1) )
			{
				//위 타일이 블럭일 경우 바로 그 다음타일 조사 블럭이 아닐경우 현재위치 반환
				if ( TileSearch (X + 1, Y - 1) )
				{
					*JumpX = X;
					*JumpY = Y;
					return true;
				}
			}
			//위쪽 타일 블럭이 아니므로 오른쪽 타일로 직선 검사
			if ( MoveStright (X, Y - 1, MOVE_RR, &checkX, &checkY) )
			{
				*JumpX = X;
				*JumpY = Y;
				return true;
			}

			//왼쪽 타일 조사 블럭일경우 true
			if ( !TileSearch (X - 1, Y) )
			{
				//왼쪽 타일이 블럭일 경우 바로 그 아래타일 조사 블럭이 아닐경우 현재위치 반환
				if ( TileSearch (X - 1, Y + 1) )
				{
					*JumpX = X;
					*JumpY = Y;
					return true;
				}
			}
			//아래쪽 타일 블럭이 아니므로 아래쪽 타일로 직선 검사
			if ( MoveStright (X - 1, Y , MOVE_DD, &checkX, &checkY) )
			{
				*JumpX = X;
				*JumpY = Y;
				return true;
			}

			//둘다 아닐 경우 아래 오른쪽 으로 한칸 가서 다시 반복
			X = X + 1;
			Y = Y + 1;

			break;

		case MOVE_LD :
			//위 타일 조사 블럭일경우 true
			if ( !TileSearch (X, Y - 1) )
			{
				//위 타일이 블럭일 경우 바로 그 전타일 조사 블럭이 아닐경우 현재위치 반환
				if ( TileSearch (X - 1, Y - 1) )
				{
					*JumpX = X;
					*JumpY = Y;
					return true;
				}
			}
			if ( MoveStright (X, Y - 1, MOVE_LL, &checkX, &checkY) )
			{
				*JumpX = X;
				*JumpY = Y;
				return true;
			}


			//오른쪽 타일 조사 블럭일경우 true
			if ( !TileSearch (X + 1, Y) )
			{
				//오른쪽 타일이 블럭일 경우 바로 그 아래타일 조사 블럭이 아닐경우 현재위치 반환
				if ( TileSearch (X + 1, Y + 1) )
				{
					*JumpX = X;
					*JumpY = Y;
					return true;
				}
			}
			if ( MoveStright (X + 1, Y , MOVE_DD, &checkX, &checkY) )
			{
				*JumpX = X;
				*JumpY = Y;
				return true;
			}

			//둘다 아닐 경우 아래 왼쪽 으로 한칸 가서 다시 반복
			X = X - 1;
			Y = Y + 1;

			break;
			
		case MOVE_UL :
			//오른쪽 타일 조사 블럭일경우 true
			if ( !TileSearch (X + 1, Y) )
			{
				//오른쪽 타일이 블럭일 경우 바로 그 위타일 조사 블럭이 아닐경우 현재위치 반환
				if ( TileSearch (X + 1, Y - 1) )
				{
					*JumpX = X;
					*JumpY = Y;
					return true;
				}
			}
			if ( MoveStright (X + 1, Y, MOVE_UU, &checkX, &checkY) )
			{
				*JumpX = X;
				*JumpY = Y;
				return true;
			}
			//아래쪽 타일 조사 블럭일경우 true
			if ( !TileSearch (X, Y + 1) )
			{
				//아래쪽 타일이 블럭일 경우 바로 그 이전타일 조사 블럭이 아닐경우 현재위치 반환
				if ( TileSearch (X - 1, Y + 1) )
				{
					*JumpX = X;
					*JumpY = Y;
					return true;
				}
			}
			if ( MoveStright (X, Y + 1, MOVE_LL, &checkX, &checkY) )
			{
				*JumpX = X;
				*JumpY = Y;
				return true;
			}
			//둘다 아닐 경우 아래 왼쪽 으로 한칸 가서 다시 반복
			X = X - 1;
			Y = Y - 1;


			break;
		}
	}




}







void JPS::MapTileSet (int X, int Y, Tile Name)
{
	if ( X < 0 || X >= _MaxX )
	{
		return;
	}
	if ( Y < 0 || Y >= _MaxY )
	{
		return;
	}


	switch ( Name )
	{
	case ROAD:
		Map[Y][X].BLOCK = ROAD;
		break;
	case BLOCK:
		Map[Y][X].BLOCK = BLOCK;
		break;
	}
	return;
}

void JPS::MapColorSet (int X, int Y, COLORREF Color)
{
	if ( X < 0 || X >= _MaxX )
	{
		return;
	}
	if ( Y < 0 || Y >= _MaxY )
	{
		return;
	}

	Map[Y][X].rgb = Color;
	
	return;
}



void JPS::MakeNode (Node *Parent, int MovePay, int X, int Y, Move iDir)
{
	//맵 안벗어나도록 체크
	if ( X < 0 || X > _MaxX - 1 )
	{
		return;
	}
	if ( Y < 0 || Y > _MaxY - 1 )
	{
		return;
	}

	//맵 블록 확인
	if ( !TileSearch(X,Y) )
	{
		return;
	}


	//노드 새로 생성 및 셋팅
	Node *NewNode = new Node;

	NewNode->Parent = Parent;
	NewNode->X = X;
	NewNode->Y = Y;

	int ParentDir = (abs (X - Parent->X) + abs (Y - Parent->Y));


	NewNode->H = 10 * (abs (X - _EndX) + abs (Y - _EndY));
	NewNode->G = Parent->G + (MovePay * ParentDir);
	NewNode->F = NewNode->H + NewNode->G;
	NewNode->iDir = iDir;


	//CloseList 검색
	NODE_FINDER find(X, Y);
	multiset<Node *>::iterator IterClose;
	IterClose = find_if (CloseList.begin (), CloseList.end (), find);

	if ( IterClose != CloseList.end () )
	{
		Node *p = *IterClose;
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
	multiset<Node *>::iterator IterOpen;
	IterOpen = find_if (OpenList.begin (), OpenList.end (), find);
	if ( IterOpen != OpenList.end () )
	{
		Node *p = *IterOpen;
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


	//오픈리스트에 등록하고 return
	OpenList.insert (NewNode);
	return;
}



void JPS::OpenList_Delete (void)
{
	multiset<Node *>::iterator IterOpen;
	for ( IterOpen = OpenList.begin (); IterOpen != OpenList.end (); )
	{

		Node *p = *IterOpen;
		delete p;
		IterOpen = OpenList.erase (IterOpen);

	}
}

void JPS::CloseList_Delete (void)
{
	multiset<Node *>::iterator IterClose;
	for ( IterClose = CloseList.begin (); IterClose != CloseList.end (); )
	{

		Node *p = *IterClose;
		delete p;
		IterClose = CloseList.erase (IterClose);

	}
}


