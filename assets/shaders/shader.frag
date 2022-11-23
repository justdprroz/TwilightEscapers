uniform sampler2D texture;
uniform float u_colorFactor;
uniform vec2 mouse_pos;
uniform vec2 resolution;

// Util functions

// // Function for generating pseudo random numbers
float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

// float sign (vec2 p1, vec2 p2, vec2 p3)
// {
//     return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
// }

// bool PointInTriangle (vec2 pt, vec2 v1, vec2 v2, vec2 v3)
// {
//     float d1, d2, d3;
//     bool has_neg, has_pos;

//     d1 = sign(pt, v1, v2);
//     d2 = sign(pt, v2, v3);
//     d3 = sign(pt, v3, v1);

//     has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
//     has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

//     return !(has_neg && has_pos);
// }

// Shaders
vec4 greyscale(vec4 sample) {
    float grey = 0.21 * sample.r + 0.71 * sample.g + 0.07 * sample.b;
    return vec4(
        sample.r * u_colorFactor + grey * (1.0 - u_colorFactor),
        sample.g * u_colorFactor + grey * (1.0 - u_colorFactor),
        sample.b * u_colorFactor + grey * (1.0 - u_colorFactor),
        sample.a
    );
}

vec4 light(vec4 sample) {
    // Normalized mouse position
    vec2 nmp = mouse_pos / resolution;
    nmp.y = 1.0 - nmp.y; // invert y-axis

    // Pixelated position
    vec2 p = gl_TexCoord[0];
    p -= mod(p, 1.0/256);
    
    // Get circle, distance, mouse
    vec2 dm = p - nmp;
    dm*=4;
    float lm = sqrt(dm.x*dm.x+dm.y*dm.y);
    lm*=lm;

    float f = rand(p);
    if (f > lm) {
        return vec4(sample.r, sample.g, sample.b, 1.0);
    } else {
        return vec4(sample.r, sample.g, sample.b, 0.0);
    }
}

void main() {
    gl_FragColor = texture2D(texture, gl_TexCoord[0].xy);
    // // Apply light
    // gl_FragColor = light(gl_FragColor);

    // Apply greyscale
    gl_FragColor = greyscale(gl_FragColor);
}	