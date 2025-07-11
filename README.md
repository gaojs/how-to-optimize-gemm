# How To Optimize Gemm. 如何优化通用矩阵乘法
https://github.com/flame/how-to-optimize-gemm/wiki

Copyright by Prof. Robert van de Geijn (rvdg@cs.utexas.edu). 版权归Robert van de Geijn教授(rvdg@cs.utexas.edu)所有. 美国德克萨斯大学计算机科学系

Adapted to Github Markdown Wiki by Jianyu Huang (jianyu@cs.utexas.edu). 黄建宇改编自Github Markdown Wiki(jianyu@cs.utexas.edu).

# Table of contents. 目录

  * [The GotoBLAS/BLIS Approach to Optimizing Matrix-Matrix Multiplication - Step-by-Step](../../wiki#the-gotoblasblis-approach-to-optimizing-matrix-matrix-multiplication---step-by-step)GotoBLAS/BLIS逐步优化矩阵矩阵乘法的方法
This page leads one, step by step, through the optimization of matrix-matrix multiplication. For now, it is assumed that the reader has a Linux account on an Intel processor based computer. We will use the gcc compiler as part of the exercise.
﻿本页将逐步介绍矩阵矩阵乘法的优化。目前，假设读者在基于Intel处理器的计算机上拥有Linux帐户。我们将使用gcc编译器作为练习的一部分。
Please send comments to rvdg@cs.utexas.edu.
请将意见发送至rvdg@cs.utexas.edu.
  * [NOTICE ON ACADEMIC HONESTY](../../wiki#notice-on-academic-honesty)关于学术诚信的通知
If you use these materials for a class project, you MUST disclose where you found this information. You will risk being accused of academic dishonesty otherwise...
如果你将这些材料用于课堂项目，你必须披露你在哪里找到这些信息。否则，你将面临被指控学术不诚实的风险。
  * [References](../../wiki#references)参考文献
This work is based on two publications. You will want to read these when you are done with this exercise. If you use information on this page in other research, please reference these papers.
这项工作基于两份出版物。完成此练习后，您将需要阅读这些内容。如果您在其他研究中使用此页面上的信息，请参考这些论文。
Anatomy of high-performance matrix multiplication. Kazushige Goto, Robert A. van de Geijn. ACM Transactions on Mathematical Software (TOMS), 2008.
解析高性能矩阵乘法。后藤和志，罗伯特·范德盖因。ACM数学软件汇刊（TOMS），2008年。
(Available without charge at the following site: http://www.cs.utexas.edu/users/flame/FLAMEPublications.html)
（以下网站免费提供：http://www.cs.utexas.edu/users/flame/FLAMEPublications.html)
BLIS: A Framework for Rapidly Instantiating BLAS Functionality. Field G. Van Zee, Robert A. van de Geijn. ACM Transactions on Mathematical Software (TOMS), 2015.
BLIS：一个快速实例化BLAS功能的框架。菲尔德·范·泽，罗伯特·范德盖因。ACM数学软件汇刊（TOMS），2015年。
(Available without charge at the following site: http://www.cs.utexas.edu/users/flame/FLAMEPublications.html)
（以下网站免费提供：http://www.cs.utexas.edu/users/flame/FLAMEPublications.html)
For more advanced exercises with recent architectures (Intel Sandy/Ivy Bridges, Haswell, etc.), you may want to try BLISlab.
对于使用最新架构（Intel Sandy/Ivy Bridges、Haswell等）的更高级练习，您可能想尝试BLISlab。
BLISlab: A Sandbox for Optimizing GEMM (Available at: https://github.com/flame/blislab)
BLISlab：优化GEMM的沙盒（可在以下网址获得：https://github.com/flame/blislab)
  * [Set Up](../../wiki#set-up)建立
This wiki page assumes that you have access to an Intel-based processor, the gnu-cc compiler, and octave (an Open Source version of MATLAB that is part of a typical Linux or Unix install).
此wiki页面假设您可以访问基于Intel的处理器、gnu-cc编译器和octave（典型Linux或Unix安装中的MATLAB开源版本）。
To be able to follow along with the below examples, you will want to download some routines, as described on the Set Up page.
为了能够按照以下示例进行操作，您需要下载一些例程，如设置页面上所述。
Make sure that the makefile starts with the following lines:
确保生成文件以以下行开头：
<pre>
```
OLD  := MMult0
NEW  := MMult0
```
</pre>
This indicates that the performance of the version of matrix-matrix multiplication in MMult0.c is measured (by virtue of the statement OLD  :=0).
这表明衡量了MMult0.c中矩阵矩阵乘法版本的性能（通过语句OLD:=0）。
Next, to make sure that when plotting the graphs are properly scaled, set certain parameters in the file proc_parameters.m. See the comments in that file. (Setting these parameters will ensure that when plotting the y-axis ranges from 0 to the peak performance of the architecture.)
接下来，为了确保在绘制图形时正确缩放，请在proc_parameters.m文件中设置某些参数。请参阅该文件中的注释。（设置这些参数将确保在绘制y轴时，从0到架构的峰值性能。）
Picking the right clock speed is a bit tricky, given that modern architectures have something called 'turbo boost' which changes the clock speed. For example, the Intel i5 core in my laptop has a clock speed of 1.7 GHz, but a turbo boost rate of 2.6 GHz. I chose to indicate in proc_parameters.m that the processor has a clock speed of 2.6 GHz, since otherwise some of the results would show that the implementation attains greater than the peak speed of the processor...
选择正确的时钟速度有点棘手，因为现代架构有一种叫做“涡轮增压”的东西可以改变时钟速度。例如，我笔记本电脑中的英特尔i5内核的时钟速度为1.7 GHz，但涡轮增压率为2.6 GHz。我选择在proc_parameters.m中指出处理器的时钟速度为2.6 GHz，否则一些结果会显示实现速度大于处理器的峰值速度。
Execute. 执行
make run This will compile, link, and execute the test driver, linking to the implementation in MMult0.c. The performance data is saved in file output0.m.
make run这将编译、链接和执行测试驱动程序，链接到MMult0.c中的实现。性能数据保存在output0.m文件中。
more output0.m This will display the contents of the output file output_MMult0.m. It should look something like
more output0.m这将显示输出文件output_MMult0.m的内容。它应该看起来像
```
version = 'MMult0';
MY_MMult = [
40 1.163636e+00 0.000000e+00 
80 8.827586e-01 0.000000e+00 
120 1.289071e+00 0.000000e+00 
160 1.200469e+00 0.000000e+00 
200 1.195100e+00 0.000000e+00 
240 1.211038e+00 0.000000e+00 
 [ lines deleted ]
720 2.096185e-01 0.000000e+00 
760 2.116985e-01 0.000000e+00 
800 2.115609e-01 0.000000e+00 
];
```
The first column equals the problem size. The second column the performance (in Gflops) when a matrix-matrix multiply with the indicated problem size m=n=k is executed. The last column reports the maximum absolute difference encountered between the implementation in REF_MMult.c and MMult0.c. It should be close to 0.00000e+00 although as different optimizations are added the difference may not be perfectly zero.
第一列等于问题大小。第二列是执行矩阵矩阵乘以指示的问题大小m=n=k时的性能（单位为Gflops）。最后一列报告了REF_MMult.c和MMult0.c中的实现之间遇到的最大绝对差异。它应该接近0.00000e+00，尽管由于添加了不同的优化，差异可能不是完全为零。
octave This will start up octave. Then, in octave,
octave 命令将启动Octave。然后，在Octave环境中，
```
octave:1> PlotAll        % this will create the plot
# octave：1>PlotAll     %这将创建绘图
```
I usually start up a separate xterm session, in which I keep octave running, so that every time I want to make a new graph, I can just execute 'PlotAll' in that session.
我通常会启动一个单独的xterm会话，在这个会话中，我会保持octave运行，这样每次我想制作一个新的图形时，我都可以在该会话中执行“PlotAll”。
The performance graph (on my 1.7GHz Intel Core i5 MacBook Air) looks something like
性能图（在我的1.7GHz Intel Core i5 MacBook Air上）看起来像（图1）
Notice that the two curves are right on top of each other because data for the same implementation are being compared. From the fact that the top of the graph represents peak performance, it is obvious that this simple implementation achieves only a fraction of the ideal performance.
请注意，这两条曲线是相互重叠的，因为正在比较相同实现的数据。从图的顶部表示峰值性能的事实来看，很明显，这种简单的实现只实现了理想性能的一小部分。
A question, of course is, is this the best we can do? We are going to walk through a sequence of optimizations, culminating in performance marked by "NEW" in the following graph:
当然，一个问题是，这是我们能做的最好的吗？我们将进行一系列优化，最终在下图中以“NEW”标记的性能：（图2）

  * [Step-by-step optimizations](../../wiki#step-by-step-optimizations)逐步优化
We will now lead the visitor through a series of optimizations. In some cases, a new implementation (optimization) merely is a small step in the right direction. We change the code a little at a time in order to be able to make sure it remains correct.
我们现在将引导访问者进行一系列优化。在某些情况下，新的实现（优化）只是朝着正确方向迈出的一小步。我们一次稍微更改代码，以确保它保持正确。

  * [Computing four elements of C at a time](../../wiki#computing-four-elements-of-c-at-a-time)一次计算C矩阵的四个元素
    * [Hiding computation in a subroutine](../../wiki#hiding-computation-in-a-subroutine)在子程序中隐藏计算
We first rewrite the basic implementation to hide the inner loop in a subroutine, AddDot:
我们首先重写基本实现，将内部循环隐藏在子例程AddDot中：
Optimization 1  优化1
Optimization 2  优化2
This does not yield better performance:
这不会产生更好的性能：
It does set us up for the next step.
这确实为我们下一步做好了准备。

    * [Computing four elements at a time](../../wiki#computing-four-elements-at-a-time)一次计算四个元素
We compute C four elements at a time in a subroutine, AddDot1x4, which performs four inner products at a time:
我们在子程序 AddDot1x4 中一次计算 C 的四个元素，该子程序一次执行四个内积：
Optimization (1x4) 3
Still, no performance benefit.
但仍未获得任何性能优势。
    
Now we inline the four separate inner products and fuse the loops into one, thereby computing the four inner products simultaneously in one loop:
现在我们内联四个独立的内积并将循环融合为一个，从而在一个循环中同时计算四个内积：
Optimization (1x4) 4
Still, no performance benefit.
但仍未获得任何性能优势。
Optimization (1x4) 5
Now we start seeing a performance benefit. The reason is that the four loops have been fused and therefore the four inner products are now being performed simultaneously. This has the following benefits:
The index p needs only be updated once every eight floating point operations.
Element A( 0, p ) needs only be brought in from memory once instead of four times. (This only becomes a benefit when the matrices no longer fit in the L2 cache.)
现在我们开始看到性能提升。这是因为四个循环被融合了，因此四个内积现在可以同时执行。这有以下好处：
索引 p 只需每 8 次浮点运算更新一次。
元素 A(0, p) 只需从内存中读取一次，而不是四次。（这只有在矩阵无法再装入 L2 缓存时才会发挥作用。）
At this point, we are starting to see some performance improvements.
此时，我们开始看到一些性能改进。


We compute C four elements at a time in a subroutine, AddDot4x4, which performs sixteen inner products at a time:
我们在子例程AddDot4x4中一次计算C的四个元素，该子例程一次执行十六个内积：
No performance benefit, yet.
没有性能优势。

    * [Further optimizing](../../wiki#further-optimizing)进一步优化
We accumulate the elements of C in registers and use a register for elements of A.
我们将 C 的元素累积到寄存器中，并使用寄存器存储 A 的元素。
Optimization (1x4) 6
Now we start seeing a performance benefit. We accumulate the updates to the current 1x4 row of C in registers and we place the element A( p, 0 ) in a register, to reduce traffic between cache and registers.
现在我们开始看到性能提升。我们将 C 当前 1x4 行的更新累积到寄存器中，并将元素 A(p, 0) 放入寄存器中，以减少缓存和寄存器之间的通信量。
We use pointers to address elements in B. 
我们使用指针来寻址 B 中的元素。
Optimization (1x4) 7
We now use four pointers, bp0_pntr, bp1_pntr, bp2_pntr, and bp3_pntr, to access the elements B( p, 0 ), B( p, 1 ), B( p, 2 ), B( p, 3 ). This reduces indexing overhead.
我们现在使用四个指针 bp0_pntr、bp1_pntr、bp2_pntr 和 bp3_pntr 来访问元素 B(p, 0)、B(p, 1)、B(p, 2)、B(p, 3)。这减少了索引开销。


We unroll the loop by four (a relatively arbitrary choice of unrolling factor)
我们将循环展开四倍（展开因子的选择相对随意）
Optimization (1x4) 8
We now unrolled the loop by four. Interestingly enough, this decreases performance slightly. What this probably means is that by adding the optimization, we confused the compiler, which therefore could not do an optimization that it did before.
现在我们将循环展开了四层。有趣的是，这稍微降低了性能。这可能意味着，通过添加优化，我们混淆了编译器，因此它无法执行之前所做的优化。

We use indirect addressing to reduce the number of times the pointers need to be updated
我们使用间接寻址来减少指针需要更新的次数
Optimization_1x4_9
Interestingly, it appears that the compiler did this optimization automatically, and hence we see no performance improvement...
有趣的是，编译器似乎自动完成了这项优化，因此我们没有看到性能提升

There is considerable improvement for problem sizes that fit (at least partially) in the L2 cache. Still, there is a lot of room for improvement.
对于适合（至少部分）L2 缓存的问题规模，性能已有显著提升。尽管如此，仍有很大的改进空间。

  * [Computing a 4 x 4 block of C at a time](../../wiki#computing-a-4-x-4-block-of-c-at-a-time)一次计算一个4 x 4的C矩阵块
We now compute a 4 x 4 block of C at a time in order to use vector instructions and vector registers effectively. The idea is as follows: There are special instructions as part of the SSE3 instruction set that allow one to perform two 'multiply accumulate' operations (two multiplies and two adds) per clock cycle for a total of four floating point operations per clock cycle. To use these, one has to place data in 'vector registers'. There are sixteen of these, each of which can hold two double precision numbers. So, we can keep 32 double precision numbers in registers. We will use sixteen of these to hold elements of C, a 4 x 4 block.
为了有效地使用矢量指令和矢量寄存器，我们现在一次计算一个 4 x 4 的 C 语言块。具体思路如下：SSE3 指令集中有一些特殊指令，允许每个时钟周期执行两次“乘法累加”运算（两次乘法和两次加法），总共可进行四次浮点运算。要使用这些指令，必须将数据放入“矢量寄存器”中。“矢量寄存器”共有 16 个，每个寄存器可容纳两个双精度数。因此，我们可以在寄存器中保存 32 个双精度数。我们将使用其中 16 个寄存器来保存 C 语言（一个 4 x 4 的块）的元素。

    * [Repeating the same optimizations](../../wiki#repeating-the-same-optimizations)重复相同的优化
We compute C four elements at a time in a subroutine, AddDot4x4, which performs sixteen inner products at a time:
我们在子程序 AddDot4x4 中一次计算 C 的四个元素，该子程序一次执行十六个内积：
Optimization (4x4) 3
No performance benefit, yet. 目前尚无性能优势。

Now we inline the sixteen separate inner products and fuse the loops into one, thereby computing the sixteen inner products simultaneously in one loop:
现在我们内联十六个独立的内积并将循环融合为一个，从而在一个循环中同时计算十六个内积：
Optimization (4x4) 4
No performance benefit, yet. 目前尚无性能优势。
Optimization (4x4) 5
Now we see a performance benefit, when the matrices becomes large, since the data is reused more after being brought into the registers. 
现在，我们看到了矩阵变大时的性能优势，因为数据在被载入寄存器后得到了更多的重用。

    * [Further optimizing](../../wiki#further-optimizing-1)进一步优化
We now start optimizing differently as we did for the 1x4 case.
我们现在开始以与 1x4 情况不同的方式进行优化。
We store a row of k x 4 matrix B in registers
我们将一行 k x 4 矩阵 B 存储在寄存器中
Optimization (4x4) 8
We now use registers to store the elements of the current row of B. (Notice that we did not do this for the case where we computed C four elements at a time.) The performance actually drops slightly. But this step enables further optimizations.
我们现在使用寄存器来存储 B 当前行的元素。（请注意，在一次计算 C 的四个元素时，我们没有这样做。）性能实际上会略有下降。但这一步可以实现进一步的优化。

Notice that we now use MANY more regular registers than physically available...
请注意，我们现在使用的常规寄存器比物理上可用的寄存器多得多
We rearrange the computation so that two rows of 4x4 block of C are computed at a time.
我们重新安排计算，以便一次计算 C 的两行 4x4 块。
Optimization (4x4) 9
From 4x4_8 to 4x4_9 is a subtle change: instead of updating the rows of 4x4 block C a row at a time, we compute them two rows at a time. This sets us up for using vector operations, where we update pairs C( 0,j ) and C( 1,j ) (j =0,...,3) with vector operations.
从 4x4_8 到 4x4_9 有一个微妙的变化：我们不再一次一行地更新 4x4 块 C 中的行，而是一次计算两行。这为我们使用向量运算做好了准备，我们用向量运算来更新 C(0,j) 和 C(1,j) (j =0,...,3) 对。

We use vector registers and vector operations.
我们使用向量寄存器和向量操作。
Optimization (4x4) 10
Finally, we use the vector registers and vector operations.
最后，我们使用向量寄存器和向量运算。

We notice a considerable performance boost:
Still, there is a lot of room for improvement.
我们注意到性能有了显著提升：
不过，仍有很大的改进空间。

    * [Blocking to maintain performance](../../wiki#blocking-to-maintain-performance)分块以维持性能
In order to maintain the performance attained for smaller problem sizes, we block matrix C (and A and B correspondingly):
为了保持较小问题规模所达到的性能，我们分块矩阵 C（以及相应的 A 和 B）：
Optimization (4x4) 11
What we noticed is that for all optimizations so far, performance degraded considerably when the matrices involved were much bigger than could fit in the L2 cache. In this optimization, we create an extra level of blocking to overcome this. We now have a main routine that calls what is the inner kernel used by the GotoBLAS and BLIS, and then the AddDot4x4 routine is the micro-kernel used by BLIS.
我们注意到，到目前为止的所有优化，当涉及的矩阵远远大于二级缓存的容量时，性能都会显著下降。在本次优化中，我们创建了一个额外的分块层来解决这个问题。现在，我们有一个主例程，它调用 GotoBLAS 和 BLIS 使用的内核，然后 AddDot4x4 例程是 BLIS 使用的微内核。
Now, performance is maintained:
现在，性能得以维持：

    * [Packing into contiguous memory](../../wiki#packing-into-contiguous-memory)打包到连续内存中
First, we pack the block of A so that we march through it contiguously.
首先，我们打包 A 块，以便我们可以连续地穿过它。
Optimization (4x4) 12
We now pack to 4xk block of A before calling AddDot4x4. We see a performance drop. If one examines the inner kernel
现在，在调用 AddDot4x4 之前，我们将 A 打包到 4xk 块中。我们发现性能有所下降。如果检查内核
one notices that each 4xk block of A is packed repeatedly, once for every time the outer loop is executed.
我们会注意到，A 中的每个 4xk 块都会被重复打包，每次执行外循环时都会打包一次。
Optimization (4x4) 13
This version saves the packed blocks of A so that after the first iteration of the outer loop of InnerKernel, the saved version is used. The performance gain is noticeable! The only change from the last version is the addition of if ( j== 0 ):
此版本保存了 A 的打包块，以便在 InnerKernel 外层循环第一次迭代后使用保存的版本。性能提升非常显著！与上一版本相比，唯一的变化是增加了 if ( j== 0 ) 语句：

This yields a surprisingly large performance boost:
这会带来令人惊讶的巨大性能提升：


Finally, we pack the block of B so that we march through it contiguously.
最后，我们打包 B 块，以便我们可以连续地穿过它。
Optimization (4x4) 14
We now pack the kx4 blocks of B. Notice that in this version, the panels are packed repeatedly, adversely affecting performance.
我们现在打包 B 的 kx4 块。请注意，在此版本中，面板被重复打包，对性能产生不利影响。
Optimization (4x4) 15
And, again, we add some code that makes it so that we avoid repacking the kx4 blocks of B. Performance is now impressive!
而且，我们再次添加了一些代码，以避免重新打包 B 的 kx4 块。性能现在令人印象深刻！

We now attain 90% of the turbo boost peak of the processor!
我们现在已经达到了处理器睿频加速峰值的 90%！

  * [Acknowledgement](../../wiki#acknowledgement)致谢

# Related Links. 相关链接
* [BLISlab: A Sandbox for Optimizing GEMM](https://github.com/flame/blislab)BLISlab：优化GEMM的沙盒
* [GEMM: From Pure C to SSE Optimized Micro Kernels](http://apfel.mathematik.uni-ulm.de/~lehn/sghpc/gemm/)GEMM：从纯C到SSE优化微内核

# Acknowledgement. 致谢
This material was partially sponsored by grants from the National Science Foundation (Awards ACI-1148125/1340293 and ACI-1550493).
本材料部分由国家科学基金会资助（ACI-1148125/1340293和ACI-1550493奖）。
_Any opinions, findings and conclusions or recommendations expressed in this material are those of the author(s) and do not necessarily reflect the views of the National Science Foundation (NSF)._
本材料中表达的任何意见、发现和结论或建议都是作者的意见、发现、结论或建议，不一定反映美国国家科学基金会（NSF）的观点。