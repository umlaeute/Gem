#extension GL_ARB_texture_rectangle : enable
// Cyrille Henry 2024

uniform sampler2DRect texture0; // input texture
uniform float t; // current time, in order to adjust the phase of signals from 1 frame to the other

varying vec2 texcoord0; // pixel coordinate in the image

float oscilator(float phase, float amplitude) { 
	return (0.5 + 0.5*amplitude*cos(radians(360.*phase)));
}

float mtof(float midi) { // midi value to frequency value
	return (440. * pow(2.,(midi-69.)/12.));
}

float ftof(float f) { // real frequency value to f value
	// f = 1 -> 1 period is 4096 sample
	return ((f/48000.) * 4096.); // for a sampling frequency of 48K
}

void main (void)
{
	float outcolor, i; 
    float time = texcoord0.s /4096. ; // use X coordinate as time
	
	outcolor = 0.;
	for(i=0.; i<200.; i++) { // 200 oscillators
		float f_i = i/199.;
		// use texture as oscillator amplitude
		float amplitude1 = texture2DRect(texture0, vec2(f_i*4095.,1)).r; // previous frame amplitude
		float amplitude2 = texture2DRect(texture0, vec2(f_i*4095.,-1)).r; // current frame amplitude
		float amplitude = mix(amplitude1, amplitude2, time); // interpolation between this 2 vectors
		amplitude *= amplitude; // pow(2) curve
		
		float f = ftof(mtof( mix(20.,110.,f_i) )); // uniform mapping between midi note 20 to 110
		
		float phase;
		//phase = fract((time+t) * f); // not accurate enough when t is high
		// so we split the computation of the phase to small value in order to keep precision 
		phase = fract(time*fract(f)) + fract(time*floor(f))  + fract(t*fract(f)); 
			// we can skip some operations since : 
				// time is < 1.; so floor(time)=0.; and fract(time) = time;
				// fract(t) = 0.; so floor(t) = t;
		
		outcolor += oscilator(phase, amplitude);
    }
    outcolor /= i; // normalisation

    outcolor = clamp (outcolor,0.,1.);
    // output to 3 int value 
    //float r = floor(outcolor*255.)/255.; // crop to 8 bits
    //float g_float = 256.*(outcolor - r);
    //float g = floor(g_float*255.)/255.; // crop to 8 bits
    //float b_float = 256.*(g_float - g);
    //float b = floor(b_float*255.)/255.; // crop to 8 bits so the 24th bit is correct. it's maybe overkill...

	// Float output
    gl_FragColor = vec4(outcolor, outcolor, outcolor, 1.0);

}
