# README
#Meta-Prism/2
# Meta-Prism 2.0
## Introduction
Pass
## Requirement
Our project require gcc 4.8.5 or higher. And we successfully compiled on CentOS 7.6 and macOS 10.15
## Install and Build
### Use git clone to download the source code:
Pass
### Build the software:
`make`
### Uninstall the software:
`make clean`
## Usage
Meta-Prism 2.0

# 中文
## 介绍
Meta-Prism 2.0 是一个速度更快的微生物组样本比对、搜索软件。默认使用[Silva进化树](https://www.arb-silva.de)并支持[EBI MGNify数据](https://www.ebi.ac.uk/metagenomics/)，也支持自定义测序进化树。可以进行微生物组样本相似度矩阵的计算、针对微生物组数据库的相似度搜索。由于性能对比[Meta-Prism 1.0](https://github.com/HUST-NingKang-Lab/metaPrism)有很大提升，因此无需索引，可以直接进行全搜索。
## 环境需求
我们在CentOS 7.6 的服务器上使用gcc 4.8.5成功编译了程序，而在macOS 10.15中也完成了编译。
## 下载与使用：
### 使用 git clone 操作去下载源代码:
Pass
### 编译程序:
`make`
### 删除程序:
`make clean`
## 使用
###  准备进化树：
Meta-Prism 2.0 需要newick格式的进化树作为计算的基础，建议使用Silva进化树，也可以根据测序结果或使用需求自行设置进化树。使用`-t [path]`或`--tree [path]`选择进化树路径。
### 装载数据：
Meta-Prism 2.0 支持EBI MGNify以及其它数据库中的SSU丰度数据文件作为输入，并且提供特殊的数据格式“.pdata”更高效地存储丰度。注意，pdata格式数据与进化树绑定。
#### 装载tsv数据：
准备tsv数据路径列表文件，使用参数`--load_from_list [path]`或`-ll [path]`读取列表内所有的tsv数据。
#### 装载package数据：
使用参数`--load_from_package [path]`或`-lp [path]`读取pdata文件中包含的所有样本数据。
### 转化数据：
Meta-Prism 2.0 可以将tsv文件中的数据包装到“.pdata”文件中实现更高效地存储，操作为：
`./bin/Meta-Prism2.0 --tree [TreePath] -ll [FileListPath] --save(-s) [PdataFileOutputPath]`
注意，生成的“.pdata”数据文件与进化树绑定，不可在另一个进化树上使用。
### 计算相似度矩阵：
Meta-Prism 2.0 可以装载微生物组样本数据并计算出样本间相似度矩阵。相关参数为：
```
--load_from_list(-ll) [FileListPath] 从样本文件列表中读取数据
--load_from_package(-lp) [PdataListPath] 从.pdata文件中读取数据（与前者互斥）
--cores(-c) [CoreNumber=1] 使用CPU核数，默认为1
--output(-o) [ResultOutputPath] 结果输出路径
```
例如：
`./bin/Meta-Prism2.0 --tree [TreePath] -ll [FileListPath] -m -o [ResultPath]`
可以读取FileListPath对应的列表文件中包含的所有样本文件路径，并计算相似矩阵，最终输出到ResultPath中。
### 数据库搜索：
Meta-Prism 2.0 可以实现单个/多个微生物样本对另一组微生物样本的搜索，并返回相似度最高的top N样本名称以及相似度。相关参数为：
```
--single_search(-ss) [FilePath] [TopN] 从单个样本文件中读取数据，并返回TopN个最相似的结果，TopN可以不加，默认为5
--multi_search(-ms) [FileListPath] [TopN] 从样本文件列表中读取数据
--package_search(-ps) [PdataListPath] [TopN]从pdata文件中读取数据（三者互斥）
--cores(-c) [CoreNumber=1] 使用CPU核数，默认为1
--output(-o) [ResultOutputPath] 结果输出路径
```
例如：
`./bin/Meta-Prism2.0 --tree [TreePath] -lp [PdataListPath] -ss [FilePath] 3 -c 5 -o [ResultPath]`
可以从pdata文件中读取多个样本作为数据库，用FilePath对应的样本搜索，并返回相似度最高的3个样本，输出到ResultPath中。并且使用5个CPU核心工作。
## 补充
我们提供了Silva进化树数据、SSU_tsv数据、pdata数据、fileList数据
[Silva进化树](https://www.arb-silva.de/fileadmin/silva_databases/living_tree/LTP_release_132/LTPs132_SSU_tree.newick)
[SSU_tsv数据](https://github.com/HUST-NingKang-Lab/Meta-Prism-2.0/releases/download/v1.0-with-data/MGYS00000337-ERR358543.tsv)
[100样本pdata数据](https://github.com/HUST-NingKang-Lab/Meta-Prism-2.0/releases/download/v1.0-with-data/100samples.pdata)
[1000样本pdata数据](https://github.com/HUST-NingKang-Lab/Meta-Prism-2.0/releases/download/v1.0-with-data/1000samples.pdata)
## 作者
康凯 sf257518@gmail.com
宁康 ningkang@hust.edu.cn