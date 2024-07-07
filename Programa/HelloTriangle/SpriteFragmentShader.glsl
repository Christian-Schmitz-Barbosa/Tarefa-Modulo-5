#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform vec2 texOffset;
uniform vec2 texScale;

void main()
{
    vec2 newTexCoord = TexCoord * texScale + texOffset;
    FragColor = texture(texture1, newTexCoord);
}
