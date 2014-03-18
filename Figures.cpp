#include "Figures.h"

void Figure::Draw( SDL_Surface * _surface ) const
{
	if( ! m_visible )
		return;

	for( int y = 0; y < 4; ++y )
		for( int x = 0; x < 4; ++x )
			if( m_data[y][x] )
				sdl::DrawBox( _surface, m_pos.x + x, m_pos.y + y, 0x0000FF00 ); // green
}

void Figure::Rotate( bool _clockwise )
{
	m_left = -1;
	bool newData[4][4];
	for( int x = 0; x < 4; ++x)
		for( int y = 0; y < 4; ++y )
		{
			if( _clockwise )
				newData[y][x] = m_data[::abs(x-3)][y];
			else
				newData[y][x] = m_data[x][::abs(y-3)];

			// determine borders
			if( newData[y][x] ) {
				if( m_left == -1 )
					m_left = x;
				m_right = x;
			}
		}
	::mempcpy( m_data, newData, 16 );
}
