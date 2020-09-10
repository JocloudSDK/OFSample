/***************** BEGIN FILE HRADER BLOCK *********************************
 *
 * \author Ning Hualong <ninghualong@yy.com> phone:15013359003
 *
 * Copyright (C) 2016-2020 YY.Inc
 * All rights reserved.
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version. Please review the following information
 * to ensure the GNU Lesser General Public License version 3 requirements
 * will be met: https://www.gnu.org/licenses/lgpl.html.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation.
 *
 * If you use the source in a your project, please consider send an e-mail
 * to me, and allow me to add your project to my home page.
 *
 ***************** END FILE HRADER BLOCK ***********************************/
#ifndef _ORANGEFILTER_WIN_
#define _ORANGEFILTER_WIN_

#include "orangefilter.h"

/************************************************************************/
/* Module export definition.                                            */
/************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
	//
	//These API is for input data is image data not texture
	//

	//
	//Init for SDK, create a runtime environment
    /* Allocate an effect context for your effect resources, OFW_Uninit()
    * can be used to free the context and everything allocated by the context
    * within it.This function must be called before all others.
    * @param[out] contextID, is set to the created context, or to NULL in case
    * of failure
    * @return, OF_Result_Success in case of success, others in case of failure.
    */
	OF_API OF_Result OFW_Init(OFHandle *contextid);

	//
	//Uninit
    /**
    * Free an context and all its resources.
    * @param[in] contextID, context to destroy.
    * @return, OF_Result_Success in case of success, others in case of failure.
    */
	OF_API OF_Result OFW_Uninit(OFHandle contextid);


    /**
    * Apply a frame, render effects contents on output pixel buffer.
    * @param[in] contextID, context to render effect.
    * @param[in] handlerIDList, effects to render.
    * @param[in] handlerIDResult, effects return OF_Result.
    * @param[in] handlerIDCount, effects number in list.
    * @param[in] srcData, input pixel buffer, 
    * @param[in] dstData, output pixel buffer,
    * @param[in] frameData, frame data for special effects.
    * @return, OF_Result_Success in case of success, others in case of failure.
    */
    OF_API OF_Result OFW_Applying(
        OFHandle contextid,							/* [in] */
        OFHandle* handlerIDList,                    /* [in] */
        OF_Result * handlerIDResult,                /* [in] */
        OFUInt16 handlerIDCount,                    /* [in] */
        OFUInt8* srcData,							/* [in] */
        OFUInt8* dstData,							/* [in] */
        OFInt32 width,								/* [in] */
        OFInt32 height,								/* [in] */
        OF_PixelFormat inputformat,					/* [in] */
        OF_PixelFormat outputformat,				/* [in] */
        OF_FrameData* frameData);					/* [in] */
    /**
    * Only set parameters for beauty, the effectID must be beauty effect.
    * @param[in] contextID, context to render effect.
    * @param[in] effectID, effects to render.
    * @param[in] neatIntensity, set neat intensity[0.0, 1.0],
    * @param[in] whiteningIntensity, set whitening intensity[0.0, 1.0].
    * @return, OF_Result_Success in case of success, others in case of failure.
    */
    OF_API OF_Result SetBeautyParameters(
        OFHandle contextID,                         /* [in] */
        OFHandle effectID,                          /* [in] */
        float neatIntensity,                        /* [in] */
        float whiteningIntensity);                  /* [in] */

    /**
    * Get OpenGL version.
    * @param[in] str, get OpenGL version string.
    * @param[in] length, the input string array's length.
    * @return, OF_Result_Success in case of success, others in case of failure.
    */
    OF_API OF_Result OFW_OpenGLVersion(char* str, int length);

#ifdef __cplusplus
}
#endif

#endif