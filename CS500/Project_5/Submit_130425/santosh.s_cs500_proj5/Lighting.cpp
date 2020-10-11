#include "Lighting.h"
#include "Shape.h"
#include "raytrace.h"
//
const bool SHADOWS			= true;
const double PROB_SURVIVE	= 0.75;
//
V Lighting::GetColor( const Hit& hit, const V& startPos )
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
	unsigned int i = ((erand48(Scene::Xi)))*lights.size();
	double t1 = 2000.0;
	Hit shadowHit;
	Ray shadowRay;
	//
	V dirHitToRay = lights[i]->GetPos()- startPos;
	//
	double ldist = dirHitToRay.length();
	double d = static_cast<Sphere*>(lights[i]->GetShape())->GetRadius() * erand48(Scene::Xi)-FLT_EPSILON;

	if(d<0.0) d = 0.0;
	double adjacent = sqrt( ldist * ldist + d * d);
	double costheta = ldist/adjacent;
	//
	dirHitToRay = (SCENE->Sample(dirHitToRay, costheta, 2.0*PI*erand48(Scene::Xi))).unit();
	shadowRay.SetDir( dirHitToRay );
	shadowRay.SetOrigin( startPos+(dirHitToRay*FLT_EPSILON) );
	//
	for( ShapeListType::const_iterator i = shapes.begin(), j = shapes.end(); i != j; ++i )
	{
		(*i)->Intersect( shadowRay, t1, shadowHit );
	}
	if( shadowHit.Obj->IsLight() )
	{
		visibleLights.push_back( lights[i] );
	}
	return visibleLights;
}
//
PhongBRDF::choice PhongBRDF::ChooseRandomProperty()
{
	double j = erand48(Scene::Xi);
	double prob1,prob2;
	prob1 = Kd/K_Sum;
	prob2 = (Kd+Ks)/K_Sum;

	if (j<(prob1))
	{
		return DIFFUSE;
	}
	else if (j<(prob2))
	{

		return SPECULAR;
	}
	else
	{
		return TRANSMISSION;
	}
	
	
}
//
double PhongBRDF::RGBtoYUV(V& v)
{
	double d;
	d = v[0]*0.299+v[1]*0.587+v[2]*0.114;
	return d;
}
//
V PhongBRDF::CalcSpecular(Light* light, const Hit &hit, const V& viewDir)
{
	V lightVec = light->GetPos() - hit.Position;
	lightVec = lightVec.unit();
	double nDotL = std::max<double>( 0.0, hit.Normal.dot(lightVec) );
	V H = (lightVec+viewDir).unit();
	double hDotN = H.dot(hit.Normal);
	V color = m_specular * pow( hDotN, m_shininess) * light->GetDiffuse() * 0.5 * ONE_PI * (m_shininess + 2.0) *nDotL ;
	return color;
}
//
V PhongBRDF::CalcDiffuse( Light* light, const Hit &hit )
{
	
	V lightVec = light->GetPos() - hit.Position;
	lightVec = lightVec.unit();
	double nDotL = std::max<double>( 0.0, hit.Normal.dot(lightVec) );
	V reflection = (hit.Normal)*2.0*nDotL-lightVec;
	V color = m_diffuse * (nDotL*nDotL) * light->GetDiffuse() * ONE_PI;
	return color;
}
//
bool PhongBRDF::Snell( double n, const V& I, const V& normal, V& T ) 
{
	double NdotI = normal.dot(I);
	double sqrtBase = 1 - n*n * ( 1-NdotI*NdotI );
	if( sqrtBase<0 ) return false;
	else T = ( ( normal*NdotI - I )*n - normal*sqrt( sqrtBase ) ).unit();
	return true;
}
//
V PhongBRDF::GetDirectColor( const Hit& hit, const V& viewDir )
{
	V color;
	LightListType lightList = GetVisibleLights( SCENE->m_lightList, SCENE->m_shapeList, hit.Position );
	for( LightListType::iterator it1 = lightList.begin(), it2 = lightList.end(); it1 != it2; ++it1 )
	{
		color += CalcDiffuse((*it1),hit);
		color += CalcSpecular( (*it1), hit, viewDir );
		color = color*SCENE->m_lightList.size();
	}
	return color;
}
//
V PhongBRDF::GetInDirectColor( const Hit& hit, const V& viewDir )
{
	choice mychoice = ChooseRandomProperty();
	V color;
	V R;
	if( erand48(Scene::Xi) < PROB_SURVIVE )
	{
		Ray reflectionRay;
		if( mychoice == DIFFUSE )
		{
			R = SCENE->Sample( hit.Normal, sqrt(erand48(Scene::Xi)), 2.0*PI*erand48(Scene::Xi) );
		}
		else
		{
			if( mychoice == TRANSMISSION )
			{
				V I = -viewDir;
				V Normal = hit.Normal;
				double etar;
				//
				R = I;
				if( I.dot(Normal)>0 )
				{
					etar = ior;
					Normal = V(-1,-1,-1)*Normal;
				}
				else
				{
					etar = 1.0/ior;
				}
				if( !Snell(etar, viewDir, Normal, I) )
				{
					return color;
				}
				else
				{
					R = I;
				}
				//
				double power = pow( erand48(Scene::Xi),(1.0/(m_shininess+1.0)) );
				R = SCENE->Sample( R, power, 2.0*PI*erand48(Scene::Xi) );
				R = R.unit();

			}
			else if( mychoice == SPECULAR )
			{
				R = hit.Normal*2.0*viewDir.dot( hit.Normal ) - viewDir;
				R = R.unit();
				double power = pow( erand48(Scene::Xi), (1.0/(m_shininess+1.0)) );
				R = SCENE->Sample(R, power, 2.0*PI*erand48(Scene::Xi) );
				R = R.unit();
			}
		}

		reflectionRay.SetDir(R);
		reflectionRay.SetOrigin(hit.Position + R * FLT_EPSILON);
		Hit reflectionHit;
		//
		if( SCENE->TraceRay( reflectionRay, reflectionHit ) )
		{	
			if(!reflectionHit.Obj->IsLight())
			{
				color += reflectionHit.Obj->GetLighting()->GetColor(reflectionHit,-R);
			}
		}
	}
	//
	if( mychoice == DIFFUSE )
	{
		color = m_diffuse * color /(Kd/K_Sum);
	}
	else if( mychoice == SPECULAR )
	{
		color = m_specular * color /(Ks/K_Sum);
	}
	else if( mychoice == TRANSMISSION )
	{
		color = Kt * color /(Kt_Y/K_Sum);
	}
	return color;
}
//
V PhongBRDF::GetColor( const Hit& hit, const V& eyePos )
{
	V color;
	color += GetDirectColor( hit, eyePos );
	color += GetInDirectColor( hit, eyePos);
	return color/PROB_SURVIVE;
}
//