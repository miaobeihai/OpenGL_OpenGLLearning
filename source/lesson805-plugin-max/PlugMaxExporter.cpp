
#include "plugmaxexporter.h"

PlugMaxExporter::PlugMaxExporter(void)
{
}

PlugMaxExporter::~PlugMaxExporter(void)
{
}


int PlugMaxExporter::ExtCount()
{
	//TODO: Returns the number _of file name extensions supported by the plug-in.
	return 1;
}

const TCHAR *PlugMaxExporter::Ext(int n)
{		
	//TODO: Return the 'i-th' file name extension (i.e. "3DS").
	return _T("OGLMesh");
}

const TCHAR *PlugMaxExporter::LongDesc()
{
	//TODO: Return long ASCII description (i.e. "Targa 2.0 Image File")
	return _T("OpenGL Export");
}

const TCHAR *PlugMaxExporter::ShortDesc() 
{			
	//TODO: Return short ASCII description (i.e. "Targa")
	return _T("OpenGL Export");
}

const TCHAR *PlugMaxExporter::AuthorName()
{			
	//TODO: Return ASCII Author name
	return _T("Matrix");
}

const TCHAR *PlugMaxExporter::CopyrightMessage() 
{	
	// Return ASCII Copyright message
	return _T("");
}

const TCHAR *PlugMaxExporter::OtherMessage1() 
{		
	//TODO: Return Other message #1 if any
	return _T("");
}

const TCHAR *PlugMaxExporter::OtherMessage2() 
{		
	//TODO: Return other message #2 in any
	return _T("");
}

unsigned int PlugMaxExporter::Version()
{				
	//TODO: Return Version number * 100 (i.e. v3.01 = 301)
	return 200;
}

void PlugMaxExporter::ShowAbout(HWND hWnd)
{			
	// Optional
}

BOOL PlugMaxExporter::SupportsOptions(int ext, DWORD options)
{
	return TRUE;
}

//bool	CreateChoiceDlg( IGameScene* pGame ,const string& strFileName);
int		PlugMaxExporter::DoExport(const TCHAR* flleName,
								  ExpInterface* pExp,
								  Interface * pInterface,
								  BOOL suppressPrompts /* = FALSE */,
								  DWORD options /* = 0 */)
{
    bool        bSelect	=	(options & SCENE_EXPORT_SELECTED) ? true : false;
    IGameScene* pGame   =	GetIGameInterface();

    IGameConversionManager* pConMg = GetConversionManager();
    pConMg->SetCoordSystem(IGameConversionManager::IGAME_OGL);
    pGame->InitialiseIGame(bSelect);
    pGame->SetStaticFrame(0);
    //CreateChoiceDlg(pGame,flleName);		
    pGame->ReleaseIGame();
	return	1;
	
}


class PlugMeshDesc:public ClassDesc2 {
public:
    int 			IsPublic() { return TRUE; }
    void *			Create(BOOL loading = FALSE) { return new PlugMaxExporter(); }
    const TCHAR *	ClassName() { return _T("OpenGL-plugin"); }
    SClass_ID		SuperClassID() { return SCENE_EXPORT_CLASS_ID; }
    Class_ID		ClassID() { return Class_ID(0x3ccc3b15, 0x3ad2611f); }
    const TCHAR* 	Category() { return _T("OpenGL"); }

    const TCHAR*	InternalName() { return _T("OpenGL-plugin"); }
    HINSTANCE		HInstance() { return 0; }

};

static PlugMeshDesc g_GameExporterDesc;
__declspec( dllexport ) const TCHAR* LibDescription()
{
    return _T("OpenGL Export Max plug");
}

//TODO: Must change this number when adding a new class
__declspec( dllexport ) int LibNumberClasses()
{
    return 1;
}


__declspec( dllexport ) ClassDesc* LibClassDesc(int i)
{
    return	&g_GameExporterDesc;
}


__declspec( dllexport ) ULONG LibVersion()
{
    return VERSION_3DSMAX;
}

// Let the plug-in register itself for deferred loading
__declspec( dllexport ) ULONG CanAutoDefer()
{
    return 1;
}

TCHAR *GetString(int id)
{
    return _T("OpenGL");
}

