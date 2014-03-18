#include <iostream>
#include <cmath>
#include <SDL/SDL.h>
#include "Application.h"

Application::Application()
	: m_surfice( NULL )
	, m_figure( NULL )
	, m_nextFigure( this->CreateRandomFigure() )
	, m_running( true )
	, m_gameOver( false )
	, m_lastMoment( utils::GetMilliseconds() )
	, m_delay( 200 )
	, m_score( 0 )
{
	::memset( m_arena, 0, sm_arenaW * sm_arenaH );
	this->ExchangeFigures();
}

Application::~Application()
{
	SDL_FreeSurface(m_surfice);
	SDL_Quit();
	delete m_figure;
	delete m_nextFigure;
}

void Application::ExchangeFigures() {
	delete m_figure;
	m_figure = m_nextFigure;
	m_figure->SetX( sm_arenaW / 2 - 2 );
	m_nextFigure = this->CreateRandomFigure();
}

int Application::Run()
{
	if( ! this->Init() ) {
		std::cerr << "SDL initialization is failed" << std::endl;
		return 1;
	}

	SDL_Event e;
	while( m_running )
	{
		if( ! m_gameOver )
		{
			const int64_t now = utils::GetMilliseconds();
			if( now - m_lastMoment > m_delay ) {
				m_lastMoment = now;
				this->MoveFigureDown();
			}
		}

		while( m_running && SDL_PollEvent( &e ) )
			this->ProcessEvent( e );

		this->Render();

		if( m_gameOver )
			utils::SleepMilliseconds( 50 );
	}

	return 0;
}

bool Application::Init()
{
	if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
		return false;

	m_surfice = SDL_SetVideoMode( sm_surficeWPix, sm_surficeHPix, 0, SDL_HWSURFACE | SDL_DOUBLEBUF );
	if( m_surfice == NULL )
		return false;

	this->Render();

	return SDL_EnableKeyRepeat( 150, 30 ) >= 0;
}

void Application::Render()
{
	sdl::DrawBoxPix( m_surfice, 0, 0, sm_surficeWPix, sm_surficeHPix, 0xFF000000 ); // black
	sdl::DrawBoxPix( m_surfice, sm_arenaWPix, 0, 1, sm_surficeHPix, 0xFFFFFF00 ); // yellow
	this->DrawArena();
	if( m_gameOver )
		this->DrawGameOver();
	else
		m_figure->Draw( m_surfice );

	SDL_Flip( m_surfice );
}

void Application::ProcessEvent( SDL_Event & _event )
{
	if( m_gameOver )
		this->ProcessControlEvent( _event );
	else
		this->ProcessGameEvent( _event );
}

bool Application::ProcessControlEvent( SDL_Event & _event )
{
	switch( _event.type ) {
		case SDL_QUIT:
			m_running = false;
			return true;

		case SDL_KEYDOWN:
			switch( _event.key.keysym.sym )
			{
			case SDLK_ESCAPE:
				m_running = false;
				return true;
			}
	}
	return false;
}

void Application::ProcessGameEvent( SDL_Event & _event )
{
	int const x = m_figure->GetX();

	bool const processed = this->ProcessControlEvent( _event );
	if( processed )
		return;

	switch( _event.type )
	{
		case SDL_KEYDOWN:
			switch( _event.key.keysym.sym )
			{
			case SDLK_DOWN:
				if( this->MoveFigureDown() )
					this->Render();
				break;

			case SDLK_RIGHT:
				m_figure->SetX( x + 1 );
				if( ! this->IsFigureValid() )
					m_figure->SetX( x ); // move it back
				else
					this->Render();
				break;

			case SDLK_LEFT:
				m_figure->SetX( x - 1 );
				if( ! this->IsFigureValid() )
					m_figure->SetX( x ); // move it back
				else
					this->Render();
				break;

			case SDLK_UP:
				m_figure->Rotate( true );
				bool bordersOnly;
				if( ! this->IsFigureValid(&bordersOnly) )
				{
					if( bordersOnly )
					{
						// check left border override
						int over = x + m_figure->GetLeftBorder();
						if( over < 0 )
							m_figure->SetX( x - over );
						else { // check right border override
							over = x + m_figure->GetRightBorder() - sm_arenaW;
							if( over > 0 )
								m_figure->SetX( x - over );
						}

						if( ! this->IsFigureValid() ) {
							m_figure->SetX( x ); // move it back
							m_figure->Rotate( false ); // move it back
						}
					}
					else
						m_figure->Rotate( false ); // move it back
				}

				this->Render();
				break;

			case SDLK_SPACE:
				while( this->MoveFigureDown() ) {
					this->Render();
					utils::SleepMilliseconds( 10 );
				}
				break;
			}
	}
}

bool Application::MoveFigureDown()
{
	bool moved = true;
	int const y = m_figure->GetY();
	m_figure->SetY( y + 1 );

	if( ! this->IsFigureValid() )
	{
		moved = false;
		m_figure->SetY( y ); // move it back
		this->FreezeFigure();
		++m_score;

		// eat full line(s)
		int eated = 0;
		int line = -1;
		while( ( line = this->GetFilledLine() ) != -1 ) {
			this->EatLine( line );
			++eated;
			m_delay -= 10;
			this->Render();
			utils::SleepMilliseconds( 200 );
		}

		if( eated != 0 )
			m_score += ::pow( 2, eated ) * 5;

		this->ExchangeFigures();
		if( ! this->IsFigureValid() )
			m_gameOver = true;
	}
	return moved;
}

void Application::FreezeFigure()
{
	m_figure->SetVisible( false );
	for( int x = 0; x < 4; ++x )
		for( int y = 0; y < 4; ++y )
			if( m_figure->m_data[y][x] )
				m_arena[ m_figure->m_pos.y + y ][ m_figure->m_pos.x + x ] = true;
}

void Application::DrawArena()
{
	for( int y = 0; y < sm_arenaH; ++y ) {
		for( int x = 0; x < sm_arenaW; ++x )
			if( m_arena[y][x] )
				sdl::DrawBox( m_surfice, x, y, 0x000000FF ); // blue
	}

	// draw next figure
	m_nextFigure->Draw( m_surfice );

	// draw vertical lines
	for( int x = 1; x < sm_arenaW; ++x )
		sdl::DrawBoxPix( m_surfice, x * outerPix, 0, 1, sm_arenaHPix, 0xFF191919 );

	// draw score
	sdl::DrawBoxPix( m_surfice,
		sm_arenaWPix + outerPix, sm_arenaHPix - outerPix * 2, m_score, outerPix, 0xFFFFFFFF );
//	char buff[ 16 ];
//	::snprintf( buff, 16, "%d", m_score );
//	::stringColor( m_surfice,
//		sm_arenaWPix + outerPix, sm_arenaHPix - outerPix, buff, 0xFFFFFFFF );
}

bool Application::IsFigureValid( bool * _bordersOnly ) const
{
	if( _bordersOnly )
		*_bordersOnly = false;
	for( int y = 0; y < 4; ++y ) {
		for( int x = 0; x < 4; ++x )
			if( m_figure->m_data[y][x] )
			{
				if( m_arena[ m_figure->m_pos.y + y ][ m_figure->m_pos.x + x ] ) // intersected
					return false;
				else if( m_figure->m_pos.y + y >= sm_arenaH )
					return false;
				else if( (m_figure->m_pos.x + x < 0) || (m_figure->m_pos.x + x >= sm_arenaW) ) {
					if( _bordersOnly )
						*_bordersOnly = true;
					return false;
				}
			}
	}
	return true;
}

Figure * Application::CreateRandomFigure()
{
	static const int figuresCount = 7;
	static const Point pos( sm_arenaW + 1, 0 );
	::srand( utils::GetMilliseconds() );
	int const index = ::rand() % figuresCount;
	switch( index ) {
		case 0 : return new FigureI ( pos ); break;
		case 1 : return new FigureL ( pos ); break;
		case 2 : return new FigureO ( pos ); break;
		case 3 : return new FigureT ( pos ); break;
		case 4 : return new FigureZ ( pos ); break;
		case 5 : return new FigureL1( pos ); break;
		case 6 : return new FigureZ1( pos ); break;
		default:
			assert(false && "Мы лохи!");
			return NULL;
	}
}

int Application::GetFilledLine() const
{
	for( int y = sm_arenaH - 1; y >= 0; --y )
	{
		bool filledLine = true;
		for( int x = 0; x < sm_arenaW; ++x )
		{
			filledLine = m_arena[y][x];
			if( filledLine == false )
				break;
		}
		if( filledLine )
			return y;
	}
	return -1;
}

void Application::EatLine( int _line )
{
	// TODO: memmove
	for( int y = _line; y > 0; --y )
		for( int x = 0; x < sm_arenaW; ++x )
			m_arena[y][x] = m_arena[y - 1][x];

	::memset( m_arena, 0, sm_arenaW );
}

void Application::DrawGameOver() {
	static int const w = 12;
	static int const h = 5;
	bool data[ h ][ w ] = {
		1,1,1,0,1,0,0,1,0,1,1,0,
		1,0,0,0,1,1,0,1,0,1,0,1,
		1,1,1,0,1,0,1,1,0,1,0,1,
		1,0,0,0,1,0,0,1,0,1,0,1,
		1,1,1,0,1,0,0,1,0,1,1,0
	};

	for( int y = 0; y < h; ++y )
		for( int x = 0; x < w; ++x )
			sdl::DrawBox( m_surfice, x + 2, y + 10, data[y][x] ? 0xFFFF0000 : 0xFF000000 );
}

