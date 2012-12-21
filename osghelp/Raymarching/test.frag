// test.frag

uniform float osg_FrameTime;		

//Util Start
vec2 ObjUnion( in vec2 obj0, in vec2 obj1 ) 
{
  if (obj0.x<obj1.x)
  	return obj0;
  else
  	return obj1;
}
//Util End

//Scene Start

//Floor
vec2 obj0(in vec3 p){
  return vec2(p.y+13.0,0);
}

//Floor Color (checkerboard)
vec3 obj0_c(in vec3 p){
 if (fract(p.x*.15)>.5)
   if (fract(p.z*.15)>.5)
     return vec3(0,0,0);
   else
     return vec3(10,0,1);
 else
   if (fract(p.z*.15)>.5)
     return vec3(1,1,1);
   else
     	return vec3(0,0,0);
}

vec2 obj1(in vec3 p){
  return vec2(length(max(abs(p) -vec3(0.2,2.0,0.8),0.0))-0.1,1);
}

//RoundBox with simple solid color
vec3 obj1_c(in vec3 p){
	return vec3(0.3,0.6,0.99);
}

// SNIPPP
// ------
vec2 f(vec3 p) {
	vec3 p0 = p;
	vec3 p1 = p;
	
	p0.xz = mod(p0.xz,4.) - 2.;
	p1.xz = mod(p1.xz+2.,4.) - 2.;
	p1.y += 0.3;
	
	float d0 = obj1(p0).x;
	float d1 = obj1(p1).x;

	float d = min(d0, d1);
	
	return vec2(d, 1.);
}
// ------

//Objects union
vec2 inObj(in vec3 p){
  return f(p);//ObjUnion(obj0(p),obj1(p));
}


void main(void)
{
	vec2 resolution = vec2(512,512);
	float time = osg_FrameTime;
	vec2 vPos=	-1.0+2.0*gl_FragCoord.xy/resolution.xy;

	//Camera animation
	vec3 vuv=vec3(0,1,0);//Change camere up vector here
	vec3 vrp=vec3(0,1,0); //Change camere view here
	float t = time*0.125;
	vec3 prp=vec3(-sin(t)*8.0,4.0+sin(t)*2.0,cos(t)*8.0); //Change camera path position here

	//Camera setup
	vec3 vpn=normalize(vrp-prp);
	vec3 u=normalize(cross(vuv,vpn));
	vec3 v=cross(vpn,u);
	vec3 vcv=(prp+vpn);
	vec3 scrCoord=vcv+vPos.x*u*resolution.x/resolution.y+vPos.y*v;
	vec3 scp=normalize(scrCoord-prp);

	//Raymarching
	const vec3 e=vec3(0.1,0,0);
	const float maxd=60.0; //Max depth

	vec2 s = vec2(0.1,0.0);
	vec3 c,p,n;

	float f=0.1;
	for(int i=0;i<56;i++)
	{
		if (abs(s.x)<.01||f>maxd)
			break;
	
		f+= s.x;
		p = prp+scp*f;
		s = inObj(p);
	}
  
  if (f<maxd){
    if (s.y==0.0)
      c=obj0_c(p);
    else
      c=obj1_c(p);
    n=normalize(
      vec3(s.x-inObj(p-e.xyy).x,
           s.x-inObj(p-e.yxy).x,
           s.x-inObj(p-e.yyx).x));
    float b=dot(n,normalize(prp-p));
    gl_FragColor=vec4((b*c+pow(b,64.0))*(0.85-f*.01),1.0);//simple phong LightPosition=CameraPosition
  }
  else gl_FragColor=vec4(0,0,0,1); //background color
}