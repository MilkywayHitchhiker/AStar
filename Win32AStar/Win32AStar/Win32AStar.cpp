// Win32AStar.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "Win32AStar.h"
#include "resource.h"
#include "AStar.h"

#define MAX_LOADSTRING 100

// ���� ����:
HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.

HBRUSH Start;	//GREEN
HBRUSH End;		//RED
HBRUSH block ;	//Gray
HBRUSH Open;	//BLUE
HBRUSH Close;	//YELLOW

HPEN PEN;
HPEN RED;
HBRUSH OldBrush;
HPEN OldPEN;
bool LMouseDOWNBLOCK;
bool LMouseDOWNCLEAR;
bool PathFinding = false;
bool MarkerStart = false;
bool MarkerEnd = false;
AStar::Node *FindEnd;

AStar Star;
int StartX;
int StartY;
int EndX;
int EndY;


// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ���⿡ �ڵ带 �Է��մϴ�.

    // ���� ���ڿ��� �ʱ�ȭ�մϴ�.
	lstrcpyW (szTitle, L"Astar_Search");//���� ǥ���� �ؽ�Ʈ�Դϴ�.
	lstrcpyW (szWindowClass, L"Astar_Main");
	Start = CreateSolidBrush (RGB (102, 255, 102));	//GREEN
	End = CreateSolidBrush (RGB (255, 51, 51));	//RED
	block = CreateSolidBrush (RGB (128, 128, 128));	//Gray

	Open = CreateSolidBrush(RGB(51,153,255));	//BLUE
	Close = CreateSolidBrush(RGB(255,255,153));	//YELLOW


	PEN = CreatePen (PS_SOLID,1,RGB (128, 128, 128));
	RED = CreatePen (PS_SOLID,2,RGB (255, 51, 51));
    MyRegisterClass(hInstance);

    // ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32ASTAR));

    MSG msg;

    // �⺻ �޽��� �����Դϴ�.
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}


//
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32ASTAR));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WIN32ASTAR);
	wcex.style = CS_DBLCLKS;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 1440, 780, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����:  �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND  - ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT    - �� â�� �׸��ϴ�.
//  WM_DESTROY  - ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//







LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{


	int X;
	int Y;


    switch (message)
    {
	case WM_CREATE:
		SetTimer (hWnd, 1, 250, NULL);
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // �޴� ������ ���� �м��մϴ�.
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
			case ID_39999:
				//��ֹ� ����� ó��
				memset (Map, 0, sizeof (Map));
				break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
		break;
	case WM_TIMER:
		switch ( wParam )
		{
		case 1:
			if ( PathFinding )
			{
				FindEnd = Star.PathFind (StartX, StartY, EndX, EndY, false);
				if ( FindEnd != nullptr )
				{
					PathFinding = false;
				}
			}
			InvalidateRect (hWnd, NULL, TRUE);
			Sleep (0);
			break;
		}
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		RECT tile;

		HDC hdc = BeginPaint (hWnd, &ps);

		OldPEN =(HPEN) SelectObject (hdc,PEN);
		
		//������
		for ( int cnt = 0; cnt <= MaxMapX; cnt++ )
		{
			MoveToEx (hdc, cnt * TileSize, 0, NULL);
			LineTo (hdc, cnt * TileSize, MaxMapY * TileSize);
		}
		//������
		for ( int cnt = 0; cnt <= MaxMapY; cnt++ )
		{
			MoveToEx (hdc, 0, cnt * TileSize, NULL);
			LineTo (hdc, MaxMapX * TileSize, cnt * TileSize);
		}

		//BLOCK ���

		OldBrush = ( HBRUSH )SelectObject (hdc,block);
		
		for ( int cntY = 0; cntY < MaxMapY; cntY++ )
		{
			for ( int cntX = 0; cntX < MaxMapX; cntX++ )
			{
				if ( Map[cntY][cntX] == BLOCK )
				{
					Rectangle (hdc, cntX * TileSize, cntY * TileSize, (cntX + 1) * TileSize + 1, (cntY + 1) * TileSize + 1);
				}
			}
		}

		//OpenList�˻�
		SelectObject (hdc, Open);
		for ( int cnt = 0; cnt < OpenListNum; cnt++ )
		{
			if ( OpenList[cnt].F != NULL )
			{
				int X = OpenList[cnt].p->X;
				int Y = OpenList[cnt].p->Y;
				Rectangle (hdc, X *TileSize, Y*TileSize, (X + 1) *TileSize + 1, (Y + 1) *TileSize + 1);
			}
		}

		//CloseList�˻�
		SelectObject (hdc, Close);
		for ( int cntY = 0; cntY < MaxMapY; cntY++ )
		{
			for ( int cntX = 0; cntX < MaxMapX; cntX++ )
			{
				if ( CloseList[cntY][cntX] != NULL )
				{
					int X = CloseList[cntY][cntX]->X;
					int Y = CloseList[cntY][cntX]->Y;
					Rectangle (hdc, X *TileSize, Y*TileSize, (X + 1) *TileSize + 1, (Y + 1) *TileSize + 1);
				}
			}
		}
		
		//������ ǥ��
		if ( MarkerStart )
		{
			tile.left = StartX * TileSize;
			tile.top = StartY * TileSize;
			tile.right = tile.left + TileSize + 1;
			tile.bottom = tile.top + TileSize + 1;

			OldBrush = ( HBRUSH )SelectObject (hdc, Start);
			Rectangle (hdc, tile.left, tile.top, tile.right, tile.bottom);
		}


		//������ ǥ��
		if ( MarkerEnd )
		{
			tile.left = EndX * TileSize;
			tile.top = EndY * TileSize;
			tile.right = tile.left + TileSize + 1;
			tile.bottom = tile.top + TileSize + 1;

			OldBrush = ( HBRUSH )SelectObject (hdc, End);
			Rectangle (hdc, tile.left, tile.top, tile.right, tile.bottom);
		}

		//�������� ���������� ���� �׸���
		SelectObject (hdc, RED);
		if ( FindEnd != nullptr )
		{
			AStar::Node *buffer = FindEnd;
			MoveToEx (hdc, (FindEnd->X) * TileSize + (TileSize / 2), (FindEnd->Y) *TileSize + (TileSize / 2), NULL);
			while ( FindEnd->Parent != NULL )
			{
				LineTo (hdc, FindEnd->Parent->X *TileSize + (TileSize / 2), FindEnd->Parent->Y *TileSize + (TileSize / 2));
				FindEnd = FindEnd->Parent;
			}
			FindEnd = buffer;
		}

		SelectObject (hdc, OldBrush);
		SelectObject (hdc, OldPEN);

		EndPaint (hWnd, &ps);
	}
	break;
	case WM_LBUTTONDBLCLK:
		//������� ����
		StartX = LOWORD (lParam) / TileSize;
		StartY = HIWORD (lParam) / TileSize;
		MarkerStart = true;
		break;
	case WM_RBUTTONDBLCLK:
		PathFinding = true;
		Star.PathFind (StartX, StartY, EndX, EndY, true);
		break;
	case WM_LBUTTONDOWN:
		if ( Map[HIWORD (lParam) / TileSize][LOWORD (lParam)] == BLOCK )
		{
			LMouseDOWNCLEAR = true;
		}
		else
		{
			LMouseDOWNBLOCK = true;
		}
		break;
	case WM_LBUTTONUP:
		LMouseDOWNCLEAR = false;
		LMouseDOWNBLOCK = false;
		break;
	case WM_MOUSEMOVE:
		//��ǥ�� �޾Ƽ� �ʿ� ��ֹ� ��ġ �� ����
		X = LOWORD(lParam) / TileSize;
		Y = HIWORD (lParam) / TileSize;
		if ( (X > 0 && X < MaxMapX) && (Y>0 && Y<MaxMapY) )
		{
			if ( true == LMouseDOWNBLOCK )
			{
				Star.GetMapTileSet (LOWORD (lParam), HIWORD (lParam), BLOCK);
			}
			else if ( true == LMouseDOWNCLEAR )
			{
				Star.GetMapTileSet (LOWORD (lParam), HIWORD (lParam), ROAD);
			}
		}
		break;
		
	case WM_RBUTTONDOWN:
		//�������� ����
		EndX = LOWORD (lParam) / TileSize;
		EndY = HIWORD (lParam) / TileSize;
		MarkerEnd = true;
		break;
	case WM_DESTROY:
		KillTimer (hWnd,0);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}