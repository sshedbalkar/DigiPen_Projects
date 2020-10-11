#include <iostream>
//
#include "hmm.h"
//
Hmm::Hmm()
{
	//
}
//
Hmm::~Hmm()
{
	//
}
//
void Hmm::AddStartingState( unsigned state )
{
	for( unsigned i = 0; i < TOTAL_STATES; ++i )
	{
		m_alpha[i] = 0.0;
	}
	m_alpha[state] = 1.0;
	m_o = state;
}
//
double Hmm::CalcA_t1_j( int o, int j )
{
	double sum = 0.0, prod;
	for( int i = 0; i < TOTAL_STATES; ++i )
	{
		prod = m_a[i][j] * m_b[j][o] * m_alpha[i];
		sum += prod;
	}
	return sum;
}
//
void Hmm::CalcAlpha()
{
	for( int i = 0; i < TOTAL_STATES; ++i )
	{
		m_alpha[i] = CalcA_t1_j( m_o, i );
	}
}
//
unsigned Hmm::GetNextState()
{
	unsigned s = 0;
	//
	CalcAlpha();
	CalcDistr();
	//
	for( int i = 1; i < TOTAL_STATES; ++i )
	{
		if( m_distr[i] > m_distr[s] )
		{
			s = i;
		}
	}
	//
	//m_o = ::rand()%TOTAL_STATES;
	return s;
}
//
void Hmm::CalcDistr()
{
	double sum = 0.0;
	for( int i = 0; i < TOTAL_STATES; ++i )
	{
		sum += m_alpha[i];
	}
	for( int i = 0; i < TOTAL_STATES; ++i )
	{
		m_distr[i] = m_alpha[i]/sum;
	}
}
void Hmm::SetObs( unsigned o )
{
	m_o = o;
}
//
void Hmm::PrintDistr()
{
	std::cout<<"Distr:\n";
	for( unsigned i = 0; i < TOTAL_STATES; ++i )
	{
		::printf( "%0.5f ", m_distr[i] );
	}
	::printf( "\n" );
}
//
void Hmm::AddSensorModel( double* arr )
{
	double p;
	for( unsigned i = 0; i < TOTAL_STATES; ++i )
	{
		for( unsigned j = 0; j < TOTAL_STATES; ++j )
		{
			p = 0.0;
			if( i%ROW_SIZE == 0 )
			{
				p = arr[1];
				if( i < ROW_SIZE )
				{
					p += arr[0];
				}
				else if( (TOTAL_STATES-i) == ROW_SIZE )
				{
					p += arr[4];
				}
				//
				if( j == i )
				{
					m_b[i][j] = p + arr[2];
				}
				else if( j == (i+1) )
				{
					m_b[i][j] = arr[3];
				}
				else
				{
					m_b[i][j] = 0.0;
				}
			}
			else if( i%ROW_SIZE == (ROW_SIZE-1) )
			{
				p = arr[3];
				if( i < ROW_SIZE )
				{
					p += arr[0];
				}
				else if( (TOTAL_STATES-i) == 0 )
				{
					p += arr[4];
				}
				//
				if( j == i )
				{
					m_b[i][j] = p + arr[2];
				}
				else if( j == (i-1) )
				{
					m_b[i][j] = arr[1];
				}
				else
				{
					m_b[i][j] = 0.0;
				}
			}
			else if( i<ROW_SIZE )
			{
				p = arr[0];
				if( j == (i-1) )
				{
					m_b[i][j] = arr[1];
				}
				else if( j == i )
				{
					m_b[i][j] = p + arr[2];
				}
				else if( j == (i+1) )
				{
					m_b[i][j] = arr[3];
				}
				else if( j == (i+ROW_SIZE) )
				{
					m_b[i][j] = arr[4];
				}
				else
				{
					m_b[i][j] = 0.0;
				}
			}
			else if( (TOTAL_STATES-i) < ROW_SIZE )
			{
				p = arr[4];
				if( j == (i-1) )
				{
					m_b[i][j] = arr[1];
				}
				else if( j == i )
				{
					m_b[i][j] = p + arr[2];
				}
				else if( j == (i+1) )
				{
					m_b[i][j] = arr[3];
				}
				else if( j == (i-ROW_SIZE) )
				{
					m_b[i][j] = arr[0];
				}
				else
				{
					m_b[i][j] = 0.0;
				}
			}
			else
			{
				if( j == (i-ROW_SIZE) )
				{
					m_b[i][j] = arr[0];
				}
				else if( j == (i-1) )
				{
					m_b[i][j] = arr[1];
				}
				else if( j == i )
				{
					m_b[i][j] = arr[2];
				}
				else if( j == (i+1) )
				{
					m_b[i][j] = arr[3];
				}
				else if( j == (i+ROW_SIZE) )
				{
					m_b[i][j] = arr[4];
				}
				else
				{
					m_b[i][j] = 0.0;
				}
			}
		}
	}
}
//
void Hmm::PrintTransModel()
{
	std::cout<<"Transition model:\n";
	for( unsigned i = 0; i < TOTAL_STATES; ++i )
	{
		for( unsigned j = 0; j <TOTAL_STATES; ++j )
		{
			::printf( "%0.3f ", m_a[i][j] );
		}
		::printf( "\n" );
	}
}
//
void Hmm::PrintSensorModel()
{
	std::cout<<"Sensor model:\n";
	for( unsigned i = 0; i < TOTAL_STATES; ++i )
	{
		for( unsigned j = 0; j <TOTAL_STATES; ++j )
		{
			::printf( "%0.3f ", m_b[i][j] );
		}
		::printf( "\n" );
	}
}
//
void Hmm::AddTransModel( double* arr )
{
	double p;
	for( unsigned i = 0; i < TOTAL_STATES; ++i )
	{
		for( unsigned j = 0; j < TOTAL_STATES; ++j )
		{
			p = 0.0;
			if( i%ROW_SIZE == 0 )
			{
				p = arr[1];
				if( i < ROW_SIZE )
				{
					p += arr[0];
				}
				else if( (TOTAL_STATES-i) == ROW_SIZE )
				{
					p += arr[4];
				}
				//
				if( j == i )
				{
					m_a[i][j] = p + arr[2];
				}
				else if( j == (i+1) )
				{
					m_a[i][j] = arr[3];
				}
				else
				{
					m_a[i][j] = 0.0;
				}
			}
			else if( i%ROW_SIZE == (ROW_SIZE-1) )
			{
				p = arr[3];
				if( i < ROW_SIZE )
				{
					p += arr[0];
				}
				else if( (TOTAL_STATES-i) == 0 )
				{
					p += arr[4];
				}
				//
				if( j == i )
				{
					m_a[i][j] = p + arr[2];
				}
				else if( j == (i-1) )
				{
					m_a[i][j] = arr[1];
				}
				else
				{
					m_a[i][j] = 0.0;
				}
			}
			else if( i<ROW_SIZE )
			{
				p = arr[0];
				if( j == (i-1) )
				{
					m_a[i][j] = arr[1];
				}
				else if( j == i )
				{
					m_a[i][j] = p + arr[2];
				}
				else if( j == (i+1) )
				{
					m_a[i][j] = arr[3];
				}
				else if( j == (i+ROW_SIZE) )
				{
					m_a[i][j] = arr[4];
				}
				else
				{
					m_a[i][j] = 0.0;
				}
			}
			else if( (TOTAL_STATES-i) < ROW_SIZE )
			{
				p = arr[4];
				if( j == (i-1) )
				{
					m_a[i][j] = arr[1];
				}
				else if( j == i )
				{
					m_a[i][j] = p + arr[2];
				}
				else if( j == (i+1) )
				{
					m_a[i][j] = arr[3];
				}
				else if( j == (i-ROW_SIZE) )
				{
					m_a[i][j] = arr[0];
				}
				else
				{
					m_a[i][j] = 0.0;
				}
			}
			else
			{
				if( j == (i-ROW_SIZE) )
				{
					m_a[i][j] = arr[0];
				}
				else if( j == (i-1) )
				{
					m_a[i][j] = arr[1];
				}
				else if( j == i )
				{
					m_a[i][j] = arr[2];
				}
				else if( j == (i+1) )
				{
					m_a[i][j] = arr[3];
				}
				else if( j == (i+ROW_SIZE) )
				{
					m_a[i][j] = arr[4];
				}
				else
				{
					m_a[i][j] = 0.0;
				}
			}
		}
	}
}