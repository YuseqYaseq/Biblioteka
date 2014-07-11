//Timer.h

#pragma once

#include <Windows.h>
#include <time.h>

namespace PORTECZKI
{
	class Timer
	{
	public:
	
		//Konstruktor i destruktor
		Timer();
		~Timer();

		//Aktualizacja czasu
		void Update();

		//Zwracaj¹ stan zegara
		double GetDelta();
		unsigned int GetSeconds()   { return m_pkTime->tm_sec;}
		unsigned int GetMinutes()   { return m_pkTime->tm_min;}
		unsigned int GetHours()     { return m_pkTime->tm_hour;}
		unsigned int GetDay()       { return m_pkTime->tm_mday;}
		unsigned int GetMonth()     { return m_pkTime->tm_mon;}
		unsigned int GetYear()      { return m_pkTime->tm_year;}

	protected:
	
		//Zmienne zegara sprzêtowego
		LARGE_INTEGER m_iFrequency;
		LARGE_INTEGER m_iLastQuery;
		LARGE_INTEGER m_iDelta;

		//Zmienne czasu i daty
		tm* m_pkTime;
	};
};