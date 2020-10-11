#include "Lighting.h"
#include "raytrace.h"
//
const V& Lighting::GetColor( const Hit& hit ) const
{
	return V( 1.0, 1.0, 1.0 );
}
//
const V& Phong::GetColor( const Hit& hit ) const
{
	V color = SCENE->ambient * m_diffuse ;
	V EyeVec = SCENE->GetCamera().GetEyePos() - hit.Position;
	EyeVec = EyeVec.unit();
	//
	for( Scene::LightListType::iterator it1 = SCENE->m_lightList.begin(), it2 = SCENE->m_lightList.end(); it1 != it2; ++it1 )
	{
		V Ldir = (*it1)->GetPos() - hit.Position;
		Ldir = Ldir.unit();
		//
		double nDotL = std::max<double>( 0.0, hit.Normal.dot(Ldir) );
		V reflection = ( ((hit.Normal) * 2.0) * nDotL )- Ldir;
		reflection = reflection.unit();
		//
		V D = ( m_diffuse * (*it1)->GetDiffuse() ) * nDotL;
		//
		double rDotE = std::max<double>(0.0, reflection.dot(EyeVec.unit()) );
		double power = pow( rDotE, m_shininess );
		//
		V S = ( m_specular * (*it1)->GetDiffuse()) * power;
		//
		color = color + D + S;
	}
	//
	return color;
}
//

const V& PhongBRDF::GetColor( const Hit& hit ) const
{
	V color = SCENE->ambient * m_diffuse ;
	V eyeVec = SCENE->GetCamera().GetEyePos() - hit.Position;
	eyeVec = eyeVec.unit();
	//
	for( Scene::LightListType::iterator it1 = SCENE->m_lightList.begin(), it2 = SCENE->m_lightList.end(); it1 != it2; ++it1 )
	{
		V Ldir = (*it1)->GetPos() - hit.Position;
		Ldir = Ldir.unit();
		//
		V reflection = ( ((hit.Normal) * 2.0) * (hit.Normal.dot(Ldir)) ) - Ldir;
		double nDotL = std::max<double>( 0.0, hit.Normal.dot(Ldir) );
		//
		color += m_diffuse * ( nDotL * nDotL ) * (*it1)->GetDiffuse() * ONE_PI;
		//
		V H = ( Ldir + eyeVec ).unit();
		double hDotN = H.dot( hit.Normal );
		//
		color += m_specular * pow( hDotN, m_shininess ) * (*it1)->GetDiffuse() * 0.5 * ONE_PI * ( m_shininess + 2.0 ) *nDotL ;
	}
	//
	return color;
}


const V& AShirley::GetColor( const Hit& hit ) const
{
	V color = SCENE->ambient * m_diffuse ;
	V eyeVec = SCENE->GetCamera().GetEyePos() - hit.Position;
	eyeVec=eyeVec.unit();
	//
	for( Scene::LightListType::iterator it1 = SCENE->m_lightList.begin(), it2 = SCENE->m_lightList.end(); it1 != it2; ++it1 )
	{
		V Ldir = (*it1)->GetPos() - hit.Position;
		Ldir = Ldir.unit();
		//
		double nDotL = std::max<double>( 0.0, hit.Normal.dot(Ldir) );
		double nDotE = std::max<double>( 0.0, hit.Normal.dot(eyeVec) );
		//
		double temp1 = 1.0 - ( nDotL * 0.5 );
		temp1 = pow( temp1, 5.0 );
		//
		double temp2 = 1.0 - ( nDotE * 0.5 );
		temp2 = pow( temp2, 5.0 );
		//
		V D = m_diffuse* 0.38750 * ( V(1.0, 1.0, 1.0) - m_specular ) * ( 1.0 - temp1 ) * ( 1.0 - temp2 ) * nDotL * (*it1)->GetDiffuse();
		color += D; 
		//
		temp1 = sqrt( (m_shininess1 + 1.0) * (m_shininess2 + 1.0) ) / 25.13274;
		V H = ( Ldir + eyeVec ).unit();
		double hDotN = std::max<double>( 0.0, H.dot(hit.Normal) );
		double HDotOmega = std::max<double>( 0.0, H.dot(eyeVec) );
		temp2 = std::max<double>( nDotL, nDotE ) * HDotOmega;
		V Fresnel = m_specular + ( V(1.0, 1.0, 1.0) - m_specular ) * pow( (1.0 - HDotOmega), 5 );
		//
		V u = ( hit.Normal.cross(eyeVec) ).unit();
		V v = ( hit.Normal.cross(u) ).unit();
		//
		double HDotU = H.dot( u );
		double HDotV = H.dot( v );
		double temp3 = ( (m_shininess1 * HDotU * HDotU ) + ( m_shininess2 * HDotV * HDotV) )/( 1.0 - hDotN * hDotN );
		//
		V S = ( (Fresnel * temp1 * pow(hDotN, temp3)) / temp2 ) * nDotL * (*it1)->GetDiffuse();
		color += S;
	}
	//
	return color;
}