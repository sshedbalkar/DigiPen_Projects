#include <iostream>
//
double	alpha[7]= {0.0, 0.0, 2.0/9.0, 1.0/18.0, 0.0, 0.0, 0.0};
int		O[]		= {2, 5, 5, 4};	//-1, 2, 2, 1
double	Pi[]	= {0.0, 0.0, 1.0/3.0, 1.0/3.0, 1.0/3.0, 0.0, 0.0};
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
//
int main()
{
	PrintAlpha( 1 );
	CalcDistr();
	PrintDistr( 1 );
	for( int t = 1; t < 4; ++t )
	{
		for( int i = 0; i < 7; ++i )
		{
			alpha[i] = CalcA_t1_j( t, i );
		}
		PrintAlpha( t+1 );
		CalcDistr();
		PrintDistr( t );
	}
	//
	std::cin.get();
	return 0;
}