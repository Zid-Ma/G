#version 330 core
out vec4 FragColor;
//out vec4 BrightColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{ 
    if(texture(screenTexture, TexCoords).a == 0)
        discard;
    else
        FragColor = texture(screenTexture, TexCoords);
}
