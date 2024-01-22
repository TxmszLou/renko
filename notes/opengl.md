## Modern OpenGL before drawing need:
* vertex buffer ( a buffer ): in GPU's vRAM
* shader: a program that runs on GPU to tell GPU how to interpret the data in the buffer

### OpenGL runs as a state machine
Workflow:
- choose this buffer (glBindBuffer)
- use this shader
- draw! (based on the current state)

### See the doc!

### Vertex Attributes

A vertex contains more data (attributes) than just the position (position, texture coordinates, normals, colors, binormals, tangents, etc).

glVertexAttribPointer
 - index: the index of the attribute to modify
 - size: how many pieces of data of type type
 - type: the type of data
 - normalized: if to normalize to 0-1
 - stride: size of each vertex (i.e. the byte offset between two vertices)
 - pointer: offset of each attribute in a vertex

### Shaders

1) Vertex shader
- called for each vertex
- tell OpenGL where I want the vertex to be in the window
2) Fragment shader (pixel shader)
- run once for each pixel need to drawn
- (all the pixels between the three vertices)
- decide the color to shade

### Index Buffers
- avoid redundant vertex data
glGenBuffers(1, &index_buffer_object);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object); // select the buffer, tells the GPU buffer is just an array
glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW); // copy data to vertex buffer

### Debugging
* glGetError
* glDebugMessageCallback

### Uniform
Get data from CPU

glGetUniformLocation -> get location of the unifrom in buffer
glUniform** -> send data to GPU at the given location

## Vertex Array Object
object containing both the vertex buffer and the layout so that we don't need to specify the layout of buffer each time we bind it.

- For GLFW_OPENGL_CORE_PROFILE there is no default vertex array object, so we cannot set vertex attributes
- Need to create vertex array objects

# Renderer (abstractions)

Keep track each vertex buffer and index buffer as an object

# Texture
picture to draw

png file -> cpu an array of pixels (rgba) -> texture in opengl, bind -> shader on gpu reads the data each time want to draw

# Blending

Determines how to combine the output color with what is already in our target buffer.
- output (source): the color we output from fragment shader
- target buffer (destination): the buffer our fragment shader is drawing to

glEnalbe(GL_BLEND)
glBlendFunc(src, dest)
- how the src RGBA factor is computed 
- how the dest RGBA factor is computed
glBlendEquation(mode)
- how we combine the src and dest colors

example:
src = GL_SRC_ALPHA -> f_src = alpha comp of src
dst = GL_ONE_MINUS_SRC_ALPHA -> f_dest = 1 - alpha comp of dest

result = src * f_src + dest * f_dest
