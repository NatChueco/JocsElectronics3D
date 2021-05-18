//this var comes from the vertex shader
//they are baricentric interpolated by pixel according to the distance to every vertex
varying vec3 v_wPos;
varying vec3 v_wNormal;


//here create uniforms for all the data we need here

uniform vec3 phong_light_;
uniform vec3 phong_view_;

uniform vec3 pka;
uniform vec3 pkd;
uniform vec3 pks;
uniform float palpha;

uniform vec3 pi_a;
uniform vec3 pi_d;
uniform vec3 pi_s; 
varying vec3 v_Color;


void main()
{

	//here we set up the normal as a color to see them as a debug

	vec3 v_wNormal_ = normalize(v_wNormal);

	//here write the computations for PHONG.
	//for GOURAUD you dont need to do anything here, just pass the color from the vertex shader
	//We separate as much as possible the equation computes in order to make it easier to find errors and debug

	vec3 phong_light = phong_light_ - v_wPos;
	phong_light = normalize(phong_light);

	vec3 phong_view = phong_view_ - v_wPos;
	phong_view = normalize(phong_view);

	vec3 phong_r = reflect(-phong_light, v_wNormal_);
	phong_r = normalize(phong_r);

	vec3 amb = pka*pi_a;
	float l_dot_n = dot(phong_light, v_wNormal_);
	l_dot_n = clamp(l_dot_n, 0.0, 1.0);

	float r_dot_view = dot(phong_r, phong_view);
	r_dot_view = clamp(r_dot_view, 0.0, 1.0);

	vec3 kd_ldotn = pkd * (l_dot_n);
	float rpowalpha = pow(r_dot_view, palpha);
	vec3 ks_r = pks * rpowalpha;
	vec3 diff = kd_ldotn * pi_d;
	vec3 spec = ks_r * pi_s;
	
	vec3 v_Color= amb + diff  + spec;

	v_Color = clamp(v_Color,0,1);

	//set the ouput color por the pixel
	gl_FragColor = vec4( v_Color, 1.0 ) * 1.0;
}
