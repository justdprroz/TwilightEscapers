uniform sampler2D texture;
uniform float u_colorFactor;

// Util functions

void main() {
    vec4 sample = texture2D(texture, gl_TexCoord[0].xy);
    float grey = 0.21 * sample.r + 0.71 * sample.g + 0.07 * sample.b;
    gl_FragColor = vec4(
        sample.r * u_colorFactor + grey * (1.0 - u_colorFactor),
        sample.g * u_colorFactor + grey * (1.0 - u_colorFactor),
        sample.b * u_colorFactor + grey * (1.0 - u_colorFactor),
        sample.a
    );
}	