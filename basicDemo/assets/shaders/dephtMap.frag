#version 330 core
in vec2 textCoord;

uniform sampler2D text;

void main()
{            
    if(texture2D(text, textCoord).a < 0.3)
        discard;
    gl_FragDepth = gl_FragCoord.z;
} 