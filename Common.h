#ifndef Common_h
#define Common_h

#include <cassert>
#include <time.h>
#include <unistd.h>

#include <SDL/SDL.h>

int const deltaPix = 2;
int const innerPix = 20;
int const outerPix = innerPix + 2 * deltaPix;

struct Point {
	Point( int _x = 0, int _y = 0 ): x( _x ), y( _y ) {}
	int x;
	int y;
};

namespace sdl
{
	static void DrawBox( SDL_Surface * _surface, int _xIndex, int _yIndex, Uint32 _color )
	{
		SDL_Rect rc;
		rc.x = _xIndex * outerPix + deltaPix;
		rc.y = _yIndex * outerPix + deltaPix;
		rc.w = rc.h = innerPix;
		int const res = ::SDL_FillRect( _surface, &rc, _color );
		assert( res == 0 );
	}

	static void DrawBoxPix( SDL_Surface * _surface, int _x, int _y, int _w, int _h, Uint32 _color )
	{
		SDL_Rect rc;
		rc.x = _x;
		rc.y = _y;
		rc.w = _w;
		rc.h = _h;
		int const res = ::SDL_FillRect( _surface, &rc, _color );
		assert( res == 0 );
	}
}

namespace utils
{
	static int64_t GetMilliseconds() {
		timespec ts;
		int const res = ::clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &ts );
		assert( res == 0 );
		return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
	}

	inline void SleepMilliseconds( uint64_t _ms ) {
		::usleep( 1000 * _ms );
	}
}

#endif // Common_h
