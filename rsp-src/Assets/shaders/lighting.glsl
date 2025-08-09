#version 330

// Input vertex attributes (from vertex shader)
in vec3 fragPosition;
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform float brightness;

// Output fragment color
out vec4 finalColor;

void main() {
    // Do the lighting
    vec4 texelColor = texture(texture0, fragTexCoord);
    vec4 tint = texelColor * fragColor;

    vec3 ambient = vec3(brightness);
    vec4 result = vec4(ambient, 1.0) * tint;

    finalColor = result;
}
