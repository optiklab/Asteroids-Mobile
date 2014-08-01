#ifndef _PACKT_ACTIVITYHANDLER_HPP_
#define _PACKT_ACTIVITYHANDLER_HPP_

#include "Constants.h"

namespace asteroids
{
    class ActivityHandler
    {
    public:
        virtual ~ActivityHandler() {};

        virtual status OnActivate() = 0;
        virtual void OnDeactivate() = 0;
        virtual status OnStep() = 0;

        //TODO Implement if need.
		//virtual void onStart() {};
		//virtual void onResume() {};
		//virtual void onPause() {};
		//virtual void onStop() {};
		//virtual void onDestroy() {};
		//
		//virtual void onSaveState(void** pData, size_t* pSize) {};
		//virtual void onConfigurationChanged() {};
		//virtual void onLowMemory() {};
		//
		//virtual void onCreateWindow() {};
		//virtual void onDestroyWindow() {};
		//virtual void onGainFocus() {};
		//virtual void onLostFocus() {};
    };
}
#endif
