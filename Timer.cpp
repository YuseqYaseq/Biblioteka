//Timer.cpp

#include "Timer.h"

using namespace PORTECZKI;

Timer::Timer()
{
	//Pobieranie czêstotliwoœci zegara sprzêtowego i jego bie¿¹cy stan
	QueryPerformanceFrequency(&m_iFrequency);
	QueryPerformanceCounter(&m_iLastQuery);
}

Timer::~Timer()
{
	m_iFrequency.QuadPart = 0;
	m_iLastQuery.QuadPart = 0;
}

void Timer::Update()
{
	//Aktualizacja zegara
	LARGE_INTEGER kTempTimer;
	time_t        iTempTimeDate;

	//Pobierz czas i oblicz ró¿nicê
	QueryPerformanceCounter(&kTempTimer);
	m_iDelta.QuadPart = kTempTimer.QuadPart - m_iLastQuery.QuadPart;

	//Zachowaj bie¿¹cy stan zegara
	m_iLastQuery.QuadPart = kTempTimer.QuadPart;

	//Pobierz bie¿¹cy czas i datê
	time(&iTempTimeDate);
	m_pkTime = localtime(&iTempTimeDate);
}

double Timer::GetDelta()
{
	//Zamiana na liczbê zmiennoprzecinkow¹ i obliczanie zmiany w sekundach
	return (double)(m_iDelta.QuadPart) / (double)(m_iFrequency.QuadPart);
}