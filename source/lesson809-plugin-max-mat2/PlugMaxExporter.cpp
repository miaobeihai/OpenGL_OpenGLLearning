
#include "plugmaxexporter.h"

#include <sstream>

using namespace std;

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
    
    int	iSize	=	pGame->GetTopLevelNodeCount();
    for ( int i = 0 ; i < iSize ; ++ i )
    {
        IGameNode*		pNode	=	pGame->GetTopLevelNode(i);
        EnumNode(pNode);						
    }
    
    //CreateChoiceDlg(pGame,flleName);	
    pGame->ReleaseIGame();
	return	1;
	
}

void PlugMaxExporter::EnumNode( IGameNode* pNode )
{
    string	        strT    =	pNode->GetName();
    IGameObject*    obj     =	pNode->GetIGameObject();
    IGameObject::ObjectTypes oType =    obj->GetIGameType();
    switch( oType )
    {
    case IGameObject::IGAME_LIGHT:
        {
        }
        break;
    case IGameObject::IGAME_MESH:
        {
            //! 导出

            IGameMesh*  mesh    =   (IGameMesh*)obj;
            ExortNode(mesh);
        }
        break;
    case IGameObject::IGAME_CAMERA:
        {
        }
        break;
    }
    /**
    *   是否存在修改器，过滤并得到骨骼动画
    */
    int numMod = obj->GetNumModifiers();
    for( int i = 0;i < numMod ; i++ )
    {
        IGameModifier*	modify = obj->GetIGameModifier(i);
        //EnumSkin(modify,pNode);
    }
    for(int i=0;i<pNode->GetChildCount();i++)
    {
        IGameNode * newchild = pNode->GetNodeChild(i);
        if(newchild->IsTarget())
            continue;
        EnumNode(newchild);
    }
}


void PlugMaxExporter::ExortNode( IGameMesh* pMesh )
{
    pMesh->GetMaxMesh()->buildNormals();

    if( !pMesh->InitializeData() )
    {
        return	;
    }
    int vertCount   =   pMesh->GetNumberOfVerts();
    int faceCount   =   pMesh->GetNumberOfFaces();

    

    //!	下面是根据采制把面分组
    for (int i = 0; i < faceCount;  ++ i)
    {
        Tab<int> texMaps		=	pMesh->GetActiveMapChannelNum();
        FaceEx* face			=	pMesh->GetFace(i);
        IGameMaterial*  pMatX	=	pMesh->GetMaterialFromFace(face);

        FACE    inds;
        for ( int vi = 0; vi < 3; ++vi )
        {
            MAXVertex   vert;
            vert.pos    =   pMesh->GetVertex(face->vert[vi]);
            
            pMesh->GetNormal(face->norm[vi], vert.nor);

            vert.uv =   pMesh->GetTexVertex(face->texCoord[vi]);

            int     index   =   AddVertex(vert);

            if (index == -1)
            {
                index   =   _vertex.size();
                _vertex.push_back(vert);
            }
            inds.p[vi]  =   index;
        }	
        _faceMap[pMatX].push_back(inds);
    }


    ToFile("c:/t.xml");
    
}

int PlugMaxExporter::AddVertex( const MAXVertex& vert )
{
    for (int i = 0 ;i < _vertex.size(); ++ i)
    {
        MAXVertex&    cur =   _vertex[i];
        if (  vert.pos.x == cur.pos.x
            &&vert.pos.y ==cur.pos.y
            &&vert.pos.z ==cur.pos.z
            &&vert.nor.x ==cur.nor.x
            &&vert.nor.y ==cur.nor.y
            &&vert.nor.z ==cur.nor.z
            &&vert.uv.x == cur.uv.x  
            &&vert.uv.y == cur.uv.y   )
        {
            return  i;
        }
    }

    return  -1;
}

void PlugMaxExporter::ToFile( const char* fileName )
{
    FILE*   file    =   fopen(fileName,"a+");
    fprintf(file,"<root>\n");

    fprintf(file,"   <vertex count = \"%d\">\n",_vertex.size());
    for (size_t i = 0 ;i < _vertex.size(); ++ i)
    {
        MAXVertex&  vert    =   _vertex[i];
        fprintf(file,"     <v p=\"%f %f %f %f %f %f %f %f \"/>\n"
            ,vert.pos.x
            ,vert.pos.y
            ,vert.pos.z
            ,vert.nor.x
            ,vert.nor.y
            ,vert.nor.z
            ,vert.uv.x
            ,vert.uv.y
            );
    }
    fprintf(file,"   </vertex>\n");

    fprintf(file,"   <face>\n");
    
    for (FaceMaterial::iterator itr = _faceMap.begin();itr != _faceMap.end() ; ++ itr )
    {
        IGameMaterial*  mat     =   itr->first;
        ArrayFace&      face    =   itr->second;
        fprintf(file,"      <faces count = \"%d\" material = \"%s\">\n",face.size(),mat->GetMaterialName());
       
        for (size_t i = 0 ;i < face.size(); ++ i)
        {
            FACE&  index    =   face[i];
            fprintf(file,"      <f p=\"%d %d %d\"/>\n"
                ,(unsigned int)index.p[0]
                ,(unsigned int)index.p[1]
                ,(unsigned int)index.p[2]
                );
        }
        fprintf(file,"      </faces>\n");
    }
    
    fprintf(file,"   </face>\n");
    fprintf(file,"</root>\n");
    fclose(file);

    for (FaceMaterial::iterator itr = _faceMap.begin();itr != _faceMap.end() ; ++ itr )
    {
        IGameMaterial*  pMat    =   itr->first;
        const char*     pName   =   pMat->GetMaterialName();

        char            szFile[1024];
        sprintf(szFile,"c:/%s.material",pName);

        ExportMaterial(pMat,szFile);
    }
}

void PlugMaxExporter::ExportMaterial( IGameMaterial* pMat,const char* fileName )
{
    FILE*   pFile   =   fopen(fileName,"wt");
    IGameProperty*  pProp =   pMat->GetAmbientData();

   
    pProp	= pMat->GetAmbientData(); 
    if (pProp) 
    {
        PropType    pt  =   pProp->GetType();
        if (pt == IGAME_POINT3_PROP) 
        {
            Point3      val3;
            pProp->GetPropertyValue(val3);
            fprintf(pFile,"MaterialAmbient  = float4(%f,%f,%f,1.0f)",val3.x,val3.y,val3.z);
        }

        if (pt == IGAME_POINT4_PROP) 
        {
            Point4      val4;
            pProp->GetPropertyValue(val4);
            fprintf(pFile,"MaterialAmbient  = float4(%f,%f,%f,%f)",val4.x,val4.y,val4.z,val4.w);
        }
    }


    pProp	= pMat->GetDiffuseData(); 
    if (pProp) 
    {
        PropType    pt  =   pProp->GetType();
        if (pt == IGAME_POINT3_PROP) 
        {
            Point3      val3;
            pProp->GetPropertyValue(val3);
            fprintf(pFile,"MaterialDiffuse  = float4(%f,%f,%f,1.0f)",val3.x,val3.y,val3.z);
        }

        if (pt == IGAME_POINT4_PROP) 
        {
            Point4      val4;
            pProp->GetPropertyValue(val4);
            fprintf(pFile,"MaterialDiffuse  = float4(%f,%f,%f,%f)",val4.x,val4.y,val4.z,val4.w);
        }
    }

    pProp	= pMat->GetSpecularData(); 
    if (pProp) 
    {
        PropType    pt  =   pProp->GetType();
        if (pt == IGAME_POINT3_PROP) 
        {
            Point3      val3;
            pProp->GetPropertyValue(val3);
            fprintf(pFile,"MaterialSpecular  = float4(%f,%f,%f,1.0f)",val3.x,val3.y,val3.z);
        }

        if (pt == IGAME_POINT4_PROP) 
        {
            Point4      val4;
            pProp->GetPropertyValue(val4);
            fprintf(pFile,"MaterialSpecular  = float4(%f,%f,%f,%f)",val4.x,val4.y,val4.z,val4.w);
        }
    }


    int numTexMaps = pMat->GetNumberOfTextureMaps();

    for (int j = 0 ;j <numTexMaps; ++ j)
    {
        IGameTextureMap*    pTMap  =   pMat->GetIGameTextureMap(j);
        if (pTMap) 
        {
            IGameUVGen*	pUVGen	=	pTMap->GetIGameUVGen();
            TCHAR*	    pClass	=	pTMap->GetTextureClass();
            size_t length = strlen(pClass);
            char chararry[20];
            memset(chararry,0,sizeof(chararry));
            for(size_t i=0 ; i<length ; i++)
            {
                if(islower(*(pClass+i)))
                {
                    chararry[i] = _toupper(*(pClass+i));
                }
                else
                {
                    chararry[i] = *(pClass+i);
                }
            }
            if (strcmp(chararry,"BITMAP") != 0 && strcmp(chararry,"DEFAULT") != 0 && strcmp(chararry,"STANDARD") != 0)
            {
                continue;
            }

            IGameProperty*  textproperty = pUVGen->GetUOffsetData();//get the U offset data of the texture
            stringstream	ssUVData;
            float  ftemp = 0.0f;
            if(textproperty->GetPropertyValue(ftemp))		//得到变量的值
            {
                ssUVData<<"float textureUoffset="<<ftemp<<";\n";
            }

            textproperty = pUVGen->GetVOffsetData(); // get the v offset data of the texture
            if(textproperty && textproperty->GetPropertyValue(ftemp))		//得到变量的值
            {
                ssUVData<<"float textureVoffset="<<ftemp<<";\n";
            }

            textproperty = pUVGen->GetUTilingData(); // get the v offset data of the texture
            if(textproperty && textproperty->GetPropertyValue(ftemp))		//得到变量的值
            {
                ssUVData<<"float textureUscale="<<ftemp<<";\n";

            }

            textproperty = pUVGen->GetVTilingData(); // get the v offset data of the texture
            if(textproperty && textproperty->GetPropertyValue(ftemp))		//得到变量的值
            {
                ssUVData<<"float textureVscale="<<ftemp<<";\n";
            }


            std::string bmap(pTMap->GetBitmapFileName());

            ssUVData    <<"texture file = "<<bmap<<std::endl;


            fprintf(pFile,ssUVData.str().c_str());
        }


    }



    fclose(pFile);
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

