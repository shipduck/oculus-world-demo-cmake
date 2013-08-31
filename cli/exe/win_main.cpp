// Ŭnicode please
#include <Windows.h>
#include "Kernel/OVR_System.h"
#include "Kernel/OVR_Array.h"
#include "Kernel/OVR_String.h"

#include "Platform/Win32_Platform.h"
#include "Platform/Win32_Gamepad.h"
#include "Render/Render_Device.h"

/*
int main(int argc, char *argv[])
{
	return 0;
}
*/

int WINAPI WinMain(HINSTANCE hinst, HINSTANCE prevInst, LPSTR inArgs, int show)
{
    using namespace OVR;
    using namespace OVR::Platform;

    OVR_UNUSED2(prevInst, show);
    
    // CreateApplication must be the first call since it does OVR::System::Initialize.
    Application*     app = Application::CreateApplication();
    Win32::PlatformCore* platform = new Win32::PlatformCore(app, hinst);
    // The platform attached to an app will be deleted by DestroyApplication.
    app->SetPlatformCore(platform);

    int exitCode = 0;

    // Nested scope for container destructors to shutdown before DestroyApplication.
    {
        Array<String>      args;
        Array<const char*> argv;
        argv.PushBack("app");

        const char* p = inArgs;
        const char* pstart = inArgs;
        while (*p)
        {
            if (*p == ' ')
            {
                args.PushBack(String(pstart, p - pstart));
                while (*p == ' ')
                    p++;
                pstart = p;
            }
            else
            {
                p++;
            }
        }
        if (p != pstart)
            args.PushBack(String(pstart, p - pstart));
        for (UPInt i = 0; i < args.GetSize(); i++)
            argv.PushBack(args[i].ToCStr());

        exitCode = app->OnStartup((int)argv.GetSize(), &argv[0]);
        if (!exitCode)
            exitCode = platform->Run();
    }

    // No OVR functions involving memory are allowed after this.
    Application::DestroyApplication(app);
    app = 0;

    OVR_DEBUG_STATEMENT(_CrtDumpMemoryLeaks());
    return exitCode;
}
