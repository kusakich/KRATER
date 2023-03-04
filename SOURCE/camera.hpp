#ifndef CAMERA_HPP_INCLUDED
#define CAMERA_HPP_INCLUDED

#include <util.hpp>

class Camera
{
private:
  glm::vec3 position_;
  glm::vec3 rotation_;
  glm::mat4 view_;
  glm::mat4 projection_;
public:
  glm::vec3 const & position;
  glm::vec3 const & rotation;
  glm::mat4 const & view;
  glm::mat4 const & projection;

  Camera(glm::vec3 pposition=glm::vec3(0.0), glm::vec3 protation=glm::vec3(0.0));
  void update();
  void translate(glm::vec3 direction);
  void rotateX(Float32 angle);
  void rotateY(Float32 angle);
  void rotateZ(Float32 angle);
  void setProjectionMatrix(glm::mat4 matrix);
};

#endif