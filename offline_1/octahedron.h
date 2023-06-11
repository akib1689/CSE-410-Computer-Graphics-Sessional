
class octahedron {
private:
  // used for top and bottom
  float vertex_color_1[3] = {1.0f, 0.0f, 0.0f}; // red
  // blue green alternates for the sides
  float vertex_color_2[3] = {0.0f, 1.0f, 0.0f}; // green
  float vertex_color_3[3] = {0.0f, 0.0f, 1.0f}; // blue

  float sides_color_1[3] = {1.0f, 0.75f, 0.7f}; // pink

  float sides_color_2[3] = {0.0f, 1.0f, 1.0f}; // cyan

  // triangle origin point
  float top[3] = {0.0f, 0.0f, 1.0f};   // top
  float left[3] = {1.0f, 0.0f, 0.0f};  // left
  float right[3] = {0.0f, 1.0f, 0.0f}; // right

  float angleX = 0.0f;
  float angleY = 0.0f;
  float angleZ = 0.0f;

  float length_of_cylinder = 1.41421356237f; // sqrt(2)

  // used for drawing the triangles
  void draw_triangle(float[3], float[3], float[3], float[3]);

public:
  octahedron(/* args */);
  ~octahedron();
  // draws the octahedron
  void draw_octahedron();
  // rotates the octahedron about the x axis by the given angle
  void rotateX(float); // a, d key listener
  // rotates the octahedron about the y axis
  void rotateY(float); // no listener until now
  // z axis
  void rotateZ(float); // no listener until now

  void transform_to_sphere(); // , key listener

  void transform_to_octahedron(); // . key listener
};
