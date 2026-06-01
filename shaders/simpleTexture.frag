#version 330 core

out vec4 colour;
in vec2 lTexCoord;

in float lChargeE;

//I believe that the number the spec gaurantees is 8
uniform sampler2D uTexture0;
uniform vec3 uLight = vec3(1.0f, 1.0f, 1.0f);

uniform bool uIgnoreTextures = false;

void main() {

    vec4 fragColour = vec4(uLight, 1.0f);

    if (lChargeE > 0.0f) {
    //Positive Charge
        fragColour = fragColour * vec4(0.0f, 0.5f, 0.7f, 1.0f);
    }
    if (lChargeE < 0.0f) {
        fragColour = fragColour * vec4(0.8f, 0.2f, 0.0f, 1.0f);
    }

    if (! uIgnoreTextures) {
        fragColour = fragColour * (texture(uTexture0, lTexCoord)) * (vec4(uLight, 1.0f));
    }

    colour = fragColour;
}