# README
#Meta-Prism/2
# Meta-Prism 2.0
## Introduction
Meta prism 2.0 is a faster microbiome sample comparison and search software. It default use [Silva phylogeny tree](https://www.arb-silva.de) and support [EBI MGNify data](https://www.ebi.ac.uk/metagenomics/). It also supports custom sequencing evolutionary trees. It can calculate the similarity matrix of microbiome samples and search the microbiome database. Because of the improvement of calculation performance compared with [Meta-Prism 1.0](https://github.com/HUST-NingKang-Lab/metaPrism), 2.0 doesn’t need index system and execute exhaustive search.
## Requirement
Our project require gcc 4.8.5 or higher. And we successfully compiled on CentOS 7.6 and macOS 10.15
## Install and Build
### Use git clone to download the source code:
`git clone https://github.com/HUST-NingKang-Lab/Meta-Prism-2.0.git`
### Build the software:
`make`
### Uninstall the software:
`make clean`
## Usage
### Prepare phylogeny tree:
Meta prism 2.0 needs the evolutionary tree of Newick format as the basis of calculation. It is recommended to use Silva evolutionary tree, or set the evolutionary tree according to the sequencing results or use requirements. Use`  -t [path]` or `--tree [path] ` to select the evolutionary tree path.
### Load data:
Meta prism 2.0 supports SSU abundance data files in EBI MGNify and other databases as input, and provides special packaged data format ". pdata" to store abundance more efficiently. Note that pdata format data is bound to the specific evolution tree.
#### Load TSV data:
Prepare the TSV data path list file, and use the arg  ` --load_from_list [path]`  or `-ll [path]` reads all TSV data in the list.
#### Load packaged data:
Use arg `--load_from_package [path]` or `-lp [path] ` to load all samples in this packaged file.
### Package data:
Meta prism 2.0 can package the data in TSV file to ". pdata" file for more efficient storage. The operation is as follows:
`./bin/Meta-Prism2.0 --tree [TreePath] -ll [FileListPath] --save(-s) [PdataFileOutputPath]`
Note that the generated ". pdata" data file is bound to the evolution tree and cannot be used on another evolution tree.
### Calculate similarity matrix:
Meta prism 2.0 can load the data of microbiome samples and calculate the similarity matrix between samples. Relevant args are:
```
--load_from_list(-ll) [FileListPath] load data from files in path list
--load_from_package(-lp) [PdataPath] load data from packaged pdata file(mutually exclusive with the former)
--load_from_OTU(-lo) [OTUPath] load data from packaged pdata file(mutually exclusive with the former)
--cores(-c) [CoreNumber=1] core usage, default is 1
--output(-o) [ResultOutputPath] result output path
```

For example:
`./bin/Meta-Prism2.0 --tree [TreePath] -ll [FileListPath] -m -o [ResultPath]`
You can read all the sample file paths contained in the list file corresponding to filelistpath, calculate the similarity matrix, and finally output it to resultpath.
### Database search:
Meta prism 2.0 can search one or more microbial samples for another group of microbial samples, and return the top n sample name with the highest similarity and similarity. Relevant args are:
```
--single_search(-ss) [FilePath] [TopN] load data from a single sample file and returns TopN most similar results. TopN is optional, default is 5
--multi_search(-ms) [FileListPath] [TopN] load data from sample file lists (mutually exclusive with the former)
--package_search(-ps) [PdataPath] [TopN] load data from packaged file (mutually exclusive with the former)
--OTU_search(-os) [OTUPath] [TopN] load data from OTU file (mutually exclusive with the former)
--cores(-c) [CoreNumber=1] core usage, default is 1
--output(-o) [ResultOutputPath] result output path
```

For example:
`./bin/Meta-Prism2.0 --tree [TreePath] -lp [PdataListPath] -ss [FilePath] 3 -c 5 -o [ResultPath]`
Multiple samples can be read from the pdata file as the database, searched with the sample in FilePath, returned the three best matched samples, and output to ResultPath. It uses 5 CPU cores to work.
## Supplement
We provide Silva tree data, SSU_TSV file, pdata file  
[Silva tree](https://www.arb-silva.de/fileadmin/silva_databases/living_tree/LTP_release_132/LTPs132_SSU_tree.newick)   (need to delete header lines after download)

[SSU_tsv data](https://github.com/HUST-NingKang-Lab/Meta-Prism-2.0/releases/download/v1.0-with-data/MGYS00000337-ERR358543.tsv)  

[100 sample pdata](https://github.com/HUST-NingKang-Lab/Meta-Prism-2.0/releases/download/v1.0-with-data/100samples.pdata)  

[1000 sample pdata](https://github.com/HUST-NingKang-Lab/Meta-Prism-2.0/releases/download/v1.0-with-data/1000samples.pdata)  

## Author
Kai Kang  sf257518@icloud.com  

Kang Ning  ningkang@hust.edu.cn  

# 中文
## 介绍
Meta-Prism 2.0 是一个速度更快的微生物组样本比对、搜索软件。默认使用[Silva进化树](https://www.arb-silva.de)并支持[EBI MGNify数据](https://www.ebi.ac.uk/metagenomics/)，也支持自定义测序进化树。可以进行微生物组样本相似度矩阵的计算、针对微生物组数据库的相似度搜索。由于性能对比[Meta-Prism 1.0](https://github.com/HUST-NingKang-Lab/metaPrism)有很大提升，因此无需索引，可以直接进行全搜索。
## 环境需求
我们在CentOS 7.6 的服务器上使用gcc 4.8.5成功编译了程序，而在macOS 10.15中也完成了编译。
## 下载与使用：
### 使用 git clone 操作去下载源代码:
`git clone https://github.com/HUST-NingKang-Lab/Meta-Prism-2.0.git`
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
--multi_search(-ms) [FileListPath] [TopN] 从样本文件列表中读取数据 （与前者互斥）
--package_search(-ps) [PdataListPath] [TopN]从pdata文件中读取数据 （与前者互斥）
--cores(-c) [CoreNumber=1] 使用CPU核数，默认为1
--output(-o) [ResultOutputPath] 结果输出路径
```
例如：
`./bin/Meta-Prism2.0 --tree [TreePath] -lp [PdataListPath] -ss [FilePath] 3 -c 5 -o [ResultPath]`
可以从pdata文件中读取多个样本作为数据库，用FilePath对应的样本搜索，并返回相似度最高的3个样本，输出到ResultPath中。并且使用5个CPU核心工作。
## 补充
我们提供了Silva进化树数据、SSU_tsv数据、pdata数据  

[Silva进化树](https://www.arb-silva.de/fileadmin/silva_databases/living_tree/LTP_release_132/LTPs132_SSU_tree.newick)  (需要在下载后自行删掉文件头)

[SSU_tsv数据](https://github.com/HUST-NingKang-Lab/Meta-Prism-2.0/releases/download/v1.0-with-data/MGYS00000337-ERR358543.tsv)  

[100样本pdata数据](https://github.com/HUST-NingKang-Lab/Meta-Prism-2.0/releases/download/v1.0-with-data/100samples.pdata)  

[1000样本pdata数据](https://github.com/HUST-NingKang-Lab/Meta-Prism-2.0/releases/download/v1.0-with-data/1000samples.pdata)  

## 作者
康凯 sf257518@icloud.com  

宁康 ningkang@hust.edu.cn
