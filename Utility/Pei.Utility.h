#ifndef _SPUtility_H
#define _SPUtility_H


#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

#include "SPErrorHandler.h"
#include "SPString.h"
#include "SPFile.h"
#include "SPMath.h"
#include "SPColor.h"
#include "SPRect.h"
#include "SPTimer.h"
#include "SPLoopManager.h"
#include "SPWindow.h"
#include "SPArchive.h"
#include "SPMutex.h"

#endif