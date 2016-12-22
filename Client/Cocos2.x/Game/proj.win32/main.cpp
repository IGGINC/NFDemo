#include "main.h"
#include "AppDelegate.h"
#include "CCEGLView.h"

USING_NS_CC;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
	
	AllocConsole();   
	freopen( "CONIN$", "r+t", stdin); // �ض��� STDIN  
	freopen("CONOUT$", "w+t", stdout); // �ض���STDOUT  

    // create the application instance
    AppDelegate app;
    CCEGLView* eglView = CCEGLView::sharedOpenGLView();
    eglView->setViewName("Game");
    eglView->setFrameSize(480, 320);
    return CCApplication::sharedApplication()->run();
}
