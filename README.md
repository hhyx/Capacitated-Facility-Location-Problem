# Capacitated-Facility-Location-Problem
Suppose there are n facilities and m customers. We wish to choose:

 1. which of the n facilities to open 
 2. the assignment of customers to facilities

Note：
 1. The objective is to minimize the sum of the opening cost and the
    assignment cost.
 2. The total demand assigned to a facility must not exceed its
    capacity.

- Input format
 
![Input format](https://github.com/hhyx/Capacitated-Facility-Location-Problem/blob/master/image/Input_format.png)

- Result_requirement
 
![Result requirement](https://github.com/hhyx/Capacitated-Facility-Location-Problem/blob/master/image/Result_requirement.png)




#  运行方式
 
- 编译：

  g++ CFTPSA.cpp -o CFTPSA -std=c++11

  g++ CFTPGreedy.cpp -o CFTPGreedy -std=c++11

  g++ runCFTP.cpp -o run

- 运行单个文件，输入文件序号

  CFTPSA 1

  CFTPGreedy 1

- 运行所有文件，输入文件起始序号，结束序号，每个文件运行次数

  run 

  1 71 10
