#ifndef GLOBALVARIABLES_H
# define GLOBALVARIABLES_H

//Mandatory format for the file passed as parameter
# define EXTENSION ".rt"

//Whitespaces that can be found within the file and not treated as errors
# define WHITESPACES "	 "

//Those are the valid indentifiers that for objects contained in the .rt file 
# define AMBIENT_LIGHTNING "A"
# define CAMERA "C"
# define LIGHT "L"
# define SPHERE "sp"
# define PLANE "pl"
# define CYLINDER "cy"

//This one only concerns the bonus part
# define CONE "co"

//Those are the identifiers we use within our object structure. Basically, any
//AMBIENT_LIGHTNING occurence turns into a EALIGHT
# define EALIGHT 65
# define ECAMERA 66
# define ELIGHT 67
# define ESPHERE 68
# define EPLANE 69
# define ECYLINDER 70
# define ECONE 71

//Those are the max and min limitations imposed by the subject on the normalized
//vector's values passed as paremeter of plates, cylinders and camera.
# define VECLIMIN -1.0
# define VECLIMAX 1.0

//Those are the max and min limitations imposed by the subject on the lighting
//and FOV ratios passed as paremeter for camera and light.
# define RATIOLIMIN 0.0
# define RATIOLIMAX 1.0
# define FOVCOEF 180.0

//Shortcuts for x, y, and z's indexes in their int *arr representation
# define X 0
# define Y 1
# define Z 2

//bvh
# define MAXBVHDEPTH 40
# define SURFACECOEFFICIENT 2.0f

#endif
