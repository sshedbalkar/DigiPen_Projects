#include <iostream>
#include <cstdlib>
#include <time.h>
#include "hmm.h";
//
/*
double	alpha[7]= {0.0, 0.0, 2.0/9.0, 1.0/18.0, 0.0, 0.0, 0.0};
//double	alpha[7]= {0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0};
int		O[]		= {2, 5, 5, 4};	//-1, 2, 2, 1
//double	Pi[]	= {0.0, 0.0, 1.0/3.0, 1.0/3.0, 1.0/3.0, 0.0, 0.0};
double	A[][7]	= {	{1.0,		0.0,		0.0,		0.0,		0.0,		0.0,		0.0		},
					{1.0/5.0,	3.0/5.0,	1.0/5.0,	0.0,		0.0,		0.0,		0.0		},
					{0.0,		1.0/5.0,	3.0/5.0,	1.0/5.0,	0.0,		0.0,		0.0		},
					{0.0,		0.0,		1.0/5.0,	3.0/5.0,	1.0/5.0,	0.0,		0.0		},
					{0.0,		0.0,		0.0,		1.0/5.0,	3.0/5.0,	1.0/5.0,	0.0		},
					{0.0,		0.0,		0.0,		0.0,		1.0/5.0,	3.0/5.0,	1.0/5.0	},
					{0.0,		0.0,		0.0,		0.0,		0.0,		0.0,		1.0		}};
double	B[][7]	= {	{1.0,		0.0,		0.0,		0.0,		0.0,		0.0,		0.0		},
					{1.0/6.0,	2.0/3.0,	1.0/6.0,	0.0,		0.0,		0.0,		0.0		},
					{0.0,		1.0/6.0,	2.0/3.0,	1.0/6.0,	0.0,		0.0,		0.0		},
					{0.0,		0.0,		1.0/6.0,	2.0/3.0,	1.0/6.0,	0.0,		0.0		},
					{0.0,		0.0,		0.0,		1.0/6.0,	2.0/3.0,	1.0/6.0,	0.0		},
					{0.0,		0.0,		0.0,		0.0,		1.0/6.0,	2.0/3.0,	1.0/6.0	},
					{0.0,		0.0,		0.0,		0.0,		0.0,		0.0,		1.0		}};
double distr[7];
//
double CalcA_t1_j( int o, int j )
{
	double sum = 0.0, prod;
	for( int i = 0; i < 7; ++i )
	{
		prod = A[i][j] * B[j][o] * alpha[i];
		sum += prod;
	}
	//
	
	return sum;
}
//
void CalcDistr()
{
	double sum = 0.0;
	for( int i = 0; i < 7; ++i )
	{
		sum += alpha[i];
	}
	for( int i = 0; i < 7; ++i )
	{
		distr[i] = alpha[i]/sum;
	}
}
//
void PrintAlpha( int t )
{
	for( int i = 0; i < 7; ++i )
	{
		std::cout<<"a("<<t<<", "<<(i-3)<<"): "<<alpha[i]<<", ";
	}
	std::cout<<"\n";
}
//
void PrintDistr( int t )
{
	double total = 0.0;
	for( int i = 0; i < 7; ++i )
	{
		std::cout<<"P("<<t<<", "<<(i-3)<<"): "<<distr[i]<<", ";
		total += distr[i];
	}
	std::cout<<"Total: "<<total<<"\n";
}
*/
double trans[] = { 0.2, 0.2, 0.2, 0.2, 0.2 };
double sensor[] = { 0.1, 0.1, 0.6, 0.1, 0.1 };
//
unsigned NextState( unsigned curr )
{
	double r = ((double)rand())/((double)RAND_MAX);
	double t[5];
	unsigned next;
	for( unsigned i = 0; i < 5; ++i )
	{
		t[i] = trans[i];
	}
	//
	if( curr%ROW_SIZE == 0 )
	{
		if( curr < ROW_SIZE )
		{
			t[2] += t[0]+t[1];
			t[0] = t[1] = 0.0;
		}
		else if( (TOTAL_STATES-curr) == ROW_SIZE )
		{
			t[2] += t[1]+t[4];
			t[1] = t[4] = 0.0;
		}
	}
	else if( curr%ROW_SIZE == (ROW_SIZE-1) )
	{
		if( curr < ROW_SIZE )
		{
			t[2] += t[0]+t[3];
			t[0] = t[3] = 0.0;
		}
		else if( (TOTAL_STATES-curr) == 0 )
		{
			t[2] += t[4]+t[3];
			t[4] = t[3] = 0.0;
		}
	}
	else if( curr<ROW_SIZE )
	{
		t[2] += t[0];
		t[0] = 0.0;
	}
	else if( (TOTAL_STATES-curr) < ROW_SIZE )
	{
		t[2] += t[4];
		t[4] = 0.0;
	}
	//
	double p = 0.0;
	p += t[0];
	if( r < p )
	{
		next = curr - ROW_SIZE;
	}
	else
	{
		p+= t[1];
		if( r < p )
		{
			next = curr - 1;
		}
		else
		{
			p+= t[2];
			if( r < p )
			{
				next = curr;
			}
			else
			{
				p+= t[3];
				if( r < p )
				{
					next = curr+1;
				}
				else
				{
					p+= t[4];
					if( r < p )
					{
						next = curr+ROW_SIZE;
					}
				}
			}
		}
	}
	//
	return next;
}
unsigned NextObsState( unsigned curr )
{
	double r = ((double)rand())/((double)RAND_MAX);
	double t[5];
	unsigned next;
	for( unsigned i = 0; i < 5; ++i )
	{
		t[i] = sensor[i];
	}
	//
	if( curr%ROW_SIZE == 0 )
	{
		if( curr < ROW_SIZE )
		{
			t[2] += t[0]+t[1];
			t[0] = t[1] = 0.0;
		}
		else if( (TOTAL_STATES-curr) == ROW_SIZE )
		{
			t[2] += t[1]+t[4];
			t[1] = t[4] = 0.0;
		}
	}
	else if( curr%ROW_SIZE == (ROW_SIZE-1) )
	{
		if( curr < ROW_SIZE )
		{
			t[2] += t[0]+t[3];
			t[0] = t[3] = 0.0;
		}
		else if( (TOTAL_STATES-curr) == 0 )
		{
			t[2] += t[4]+t[3];
			t[4] = t[3] = 0.0;
		}
	}
	else if( curr<ROW_SIZE )
	{
		t[2] += t[0];
		t[0] = 0.0;
	}
	else if( (TOTAL_STATES-curr) < ROW_SIZE )
	{
		t[2] += t[4];
		t[4] = 0.0;
	}
	//
	double p = 0.0;
	p += t[0];
	if( r < p )
	{
		next = curr - ROW_SIZE;
	}
	else
	{
		p+= t[1];
		if( r < p )
		{
			next = curr - 1;
		}
		else
		{
			p+= t[2];
			if( r < p )
			{
				next = curr;
			}
			else
			{
				p+= t[3];
				if( r < p )
				{
					next = curr+1;
				}
				else
				{
					p+= t[4];
					if( r < p )
					{
						next = curr+ROW_SIZE;
					}
				}
			}
		}
	}
	//
	return next;
}
//
int main()
{
	time_t seconds;
	time(&seconds);
	::srand((unsigned int) seconds);
	//
	unsigned Astate = 1;
	unsigned Ostate = Astate;
	Hmm h;
	h.AddTransModel( trans );
	//h.PrintTransModel();
	h.AddSensorModel( sensor );
	//h.PrintSensorModel();
	h.AddStartingState( Astate );
	while( true )
	{
		std::cout<<"Actual state: "<<Astate<<", Obs state: "<<Ostate<<"\n";
		std::cout<<"Calculated state: "<<h.GetNextState()<<"\n";
		h.PrintDistr();
		//
		Astate = NextState( Astate );
		Ostate = NextObsState( Astate );
		h.SetObs( Ostate );
		std::cin.get();
	}
	//
	/*
	PrintAlpha( 1 );
	CalcDistr();
	PrintDistr( 1 );
	for( int t = 1; t < 4; ++t )
	{
		for( int i = 0; i < 7; ++i )
		{
			alpha[i] = CalcA_t1_j( O[t], i );
		}
		PrintAlpha( t+1 );
		CalcDistr();
		PrintDistr( t );
	}
	*/
	//
	std::cin.get();
	return 0;
}