// Win32AStar.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Win32AStar.h"
#include "resource.h"
//#include "AStar.h"
#include "JPS.h"

#define MAX_LOADSTRING 100


#define MaxMapX 64
#define MaxMapY 40
#define TileSize 16

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

HBRUSH Start;	//GREEN
HBRUSH End;		//RED
HBRUSH block ;	//Gray
HBRUSH Open;	//BLUE
HBRUSH Close;	//YELLOW
HBRUSH Color;

HPEN PEN;
HPEN RED;
HBRUSH OldBrush;
HPEN OldPEN;
bool LMouseDOWNBLOCK;
bool LMouseDOWNCLEAR;
bool PathFinding = false;
bool MarkerStart = false;
bool MarkerEnd = false;
JPS::Node *FindEnd;

//AStar Star;
int StartX;
int StartY;
int EndX;
int EndY;
JPS Astar(MaxMapX,MaxMapY);

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
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

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
	lstrcpyW (szTitle, L"Astar_Search");//제목 표시줄 텍스트입니다.
	lstrcpyW (szWindowClass, L"Astar_Main");

    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32ASTAR));

    MSG msg;

    // 기본 메시지 루프입니다.
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
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
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
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

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
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//





LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{


	int X;
	int Y;


    switch (message)
    {
	case WM_CREATE:
		Start = CreateSolidBrush (RGB (102, 255, 102));	//GREEN
		End = CreateSolidBrush (RGB (255, 51, 51));	//RED
		block = CreateSolidBrush (RGB (128, 128, 128));	//Gray

		Open = CreateSolidBrush (RGB (51, 153, 255));	//BLUE
		Close = CreateSolidBrush (RGB (255, 255, 153));	//YELLOW


		PEN = CreatePen (PS_SOLID, 1, RGB (128, 128, 128));
		RED = CreatePen (PS_SOLID, 2, RGB (255, 51, 51));
		SetTimer (hWnd, 1, 150, NULL);
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다.
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
			case ID_39999:
				//장애물 지우기 처리
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
				FindEnd = Astar.Jump (StartX, StartY, EndX, EndY, false);
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
		
		//세로줄
		for ( int cnt = 0; cnt <= MaxMapX; cnt++ )
		{
			MoveToEx (hdc, cnt * TileSize, 0, NULL);
			LineTo (hdc, cnt * TileSize, MaxMapY * TileSize);
		}
		//가로줄
		for ( int cnt = 0; cnt <= MaxMapY; cnt++ )
		{
			MoveToEx (hdc, 0, cnt * TileSize, NULL);
			LineTo (hdc, MaxMapX * TileSize, cnt * TileSize);
		}


		//타일 색칠하기

		for ( int cntY = 0; cntY < MaxMapY; cntY++ )
		{
			for ( int cntX = 0; cntX < MaxMapX; cntX++ )
			{
				Color = CreateSolidBrush (Astar.Map[cntY][cntX].rgb);
				( HBRUSH )SelectObject (hdc, Color);
				Rectangle (hdc, cntX * TileSize, cntY * TileSize, (cntX + 1) * TileSize + 1, (cntY + 1) * TileSize + 1);
				DeleteObject (Color);

			}
		}

		//BLOCK 출력
		for ( int cntY = 0; cntY < MaxMapY; cntY++ )
		{
			for ( int cntX = 0; cntX < MaxMapX; cntX++ )
			{
				if ( !Astar.TileSearch (cntX, cntY) )
				{
					Rectangle (hdc, cntX * TileSize, cntY * TileSize, (cntX + 1) * TileSize + 1, (cntY + 1) * TileSize + 1);
				}
			}
		}


		//OpenList검색
		SelectObject (hdc, Open);
		multiset<JPS::Node *>::iterator IterOpen;
		for ( IterOpen = Astar.OpenList.begin (); IterOpen != Astar.OpenList.end (); IterOpen++ )
		{
			JPS::Node *p = *IterOpen;
			int X = p->X;
			int Y = p->Y;
			Rectangle (hdc, X *TileSize, Y*TileSize, (X + 1) *TileSize + 1, (Y + 1) *TileSize + 1);
		}

		//CloseList검색
		SelectObject (hdc, Close);
		multiset<JPS::Node *>::iterator IterClose;
		for ( IterClose = Astar.CloseList.begin (); IterClose != Astar.CloseList.end (); IterClose++ )
		{
			JPS::Node *p = *IterClose;
			int X = p->X;
			int Y = p->Y;
			Rectangle (hdc, X *TileSize, Y*TileSize, (X + 1) *TileSize + 1, (Y + 1) *TileSize + 1);
		}
		

		//시작점 표시
		if ( MarkerStart )
		{
			tile.left = StartX * TileSize;
			tile.top = StartY * TileSize;
			tile.right = tile.left + TileSize + 1;
			tile.bottom = tile.top + TileSize + 1;

			OldBrush = ( HBRUSH )SelectObject (hdc, Start);
			Rectangle (hdc, tile.left, tile.top, tile.right, tile.bottom);
		}


		//도착점 표시
		if ( MarkerEnd )
		{
			tile.left = EndX * TileSize;
			tile.top = EndY * TileSize;
			tile.right = tile.left + TileSize + 1;
			tile.bottom = tile.top + TileSize + 1;

			OldBrush = ( HBRUSH )SelectObject (hdc, End);
			Rectangle (hdc, tile.left, tile.top, tile.right, tile.bottom);
		}

		//도착점에 도달했을때 라인 그리기
		SelectObject (hdc, RED);
		if ( FindEnd != nullptr )
		{
			JPS::Node *buffer = FindEnd;
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
		//출발지점 설정
		X = LOWORD (lParam) / TileSize;
		Y = HIWORD (lParam) / TileSize;
		if ( (X >= 0 && X < MaxMapX) && (Y>= 0 && Y < MaxMapY) )
		{
			StartX = X;
			StartY = Y;
			MarkerStart = true;
		}
		break;
	case WM_RBUTTONDBLCLK:
		PathFinding = true;
		Astar.Jump (StartX, StartY, EndX, EndY, true);
		break;
	case WM_LBUTTONDOWN:
		if ( !Astar.TileSearch (LOWORD (lParam) / TileSize, HIWORD (lParam) / TileSize) )
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
		//좌표를 받아서 맵에 장애물 설치 및 해제
		X = LOWORD(lParam) / TileSize;
		Y = HIWORD (lParam) / TileSize;
		if ( (X >= 0 && X < MaxMapX) && (Y>=0 && Y<MaxMapY) )
		{
			if ( true == LMouseDOWNBLOCK )
			{
				Astar.MapTileSet (X, Y, BLOCK);
			}
			else if ( true == LMouseDOWNCLEAR )
			{
				Astar.MapTileSet (X, Y, ROAD);
			}
		}
		break;
		
	case WM_RBUTTONDOWN:
		//도착지점 설정
		X = LOWORD (lParam) / TileSize;
		Y = HIWORD (lParam) / TileSize;
		if ( (X > 0 && X < MaxMapX) && (Y > 0 && Y < MaxMapY) )
		{
			EndX = X;
			EndY = Y;
			MarkerEnd = true;
		}
		break;
	case WM_DESTROY:
		DeleteObject (Start);	//GREEN
		DeleteObject (End);		//RED
		DeleteObject (block);	//Gray
		DeleteObject (Open);	//BLUE
		DeleteObject (Close);	//YELLOW

		DeleteObject (PEN);
		DeleteObject (RED);
		DeleteObject (OldBrush);
		DeleteObject (OldPEN);
		KillTimer (hWnd,0);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
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
