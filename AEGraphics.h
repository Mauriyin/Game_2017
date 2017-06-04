// ---------------------------------------------------------------------------
// Project Name		:	Alpha Engine
// File Name		:	AEGraphics.h
// Author			:	Sun Tjen Fam
// Creation Date	:	2008/01/30
// Purpose			:	Graphics component header file
// History			:
// - 2008/01/30		:	- initial implementation
// ---------------------------------------------------------------------------

#ifndef AE_GRAPHICS_H
#define AE_GRAPHICS_H

// ---------------------------------------------------------------------------
// Defines

typedef	enum
{
	AE_GFX_RM_NONE, 
	AE_GFX_RM_COLOR, 
	AE_GFX_RM_TEXTURE,
	

	AE_GFX_RM_NUM
}AEGfxRenderMode;


// ---------------------------------------------------------------------------


typedef enum
{
	AE_GFX_BM_NONE = 0, 
	AE_GFX_BM_BLEND,  
	AE_GFX_BM_ADD,  
	AE_GFX_BM_MULTIPLY,  

	AE_GFX_BM_NUM
}AEGfxBlendMode;


// ---------------------------------------------------------------------------


typedef enum
{
	AE_GFX_TM_PRECISE = 0,
	AE_GFX_TM_AVERAGE
}AEGfxTextureMode;


// ---------------------------------------------------------------------------

typedef enum
{
	AE_GFX_MDM_POINTS = 0,
	AE_GFX_MDM_LINES,
	AE_GFX_MDM_LINES_STRIP,
	AE_GFX_MDM_TRIANGLES,
	
	// Keep this one last
	AE_GFX_MDM_NUM
}AEGfxMeshDrawMode;

// ---------------------------------------------------------------------------

typedef enum
{
	AE_WOM_NORMAL = 0,
	AE_WOM_TOP_LEFT,

	
	// Keep this one last
	AE_WOM_NUM,
}WorldOriginMode;

// ---------------------------------------------------------------------------
// Struct/Class definitions

typedef struct AEGfxVertexBuffer AEGfxVertexBuffer;

typedef struct AEGfxVertexList
{
	AEGfxVertexBuffer		*mpVtxBuffer;
	u32						vtxNum;
}AEGfxVertexList;

// ---------------------------------------------------------------------------

typedef struct AEGfxSurface AEGfxSurface;

typedef struct AEGfxTexture
{
	AEGfxSurface *mpSurface;
	char mpName[256];
}AEGfxTexture;

// ---------------------------------------------------------------------------
// Extern variables

// ---------------------------------------------------------------------------
// Function prototypes
// ---------------------------------------------------------------------------

#ifdef __cplusplus

extern "C"
{
#endif

// ---------------------------------------------------------------------------

AE_API int AEGfxInit(int Width, int Height);

DECLARE_FUNCTION_FOR_ANDROID_2_INT(AEGfxInit, int Width, int Height);


AE_API void AEGfxReset();
//AE_API void AEGfxUpdate();
AE_API void AEGfxExit();

AE_API void AEGfxStart();
DECLARE_FUNCTION_FOR_ANDROID(AEGfxStart);

AE_API void AEGfxEnd();
DECLARE_FUNCTION_FOR_ANDROID(AEGfxEnd);

AE_API void AEGfxSetBackgroundColor(float Red, float Green, float Blue);
AE_API void AEGfxSetRenderMode(unsigned int RenderMode);

AE_API f32 AEGfxGetWinMinX(void);
AE_API f32 AEGfxGetWinMaxX(void);
AE_API f32 AEGfxGetWinMinY(void);
AE_API f32 AEGfxGetWinMaxY(void);

// Sets/Gets the camera position
AE_API void AEGfxSetCamPosition(f32 X, f32 Y);
AE_API void AEGfxGetCamPosition(f32 *pX, f32 *pY);

// Sets/Gets the camera distance, used to zoom in/out
//AE_API void AEGfxSetCamZoom(f32 Distance);
//AE_API f32 AEGfxGetCamZoom();

// function to set the position (translation)
AE_API void AEGfxSetPosition(f32 X, f32 Y);

// functions to set the current transformation matrix to use
AE_API void AEGfxSetTransform(float pTransform[3][3]);
AE_API void AEGfxSetTransform3D(float pTransform[4][4]);
AE_API void AEGfxSetFullTransform(float PosX, float PosY, float RotAngleDeg, float ScaleX, float ScaleY);

// Sets a color that will be blended with the original material
AE_API void AEGfxSetBlendColor(float Red, float Green, float Blue, float Alpha);

// Set a global transparency value for the material
AE_API void AEGfxSetTransparency(float Alpha);

// Sets a color that will be used to tint the original material
AE_API void AEGfxSetTintColor(float Red, float Green, float Blue, float Alpha);

// Sets the alpha blending mode (Transparency)
AE_API void	AEGfxSetBlendMode(unsigned int BlendMode);

// Sets where the origin of the world is
AE_API void	AEGfxSetWorldOriginMode(unsigned int WorldOriginMode);

// Sets the viewport position, width and height (Needed only if the main window has multiple viewports)
// Set one to a negative value to reset to the full window size
AE_API void AEGfxSetViewportPositionAndDimensions(int ViewportX, int ViewportY, int ViewportWidth, int ViewportHeight);


// functions to create a mesh

//AE_API void			AEGfxTriListInitialize	(AEGfxTriList *pTriList);
AE_API void			AEGfxMeshStart	();
AE_API void			AEGfxTriAdd		(f32 x0, f32 y0, u32 c0, f32 tu0, f32 tv0,
										 f32 x1, f32 y1, u32 c1, f32 tu1, f32 tv1,
										 f32 x2, f32 y2, u32 c2, f32 tu2, f32 tv2);
AE_API void			AEGfxVertexAdd		(f32 x0, f32 y0, u32 c0, f32 tu0, f32 tv0);

AE_API AEGfxVertexList*	AEGfxMeshEnd		();
AE_API void				AEGfxMeshDraw	(AEGfxVertexList* pVertexList, unsigned int MeshDrawMode);
AE_API void				AEGfxMeshFree	(AEGfxVertexList* pVertexList);


AE_API AEGfxTexture*	AEGfxTextureLoad(const char *pFileName);
AE_API void				AEGfxTextureUnload(AEGfxTexture *pTexture);
AE_API void				AEGfxTextureSet(AEGfxTexture *pTexture, f32 offset_x, f32 offset_y);			// Use NULL for 'no texture'


AE_API AEGfxTexture*	AEGfxTextureLoadFromMemory(unsigned char *pColors, unsigned int Width, unsigned int Height);
AE_API void				AEGfxSaveTextureToFile(AEGfxTexture* pTexture, char *pFileName);

AE_API void				AEGfxSetTextureMode(unsigned int TextureMode);

// functions to draw simple primitives
// * these are slow!!! use with caution!!
// * DO NOT call any of these functions while building mesh using the above functions
AE_API void			AEGfxPoint		(f32 x0, f32 y0, f32 z0, u32 c0);
AE_API void			AEGfxLine		(f32 x0, f32 y0, f32 z0, f32 r0, f32 g0, f32 b0, f32 a0, f32 x1, f32 y1, f32 z1, f32 r1, f32 g1, f32 b1, f32 a1);

AE_API void			AEGfxTri		(f32 x0, f32 y0, f32 z0, u32 c0, 
								 f32 x1, f32 y1, f32 z1, u32 c1, 
								 f32 x2, f32 y2, f32 z2, u32 c2);
AE_API void			AEGfxQuad		(f32 x0, f32 y0, f32 z0, u32 c0, 
								 f32 x1, f32 y1, f32 z1, u32 c1, 
								 f32 x2, f32 y2, f32 z2, u32 c2, 
								 f32 x3, f32 y3, f32 z3, u32 c3);
AE_API void			AEGfxBox		(f32 x0, f32 y0, f32 z0, f32 sizeX, f32 sizeY, f32 sizeZ, u32 c0, u32 c1);
AE_API void			AEGfxSphere		(f32 x0, f32 y0, f32 z0, f32 radius, u32 c0, u32 c1, u32 division);
AE_API void			AEGfxCone		(f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, f32 radius, u32 c0, u32 c1, u32 division);
AE_API void			AEGfxAxis		(f32 scale);

AE_API u32			AEGfxColInterp	(u32 c0, u32 c1, f32 t);


// font related functions
// This function has been deprecated.
//AE_API void			AEGfxPrint		(s32 x, s32 y, u32 color, s8* pStr);

#if(EXPORT_WINDOWS == 1)
AE_API HDC			AEGfxGetDeviceContext(void);
AE_API HGLRC		AEGfxGetRenderContext(void);
AE_API int			AEGfxGetPixelFormat(void);
#endif

// ---------------------------------------------------------------------------

#ifdef __cplusplus 
}
#endif

// ---------------------------------------------------------------------------

#endif // AE_GRAPHICS_H

