# gsl-vc2015

Static lib projects of GNU GSL 2.1 build with VC2015. by Liigo, 20160616.

[GNU GSL](http://www.gnu.org/software/gsl/) v2.1 的 VC2015 工程，可编译出在Windows系统下使用的静态库(.lib)。

本项目生成的静态库使用x86指令集，支持链接到32位可执行程序。如果您需要64位版本，可考虑修改编译配置重新生成，或者考虑使用[ampl-gsl](https://github.com/ampl/gsl/)编译。

由于技术原因，我将GSL各子模块分别编译，共生成 43 个静态库文件（[打包下载](https://github.com/liigo/gsl-vc2015/files/318076/gsl2.1-vc2015-20160616.zip)），列表如下：

- gsl-block.lib
- gsl-bspline.lib
- gsl-cblas.lib
- gsl-cdf.lib
- gsl-cheb.lib
- gsl-combination.lib
- gsl-complex.lib
- gsl-eigen.lib
- gsl-fft.lib
- gsl-histogram.lib
- gsl-integration.lib
- gsl-interpolation.lib
- gsl-linalg.lib
- gsl-matrix.lib
- gsl-min.lib
- gsl-monte.lib
- gsl-multifit.lib
- gsl-multilarge.lib
- gsl-multimin.lib
- gsl-multiroots.lib
- gsl-multiset.lib
- gsl-ode-initval.lib
- gsl-ode-initval2.lib
- gsl-other.lib
- gsl-permutation.lib
- gsl-poly.lib
- gsl-qrng.lib
- gsl-randist.lib
- gsl-rng.lib
- gsl-roots.lib
- gsl-rstat.lib
- gsl-siman.lib
- gsl-sort.lib
- gsl-spblas.lib
- gsl-specfunc.lib
- gsl-splinalg.lib
- gsl-spmatrix.lib
- gsl-statistics.lib
- gsl-sum.lib
- gsl-sys.lib
- gsl-test.lib
- gsl-vector.lib
- gsl-wavelet.lib

以上静态库文件的命名，绝大多数情况下都跟GSL各子模块有一一对应关系，如`gsl-cblas.lib`是`cblas`模块编译而成。

仅有两个特殊情况：

- `gsl-sys.lib`包含了`sys` `err` `ieee-utils`三个子模块（`gsl-test.lib`同时依赖这三个，合一起算了）。
- `gsl-other.lib`包含了没有单独成库的6个子模块（`blas` `deriv` `dht` `diff` `fit` `ntuple`），它们都仅有一个源文件，似乎不值得独立成库。

拆分出这么多lib文件，使用起来可能很不方便，例如`multifit`模块的测试代码就依赖其中十几个lib，要一一找出并不简单（当然也不是特别难）。我维护起来其实也很不方便，如果要修改某项编译参数，几十个VC工程文件都要一一修改。然而这就是现实，我目前找不到更好的方案。

前面说了，这么做有技术方面的原因。GSL源码里面有很多重名的.c文件，例如`block` `matrix` `vector`等多个子目录内都有`file.c`，它们编译后都生成`file.obj`。VC2015生成lib文件时好像随便挑了一个`file.obj`，把其他的`file.obj`都被丢弃，根本不可能生成完整的lib——VC2015对这类情况有编译警告。另一种可选方案是，重命名文件，让它们不再重名，这个工作量也挺大，而且动了许多GSL本身的源码文件，不利于以后升级合并代码，我放弃了这一方案。

## Compile & Test

首先将源码根目录设置为VC的inluce目录。

打开`build/gnu-gsl.sln`，确认选中Release和x86选项，Build Solution，即可编译出42个库文件到`build/lib`目录。

打开`build/gsl-tests/gsl-tests.sln`，确认选中Release和x86选项，Build项目`gsl-testlib`，即可编译出库文件`gsl-test.lib`到`build/lib`目录。

编译`build/gsl-tests/gsl-tests.sln`内的其他项目，执行即可看到测试结果。想看测试细节的话，运行前先设置环境变量`set GSL_TEST_VERBOSE=1`。

## License

[GNU GSL](http://www.gnu.org/software/gsl/)的开源协议是GPL，因而本项目的开源协议也是GPL。