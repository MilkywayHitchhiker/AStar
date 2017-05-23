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
		NewNode->H = 10 * (abs (StartX - EndX) + abs (StartY - EndY));	//����ź ������
		NewNode->F = NewNode->G + NewNode->H;

		NewNode->Parent = NULL;
		NewNode->X = StartX;
		NewNode->Y = StartY;
		OpenList.insert (NewNode);
	}

	//��ã�� ����
	//while(1)
	{
		multiset<Node *>::iterator Iter;
			
		//���¸���Ʈ���� �ϳ� �̰� ����.
		Iter = OpenList.begin ();

		//���¸���Ʈ�� ����ٸ� ���� ���� ���̹Ƿ� �ٷ� ����.
		if ( Iter == OpenList.end() )
		{
			return NULL;
		}

		Node *p = *Iter;

		int pX = p->X;
		int pY = p->Y;

		//���¸���Ʈ���� ���� ��� ���� �� CloseList�� ����.
		OpenList.erase (Iter);
		CloseList.insert (p);

		//�������� �����ߴٸ� ��带 ��ȯ�ϰ� ����
		if ( pX == EndX && pY == EndY )
		{
			return p;
		}

		/*

		//12��
		MakeNode (p, 10, pX, pY - 1);
		//3��
		MakeNode (p, 10, pX + 1, pY);
		//6��
		MakeNode (p, 10, pX, pY + 1);
		//9��
		MakeNode (p, 10, pX - 1, pY);

		//2��
		MakeNode (p, 14, pX + 1, pY - 1);
		//5��
		MakeNode (p, 14, pX + 1, pY + 1);
		//8��
		MakeNode (p, 14, pX - 1, pY + 1);
		//11��
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
		NewNode->H = 10 * (abs (StartX - EndX) + abs (StartY - EndY));	//����ź ������
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

		//���¸���Ʈ���� �ϳ� �̰� ����.
		Iter = OpenList.begin ();

		//���¸���Ʈ�� ����ٸ� ���� ���� ���̹Ƿ� �ٷ� ����.
		if ( Iter == OpenList.end () )
		{
			return NULL;
		}

		Node *p = *Iter;

		int pX = p->X;
		int pY = p->Y;

		//���¸���Ʈ���� ���� ��� ���� �� CloseList�� ����.
		OpenList.erase (Iter);
		CloseList.insert (p);

		//�������� �����ߴٸ� ��带 ��ȯ�ϰ� ����
		if ( pX == EndX && pY == EndY )
		{
			return p;
		}





		switch ( p->iDir )
		{
		case MOVE_UU:
			//�⺻
			if ( MoveStright (pX, pY - 1, MOVE_UU, &JumpX, &JumpY) )
			{
				MakeNode (p, 10, JumpX, JumpY, MOVE_UU);
			}

			//���� Ÿ�ϰ� ������Ÿ�� ���� ���ϰ�� true
			if ( !TileSearch (pX - 1, pY) )
			{
				//����Ÿ���� ���� ��� �ٷ� �� ��Ÿ�� ���� ���� �ƴҰ�� 
				if ( Movediagonal (pX - 1, pY - 1, MOVE_UL, &JumpX, &JumpY) )
				{
					MakeNode (p, 14, JumpX, JumpY, MOVE_UL);
				}
			}
			if ( !TileSearch (pX + 1, pY) )
			{
				//������Ÿ���� ���� ��� �ٷ� �� ��Ÿ�� ���� ���� �ƴҰ�� ������ġ ��ȯ
				if ( Movediagonal (pX + 1, pY - 1, MOVE_UR, &JumpX, &JumpY) )
				{
					MakeNode (p, 14, JumpX, JumpY, MOVE_UR);
				}
			}

			break;

		case MOVE_UR :
			//�⺻
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
			//�ɼ�
			//���� Ÿ�ϰ� �Ʒ�Ÿ�� ���� ���ϰ�� true
			if ( !TileSearch (pX - 1, pY) )
			{
				//����Ÿ���� ���� ��� �ٷ� �� ��Ÿ�� ���� ���� �ƴҰ�� 
				if ( Movediagonal (pX - 1, pY - 1, MOVE_UL, &JumpX, &JumpY) )
				{
					MakeNode (p, 14, JumpX, JumpY, MOVE_UL);
				}
				
			}
			if( !TileSearch (pX, pY + 1))
			{
				//�Ʒ�Ÿ���� ���� ��� �ٷ� �� ����Ÿ�� ���� ���� �ƴҰ�� ������ġ ��ȯ
				if ( Movediagonal (pX + 1, pY + 1, MOVE_RD, &JumpX, &JumpY) )
				{
					MakeNode (p, 14, JumpX, JumpY, MOVE_RD);
				}
			}
			break;

		case MOVE_RR :
			//�⺻
			if ( MoveStright (pX + 1, pY , MOVE_RR, &JumpX, &JumpY) )
			{
				MakeNode (p, 10, JumpX, JumpY, MOVE_RR);
			}

			//��Ÿ�ϰ� �Ʒ�Ÿ�� ���� ���ϰ�� true
			if ( !TileSearch (pX, pY - 1) )
			{
				//��Ÿ���� ���� ��� �ٷ� �� ��Ÿ�� ���� ���� �ƴҰ�� 
					if ( Movediagonal (pX + 1, pY - 1, MOVE_UR, &JumpX, &JumpY) )
					{
						MakeNode (p, 14, JumpX, JumpY, MOVE_UR);
					}
			}
			if( !TileSearch (pX, pY + 1) )
			{
				//�Ʒ�Ÿ���� ���� ��� �ٷ� �� ��Ÿ�� ���� ���� �ƴҰ�� ������ġ ��ȯ
					if ( Movediagonal (pX + 1, pY + 1, MOVE_RD, &JumpX, &JumpY) )
					{
						MakeNode (p, 14, JumpX, JumpY, MOVE_RD);
					}
			}

			break;

		case MOVE_RD :
			//�⺻
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
			//�ɼ�
			//���� Ÿ�ϰ� ����Ÿ�� ���� ���ϰ�� true
			if ( !TileSearch (pX, pY - 1) )
			{
				//����Ÿ���� ���� ��� �ٷ� �� ����Ÿ�� ���� ���� �ƴҰ�� 
					if ( Movediagonal (pX + 1, pY - 1, MOVE_UR, &JumpX, &JumpY) )
					{
						MakeNode (p, 14, JumpX, JumpY, MOVE_UR);
					}
			}
			if ( !TileSearch (pX - 1, pY) )
			{
				//�Ʒ�Ÿ���� ���� ��� �ٷ� �� �Ʒ�Ÿ�� ���� ���� �ƴҰ�� ������ġ ��ȯ
					if ( Movediagonal (pX -1, pY + 1, MOVE_LD, &JumpX, &JumpY) )
					{
						MakeNode (p, 14, JumpX, JumpY, MOVE_LD);
					}
			}
			break;

		case MOVE_DD :
			//�⺻
			if ( MoveStright (pX, pY + 1, MOVE_DD, &JumpX, &JumpY) )
			{
				MakeNode (p, 10, JumpX, JumpY, MOVE_DD);
			}

			//���� Ÿ�ϰ� ������Ÿ�� ���� ���ϰ�� true
			if ( !TileSearch (pX - 1, pY) )
			{
				//����Ÿ���� ���� ��� �ٷ� �� �Ʒ�Ÿ�� ���� ���� �ƴҰ�� 
					if ( Movediagonal (pX - 1, pY + 1, MOVE_LD, &JumpX, &JumpY) )
					{
						MakeNode (p, 14, JumpX, JumpY, MOVE_LD);
					}
			}
			if( !TileSearch (pX + 1, pY) )
			{
				//������Ÿ���� ���� ��� �ٷ� �� �Ʒ�Ÿ�� ���� ���� �ƴҰ�� ������ġ ��ȯ
					if ( Movediagonal (pX + 1, pY + 1, MOVE_RD, &JumpX, &JumpY) )
					{
						MakeNode (p, 14, JumpX, JumpY, MOVE_RD);
					}
			}
			break;

		case MOVE_LD :
			//�⺻
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
			//�ɼ�
			//���� Ÿ�ϰ� ������Ÿ�� ���� ���ϰ�� true
			if ( !TileSearch (pX, pY - 1) )
			{
				//����Ÿ���� ���� ��� �ٷ� �� ����Ÿ�� ���� ���� �ƴҰ�� 
					if ( Movediagonal (pX - 1, pY - 1, MOVE_UL, &JumpX, &JumpY) )
					{
						MakeNode (p, 14, JumpX, JumpY, MOVE_UL);
					}
			}
			if ( !TileSearch (pX + 1, pY) )
			{
				//�Ʒ�Ÿ���� ���� ��� �ٷ� �� �Ʒ�Ÿ�� ���� ���� �ƴҰ�� ������ġ ��ȯ
					if ( Movediagonal (pX + 1, pY + 1, MOVE_RD, &JumpX, &JumpY) )
					{
						MakeNode (p, 14, JumpX, JumpY, MOVE_RD);
					}
			}
			break;

		case MOVE_LL :
			//�⺻
			if ( MoveStright (pX - 1, pY, MOVE_LL, &JumpX, &JumpY) )
			{
				MakeNode (p, 10, JumpX, JumpY, MOVE_LL);
			}

			//��Ÿ�ϰ� �Ʒ�Ÿ�� ���� ���ϰ�� true
			if ( !TileSearch (pX, pY - 1) )
			{
				//��Ÿ���� ���� ��� �ٷ� �� ����Ÿ�� ���� ���� �ƴҰ�� 
					if ( Movediagonal (pX - 1, pY - 1, MOVE_UL, &JumpX, &JumpY) )
					{
						MakeNode (p, 14, JumpX, JumpY, MOVE_UL);
					}
			}
			if( !TileSearch (pX, pY + 1) )
			{
				//�Ʒ�Ÿ���� ���� ��� �ٷ� �� ����Ÿ�� ���� ���� �ƴҰ�� ������ġ ��ȯ
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
			//�⺻
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
			//�ɼ�
			//������Ÿ�ϰ� �Ʒ���Ÿ�� ���� ���ϰ�� true
			if ( !TileSearch (pX + 1, pY) )
			{
				//����Ÿ���� ���� ��� �ٷ� �� ����Ÿ�� ���� ���� �ƴҰ�� 
					if ( Movediagonal (pX + 1, pY - 1, MOVE_UR, &JumpX, &JumpY) )
					{
						MakeNode (p, 14, JumpX, JumpY, MOVE_UR);
					}
			}
			if ( !TileSearch (pX, pY + 1) )
			{
				//�Ʒ�Ÿ���� ���� ��� �ٷ� �� ����Ÿ�� ���� ���� �ƴҰ�� ������ġ ��ȯ
					if ( Movediagonal (pX - 1, pY + 1, MOVE_LD, &JumpX, &JumpY) )
					{
						MakeNode (p, 14, JumpX, JumpY, MOVE_LD);
					}
			}
			break;

		case MOVE_First :

			//���� 4���� üũ
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
			//�밢�� 4���� üũ
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


//����üũ
bool JPS::MoveStright (int X, int Y, Move iDir, int *JumpX, int *JumpY)
{
	while ( 1 )
	{
		//���� ���� X,Y��ǥ�� ���̶�� �״�� ����.
		if ( !TileSearch (X, Y) )
		{
			return false;
		}
		//���� ��ġ�� �� ��ǥ�� ����� �״�� ����
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


		//���� üũ�� Ÿ�Ͽ� �÷� ����
		MapColorSet (X, Y, TileColor);

		switch ( iDir )
		{
		case MOVE_UU :
			//���� Ÿ�� ���� ���ϰ�� true
			if ( !TileSearch (X - 1, Y) )
			{
				//����Ÿ���� ���� ��� �ٷ� �� ��Ÿ�� ���� ���� �ƴҰ�� ������ġ ��ȯ
				if ( TileSearch (X - 1, Y - 1) )
				{
					*JumpX = X;
					*JumpY = Y;
					return true;
				}
			}
			//������ Ÿ�� ���� ���ϰ�� true
			if ( !TileSearch (X + 1, Y) )
			{
				//������Ÿ���� ���� ��� �ٷ� �� ��Ÿ�� ���� ���� �ƴҰ�� ������ġ ��ȯ
				if ( TileSearch (X + 1, Y - 1) )
				{
					*JumpX = X;
					*JumpY = Y;
					return true;
				}
			}
			//�Ѵ� �ƴ� ��� ���� ��ĭ ���� �ٽ� �ݺ�
			Y = Y - 1;

			break;

		case MOVE_RR :
			//�� Ÿ�� ���� ���ϰ�� true
			if ( !TileSearch (X, Y - 1) )
			{
				//�� Ÿ���� ���� ��� �ٷ� �� ����Ÿ�� ���� ���� �ƴҰ�� ������ġ ��ȯ
				if ( TileSearch (X + 1, Y - 1) )
				{
					*JumpX = X;
					*JumpY = Y;
					return true;
				}
			}
			//�Ʒ� Ÿ�� ���� ���ϰ�� true
			if ( !TileSearch (X , Y + 1) )
			{
				//�Ʒ���Ÿ���� ���� ��� �ٷ� �� ����Ÿ�� ���� ���� �ƴҰ�� ������ġ ��ȯ
				if ( TileSearch (X + 1, Y + 1) )
				{
					*JumpX = X;
					*JumpY = Y;
					return true;
				}
			}
			//�Ѵ� �ƴ� ��� ���������� ��ĭ ���� �ٽ� �ݺ�
			X = X + 1;

			break;

		case MOVE_DD :
			//���� Ÿ�� ���� ���ϰ�� true
			if ( !TileSearch (X - 1, Y) )
			{
				//����Ÿ���� ���� ��� �ٷ� �� �Ʒ�Ÿ�� ���� ���� �ƴҰ�� ������ġ ��ȯ
				if ( TileSearch (X - 1, Y + 1) )
				{
					*JumpX = X;
					*JumpY = Y;
					return true;
				}
			}
			//������ Ÿ�� ���� ���ϰ�� true
			if ( !TileSearch (X + 1, Y) )
			{
				//������Ÿ���� ���� ��� �ٷ� �� �Ʒ�Ÿ�� ���� ���� �ƴҰ�� ������ġ ��ȯ
				if ( TileSearch (X + 1, Y + 1) )
				{
					*JumpX = X;
					*JumpY = Y;
					return true;
				}
			}
			//�Ѵ� �ƴ� ��� ���� ��ĭ ���� �ٽ� �ݺ�
			Y = Y + 1;

			break;

		case MOVE_LL :

			//�� Ÿ�� ���� ���ϰ�� true
			if ( !TileSearch (X, Y - 1) )
			{
				//�� Ÿ���� ���� ��� �ٷ� �� ��Ÿ�� ���� ���� �ƴҰ�� ������ġ ��ȯ
				if ( TileSearch (X - 1, Y - 1) )
				{
					*JumpX = X;
					*JumpY = Y;
					return true;
				}
			}
			//�Ʒ� Ÿ�� ���� ���ϰ�� true
			if ( !TileSearch (X, Y + 1) )
			{
				//�Ʒ���Ÿ���� ���� ��� �ٷ� �� ��Ÿ�� ���� ���� �ƴҰ�� ������ġ ��ȯ
				if ( TileSearch (X - 1, Y + 1) )
				{
					*JumpX = X;
					*JumpY = Y;
					return true;
				}
			}
			//�Ѵ� �ƴ� ��� �������� ��ĭ ���� �ٽ� �ݺ�
			X = X - 1;

			break;

		}

	}
	return false;

}
//�밢�� üũ
bool JPS::Movediagonal (int X, int Y, Move iDir, int *JumpX, int *JumpY)
{
	int checkX;
	int checkY;
	while ( 1 )
	{
		//���� ���� X,Y��ǥ�� ���̶�� �״�� ����.
		if ( !TileSearch (X, Y) )
		{
			return false;
		}
		//���� ��ġ�� �� ��ǥ�� ����� �״�� ����
		if ( X < 0 || X >= _MaxX )
		{
			return false;
		}
		if ( Y < 0 || Y >= _MaxY )
		{
			return false;
		}

		//���� üũ�� Ÿ�Ͽ� �÷� ����
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
			//���� Ÿ�� ���� ���ϰ�� true
			if ( !TileSearch (X - 1, Y) )
			{
				//����Ÿ���� ���� ��� �ٷ� �� ��Ÿ�� ���� ���� �ƴҰ�� ������ġ ��ȯ
				if ( TileSearch (X - 1, Y - 1) )
				{
					*JumpX = X;
					*JumpY = Y;
					return true;
				}
			}
			//���� Ÿ�� ���� �ƴϹǷ� ���� Ÿ�Ϸ� ���� ���� �˻�
			if ( MoveStright (X - 1, Y, MOVE_UU, &checkX, &checkY) )
			{
				*JumpX = X;
				*JumpY = Y;
				return true;
			}
			//�Ʒ� Ÿ�� ���� ���ϰ�� true
			if ( !TileSearch (X , Y + 1) )
			{
				//�Ʒ� Ÿ���� ���� ��� �ٷ� �� ����Ÿ�� ���� ���� �ƴҰ�� ������ġ ��ȯ
				if ( TileSearch (X + 1, Y + 1) )
				{
					*JumpX = X;
					*JumpY = Y;
					return true;
				}
			}
			//�Ʒ��� Ÿ�� ���� �ƴϹǷ� ������ Ÿ�Ϸ� ���� ���� �˻�
			if ( MoveStright (X, Y + 1, MOVE_RR, &checkX, &checkY) )
			{
				*JumpX = X;
				*JumpY = Y;
				return true;
			}

			//�Ѵ� �ƴ� ��� �� ������ ���� ��ĭ ���� �ٽ� �ݺ�
			X = X + 1;
			Y = Y - 1;


			break;

		case MOVE_RD :
			//�� Ÿ�� ���� ���ϰ�� true
			if ( !TileSearch (X, Y - 1) )
			{
				//�� Ÿ���� ���� ��� �ٷ� �� ����Ÿ�� ���� ���� �ƴҰ�� ������ġ ��ȯ
				if ( TileSearch (X + 1, Y - 1) )
				{
					*JumpX = X;
					*JumpY = Y;
					return true;
				}
			}
			//���� Ÿ�� ���� �ƴϹǷ� ������ Ÿ�Ϸ� ���� �˻�
			if ( MoveStright (X, Y - 1, MOVE_RR, &checkX, &checkY) )
			{
				*JumpX = X;
				*JumpY = Y;
				return true;
			}

			//���� Ÿ�� ���� ���ϰ�� true
			if ( !TileSearch (X - 1, Y) )
			{
				//���� Ÿ���� ���� ��� �ٷ� �� �Ʒ�Ÿ�� ���� ���� �ƴҰ�� ������ġ ��ȯ
				if ( TileSearch (X - 1, Y + 1) )
				{
					*JumpX = X;
					*JumpY = Y;
					return true;
				}
			}
			//�Ʒ��� Ÿ�� ���� �ƴϹǷ� �Ʒ��� Ÿ�Ϸ� ���� �˻�
			if ( MoveStright (X - 1, Y , MOVE_DD, &checkX, &checkY) )
			{
				*JumpX = X;
				*JumpY = Y;
				return true;
			}

			//�Ѵ� �ƴ� ��� �Ʒ� ������ ���� ��ĭ ���� �ٽ� �ݺ�
			X = X + 1;
			Y = Y + 1;

			break;

		case MOVE_LD :
			//�� Ÿ�� ���� ���ϰ�� true
			if ( !TileSearch (X, Y - 1) )
			{
				//�� Ÿ���� ���� ��� �ٷ� �� ��Ÿ�� ���� ���� �ƴҰ�� ������ġ ��ȯ
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


			//������ Ÿ�� ���� ���ϰ�� true
			if ( !TileSearch (X + 1, Y) )
			{
				//������ Ÿ���� ���� ��� �ٷ� �� �Ʒ�Ÿ�� ���� ���� �ƴҰ�� ������ġ ��ȯ
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

			//�Ѵ� �ƴ� ��� �Ʒ� ���� ���� ��ĭ ���� �ٽ� �ݺ�
			X = X - 1;
			Y = Y + 1;

			break;
			
		case MOVE_UL :
			//������ Ÿ�� ���� ���ϰ�� true
			if ( !TileSearch (X + 1, Y) )
			{
				//������ Ÿ���� ���� ��� �ٷ� �� ��Ÿ�� ���� ���� �ƴҰ�� ������ġ ��ȯ
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
			//�Ʒ��� Ÿ�� ���� ���ϰ�� true
			if ( !TileSearch (X, Y + 1) )
			{
				//�Ʒ��� Ÿ���� ���� ��� �ٷ� �� ����Ÿ�� ���� ���� �ƴҰ�� ������ġ ��ȯ
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
			//�Ѵ� �ƴ� ��� �Ʒ� ���� ���� ��ĭ ���� �ٽ� �ݺ�
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
	//�� �ȹ������ üũ
	if ( X < 0 || X > _MaxX - 1 )
	{
		return;
	}
	if ( Y < 0 || Y > _MaxY - 1 )
	{
		return;
	}

	//�� ��� Ȯ��
	if ( !TileSearch(X,Y) )
	{
		return;
	}


	//��� ���� ���� �� ����
	Node *NewNode = new Node;

	NewNode->Parent = Parent;
	NewNode->X = X;
	NewNode->Y = Y;

	int ParentDir = (abs (X - Parent->X) + abs (Y - Parent->Y));


	NewNode->H = 10 * (abs (X - _EndX) + abs (Y - _EndY));
	NewNode->G = Parent->G + (MovePay * ParentDir);
	NewNode->F = NewNode->H + NewNode->G;
	NewNode->iDir = iDir;


	//CloseList �˻�
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

	//OpenList �˻�
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


	//���¸���Ʈ�� ����ϰ� return
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


