# 3D Lidar Raw Ping and Tracked Static and Dynamic Objects
This Master project entitled "Hot Topic Project in Computer Vision" was conducted at the TU Berlin Computer Vision in collaboration with HELLA Aglaia Mobile Vision GmbH.
In autonomous driving, to overcome the limitations of Camera and Radar for object detection and clustering, LiDAR sensor is
used. Initially, the point cloud of each cycle is pre-processed by defining a region of Interest (ROI) and outliers removal.
Secondly, the point cloud was mapped into a two dimensional grid. Continually, a region growing approach was used to cluster
the cells of the grid. Then LiDAR pings inherited the labels of the cells and the bounding boxes as well as the covariance
matrices are computed for each cluster. The approach is performed in real time and the result of the object detection as well
relative extents are reliable and were a major contribution to the project outcome.
