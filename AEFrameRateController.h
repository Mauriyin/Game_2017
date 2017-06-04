// ---------------------------------------------------------------------------
// Project Name		:	Alpha Engine
// File Name		:	AEFrameRateController.h
// Author			:	Sun Tjen Fam
// Creation Date	:	2007/04/26
// Purpose			:	header file for the frame rate controller
// History			:
// - 2007/04/26		:	- initial implementation
// ---------------------------------------------------------------------------

#ifndef AE_FRAME_RATE_CONTROLLER_H
#define AE_FRAME_RATE_CONTROLLER_H

// ---------------------------------------------------------------------------
// Externs

// ---------------------------------------------------------------------------
// Function prototypes
// ---------------------------------------------------------------------------

#ifdef __cplusplus

extern "C"
{
#endif



// ---------------------------------------------------------------------------
#if(EXPORT_WINDOWS == 1)
// Frame management
AE_API void	AEFrameRateControllerInit	(unsigned int FrameRateMax);
AE_API void	AEFrameRateControllerReset	();
AE_API void	AEFrameRateControllerStart	();
AE_API void	AEFrameRateControllerEnd	();


AE_API f64 AEFrameRateControllerGetFrameTime();
AE_API u32 AEFrameRateControllerGetFrameCount();
#endif
// ---------------------------------------------------------------------------



#ifdef __cplusplus 
}
#endif

// ---------------------------------------------------------------------------

#endif // AE_FRAME_RATE_CONTROLLER_H

