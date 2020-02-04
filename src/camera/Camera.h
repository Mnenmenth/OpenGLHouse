/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */

#ifndef OPENGLHOUSE_CAMERA_H
#define OPENGLHOUSE_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Camera {
public:
    Camera(glm::vec3 EyePos, glm::vec3 LookingAt, glm::vec3 WorldUp);
    Camera(glm::vec3 EyePos, glm::vec3 LookingAt, glm::vec3 WorldUp, glm::vec3 WorldForward, glm::vec3 WorldRight);

/// Camera manipulation functions
//!! 'Lat' is an abbreviation for 'Lateral', for left/right movement
//!! 'Vert' is an abbreviation for 'Vertical', for up/down movement
//!! 'Long' is an abbreviation for 'Longitudinal', for forward/back movement
//TODO:
// 1) Fix move<x>Locked - Movement speed is affected by camera pitch
// 2)   Add ability to reset roll/pitch/yaw individually

    /// Sets the translation of the world around the camera
    void setWorldTranslation(glm::vec3 translation);

    /// Moves world by delta relative to direction vectors derived from class parameters
        //! These movements are not affected by the cameras current orientation

    void moveWorld(glm::vec3 delta);
    void moveWorldLat(float delta);
    void moveWorldVert(float delta);
    void moveWorldLong(float delta);

    /// Rotates world by delta around WorldUp, WorldRight, and WorldForward direction vectors
        //! Theta is in degrees

    //void rotateWorld(glm::quat orientation);
    void rotateWorldYaw(float theta);
    void rotateWorldPitch(float theta);
    //TODO: Include rotateWorldRoll and have it rotate around (EyePos - CamPos)?
    // World direction vectors would need to be updated

    /// Sets the position of the camera within the world.
        //! This means that the camera position is set according to world perspective,
        //! not setting the translation of the entire scene/world relative to camera
        // Basically this just multiplies pos by -1.0f to turn a position within the world
        // into a scene translation that "moves" the camera to that position

    void setPosition(glm::vec3 pos);

    /// Move camera along its relatively oriented axis

    void move(glm::vec3 delta);
    void moveLat(float delta);
    void moveVert(float delta);
    void moveLong(float delta);

    /// Move camera along its relative axis, ignoring components from perpendicular axis
    /// to effectively "lock" its values along those axis
        //! Lat & Long ignore Y, Vert ignores X & Z
        // Think of it as how in an FPS the camera/character don't fly off into the sky
        // while walking around, regardless of where the camera is facing. They're "locked"
        // onto the ground

    void moveLocked(glm::vec3 delta);
    void moveLatLocked(float delta);
    void moveVertLocked(float delta);
    void moveLongLocked(float delta);

    /// Rotates camera around its LookingAt point
        //! Theta is in degrees
        // Be aware that with a first person EyePos & LookingAt configuration
        // "looking left" is a positive rotation and "looking right" is a negative
        // rotation since rotating the camera left makes it "look" to the right

    void rotate(glm::quat orientation);
    void rotateYaw(float theta);
    void rotatePitch(float theta);
    void rotateRoll(float theta);

    void setOrientation(glm::quat newOrient);
    void resetOrientation();

    /// Moves eye position closer to and farther away from "LookingAt"
    //! Min defaults to .01f and Max defaults to 100.0f
    void zoom(float delta);
    void setZoom(float newZoom);
    void setMinZoom(float zoomMin);
    void setMaxZoom(float zoomMax);
    void resetZoom();

    /// Getters
    // Untransformed eye pos
    glm::vec3 getBaseEyePos() const;
    // Transformed eye position
    glm::vec3 getWorldEyePos() const;
    glm::vec3 getLookingAt() const;
    glm::vec3 getWorldUp() const;
    glm::vec3 getWorldForward() const;
    glm::vec3 getWorldRight() const;
    glm::vec3 getWorldTranslation() const;

    // Position of Eye within world
    glm::vec3 getEyePos() const;
    glm::vec3 getPosition() const;
    glm::quat getOrientation() const;

    float getZoom() const;

    glm::mat4 getViewMatrix() const;

protected:
    glm::vec3       m_EyePos;
    const glm::vec3 m_LookingAt;

    const glm::vec3 m_WorldUp;
    const glm::vec3 m_WorldForward;
    const glm::vec3 m_WorldRight;

    glm::quat       m_Orientation;
    glm::vec3       m_Translation;

    float           m_Zoom;
    float           m_ZoomMin;
    float           m_ZoomMax;
};


#endif //OPENGLHOUSE_CAMERA_H
