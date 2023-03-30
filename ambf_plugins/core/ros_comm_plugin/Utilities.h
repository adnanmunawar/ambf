#ifndef PLUGIN_UTILITIES_H
#define PLUGIN_UTILITIES_H

#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/WrenchStamped.h>
#include <afFramework.h>

namespace chai3d{

    cQuaternion toQuaternion(const geometry_msgs::Quaternion& msg){
        return cQuaternion(msg.w, msg.x, msg.y, msg.z);
    }

    cMatrix3d toRotationMatrix(const geometry_msgs::Quaternion& msg){
        cQuaternion quat = toQuaternion(msg);
        cMatrix3d rotMat;
        quat.toRotMat(rotMat);
        return rotMat;
    }

    cVector3d toVector(const geometry_msgs::Vector3& msg){
        return cVector3d(msg.x, msg.y, msg.z);
    }

    cVector3d toVector(const geometry_msgs::Point& msg){
        return cVector3d(msg.x, msg.y, msg.z);
    }

    cTransform toTransform(const geometry_msgs::Pose& msg){
        return cTransform(toVector(msg.position), toRotationMatrix(msg.orientation));
    }

    cTransform toTransform(const geometry_msgs::PoseStamped& msg){
        return toTransform((msg.pose));
    }
}

namespace bullet{

    btQuaternion toQuaternion(const geometry_msgs::Quaternion& msg){
        return btQuaternion(msg.x, msg.y, msg.z, msg.w);
    }

    btMatrix3x3 toRotationMatrix(const geometry_msgs::Quaternion& msg){
        return btMatrix3x3(toQuaternion(msg));

    }

    btVector3 toVector(const geometry_msgs::Vector3& msg){
        return btVector3(msg.x, msg.y, msg.z);
    }

    btVector3 toVector(const geometry_msgs::Point& msg){
        return btVector3(msg.x, msg.y, msg.z);
    }

    btTransform toTransform(const geometry_msgs::Pose& msg){
        return btTransform(toQuaternion(msg.orientation), toVector(msg.position));
    }

    btTransform toTransform(const geometry_msgs::PoseStamped& msg){
        return toTransform(msg.pose);
    }
}

#endif
