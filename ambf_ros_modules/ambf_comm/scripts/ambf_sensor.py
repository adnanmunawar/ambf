#!/usr/bin/env python
#!/usr/bin/env python
# //==============================================================================
# /*
#     Software License Agreement (BSD License)
#     Copyright (c) 2019, AMBF
#     (www.aimlab.wpi.edu)

#     All rights reserved.

#     Redistribution and use in source and binary forms, with or without
#     modification, are permitted provided that the following conditions
#     are met:

#     * Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.

#     * Redistributions in binary form must reproduce the above
#     copyright notice, this list of conditions and the following
#     disclaimer in the documentation and/or other materials provided
#     with the distribution.

#     * Neither the name of authors nor the names of its contributors may
#     be used to endorse or promote products derived from this software
#     without specific prior written permission.

#     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#     "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#     LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
#     FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
#     COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
#     INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
#     BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
#     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
#     CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
#     LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
#     ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
#     POSSIBILITY OF SUCH DAMAGE.

#     \author    <http://www.aimlab.wpi.edu>
#     \author    <amunawar@wpi.edu>
#     \author    Adnan Munawar
#     \version   0.1
# */
# //==============================================================================
# Initial python2 code, but had few errors while importing in python3
# from tf import transformations
# Edited python3 code, taken from tf.transformations.py code written by Christoph Gohlke (University of California)

from transformations import quaternion_from_euler, euler_from_quaternion
from ambf_msgs.msg import SensorState
from ambf_msgs.msg import SensorCmd
from ambf_base_object import BaseObject
import rospy
from geometry_msgs.msg import Pose, Wrench


class Sensor(BaseObject):
    def __init__(self, a_name, time_out=0.1):
        """
        Constructor
        :param a_name:
        """
        super(Sensor, self).__init__(a_name, time_out)  # Set duration of Watchdog expiry

    def _clear_command(self):
        """
        Clear wrench if watchdog is expired
        :return:
        """

    def get_cound(self):
        return self._state.count

    def is_triggered(self, idx):
        """
        Check if a specific sensor element belonging to an array of sensor elements is triggered
        :param idx:
        :return:
        """
        if idx < len(self._state.triggered):
            return self._state.triggered[idx]

    def get_range(self):
        return self._state.range[0]

    def get_measurement(self, idx):
        """
        Get the measurement of a specific sensor element. The value is normalized.
        :param idx:
        :return:
        """
        if idx < len(self._state.measurement):
            return self._state.measurement[idx]

    def get_sensed_objects(self, idx):
        """
        Get the name of sensed object by a specific sensor element.
        :param idx:
        :return:
        """
        if idx < len(self._state.sensed_objects):
            return self._state.sensed_objects[idx]