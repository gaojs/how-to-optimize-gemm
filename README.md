# How To Optimize Gemm wiki pages. 如何优化Gemm wiki页面
https://github.com/flame/how-to-optimize-gemm/wiki

Copyright by Prof. Robert van de Geijn (rvdg@cs.utexas.edu). 版权所有Robert van de Geijn教授(rvdg@cs.utexas.edu). 美国德克萨斯大学计算机科学系

Adapted to Github Markdown Wiki by Jianyu Huang (jianyu@cs.utexas.edu). 黄建宇改编自Github Markdown Wiki(jianyu@cs.utexas.edu).

# Table of contents. 目录

  * [The GotoBLAS/BLIS Approach to Optimizing Matrix-Matrix Multiplication - Step-by-Step](../../wiki#the-gotoblasblis-approach-to-optimizing-matrix-matrix-multiplication---step-by-step)GotoBLAS/BLIS逐步优化矩阵矩阵乘法的方法
  * [NOTICE ON ACADEMIC HONESTY](../../wiki#notice-on-academic-honesty)关于学术诚信的通知
  * [References](../../wiki#references)参考文献
  * [Set Up](../../wiki#set-up)建立
  * [Step-by-step optimizations](../../wiki#step-by-step-optimizations)逐步优化
  * [Computing four elements of C at a time](../../wiki#computing-four-elements-of-c-at-a-time)一次计算C的四个元素
    * [Hiding computation in a subroutine](../../wiki#hiding-computation-in-a-subroutine)在子程序中隐藏计算
    * [Computing four elements at a time](../../wiki#computing-four-elements-at-a-time)一次计算四个元素
    * [Further optimizing](../../wiki#further-optimizing)进一步优化
  * [Computing a 4 x 4 block of C at a time](../../wiki#computing-a-4-x-4-block-of-c-at-a-time)一次计算一个4 x 4的C块
    * [Repeating the same optimizations](../../wiki#repeating-the-same-optimizations)重复相同的优化
    * [Further optimizing](../../wiki#further-optimizing-1)进一步优化
    * [Blocking to maintain performance](../../wiki#blocking-to-maintain-performance)分块以维持性能
    * [Packing into contiguous memory](../../wiki#packing-into-contiguous-memory)打包到连续内存中
  * [Acknowledgement](../../wiki#acknowledgement)致谢

# Related Links. 相关链接
* [BLISlab: A Sandbox for Optimizing GEMM](https://github.com/flame/blislab)BLISlab：优化GEMM的沙盒
* [GEMM: From Pure C to SSE Optimized Micro Kernels](http://apfel.mathematik.uni-ulm.de/~lehn/sghpc/gemm/)GEMM：从纯C到SSE优化微内核

# Acknowledgement. 致谢
This material was partially sponsored by grants from the National Science Foundation (Awards ACI-1148125/1340293 and ACI-1550493).
本材料部分由国家科学基金会资助（ACI-1148125/1340293和ACI-1550493奖）。
_Any opinions, findings and conclusions or recommendations expressed in this material are those of the author(s) and do not necessarily reflect the views of the National Science Foundation (NSF)._
本材料中表达的任何意见、发现和结论或建议都是作者的意见、发现、结论或建议，不一定反映美国国家科学基金会（NSF）的观点。