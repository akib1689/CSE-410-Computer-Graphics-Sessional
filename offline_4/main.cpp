#define _USE_MATH_DEFINES
#define PI_DEGREE 180.0

// iostream and fstream for reading and writing files
#include <fstream>
#include <iomanip>
#include <iostream>

#include <GL/glut.h>  // GLUT, includes glu.h and gl.h

// math library
#include <cmath>
#include <sstream>
#include <string>
#include <vector>

#include "bitmap_image.hpp"
#include "checker_board.cpp"
#include "color.cpp"
#include "cube.cpp"
#include "light.cpp"
#include "line.cpp"
#include "pixel_line_map.cpp"
#include "pyramid.cpp"
#include "shape.cpp"
#include "sphere.cpp"
#include "spot_light.cpp"
#include "triangle.cpp"
#include "vector3d.cpp"

using namespace std;

bool draw_axis_flag = true;

// global variable
// camera
Vector3D camera;
// look vector
Vector3D look;
// up vector
Vector3D up;

// test
Triangle t;

// parameters
double near_plane, far_plane, fov_y, aspect_ratio;
// level of recursion
int level_of_recursion;
// number of pixels
int number_of_pixels_y;

double width_of_cell;
double ambient_coefficient, diffuse_coefficient, reflection_coefficient;

int number_of_shapes;
int number_of_normal_light_sources;
int number_of_spot_light_sources;

// vector of shapes
vector<Shape*> shapes;
vector<Light*> normal_light_sources;
vector<SpotLight*> spot_light_sources;
/**
 * This function captures the image
 * @param filename the name of the file to be saved
 */
void capture_image(string filename, Color** frame_buffer) {
  // create the image
  bitmap_image image(number_of_pixels_y * aspect_ratio, number_of_pixels_y);
  // capture the image
  for (int i = 0; i < number_of_pixels_y * aspect_ratio; i++) {
    for (int j = 0; j < number_of_pixels_y; j++) {
      // set the color of the pixel
      image.set_pixel(i, j, frame_buffer[i][j][0] * 255,
                      frame_buffer[i][j][1] * 255, frame_buffer[i][j][2] * 255);
    }
  }
  // save the image
  image.save_image(filename.c_str());
}
/**
 * @brief This generates the lines from camera to each pixel
 * @return vector<Line> the vector of lines
 */
vector<PixelLineMap> generate_lines() {
  // calculate the x,y,z point for each pixel
  // then generate the line from camera to that point
  // we have look, camera and up vector
  // generate looking direction by look - camera
  // find right vector by cross product of looking direction and up vector
  // find up vector by cross product of right vector and looking direction
  // normalize all the vectors
  Vector3D look_vec(look - camera);
  Vector3D cross = look_vec * up;
  Vector3D up_vec = cross * look_vec;
  look_vec.normalize();
  cross.normalize();
  up_vec.normalize();
  vector<PixelLineMap> map;
  // calculate the mid point of the screen (near plane)
  Vector3D mid_point = camera + look_vec * near_plane;

  // calculate the height and width of the near plane
  float screen_height = 2 * near_plane * tan(fov_y * M_PI / (2 * PI_DEGREE));
  cout << "screen height : " << screen_height << endl;
  float screen_width = screen_height * aspect_ratio;
  cout << "screen width : " << screen_width << endl;

  // calculate the step size
  float step_x = screen_width / (number_of_pixels_y * aspect_ratio);
  float step_y = screen_height / number_of_pixels_y;
  for (int y = 0; y < number_of_pixels_y; y++) {
    // calculate the y scale factor the range can be -screen_height/2 to
    // +screen_height/2
    float y_scale = -screen_height / 2 + step_y * y + step_y / 2;
    // iterate over the x axis
    for (int x = 0; x < number_of_pixels_y * aspect_ratio; x++) {
      // calculate the x scale factor the range can be -screen_width/2 to
      // +screen_width/2
      float x_scale = -screen_width / 2 + step_x * x + step_x / 2;

      // calculate the point on the near plane
      Vector3D point = mid_point + cross * x_scale - up_vec * y_scale;

      // generate the line from camera to the point
      Vector3D direction = point - camera;
      Line line(point, direction);
      // create the pixel line map
      PixelLineMap pixel_line_map(x, y, line);
      // add the pixel line map to the vector
      map.push_back(pixel_line_map);
    }
  }
  return map;
}

/**
 * This function calculates the color of the pixel and returns it in frame
 * buffer
 * @return Color** the frame buffer
 */
Color** generate_image() {
  // create the frame buffer
  Color** frame_buffer = new Color*[((int)(number_of_pixels_y * aspect_ratio))];
  for (int i = 0; i < number_of_pixels_y * aspect_ratio; i++) {
    frame_buffer[i] = new Color[number_of_pixels_y];
  }

  // generate the lines
  vector<PixelLineMap> pixel_line_map = generate_lines();

  cout << "lines generated.........." << endl;

  // calculate the color of each pixel
  // iterate over the lines
  for (int i = 0; i < pixel_line_map.size(); i++) {
    // get the line
    PixelLineMap pixel_line = pixel_line_map[i];
    Line line = pixel_line.getLine();
    double progress = (double)i / pixel_line_map.size() * 100;
    cout << "progress : " << fixed << setprecision(2) << progress << "%\r"
         << flush;
    // cout << "line : " << i << endl;
    // line.print();

    // find the nearest intersection point
    double t_min = 1000000000;
    int nearest_shape_index = -1;
    for (int j = 0; j < shapes.size(); j++) {
      // get the shape
      Shape* shape = shapes[j];
      // calculate the intersection point
      Color color(0, 0, 0);
      double t = shape->getT(line);
      // cout << "x : " << pixel_line.getX() << " y : " << pixel_line.getY()
      //      << " t : " << t << endl;
      // check if the intersection point is valid
      if (t > 0 && (nearest_shape_index == -1 || t < t_min)) {
        // update the nearest shape index
        nearest_shape_index = j;
        // update the t_min
        t_min = t;
      }
    }

    // check if there is an intersection point
    if (nearest_shape_index != -1) {
      // get the shape
      Shape* shape = shapes[nearest_shape_index];
      // get the color
      Color color(0, 0, 0);
      // calculate the color
      double t =
          shape->intersect(line, normal_light_sources, spot_light_sources,
                           shapes, color, 1, level_of_recursion);
      // now we have the color
      // set the color in the frame buffer
      // sanity check for color
      for (int i = 0; i < 3; i++) {
        if (color[i] < 0) {
          color[i] = 0;
        }
        if (color[i] > 1) {
          color[i] = 1;
        }
      }

      frame_buffer[pixel_line.getX()][pixel_line.getY()] = color;
    }
  }

  // return the frame buffer
  return frame_buffer;
}

/**
 * @brief draw_axis
 * draws the axis
 */
void draw_axis() {
  // define 3 axis
  Vector3D X(100, 0, 0);
  Vector3D Y(0, 100, 0);
  Vector3D Z(0, 0, 100);
  Vector3D origin(0, 0, 0);

  // 3 colors for 3 axis
  Color red(1, 0, 0);
  Color green(0, 1, 0);
  Color blue(0, 0, 1);

  // draw axis
  if (draw_axis_flag) {
    glLineWidth(2.5);
    glBegin(GL_LINES);
    // x axis
    glColor3f(red[0], red[1], red[2]);
    glVertex3d(origin[0], origin[1], origin[2]);
    glVertex3d(X[0], X[1], X[2]);

    glColor3f(green[0], green[1], green[2]);
    glVertex3d(origin[0], origin[1], origin[2]);
    glVertex3d(Y[0], Y[1], Y[2]);

    glColor3f(blue[0], blue[1], blue[2]);
    glVertex3d(origin[0], origin[1], origin[2]);
    glVertex3d(Z[0], Z[1], Z[2]);

    glEnd();
  }
}

/**
 * @brief Loads the data from the file
 * @param filename the name of the file
 */
void load_parameters(string filename) {
  ifstream file(filename.c_str());
  string line;

  // read near plane, far plane, fov_y, aspect_ratio from 1st line
  getline(file, line);
  stringstream ss(line);
  ss >> near_plane >> far_plane >> fov_y >> aspect_ratio;
  // read level of recursion from 2nd line
  getline(file, line);
  stringstream ss2(line);
  ss2 >> level_of_recursion;
  // read number of pixels from 3rd line
  getline(file, line);
  stringstream ss3(line);
  ss3 >> number_of_pixels_y;
  // width of each cell of checker board
  getline(file, line);  // consume the empty line
  getline(file, line);
  stringstream ss4(line);
  ss4 >> width_of_cell;
  // ambient , diffuse and reflection coefficients for checker board
  getline(file, line);
  stringstream ss5(line);
  ss5 >> ambient_coefficient >> diffuse_coefficient >> reflection_coefficient;
  CheckerBoard* floor = new CheckerBoard(
      Vector3D(0, 0, 0), Color(1, 1, 1), ambient_coefficient,
      diffuse_coefficient, 0, reflection_coefficient, width_of_cell);
  floor->print();

  // add the floor checker board to the shapes vector
  shapes.push_back(floor);

  // consume the empty line
  getline(file, line);
  // read the number of shapes
  getline(file, line);
  stringstream ss6(line);
  ss6 >> number_of_shapes;
  cout << "number of shapes : " << number_of_shapes << endl;
  // consume the empty line
  getline(file, line);
  // read the shapes
  for (int i = 0; i < number_of_shapes; i++) {
    // read the shape type
    getline(file, line);
    stringstream ss7(line);
    // cube is written in 'cube' format
    // same for pyramid and sphere
    string shape_type;
    ss7 >> shape_type;
    cout << "i : " << i << " shape type : " << shape_type << endl;

    // switch case for different shapes
    if (shape_type == "sphere") {
      // read the position
      getline(file, line);
      stringstream ss8(line);
      double x, y, z;
      ss8 >> x >> y >> z;
      Vector3D position(x, y, z);
      // read the radius
      getline(file, line);
      stringstream ss9(line);
      double radius;
      ss9 >> radius;
      // read the color
      getline(file, line);
      stringstream ss10(line);
      float r, g, b;
      ss10 >> r >> g >> b;
      // convert the color to integer value between 0 and 255 (current range is
      // 0 and 1)
      Color color(r, g, b);
      // read the ambient, diffuse and specular, reflection coefficients
      getline(file, line);
      stringstream ss11(line);
      double ka, kd, ks, kr;
      ss11 >> ka >> kd >> ks >> kr;
      // read the specular exponent (shine)
      getline(file, line);
      stringstream ss12(line);
      int shine;
      ss12 >> shine;
      // create the sphere
      Sphere* sphere = new Sphere(position, color, ka, kd, ks, kr, shine,
                                  radius);  // create the sphere
      // add the sphere to the shapes vector
      shapes.push_back(sphere);
      cout << "sphere added" << endl;
    } else if (shape_type == "pyramid") {
      // read the position
      getline(file, line);
      stringstream ss8(line);
      double x, y, z;
      ss8 >> x >> y >> z;
      Vector3D position(x, y, z);
      // read width and height of base
      getline(file, line);
      stringstream ss9(line);
      double width, height;
      ss9 >> width >> height;

      // read the color
      getline(file, line);
      stringstream ss10(line);
      float r, g, b;
      ss10 >> r >> g >> b;
      Color color(r, g, b);
      // read the ambient, diffuse and specular, reflection coefficients
      getline(file, line);
      stringstream ss11(line);
      double ka, kd, ks, kr;
      ss11 >> ka >> kd >> ks >> kr;
      // read the specular exponent (shine)
      getline(file, line);
      stringstream ss12(line);
      int shine;
      ss12 >> shine;
      cout << "creating pyramid" << endl;
      //  create the pyramid
      Pyramid* pyramid =
          new Pyramid(position, color, ka, kd, ks, kr, shine, width, height);
      // add the pyramid to the shapes vector
      shapes.push_back(pyramid);
    } else if (shape_type == "cube") {
      // read the position
      getline(file, line);
      stringstream ss8(line);
      double x, y, z;
      ss8 >> x >> y >> z;
      Vector3D position(x, y, z);
      // read the length of each side of the cube
      getline(file, line);
      stringstream ss9(line);
      double length;
      ss9 >> length;
      // read the color
      getline(file, line);
      stringstream ss10(line);
      float r, g, b;
      ss10 >> r >> g >> b;
      // convert the color to integer value between 0 and 255 (current range is
      // 0 and 1)
      Color color(r, g, b);
      // read the ambient, diffuse and specular, reflection coefficients
      getline(file, line);
      stringstream ss11(line);
      double ka, kd, ks, kr;
      ss11 >> ka >> kd >> ks >> kr;
      // read the specular exponent (shine)
      getline(file, line);
      stringstream ss12(line);
      int shine;
      ss12 >> shine;
      cout << "creating cube" << endl;
      // create the cube
      Cube* cube = new Cube(position, color, ka, kd, ks, kr, shine, length);
      // add the cube to the shapes vector
      shapes.push_back(cube);
    }

    // consume the empty line
    getline(file, line);
  }

  // capture information about light sources
  // read the number of normal light sources
  getline(file, line);
  stringstream ss13(line);
  ss13 >> number_of_normal_light_sources;
  cout << "number of normal light sources : " << number_of_normal_light_sources
       << endl;
  // read the normal light sources
  for (int i = 0; i < number_of_normal_light_sources; i++) {
    // read the position
    getline(file, line);
    stringstream ss14(line);
    double x, y, z;
    ss14 >> x >> y >> z;
    Vector3D position(x, y, z);
    // read the fall of parameter
    getline(file, line);
    stringstream ss15(line);
    double fall_of_parameter;
    ss15 >> fall_of_parameter;
    // create the light source (color is white)
    Light* light = new Light(position, Color(1, 1, 1), fall_of_parameter);
    // add the light source to the vector
    normal_light_sources.push_back(light);
    // consume the empty line
    getline(file, line);
    cout << "normal light source parameter" << endl;
    position.print();
    cout << fall_of_parameter << endl;
  }
  // read the number of spot light sources
  getline(file, line);
  stringstream ss16(line);
  ss16 >> number_of_spot_light_sources;
  cout << "number of spot light sources : " << number_of_spot_light_sources
       << endl;
  // read the spot light sources
  for (int i = 0; i < number_of_spot_light_sources; i++) {
    // read the position
    getline(file, line);
    stringstream ss17(line);
    double x, y, z;
    ss17 >> x >> y >> z;
    Vector3D position(x, y, z);
    // read the fall of parameter
    getline(file, line);
    stringstream ss18(line);
    double fall_of_parameter;
    ss18 >> fall_of_parameter;
    // read the point to which the spot light is looking
    getline(file, line);
    stringstream ss19(line);
    double x1, y1, z1, angle;
    ss19 >> x1 >> y1 >> z1 >> angle;
    Vector3D pointing(x1, y1, z1);
    // calculate the direction vector
    Vector3D direction = pointing - position;
    // normalize the direction vector
    direction.normalize();

    // create the spot light source (color is white)
    SpotLight* spot_light = new SpotLight(position, Color(1, 1, 1),
                                          fall_of_parameter, direction, angle);
    // add the spot light source to the vector
    spot_light_sources.push_back(spot_light);
    // consume the empty line
    getline(file, line);
    cout << "spot light source parameter" << endl;
    position.print();
    cout << fall_of_parameter << endl;
    direction.print();
    cout << angle << endl;
  }

  // close the file
  file.close();
  cout << "normal light sources : " << normal_light_sources.size() << endl;
  cout << "spot light sources : " << spot_light_sources.size() << endl;
  cout << "shapes : " << shapes.size() << endl;
}

/* Initialize OpenGL Graphics */
void initGL() {
  glClearColor(0.0f, 0.0f, 0.0f,
               1.0f);       // Set background color to black and opaque
  glClearDepth(1.0f);       // Set background depth to farthest
  glEnable(GL_DEPTH_TEST);  // Enable depth testing for z-culling
  glDepthFunc(GL_LEQUAL);   // Set the type of depth-test
  glShadeModel(GL_SMOOTH);  // Enable smooth shading
  glHint(GL_PERSPECTIVE_CORRECTION_HINT,
         GL_NICEST);  // Nice perspective corrections
}

/* Handler for window-repaint event. Call back when the window first appears and
   whenever the window needs to be re-painted. */
void display() {
  glClear(GL_COLOR_BUFFER_BIT |
          GL_DEPTH_BUFFER_BIT);  // Clear color and depth buffers

  glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
  glLoadIdentity();             // Reset the projection matrix
  gluPerspective(fov_y, aspect_ratio, near_plane,
                 far_plane);  // Apply perspective projection matrix

  glMatrixMode(GL_MODELVIEW);  // To operate on model-view matrix
  glLoadIdentity();            // Reset the model-view matrix
  gluLookAt(camera[0], camera[1], camera[2], look[0], look[1], look[2], up[0],
            up[1], up[2]);

  // draw the shapes
  for (int i = 0; i < shapes.size(); i++) {
    shapes[i]->draw();
  }
  // draw the light sources
  for (int i = 0; i < normal_light_sources.size(); i++) {
    normal_light_sources[i]->draw();
  }

  // draw the spot light sources
  for (int i = 0; i < spot_light_sources.size(); i++) {
    spot_light_sources[i]->draw();
  }

  draw_axis();
  glutSwapBuffers();  // Swap the front and back frame buffers (double
                      // buffering)
}

/* Handler for window re-size event. Called back when the window first appears
   and whenever the window is re-sized with its new width and height */
void reshape(GLsizei width,
             GLsizei height) {  // GLsizei for non-negative integer
  // Compute aspect ratio of the new window
  if (height == 0)
    height = 1;  // To prevent divide by 0
  GLfloat aspect = (GLfloat)width / (GLfloat)height;

  // Set the viewport to cover the new window
  glViewport(0, 0, width, height);

  // Set the aspect ratio of the clipping volume to match the viewport
  glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
  glLoadIdentity();             // Reset
  // Enable perspective projection with fovy, aspect, zNear and zFar
  gluPerspective(fov_y, aspect, near_plane, far_plane);
}

/**
 * callback function for key press (normal keys)
 */
void key_pressed(unsigned char key, int x, int y) {
  // rate of rotation
  float rate_rotation = M_PI / 60;
  float rate_translation = 0.1;
  // calculate the look vector
  Vector3D look_vec(look - camera);
  Color** frame_buffer;

  // normalize the up vector and look vector
  up.normalize();

  // now find the cross product of look_vec and up
  Vector3D cross = look_vec * up;
  cross.normalize();

  // the vectors up look and cross are perpendicular to each other
  switch (key) {
    case '1':
      // rotate the look vector to cross vector here the axis of rotation is up
      look_vec.rotate(up, rate_rotation);
      break;
    case '2':
      // rotate the look vector away from cross vector here the axys of rotation
      // is up
      look_vec.rotate(up, -rate_rotation);
      break;
    case '3':
      // rotate the look vector to up vector here the axis of rotation is cross
      look_vec.rotate(cross, rate_rotation);
      // rotate the up vector away from look vector here the axis of rotation is
      // cross
      up.rotate(cross, rate_rotation);
      up.normalize();
      break;
    case '4':
      // rotate the look vector away from up vector here the axis of rotation is
      // cross
      look_vec.rotate(cross, -rate_rotation);
      // rotate the up vector to the look vector
      up.rotate(cross, -rate_rotation);
      up.normalize();
      break;
    case '5':
      // rotate the up vector to the cross vector here the axis of rotation is
      // look
      up.rotate(look_vec, rate_rotation / 60);
      up.normalize();
      break;
    case '6':
      // rotate the up vector away from the cross vector here the axis of
      // rotation is look
      up.rotate(look_vec, -rate_rotation / 60);
      up.normalize();
      break;
    case 'l':
      // toggle the draw axis flag
      draw_axis_flag = !draw_axis_flag;
      break;
    case 'w':
      // move the camera in the direction of up vector
      // don't change the look position
      camera[0] += up[0] * rate_translation;
      camera[1] += up[1] * rate_translation;
      camera[2] += up[2] * rate_translation;
      // new look vector
      look_vec = look - camera;
      break;
    case 's':
      // move the camera in the opposite direction of up vector
      // don't change the look position
      camera[0] -= up[0] * rate_translation;
      camera[1] -= up[1] * rate_translation;
      camera[2] -= up[2] * rate_translation;
      // new look vector
      look_vec = look - camera;
      break;
    case '0':
      // capture the image
      frame_buffer = generate_image();
      cout << "frame buffer generated" << endl;
      // save the image
      capture_image("output.bmp", frame_buffer);
      cout << "image captured" << endl;
      break;
    default:
      break;
  }
  // recalculating the look vector
  look = camera + look_vec;
  glutPostRedisplay();
}

/**
 * callback function for key press (special keys)
 */
void special_key_pressed(int key, int x, int y) {
  float rate_translation = 0.1;
  // find the up vector's perpendicular vector
  // we can find the perpendicular vector by cross product of up vector and look
  // vector
  Vector3D look_vec(look - camera);
  Vector3D up_perp = up * look_vec;
  switch (key) {
    case GLUT_KEY_LEFT:
      // move the camera in the direction of up_perp vector
      camera[0] += up_perp[0] * rate_translation;
      camera[1] += up_perp[1] * rate_translation;
      camera[2] += up_perp[2] * rate_translation;
      look[0] += up_perp[0] * rate_translation;
      look[1] += up_perp[1] * rate_translation;
      look[2] += up_perp[2] * rate_translation;
      break;
    case GLUT_KEY_RIGHT:
      // move the camera to the opposite direction of up_perp vector
      camera[0] -= up_perp[0] * rate_translation;
      camera[1] -= up_perp[1] * rate_translation;
      camera[2] -= up_perp[2] * rate_translation;
      look[0] -= up_perp[0] * rate_translation;
      look[1] -= up_perp[1] * rate_translation;
      look[2] -= up_perp[2] * rate_translation;
      break;
    case GLUT_KEY_UP:
      // move the camera in the direction of look vector
      camera[0] += look_vec[0] * rate_translation;
      camera[1] += look_vec[1] * rate_translation;
      camera[2] += look_vec[2] * rate_translation;
      look[0] += look_vec[0] * rate_translation;
      look[1] += look_vec[1] * rate_translation;
      look[2] += look_vec[2] * rate_translation;
      break;
    case GLUT_KEY_DOWN:
      // move the camera in the opposite direction of look vector
      camera[0] -= look_vec[0] * rate_translation;
      camera[1] -= look_vec[1] * rate_translation;
      camera[2] -= look_vec[2] * rate_translation;
      look[0] -= look_vec[0] * rate_translation;
      look[1] -= look_vec[1] * rate_translation;
      look[2] -= look_vec[2] * rate_translation;
      break;
    case GLUT_KEY_PAGE_UP:
      // move the camera in the up vectors direction
      camera[0] += up[0] * rate_translation;
      camera[1] += up[1] * rate_translation;
      camera[2] += up[2] * rate_translation;
      look[0] += up[0] * rate_translation;
      look[1] += up[1] * rate_translation;
      look[2] += up[2] * rate_translation;
      break;
    case GLUT_KEY_PAGE_DOWN:
      // move the camera in the opposite direction of up vector
      camera[0] -= up[0] * rate_translation;
      camera[1] -= up[1] * rate_translation;
      camera[2] -= up[2] * rate_translation;
      look[0] -= up[0] * rate_translation;
      look[1] -= up[1] * rate_translation;
      look[2] -= up[2] * rate_translation;
      break;
    default:
      break;
  }
  glutPostRedisplay();
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
  // load the parameters
  load_parameters("scene.txt");
  // initialize the camera
  look[0] = 0;
  look[1] = 0;
  look[2] = 0;
  camera[0] = 100;
  camera[1] = 100;
  camera[2] = 100;
  up[0] = 0;
  up[1] = 0;
  up[2] = 1;
  glutInit(&argc, argv);  // Initialize GLUT
  int window_width = number_of_pixels_y * aspect_ratio;
  glutInitWindowSize(
      window_width,
      number_of_pixels_y);         // Set the window's initial width & height
  glutInitWindowPosition(50, 50);  // initial window position
  glutCreateWindow(
      "Offline 4: Ray Tracing");  // Create a window with the given title
  glutDisplayFunc(
      display);  // Register display callback handler for window re-paint
  glutReshapeFunc(
      reshape);  // Register reshape callback handler for window re-size
  glutKeyboardFunc(key_pressed);         // register keyboard press callback
  glutSpecialFunc(special_key_pressed);  // register special key press callback
  initGL();                              // Our own OpenGL initialization
  glutMainLoop();  // Enter the infinitely event-processing loop
  return 0;
}