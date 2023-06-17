
class octahedron {
private:
  // used for top and bottom
  float vertex_color_1[3] = {1.0f, 0.0f, 0.0f}; // red
  // blue green alternates for the sides
  float vertex_color_2[3] = {0.0f, 1.0f, 0.0f}; // green
  float vertex_color_3[3] = {0.0f, 0.0f, 1.0f}; // blue

  float cylinder_color[3] = {1.0f, 1.0f, 0.0f}; // white

  float sides_color_1[3] = {1.0f, 0.0f, 0.5f}; // pink

  float sides_color_2[3] = {0.0f, 1.0f, 1.0f}; // cyan

  float background_color[3] = {0.1f, 0.1f, 0.1f}; // black

  // triangle origin point
  float top[3] = {0.0f, 0.0f, 1.0f};   // top - z axis
  float left[3] = {1.0f, 0.0f, 0.0f};  // left - x axis
  float right[3] = {0.0f, 1.0f, 0.0f}; // right - y axis

  float restoring_top[3];   // top - z axis
  float restoring_left[3];  // left - x axis
  float restoring_right[3]; // right - y axis

  float angleX = 0.0f;
  float angleY = 0.0f;
  float angleZ = 0.0f;

  float cylinder_radius = 0.0f; // radius of the cylinder
  float sphere_radius = 0.0f;   // radius of the sphere

  int cylinder_slices = 64; // number of slices in the cylinder
  int stack_count = 32;     // number of stacks of sphere
  int sector_count = 128;   // number of sectors of sphere
                            // ! we have to use power of 2 for sector count
                            // ! for smoothing of the sphere

  bool is_sphere = false; // is the octahedron a sphere or not

  float clipping_distance = 1.0f; // clipping distance

  float cylinder_dist_x = 1.41421356237f / 2.0f; // distance between
                                                 // the center and the cylinder
  float sphere_center_x = 1.0f;                  // distance between
                                                 // the center and the sphere
  float sphere_z_limit = M_PI / 2;               // z angle limit for the sphere
  float sphere_z_start = M_PI / 6;               // z angle start for the sphere
  bool triangle_visibility = true;               // visibility of the triangles
  bool cylinder_visibility = true;               // visibility of the cylinder
  bool sphere_visibility = true;                 // visibility of the sphere

  float phi = acos(-1.0f / 3.0f); // angle between the top and the side

  void draw_triangle(float[3], float[3], float[3], float[3]);
  void draw_triangle(float[3], float[3], float[3], float[3], bool[3]);
  void draw_face(float[3], float[3], float[3], float[3], bool);
  void draw_partial_cylinder(double, double, float[3], int);
  void draw_partial_sphere(double, float[3]);
  void draw_axis();
  void draw_quads(float[3], float[3], float[3], float[3]);

  // updating radius
  void update_sphere_param();

  // updating cylinder
  void increase_cylinder_param(float);
  void decrease_cylinder_param(float);

  // make the octahedron a sphere
  void make_sphere();

  // make the octahedron an octahedron
  void make_octahedron();

  // save the restoring values
  void save_restoring_values();
  // restore the values
  void restore_values();

  // debug print
  void print_all_parameters();

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

  void toggle_triangle_visibility(); // v key listener
  void toggle_cylinder_visibility(); // c key listener
  void toggle_sphere_visibility();   // b key listener
};
