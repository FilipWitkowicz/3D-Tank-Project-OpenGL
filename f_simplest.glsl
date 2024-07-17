#version 330

uniform sampler2D textureMap0; 

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

in vec2 iTexCoord0;
in vec4 n;
in vec4 l1;
in vec4 l2;
in vec4 v;

void main(void) {

	//Znormalizowane interpolowane wektory
	vec4 ml1 = normalize(l1);
	vec4 ml2 = normalize(l2);
	vec4 mn = normalize(n);
	vec4 mv = normalize(v);


	//Wektor odbity
	vec4 mr1 = reflect(-ml1, mn);
	vec4 mr2 = reflect(-ml2, mn);

	//Parametry powierzchni
	vec4 kd = texture(textureMap0, iTexCoord0);
	vec4 ks = vec4(1, 1, 1, 1);

	//Obliczenie modelu oœwietlenia
	float nl1 = clamp(dot(mn, ml1), 0, 1);
	float nl2 = clamp(dot(mn, ml2), 0, 1);
	float rv1 = pow(clamp(dot(mr1, mv), 0, 1), 50);
	float rv2 = pow(clamp(dot(mr2, mv), 0, 1), 50);


	vec3 light1 = kd.rgb * nl1;
	vec3 light2 = kd.rgb * nl2;
	vec3 lightSum =  light1 + light2;

	vec3 tex1 = ks.rgb*rv1;
	vec3 tex2 = ks.rgb*rv2;
	vec3 texSum = tex1 + tex2;


	pixelColor= vec4(lightSum , kd.a) + vec4(texSum, 0);
}
