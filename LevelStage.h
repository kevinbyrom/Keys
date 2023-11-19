#ifndef __LEVELSTAGE_H
#define __LEVELSTAGE_H


#include "cdx2.h"


class CLevelStage : public CDXStage
	{
	public:

		virtual void Opening();
		virtual void Closing(CDXStage * stage);
		virtual void Loop();
	};



#endif