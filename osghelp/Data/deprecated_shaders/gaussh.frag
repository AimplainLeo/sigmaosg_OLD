// gaussh.frag

uniform sampler2D Src;
uniform float fTexH;
varying vec2 texCoord;

vec4 gaussFilter[7];

void main(void)
{
	float texScaler = 1.0/fTexH;
	float texOffset = -0.5/fTexH;

   vec4 color = vec4(0.0,0.0,0.0,0.0);

   gaussFilter[0] = vec4(-3.0, 0.0, 0.0,  1.0/64.0);
   gaussFilter[1] = vec4(-2.0, 0.0, 0.0,  6.0/64.0);
   gaussFilter[2] = vec4(-1.0, 0.0, 0.0, 15.0/64.0);
   gaussFilter[3] = vec4( 0.0, 0.0, 0.0, 20.0/64.0);
   gaussFilter[4] = vec4( 1.0, 0.0, 0.0, 15.0/64.0);
   gaussFilter[5] = vec4( 2.0, 0.0, 0.0,  6.0/64.0);
   gaussFilter[6] = vec4( 3.0, 0.0, 0.0,  1.0/64.0);
   
   int i;
   for (i=0;i<7;i++)
   {
      color += texture2D(Src,vec2(texCoord.x + gaussFilter[i].x * texScaler + texOffset,
                                  texCoord.y + gaussFilter[i].y * texScaler + texOffset)) * 
                    gaussFilter[i].w;
   } 
   
   color.a *= 2.0;
   
   gl_FragColor = color;
}