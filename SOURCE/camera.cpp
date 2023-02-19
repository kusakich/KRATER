#include <util.hpp>
#include <core.hpp>
#include <camera.hpp>

#include <glad/glad.h>

Camera::Camera(glm::vec3 pposition, glm::vec3 protation) :
position_(pposition),
rotation_(protation),
position(position_),
rotation(rotation_),
viewVector(viewVector_),
view(view_),
projection(projection_)
{
  view_ = glm::mat4(1);
  view_ = glm::rotate(view,  rotation_.y, glm::vec3(0,1,0));
  view_ = glm::rotate(view, -rotation_.x, glm::vec3(1,0,0));
  view_ = glm::rotate(view,  rotation_.z, glm::vec3(0,0,1));
  view_ = glm::translate(view, -position_);
  projection_ = glm::mat4(1);
}

void Camera::update()
{
  view_ = glm::mat4(1);
  view_ = glm::rotate(view,  rotation_.y, glm::vec3(0,1,0));
  view_ = glm::rotate(view, -rotation_.x, glm::vec3(1,0,0));
  view_ = glm::rotate(view,  rotation_.z, glm::vec3(0,0,1));
  view_ = glm::translate(view, -position_);
}

void Camera::translate(glm::vec3 direction)
{
  position_ += direction;
}

void Camera::rotateX(Float32 angle)
{
  rotation_.x +=   angle;
  if(rotation_.x > 0.99*PI)
    rotation_.x  = 0.99*PI;
  if(rotation_.x < 0.01)
    rotation_.x  = 0.01;
}

void Camera::rotateY(Float32 angle)
{
  rotation_.y    +=  angle;
  if(rotation_.y >   2.0*PI)
    rotation_.y  -=  2.0*PI;
  if(rotation_.y <   0.0)
    rotation_.y  +=  2.0*PI;
}

void Camera::rotateZ(Float32 angle)
{
  rotation_.z    +=  angle;
  if(rotation_.z >   2.0*PI)
    rotation_.z  -=  2.0*PI;
  if(rotation_.z <   0.0)
    rotation_.z  +=  2.0*PI;
}

void Camera::setProjectionMatrix(glm::mat4 matrix)
{
  projection_ = matrix;
}

glm::vec3 Camera::raycast(Float32 delta)
{
  UInt32 width = core::window::getWidth();
  UInt32 height = core::window::getHeight();
  Float32 z;
  glm::vec3 result = glm::vec3(1.0);
  glReadPixels(width/2, height/2, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z);
  result = glm::unProject(glm::vec3(width/2,height/2,z),
                          view_, projection_,
                          glm::vec4(0,0,width,height));

  if(delta != 0.0)
  {
    viewVector_ = result - position_;
    viewVector_ = glm::normalize(viewVector_);
    result += viewVector_*delta;
  }
  return result;
}