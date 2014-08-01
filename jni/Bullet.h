#pragma once
#ifndef __bullet_h__
#define __bullet_h__

namespace asteroids
{
	struct Bullet
	{
		bool inuse;

		float x;
		float y;

		float v;
		float xv;
		float yv;

		int expire;
	};
}

#endif /* __bullet_h__ */

