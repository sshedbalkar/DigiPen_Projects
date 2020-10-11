#pragma once
//
#define ROW_SIZE		3
#define TOTAL_STATES	9
//
class Hmm
{
public:
	Hmm();
	~Hmm();
	//
public:
	void	AddTransModel( double* arr );
	void	AddSensorModel( double* arr );
	void	PrintTransModel();
	void	PrintSensorModel();
	void	AddStartingState( unsigned state );
	void	PrintDistr();
	unsigned GetNextState();
	void	SetObs( unsigned o );
	//
private:
	double	CalcA_t1_j( int o, int j );
	void	CalcAlpha();
	void	CalcDistr();
	//
private:
	double m_a[TOTAL_STATES][TOTAL_STATES];
	double m_b[TOTAL_STATES][TOTAL_STATES];
	double m_distr[TOTAL_STATES];
	double m_alpha[TOTAL_STATES];
	unsigned m_o;
};