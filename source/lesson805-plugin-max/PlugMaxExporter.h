#pragma once


#include <max.h>
#include <IGame/IGame.h>
#include <IGame/IGameObject.h>
#include <IGame/IGameProperty.h>
#include <IGame/IGameControl.h>
#include <IGame/IGameModifier.h>
#include <IGame/IConversionManager.h>
#include <IGame/IGameError.h>
#include <IGame/IGameFX.h>
#include <IGame/IGameObject.h>
#include <IGame/IConversionManager.h >
#include <3dsmaxport.h>
#include <iparamb2.h>
#include <decomp.h>
#include <string>
#include <iparamm2.h>
#include <modstack.h>
#include <iskin.h>

#include <string>
using namespace std;

class PlugMaxExporter : public SceneExport 
{
public:
	PlugMaxExporter(void);
	~PlugMaxExporter(void);
	int				ExtCount();					// Number of extensions supported
	const TCHAR *	Ext(int n);					// Extension #n (i.e. "3DS")
	const TCHAR *	LongDesc();					// Long ASCII description (i.e. "Autodesk 3D Studio File")
	const TCHAR *	ShortDesc();				// Short ASCII description (i.e. "3D Studio")
	const TCHAR *	AuthorName();				// ASCII Author name
	const TCHAR *	CopyrightMessage();			// ASCII Copyright message
	const TCHAR *	OtherMessage1();			// Other message #1
	const TCHAR *	OtherMessage2();			// Other message #2
	unsigned int	Version();					// Version number * 100 (i.e. v3.01 = 301)
	void			ShowAbout(HWND hWnd);		// Show DLL's "About..." box

	BOOL			SupportsOptions(int ext, DWORD options);
	int				DoExport(const TCHAR *strName,ExpInterface* pExport,Interface* pInterface, BOOL suppressPrompts=FALSE, DWORD options=0);


public:
};