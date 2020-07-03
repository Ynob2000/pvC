//#include "Includes.h"
#include "tarkov/GameSpecific/Tarkov/Tarkov.h"
#include "tarkovread.h"
extern "C" {
#include "common/ivshmem.h"
#include "common/debug.h"
}
#include <chrono>
#include <thread>
#include "render.h"

using namespace std::chrono_literals;

struct test{
    int a;
};

bool waitOk(void* mem)
{
    return *static_cast<char*>(mem) == 'Y';
}

void send_data(IVSHMEM* shm, TarkovESPArray* data)
{
    void* memory = shm->mem;
    size_t size = data->size;
    TarkovESPObject * array = data->array;
    for (size_t t = 0; t < size; ++t) {
        memcpy((void*)((uintptr_t)memory + t * sizeof(TarkovESPObject)),
                array + t,
                sizeof(TarkovESPObject));
    }
}

bool prepare_ivshmem(IVSHMEM* shm)
{
    ivshmemOptionsInit();
    if (!ivshmemOpen(shm))
    {
        DEBUG_ERROR("Failed to map memory");
        return false;
    }
    return true;
}


float getFactorFromDistance(float distance)
{
    if (distance > 200.f)
        return 0.f;
    return pow(1 - pow(distance / 200.f, 0.4), 3);
}

void main_loop_win(TarkovGame* tarkov, IVSHMEM* shm) {
    TarkovESPArray dataarray;
    initArray(&dataarray, 100);
    while (true)
    {
        // ... all events processed, now do other stuff ...
        bool ret = tarkov->Tick();
        if (!ret)
            break;

        GetTarkovPlayers(tarkov, &dataarray, 1920, 1080);
        GetTarkovLoot(tarkov, &dataarray, 1920, 1080);
        send_data(shm, &dataarray);
    }
}

void main_loop(TarkovGame* tarkov)
{
    TarkovESPArray playerarray;
    initArray(&playerarray, 50);
    Display    * display = XOpenDisplay( 0 ) ;
    const char * xserver = getenv( "DISPLAY" ) ;

    if (display == 0)
    {
        printf("Could not establish a connection to X-server '%s'\n", xserver ) ;
        exit(1) ;
    }

    // query Visual for "TrueColor" and 32 bits depth (RGBA)
    XVisualInfo visualinfo ;
    XMatchVisualInfo(display, DefaultScreen(display), 32, TrueColor, &visualinfo);

    // create window
    Window   win ;
    GC       gc ;
    XSetWindowAttributes attr ;
    attr.colormap   = XCreateColormap( display, DefaultRootWindow(display), visualinfo.visual, AllocNone) ;
    attr.event_mask = ExposureMask | KeyPressMask ;
    attr.background_pixmap = None ;
    attr.border_pixel      = 0 ;
    win = XCreateWindow(    display, DefaultRootWindow(display),
                            0, 0, 1920, 1080, // x,y,width,height : are possibly opverwriteen by window manager
                            0,
                            visualinfo.depth,
                            InputOutput,
                            visualinfo.visual,
                            CWColormap|CWEventMask|CWBackPixmap|CWBorderPixel,
                            &attr
    ) ;
    gc = XCreateGC( display, win, 0, 0) ;

    // set title bar name of window
    XStoreName( display, win, "Transparent Window with OpenGL Support" ) ;

    // say window manager which position we would prefer
    XSizeHints sizehints ;
    sizehints.flags = PPosition | PSize ;
    sizehints.x     = 0 ;  sizehints.y = 0 ;
    sizehints.width = 1920 ; sizehints.height = 1080 ;
    XSetWMNormalHints( display, win, &sizehints ) ;
    // Switch On >> If user pressed close key let window manager only send notification >>
    Atom wm_delete_window = XInternAtom( display, "WM_DELETE_WINDOW", 0) ;
    XSetWMProtocols( display, win, &wm_delete_window, 1) ;

    {
        // change foreground color to brown
        XColor    xcol ;
        xcol.red   = 153 * 256 ;   // X11 uses 16 bit colors !
        xcol.green = 0 * 256 ;
        xcol.blue  = 0  * 256 ;
        XAllocColor( display, attr.colormap, &xcol) ;
        XGCValues gcvalues ;
        gcvalues.foreground = xcol.pixel ;
        XChangeGC( display, gc, GCForeground, &gcvalues) ;
    }

    // create OpenGL context
    GLXContext glcontext = glXCreateContext( display, &visualinfo, 0, True ) ;
    if (!glcontext)
    {
        printf("X11 server '%s' does not support OpenGL\n", xserver ) ;
        exit(1) ;
    }
    glXMakeCurrent( display, win, glcontext ) ;

    // now let the window appear to the user
    XMapWindow( display, win) ;


    int isUserWantsWindowToClose = 0 ;

    while( !isUserWantsWindowToClose )
    {
        int  isRedraw = 0 ;

        /* XPending returns number of already queued events.
         * If no events are queued XPending sends all queued requests to the X-server
         * and tries to read new incoming events. */

        while( XPending(display) > 0 )
        {
            // process event
            XEvent   event ;
            XNextEvent( display, &event) ;

            switch(event.type)
            {  // see 'man XAnyEvent' for a list of available events
                case ClientMessage:
                    // check if the client message was send by window manager to indicate user wants to close the window
                    if (  event.xclient.message_type  == XInternAtom( display, "WM_PROTOCOLS", 1)
                          && event.xclient.data.l[0]  == XInternAtom( display, "WM_DELETE_WINDOW", 1)
                            )
                    {
                        isUserWantsWindowToClose = 1 ;
                    }
                case KeyPress:
                    if (XLookupKeysym(&event.xkey, 0) == XK_Escape)
                    {
                        isUserWantsWindowToClose = 1 ;
                    }
                    break ;
                case Expose:
                    isRedraw = 1 ;
                    break ;
                default:
                    // do no thing
                    break ;
            }

        }

        // ... all events processed, now do other stuff ...
        bool ret = tarkov->Tick();
        if (!ret)
            break;

        GetTarkovPlayers(tarkov, &playerarray, 1920, 1080);
        glClearColor( 0., 0., 0., 0) ;
        glClear(GL_COLOR_BUFFER_BIT) ;
        glXSwapBuffers( display, win) ;
        glXWaitGL() ;
        for (int i = 0; i < playerarray.used; i++)
        {
            TarkovESPObject Player = playerarray.array[i];
            char buf[100];
            sprintf(buf, "%s (%5.2fm)", Player.IsScavPlayer ? "Player Scav" : Player.pName, Player.inGameDistance);
            std::string buff = buf; // ugly
            if (!Player.render)
                continue;
            XRectangle rect;
            rect.width=300*getFactorFromDistance(Player.inGameDistance);
            rect.height=1000*getFactorFromDistance(Player.inGameDistance);
            rect.x=(Player.x + 1920) / 2 - rect.width / 2;
            rect.y=  (-Player.y + 1080)/2 - rect.height / 2;

            XDrawString( display, win, gc, rect.x, rect.y, buff.c_str(), buff.length()) ;
            XDrawRectangles(display, win, gc, &rect, 1);
        }

        if (isRedraw)
        {  // needs redraw
            // use opengl to clear background in (transparent) light grey
            glClearColor( 0., 0., 0., 0) ;
            glClear(GL_COLOR_BUFFER_BIT) ;
            glXSwapBuffers( display, win) ;
            glXWaitGL();
        }
        XClearWindow(display, win);
        XFlush(display);
        // ... do something else ...

    }

    XDestroyWindow( display, win ) ;
    win = 0 ;
    XCloseDisplay( display ) ;
    display = 0 ;
}


int main()
{
    IVSHMEM shm;
    if (!prepare_ivshmem(&shm))
    {
        DEBUG_ERROR("Failed to prepare IVSHMEM");
        return 1;
    }
    TarkovGame *Tarkov;
    while (!GetTarkovGame(&Tarkov, "EscapeFromTarkov.exe", "UnityPlayer.dll"))
    {
        DEBUG_ERROR("Failed to find Tarkov");
        std::this_thread::sleep_for(2s);
    }
    while (!TarkovInGame(Tarkov))
    {
        DEBUG_INFO("Waiting for game to start");
        Tarkov->Tick();
        std::this_thread::sleep_for(2s);
    }
    //main_loop(Tarkov);
    main_loop_win(Tarkov, &shm);
}
