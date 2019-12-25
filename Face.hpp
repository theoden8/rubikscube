#pragma once


enum class Face {
  NFACE  ,
  Z_FRONT,
  Z_BACK ,
  Y_FRONT,
  Y_BACK ,
  X_FRONT,
  X_BACK ,
};

enum class Rotation {
  NROTATION,
  CLOCKWISE,
  COUNTERCLOCKWISE,
};

bool is_opposite_face(const Face a, const Face b) {
  return
    (a == Face::Z_FRONT && b == Face::Z_BACK)
    || (a == Face::Z_BACK && b == Face::Z_FRONT)
    || (a == Face::Y_FRONT && b == Face::Y_BACK)
    || (a == Face::Y_BACK && b == Face::Y_FRONT)
    || (a == Face::X_FRONT && b == Face::X_BACK)
    || (a == Face::X_BACK && b == Face::X_FRONT);
}

std::string face_to_string(const Face f) {
  switch(f) {
    case Face::NFACE:return "nface";
    case Face::X_FRONT:return "x_front";
    case Face::X_BACK: return "x_back";
    case Face::Y_FRONT:return "y_front";
    case Face::Y_BACK: return "y_back";
    case Face::Z_FRONT:return "z_front";
    case Face::Z_BACK: return "z_back";
  }
  TERMINATE("no such face");
}

Face get_random_face() {
  std::array<Face, 6> faces = {
    Face::X_FRONT, Face::X_BACK,
    Face::Y_FRONT, Face::Y_BACK,
    Face::Z_FRONT, Face::Z_BACK,
  };
  return faces[rand() % faces.size()];
}

Rotation get_random_rotation() {
  return (rand() & 1) ? Rotation::CLOCKWISE : Rotation::COUNTERCLOCKWISE;
}

std::string rotation_to_string(const Rotation r) {
  switch(r) {
    case Rotation::NROTATION:return "nrot";
    case Rotation::CLOCKWISE:return "cw";
    case Rotation::COUNTERCLOCKWISE:return "ccw";
  }
  TERMINATE("no such rotation");
}

Face map_face_rotation(const Face face, const Rotation rot, const Face axis_face) {
  if(rot == Rotation::NROTATION || axis_face == Face::NFACE)return face;
  if(face == Face::NFACE)return Face::NFACE;

  bool axis_back = true;
  std::vector<std::vector<Face>> subgroup;
  switch(axis_face) {
    case Face::Z_FRONT:
      axis_back = false;
    case Face::Z_BACK:
      subgroup = {
        {Face::Z_FRONT},
        {Face::Z_BACK},
        {Face::X_FRONT, Face::Y_FRONT, Face::X_BACK, Face::Y_BACK},
      };
    break;
    case Face::Y_FRONT:
      axis_back = false;
    case Face::Y_BACK:
      subgroup = {
        {Face::Y_FRONT},
        {Face::Y_BACK},
        {Face::Z_FRONT, Face::X_FRONT, Face::Z_BACK, Face::X_BACK},
      };
    break;
    case Face::X_FRONT:
      axis_back = false;
    case Face::X_BACK:
      subgroup = {
        {Face::X_FRONT},
        {Face::X_BACK},
        {Face::Y_FRONT, Face::Z_FRONT, Face::Y_BACK, Face::Z_BACK},
      };
    break;
  }
  const bool hit_inverse = (
    axis_back && rot == Rotation::CLOCKWISE)
    || (!axis_back && rot == Rotation::COUNTERCLOCKWISE
    );
  for(auto &orbit : subgroup) {
    for(int i = 0; i < orbit.size(); ++i) {
      if(orbit[i] == face) {
        if(hit_inverse) {
          return orbit[(i + orbit.size() - 1) % orbit.size()];
        } else {
          return orbit[(i + 1) % orbit.size()];
        }
        /* std::string s = ""; */
        /* for(auto &e : orbit) { */
        /*   s += face_to_string(e) + " "; */
        /* } */
        /* while(s.length() < 30)s.push_back(' '); */
        /* if(hit_inverse) { */
        /*   printf("    %s [%s] -> %s\n", */
        /*     face_to_string(face).c_str(), */
        /*     s.c_str(), */
        /*     face_to_string(orbit[(i + orbit.size() - 1) % orbit.size()]).c_str()); */
        /*   return orbit[(i + orbit.size() - 1) % orbit.size()]; */
        /* } else { */
        /*   printf("    %s [%s] -> %s\n", */
        /*     face_to_string(face).c_str(), */
        /*     s.c_str(), */
        /*     face_to_string(orbit[(i + 1) % orbit.size()]).c_str()); */
        /*   return orbit[(i + 1) % orbit.size()]; */
        /* } */
      }
    }
  }
  TERMINATE("appropriate subgorup orbit not found");
}

