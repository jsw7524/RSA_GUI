#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <stdio.h>
#include <Commdlg.h>
#include <math.h>

extern unsigned long long P,Q,E,D,PQ,PQ_Len;

extern void GenerateKeys();
extern unsigned long long MyEncrypt(unsigned long long M);
extern unsigned long long MyDecrypt(unsigned long long C);
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
               0,                   /* Extended possibilites for variation */
               szClassName,         /* Classname */
               _T("JSW's RSA Maker"),       /* Title Text */
               WS_SYSMENU|WS_MINIMIZEBOX, /* default window */
               CW_USEDEFAULT,       /* Windows decides the position */
               CW_USEDEFAULT,       /* where the window ends up on the screen */
               305,                 /* The programs width */
               129,                 /* and height in pixels */
               HWND_DESKTOP,        /* The window is a child-window to desktop */
               NULL,                /* No menu */
               hThisInstance,       /* Program Instance handler */
               NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */


    HWND HwndButton1 = CreateWindow(
                           "BUTTON",  // Predefined class; Unicode assumed
                           "Generate Keys",      // Button text
                           WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
                           0,         // x position
                           0,         // y position
                           100,        // Button width
                           100,        // Button height
                           hwnd,     // Parent window
                           NULL,       // No menu.
                           (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
                           NULL);      // Pointer not needed.

    SetWindowLong(HwndButton1,   GWL_ID,  101);

    HWND HwndButton2 = CreateWindow(
                           "BUTTON",  // Predefined class; Unicode assumed
                           "Encryt",      // Button text
                           WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
                           100,         // x position
                           0,         // y position
                           100,        // Button width
                           100,        // Button height
                           hwnd,     // Parent window
                           NULL,       // No menu.
                           (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
                           NULL);
    SetWindowLong(HwndButton2,   GWL_ID,  102);

    HWND HwndButton3 = CreateWindow(
                           "BUTTON",  // Predefined class; Unicode assumed
                           "Decryt",      // Button text
                           WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
                           200,         // x position
                           0,         // y position
                           100,        // Button width
                           100,        // Button height
                           hwnd,     // Parent window
                           NULL,       // No menu.
                           (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
                           NULL);
    SetWindowLong(HwndButton3,   GWL_ID,  103);

    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HANDLE FilePub,FilePri,TargetFile,EncryptedFile,DecryptedFile;
    DWORD I,J,K;
    OPENFILENAME MyFileName ;
    char FileName[100] ;
    unsigned char RD;
    unsigned long long Data;

    switch (message)                  /* handle the messages */
    {
    case WM_CREATE:
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case 101:
            GenerateKeys();
            FilePub=CreateFile("PublicKey.hex", GENERIC_WRITE, FILE_SHARE_READ, 0, 2, FILE_ATTRIBUTE_NORMAL, NULL);
            FilePri=CreateFile("PrivateKey.hex", GENERIC_WRITE, FILE_SHARE_READ, 0, 2, FILE_ATTRIBUTE_NORMAL, NULL);
            printf("%016llx %016llx\n",E,PQ);
            WriteFile(FilePub,&E,8,&(J),NULL);
            WriteFile(FilePub,&PQ,8,&(J),NULL);
            WriteFile(FilePri,&D,8,&(J),NULL);
            WriteFile(FilePri,&PQ,8,&(J),NULL);
            CloseHandle(FilePub);
            CloseHandle(FilePri);
            break;
        case 102:
            ZeroMemory( &MyFileName , sizeof( MyFileName));
            MyFileName.lStructSize = sizeof ( MyFileName );
            MyFileName.hwndOwner = NULL ;
            MyFileName.lpstrFile = FileName ;
            MyFileName.lpstrFile[0] = '\0';
            MyFileName.nMaxFile = sizeof( FileName );
            MyFileName.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
            MyFileName.nFilterIndex =1;
            MyFileName.lpstrFileTitle = NULL ;
            MyFileName.nMaxFileTitle = 0 ;
            MyFileName.lpstrInitialDir=NULL ;
            MyFileName.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST ;
            //GetOpenFileName( &MyFileName );
            FilePub=CreateFile("PublicKey.hex", GENERIC_READ, FILE_SHARE_READ, 0, 4, FILE_ATTRIBUTE_NORMAL, NULL);
            if(FilePub == INVALID_HANDLE_VALUE)
                break;
            ReadFile(FilePub,&E,8,&(J),NULL);
            ReadFile(FilePub,&PQ,8,&(J),NULL);
            //printf("%llx\n",PQ);
            CloseHandle(FilePub);
            GetOpenFileName( &MyFileName );
            PQ_Len=ceil(log(PQ))/8;
            TargetFile=CreateFile(MyFileName.lpstrFile, GENERIC_READ, FILE_SHARE_READ, 0, 4, FILE_ATTRIBUTE_NORMAL, NULL);
            if(TargetFile == INVALID_HANDLE_VALUE)
                break;
            EncryptedFile=CreateFile("Test.rsa", GENERIC_WRITE, FILE_SHARE_READ, 0, 2, FILE_ATTRIBUTE_NORMAL, NULL);
            do
            {
                Data=0;
                ReadFile(TargetFile,&Data,PQ_Len,&(K),NULL);
                if (K==0)
                    break;
                Data=MyEncrypt(Data);
                WriteFile(EncryptedFile,&Data,8,&(J),NULL);
            }
            while (1);
            CloseHandle(TargetFile);
            CloseHandle(EncryptedFile);
            MessageBox(hwnd,"Encryption Done","OK",0);
            printf("Encryption Done\n");
            break;
        case 103:
            ZeroMemory( &MyFileName , sizeof( MyFileName));
            MyFileName.lStructSize = sizeof ( MyFileName );
            MyFileName.hwndOwner = NULL ;
            MyFileName.lpstrFile = FileName ;
            MyFileName.lpstrFile[0] = '\0';
            MyFileName.nMaxFile = sizeof( FileName );
            MyFileName.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
            MyFileName.nFilterIndex =1;
            MyFileName.lpstrFileTitle = NULL ;
            MyFileName.nMaxFileTitle = 0 ;
            MyFileName.lpstrInitialDir=NULL ;
            MyFileName.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST ;
            //GetOpenFileName( &MyFileName );
            FilePri=CreateFile("PrivateKey.hex", GENERIC_READ, FILE_SHARE_READ, 0, 4, FILE_ATTRIBUTE_NORMAL, NULL);
            if(FilePri == INVALID_HANDLE_VALUE)
                break;
            ReadFile(FilePri,&D,8,&(J),NULL);
            ReadFile(FilePri,&PQ,8,&(J),NULL);
            //printf("%llx\n",PQ);
            CloseHandle(FilePri);
            PQ_Len=ceil(log(PQ))/8;
            GetOpenFileName( &MyFileName );
            TargetFile=CreateFile(MyFileName.lpstrFile, GENERIC_READ, FILE_SHARE_READ, 0, 4, FILE_ATTRIBUTE_NORMAL, NULL);
            if(TargetFile == INVALID_HANDLE_VALUE)
                break;
            DecryptedFile=CreateFile("deTest.txt", GENERIC_WRITE, FILE_SHARE_READ, 0, 2, FILE_ATTRIBUTE_NORMAL, NULL);
            do
            {
                Data=0;
                ReadFile(TargetFile,&Data,8,&(K),NULL);
                if (K==0)
                    break;
                Data=MyDecrypt(Data);
                WriteFile(DecryptedFile,&Data,PQ_Len,&(J),NULL);
            }
            while (1);
            CloseHandle(TargetFile);
            CloseHandle(DecryptedFile);
            MessageBox(hwnd,"Decryption Done","OK",0);
            printf("Decryption Done\n");
            break;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
        break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc (hwnd, message, wParam, lParam);
    }
    return 0;
}
