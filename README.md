# gsl-vc2015

Static lib projects of GNU GSL 2.2.1 build with VC2015. by Liigo, 2016/11.

[GNU GSL](http://www.gnu.org/software/gsl/) v2.2.1 的 VC2015 工程，可编译出在Windows系统下使用的静态链接库(.lib)。

本项目生成的静态库使用x86指令集，支持静态链接到32位可执行程序，支持XP系统。如果您需要64位版本，可考虑修改编译配置重新生成，或者考虑使用[ampl-gsl](https://github.com/ampl/gsl/)编译。

编译本项目静态库所用的重要编译参数：

- Platform toolset: Visual Studio 2015 - Windows XP (v140_xp)
- C/C++ Code Generation: Runtime Library: Multi-threaded (/MT)

其中`/MT`参数意味着静态链接C运行库，即运行时不依赖C运行库。

使用这些静态库的程序和库最好也用同样的编译参数编译。

由于技术原因，我将GSL各子模块分别编译，共生成 45 个静态库文件（[打包下载](https://github.com/liigo/gsl-vc2015/files/615772/gsl2.2.1-vc2015-20161128.zip)），列表如下：

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
- gsl-multifit_nlinear.lib
- gsl-multilarge.lib
- gsl-multilarge_nlinear.lib
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

## 版本升级

如果GNU官方GSL版本升级，需下载其源码合并到子目录src。由于我们未对gsl代码做重要修改，甚至可以直接覆盖目录。注意文件`ieee-utils/fp.c`必须保留以下两行————这是支持Windows的关键：
```
#elif _MSC_VER
#include "fp-win.c"
```

此外还应注意检查gsl新版对源代码文件的添加和删除，添加的文件要添加进git并添加进相应地vc项目(.vcproj)，删除的文件应从git中删除。如果新增了子模块，参见下文处理。

还要更新`src/gsl/`目录内的所有头文件，它们是从`src`及其各子目录内收集并复制粘贴而来的以`gsl-`为前缀以`.h`为后缀的所有文件。我目前是手工处理的，以后可考虑编程序自动化。

### 添加新項目

如新的GSL版本新增了某个子模块（或称子目录）`xxx`，需在`build/gnu-gsl.sln`解决方案内创建新的静态库项目(Project)。

将新项目命名为`gsl-xxx`，项目类型为"Win32"-"Static library"，在附加选项中取消选中"Precompiled header"和"Security Development Lifecycle (SDL) checks"。

创建完成后，设置如下项目属性（选中"All Configrations"后设置）：

- Output derictory: $(SolutionDir)\lib\
- Platform toolset: Visual Studio 2015 - Windows XP (v140_xp)
- C/C++ Code Generation: Runtime Library: Multi-threaded (/MT) （调试版为/MTd）

然后为项目添加虚拟目录（Project - Add new filter），将源文件加入其中。

最好也创建对应的测试项目。测试项目均在`build/gsl-tests/gsl-tests.sln`解决方案内。现有的测试项目仅有少数几个，还非常不全面，有很多的工作要做。

## 在Rust项目中使用

- 安装`stable-i686-pc-windows-msvc`工具链

```
rustup toolchain install stable-i686-pc-windows-msvc
rustup default stable-i686-pc-windows-msvc
```

- 复制以下代码到`lib.rs`(或其他任意.rs文件)

```
#[link(name="gsl-block")]
#[link(name="gsl-bspline")]
#[link(name="gsl-cblas")]
#[link(name="gsl-cdf")]
#[link(name="gsl-cheb")]
#[link(name="gsl-combination")]
#[link(name="gsl-complex")]
#[link(name="gsl-eigen")]
#[link(name="gsl-fft")]
#[link(name="gsl-histogram")]
#[link(name="gsl-integration")]
#[link(name="gsl-interpolation")]
#[link(name="gsl-linalg")]
#[link(name="gsl-matrix")]
#[link(name="gsl-min")]
#[link(name="gsl-monte")]
#[link(name="gsl-multifit")]
#[link(name="gsl-multifit_nlinear")]
#[link(name="gsl-multilarge")]
#[link(name="gsl-multilarge_nlinear")]
#[link(name="gsl-multimin")]
#[link(name="gsl-multiroots")]
#[link(name="gsl-multiset")]
#[link(name="gsl-ode-initval")]
#[link(name="gsl-ode-initval2")]
#[link(name="gsl-other")]
#[link(name="gsl-permutation")]
#[link(name="gsl-poly")]
#[link(name="gsl-qrng")]
#[link(name="gsl-randist")]
#[link(name="gsl-rng")]
#[link(name="gsl-roots")]
#[link(name="gsl-rstat")]
#[link(name="gsl-siman")]
#[link(name="gsl-sort")]
#[link(name="gsl-spblas")]
#[link(name="gsl-specfunc")]
#[link(name="gsl-splinalg")]
#[link(name="gsl-spmatrix")]
#[link(name="gsl-statistics")]
#[link(name="gsl-sum")]
#[link(name="gsl-sys")]
#[link(name="gsl-test")]
#[link(name="gsl-vector")]
#[link(name="gsl-wavelet")]
extern {}
```

- 在项目根目录内创建子目录`.cargo`，并在其中创建文本文件`config`

文件内容如下：

```
[build]
rustflags = ["-Clink_args=/LIBPATH:lib"]
```

配置文件`.cargo\config`指示VC连接器在`lib`目录内加载`gsl-*.lib`文件。

- 在项目根目录内创建子目录`lib`，把编译或下载的所有`gsl-*.lib`文件放进去

- 在项目根目录内执行 `cargo build` 编译项目

注：可考虑配合 https://github.com/GuillaumeGomez/rust-GSL 使用。

## License

[GNU GSL](http://www.gnu.org/software/gsl/)的开源协议是GPL，因而本项目的开源协议也是GPL。
