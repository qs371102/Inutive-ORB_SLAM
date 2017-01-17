/*
 * File - InuStreams.h
 *
 * This file is part of the Inuitive SDK
 *
 * Copyright (C) 2014 All rights reserved to Inuitive  
 *
 */

#ifndef __INUSTREAMS_H__
#define __INUSTREAMS_H__


////////////////////////////////////////////////////////////////////////
/// 
/// Includes all InuDev streams (easier inclusion into projects)
///
////////////////////////////////////////////////////////////////////////

// Role: Creates and provides NUI streams objects
#include "InuSensor.h"

// Role: Interface for InuDev Video Streaming Service.
#include "VideoStream.h"

// Role: Interface for InuDev Depth streaming Service. 
#include "DepthStream.h"

// Role: Interface for InuDev WebCam streaming Service. 
#include "WebCamStream.h"

// Role: Interface for InuDev Head streaming Service. 
#include "HeadStream.h"

// Role: Interface for InuDev Hands tracking streaming Service. 
#include "HandsStream.h"

// Role: Interface for InuDev Gaze streaming Service. 
#include "GazeStream.h"


#endif // __INUSTREAMS_H__

