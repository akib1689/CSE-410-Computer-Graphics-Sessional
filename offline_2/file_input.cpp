// @author: akibur rahman
#include <iostream>

#include <fstream>

#include <sstream>
#include <string>
#include <vector>

using namespace std;

// enum for the different types of commands
enum Command { TRIANGLE, TRANSLATE, SCALE, ROTATE, PUSH, POP, END, NONE };

/**
 * from a line returns the parameters of that line
 * @param line the line to be parsed
 * @return vector of doubles
 */
vector<double> splitLine(const string &line) {
  vector<double> values;
  istringstream iss(line);
  string token;

  while (iss >> token) {
    double value = stod(token);
    values.push_back(value);
  }

  return values;
}

/**
 * from a string returns the enum of that string
 */
Command getCommand(const string &command) {
  if (command == "triangle") {
    return TRIANGLE;
  } else if (command == "translate") {
    return TRANSLATE;
  } else if (command == "scale") {
    return SCALE;
  } else if (command == "rotate") {
    return ROTATE;
  } else if (command == "push") {
    return PUSH;
  } else if (command == "pop") {
    return POP;
  } else if (command == "end") {
    return END;
  } else {
    return NONE;
  }
}

int main() {
  // open the scene.txt file
  ifstream scene_file("scene.txt");

  // read the first line
  string line;
  getline(scene_file, line);

  // parse the line for eye.x, eye.y, eye.z
  vector<double> eye = splitLine(line);

  // read the second line
  getline(scene_file, line);

  // parse the line for look.x, look.y, look.z
  vector<double> look = splitLine(line);

  // read the third line
  getline(scene_file, line);

  // parse the line for up.x, up.y, up.z
  vector<double> up = splitLine(line);

  // read the fourth line
  getline(scene_file, line);

  // parse the line for fovY, aspectRatio, near, far
  vector<double> perspective = splitLine(line);

  // print the values
  cout << "eye: " << eye[0] << " " << eye[1] << " " << eye[2] << endl;
  cout << "look: " << look[0] << " " << look[1] << " " << look[2] << endl;
  cout << "up: " << up[0] << " " << up[1] << " " << up[2] << endl;
  cout << "fovY: " << perspective[0] << endl;
  cout << "aspectRatio: " << perspective[1] << endl;
  cout << "near: " << perspective[2] << endl;
  cout << "far: " << perspective[3] << endl;

  // while loop to read the rest of the lines
  while (getline(scene_file, line)) {
    // check the line for the following cases
    // 1. triangle
    // 2. translate
    // 3. scale
    // 4. rotate
    // 5. push
    // 6. pop
    // 7. end
    // cout << line << endl;

    switch (getCommand(line)) {
    case TRIANGLE:
      cout << "triangle" << endl;
      break;
    case TRANSLATE:
      cout << "translate" << endl;
      break;
    case SCALE:
      cout << "scale" << endl;
      break;
    case ROTATE:
      cout << "rotate" << endl;
      break;
    case PUSH:
      cout << "push" << endl;
      break;
    case POP:
      cout << "pop" << endl;
      break;
    case END:
      cout << "end" << endl;
      break;
    }
  }

  return 0;
}