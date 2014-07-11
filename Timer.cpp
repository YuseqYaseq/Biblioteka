//Timer.cpp

#include "Timer.h"

using namespace PORTECZKI;

Timer::Timer()
{
	//Pobieranie cz�stotliwo�ci zegara sprz�towego i jego bie��cy stan
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

	//Pobierz czas i oblicz r�nic�
	QueryPerformanceCounter(&kTempTimer);
	m_iDelta.QuadPart = kTempTimer.QuadPart - m_iLastQuery.QuadPart;

	//Zachowaj bie��cy stan zegara
	m_iLastQuery.QuadPart = kTempTimer.QuadPart;

	//Pobierz bie��cy czas i dat�
	time(&iTempTimeDate);
	m_pkTime = localtime(&iTempTimeDate);
}

double Timer::GetDelta()
{
	//Zamiana na liczb� zmiennoprzecinkow� i obliczanie zmiany w sekundach
	return (double)(m_iDelta.QuadPart) / (double)(m_iFrequency.QuadPart);
}