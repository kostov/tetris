#ifndef Figures_h
#define Figures_h

#include "Common.h"

class Figure
{
public:
	Figure( Point const & _pos )
		: m_pos( _pos )
		, m_left( 0 )
		, m_right( 0 )
		, m_visible( true )
	{}

	void Draw( SDL_Surface * _surface ) const;

	virtual void Rotate( bool _clockwise );

	int GetX() const { return m_pos.x; }
	int GetY() const { return m_pos.y; }
	void SetX( int _x ) { m_pos.x = _x; }
	void SetY( int _y ) { m_pos.y = _y; }
	void SetVisible( bool _visible ) { m_visible = _visible; }

	int GetLeftBorder() const { return m_left; }
	int GetRightBorder() const { return m_right + 1; }

protected:
	Point m_pos;
	int m_left;
	int m_right;
	bool m_data[4][4];
	bool m_visible;

	friend class Application;
};

class FigureI: public Figure
{
public:
	FigureI( Point const & _pos ): Figure( _pos ) {
		static bool const _data[4][4] = {
			0,1,0,0,
			0,1,0,0,
			0,1,0,0,
			0,1,0,0
		};
		::mempcpy( m_data, _data, 16 );
		m_left = 1;
		m_right = 1;
	}
};

class FigureT: public Figure
{
public:
	FigureT( Point const & _pos ): Figure( _pos ) {
		static bool const _data[4][4] = {
			0,0,0,0,
			1,1,1,0,
			0,1,0,0,
			0,0,0,0
		};
		::mempcpy( m_data, _data, 16 );
		m_left = 0;
		m_right = 2;
	}
};

class FigureO: public Figure
{
public:
	FigureO( Point const & _pos ): Figure( _pos ) {
		static bool const _data[4][4] = {
			0,0,0,0,
			0,1,1,0,
			0,1,1,0,
			0,0,0,0
		};
		::mempcpy( m_data, _data, 16 );
		m_left = 1;
		m_right = 2;
	}
	virtual void Rotate() {}
};

class FigureL: public Figure
{
public:
	FigureL( Point const & _pos ): Figure( _pos ) {
		static bool const _data[4][4] = {
			0,1,0,0,
			0,1,0,0,
			0,1,1,0,
			0,0,0,0
		};
		::mempcpy( m_data, _data, 16 );
		m_left = 1;
		m_right = 2;
	}
};

class FigureZ: public Figure
{
public:
	FigureZ( Point const & _pos ): Figure( _pos ) {
		static bool const _data[4][4] = {
			0,0,0,0,
			1,1,0,0,
			0,1,1,0,
			0,0,0,0
		};
		::mempcpy( m_data, _data, 16 );
		m_left = 0;
		m_right = 2;
	}
};

class FigureL1: public Figure
{
public:
	FigureL1( Point const & _pos ): Figure( _pos ) {
		static bool const _data[4][4] = {
			0,0,1,0,
			0,0,1,0,
			0,1,1,0,
			0,0,0,0
		};
		::mempcpy( m_data, _data, 16 );
		m_left = 1;
		m_right = 2;
	}
};

class FigureZ1: public Figure
{
public:
	FigureZ1( Point const & _pos ): Figure( _pos ) {
		static bool const _data[4][4] = {
			0,0,0,0,
			0,1,1,0,
			1,1,0,0,
			0,0,0,0
		};
		::mempcpy( m_data, _data, 16 );
		m_left = 0;
		m_right = 2;
	}
};

#endif // Figures_h
