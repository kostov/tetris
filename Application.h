#ifndef Application_h
#define Application_h

#include "Figures.h"

class Application
{
public:
	Application();
	~Application();
	int Run();

private:
	bool Init();
	void Render();
	void ProcessEvent( SDL_Event & _event );
	bool ProcessControlEvent( SDL_Event & _event );
	void ProcessGameEvent( SDL_Event & _event );
	void DrawArena();
	void FreezeFigure();
	void ExchangeFigures();
	int GetFilledLine() const;
	void EatLine( int _line );
	bool IsFigureValid( bool * _bordersOnly = NULL ) const;
	bool & AreaPosFromFigurePos( int x, int y );
	static Figure * CreateRandomFigure();
	bool MoveFigureDown();
	void DrawGameOver();

private:
	static int const sm_arenaW = 16;
	static int const sm_arenaH = 28;
	static int const sm_arenaWPix = sm_arenaW * outerPix;
	static int const sm_arenaHPix = sm_arenaH * outerPix;
	static int const sm_infoPanelWPix = 160;
	static int const sm_surficeWPix = sm_arenaWPix + sm_infoPanelWPix;
	static int const sm_surficeHPix = sm_arenaHPix;

private:
	SDL_Surface * m_surfice;
	Figure * m_figure;
	Figure * m_nextFigure;
	bool m_running;
	bool m_gameOver;
	bool m_arena[ sm_arenaH ][ sm_arenaW ];
	int64_t m_lastMoment;
	int m_delay;
	int m_score;
};

#endif // Application_h
