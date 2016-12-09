FROM ros:kinetic
RUN apt-get update
RUN apt-get install build-essential -y
RUN apt-get install -y ros-kinetic-desktop

# ros env variables
ENV ROS_ROOT=/opt/ros/kinetic/share/ros
ENV ROS_PACKAGE_PATH=/root/catkin_ws/src:/opt/ros/kinetic/share:/opt/ros/kinetic/stacks
ENV ROS_MASTER_URI=http://localhost:11311
ENV LD_LIBRARY_PATH=/root/catkin_ws/devel/lib:/root/catkin_ws/devel/lib/x86_64-linux-gnu:/opt/ros/kinetic/lib/x86_64-linux-gnu:/opt/ros/kinetic/lib
ENV CATKIN_TEST_RESULTS_DIR=/root/catkin_ws/build/test_results
ENV CPATH=/root/catkin_ws/devel/include:/opt/ros/kinetic/include
ENV ROS_TEST_RESULTS_DIR=/root/catkin_ws/build/test_results
ENV PATH=/root/catkin_ws/devel/bin:/opt/ros/kinetic/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
ENV ROSLISP_PACKAGE_DIRECTORIES=/root/catkin_ws/devel/share/common-lisp
ENV ROS_DISTRO=kinetic
ENV PYTHONPATH=/root/catkin_ws/devel/lib/python2.7/dist-packages:/opt/ros/kinetic/lib/python2.7/dist-packages
ENV PKG_CONFIG_PATH=/root/catkin_ws/devel/lib/pkgconfig:/root/catkin_ws/devel/lib/x86_64-linux-gnu/pkgconfig:/opt/ros/kinetic/lib/x86_64-linux-gnu/pkgconfig:/opt/ros/kinetic/lib/pkgconfig
ENV CMAKE_PREFIX_PATH=/root/catkin_ws/devel:/opt/ros/kinetic
ENV ROS_ETC_DIR=/opt/ros/kinetic/etc/ros

WORKDIR /root/
RUN git clone https://github.com/KDE/extra-cmake-modules /root/ecm && cd ecm && cmake . && make install

WORKDIR /opt
RUN git clone https://github.com/papyros/qml-material /opt/qml-material && cd qml-material && git checkout 4af0e33123f3 && cmake . && make install

# clone the actual roboy ros_control repo and build it
WORKDIR /root/catkin_ws
RUN git clone --recursive https://github.com/Roboy/roboy_gui /root/catkin_ws/roboy_gui
RUN cd roboy_gui && catkin_make 
