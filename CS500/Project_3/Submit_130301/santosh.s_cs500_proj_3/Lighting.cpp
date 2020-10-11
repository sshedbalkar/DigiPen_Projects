#include "Lighting.h"
#include "Shape.h"
//
const int RECURSION_DEPTH = 5;
const bool SHADOWS = true;
//
V Lighting::GetColor( const Hit& hit ) const
{
	V viewdir = (SCENE->GetCamera().GetEyePos() - hit.Position).unit();
	return GetColorRecursive( hit, viewdir, RECURSION_DEPTH );
}
//
V Lighting::GetColorRecursive( const Hit& hit, const V& eyePos, int recursionlevel ) const
{
	return V( 1.0, 1.0, 1.0 );
}
//
std::vector< Light* > Lighting::GetVisibleLights( const LightListType& lights, const ShapeListType& shapes, const V& startPos ) const
{
	if( !SHADOWS )
	{
		return lights;
	}
	//
	LightListType visibleLights;
	for( LightListType::const_iterator it1 = lights.begin(), it2 = lights.end(); it1 != it2; ++it1 )
	{
		double t1 = 2000.0;
		Hit shadowHit;
		Ray shadowRay;

		V dirHitToRay = ((*it1)->GetPos()- startPos).unit();
		shadowRay.SetDir( dirHitToRay );
		shadowRay.SetOrigin( startPos+(dirHitToRay*FLT_EPSILON) );

		for( ShapeListType::const_iterator i = shapes.begin(), j = shapes.end(); i != j; ++i )
		{
			(*i)->Intersect( shadowRay, t1, shadowHit );
		}
		if( shadowHit.Obj->IsLight() )
		{
			visibleLights.push_back( *it1 );
		}
	}
	//
	return visibleLights;
}
//
V Phong::GetColorRecursive( const Hit& hit, const V& eyePos, int recursionlevel ) const
{
	V color = SCENE->ambient * m_diffuse ;
	//
	for( LightListType::iterator it1 = SCENE->m_lightList.begin(), it2 = SCENE->m_lightList.end(); it1 != it2; ++it1 )
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
		double rDotE = std::max<double>(0.0, reflection.dot(eyePos.unit()) );
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
V PhongBRDF::GetColorRecursive( const Hit& hit, const V& eyePos, int recursionlevel ) const
{
	V color = SCENE->ambient * m_diffuse ;
	LightListType lights = GetVisibleLights( SCENE->m_lightList, SCENE->m_shapeList, hit.Position );
	//
	for( LightListType::iterator it1 = lights.begin(), it2 = lights.end(); it1 != it2; ++it1 )
	{
		V Ldir = ((*it1)->GetPos() - hit.Position).unit();
		//
		double nDotL = std::max<double>( 0.0, hit.Normal.dot(Ldir) );
		V reflection = ( ((hit.Normal) * 2.0) * nDotL ) - Ldir;
		//
		color += m_diffuse * ( nDotL * nDotL ) * (*it1)->GetDiffuse() * ONE_PI;
		//
		V H = ( Ldir + eyePos ).unit();
		double hDotN = H.dot( hit.Normal );
		//
		color += m_specular * pow( hDotN, m_shininess ) * (*it1)->GetDiffuse() * 0.5 * ONE_PI * ( m_shininess + 2.0 ) *nDotL ;
	}
	//
	V R = (hit.Normal * 2.0 * eyePos.dot(hit.Normal) - eyePos).unit();
	double NdotR = std::max<double>( 0.0, hit.Normal.dot(R) );
	V Fresnel = m_specular + ( V(1, 1, 1)-m_specular) * pow( 1.0 - NdotR, 5 );
	V col;
	if( recursionlevel > 0 )
	{
		Ray ReyReflected;
		Hit HitReflected;
		//
		ReyReflected.SetDir( R );
		ReyReflected.SetOrigin( hit.Position + R * FLT_EPSILON );
		if( SCENE->TraceRay( ReyReflected, HitReflected ) )
		{
			col += HitReflected.Obj->GetLighting()->GetColorRecursive( HitReflected, -R, recursionlevel-1 );
		}
	}
	color += Fresnel * col * NdotR  * PI ;
	//
	return color;
}
//
V AShirley::GetColorRecursive( const Hit& hit, const V& eyePos, int recursionlevel ) const
{
	V color = SCENE->ambient * m_diffuse ;
	//
	for( LightListType::iterator it1 = SCENE->m_lightList.begin(), it2 = SCENE->m_lightList.end(); it1 != it2; ++it1 )
	{
		V Ldir = (*it1)->GetPos() - hit.Position;
		Ldir = Ldir.unit();
		//
		double nDotL = std::max<double>( 0.0, hit.Normal.dot(Ldir) );
		double nDotE = std::max<double>( 0.0, hit.Normal.dot(eyePos) );
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
		V H = ( Ldir + eyePos ).unit();
		double hDotN = std::max<double>( 0.0, H.dot(hit.Normal) );
		double HDotOmega = std::max<double>( 0.0, H.dot(eyePos) );
		temp2 = std::max<double>( nDotL, nDotE ) * HDotOmega;
		V Fresnel = m_specular + ( V(1.0, 1.0, 1.0) - m_specular ) * pow( (1.0 - HDotOmega), 5 );
		//
		V u = ( hit.Normal.cross(eyePos) ).unit();
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
//
bool Dielectric::Snell( double n, const V& I, const V& normal, V& T ) const
{
	double NdotI = normal.dot(I);
	double sqrtBase = 1-n*n*( 1-NdotI*NdotI );
	if( sqrtBase<0 ) return false;
	else T = ( ( normal*NdotI - I )*n - normal*sqrt( sqrtBase ) ).unit();
	return true;
}
//
V Dielectric::GetColorRecursive( const Hit& hit, const V& eyePos, int recursionlevel ) const
{
	V color;
	double NdotD = std::max<double>( 0.0, hit.Normal.dot( eyePos ));
	double Fresnel = m_reflectivity + (1.0-m_reflectivity)*pow(1.0 - NdotD, 5);
	//
	V colorReflection;
	V R = (hit.Normal*2.0*eyePos.dot(hit.Normal) - eyePos).unit();
	//
	if( recursionlevel >0 )
	{
		Ray reflectionRay;
		Hit reflectionHit;
		//
		reflectionRay.SetDir(R);
		reflectionRay.SetOrigin(hit.Position + R * FLT_EPSILON);
		// first hit is at the ray entering the object
		if( SCENE->TraceRay(reflectionRay, reflectionHit) )
		{
			colorReflection += reflectionHit.Obj->GetLighting()->GetColorRecursive( reflectionHit, -R, recursionlevel-1 );
		}
	}
	//
	V colorTransmission;

	Ray rayTransmitted;
	rayTransmitted.SetDir(-eyePos);
	V dirTemp = rayTransmitted.GetDir();
	//
	if( !Snell( 1.0/m_refractivity, eyePos, hit.Normal, dirTemp) )
	{
		return colorReflection;
	}
	else
	{
		rayTransmitted.SetDir(dirTemp);
	}

	rayTransmitted.SetOrigin(hit.Position + rayTransmitted.GetDir()* FLT_EPSILON);

	if(recursionlevel >0  )
	{
		Hit hitTransmission;
		//next hit is leaving the dielectric object
		if( SCENE->TraceRay(rayTransmitted, hitTransmission) )
		{
			Ray rayLeave;
			rayLeave.SetDir(rayTransmitted.GetDir());
			dirTemp = rayLeave.GetDir();
			if( !Snell( m_refractivity, -rayTransmitted.GetDir(), -hitTransmission.Normal,dirTemp ) )
			{
				return colorReflection;
			}
			else
			{
				rayLeave.SetDir(dirTemp);
			}
			rayLeave.SetOrigin(hitTransmission.Position + rayLeave.GetDir() * FLT_EPSILON);
			rayLeave.SetOrigin(hitTransmission.Position + rayLeave.GetDir() * FLT_EPSILON);
			
			Hit hitDielectric;
			if( SCENE->TraceRay( rayLeave, hitDielectric ) )
			{
				colorTransmission += hitDielectric.Obj->GetLighting()->GetColorRecursive( hitDielectric, -rayLeave.GetDir(), recursionlevel-1 );
			}
		}
	}
	color += colorReflection * Fresnel + colorTransmission*( 1-Fresnel );
	return color;
}