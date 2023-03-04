#include <util.hpp>
#include <camera.hpp>

Camera::Camera(glm::vec3 pposition, glm::vec3 protation) :
position_(pposition),
rotation_(protation),
position(position_),
rotation(rotation_),
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