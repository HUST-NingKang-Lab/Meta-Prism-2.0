# README
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
Meta prism 2.0 supports SSU abundance data files in EBI MGNify and other databases as input, and provides special packaged data format with two types: ascii and binary to store abundance more efficiently. Note that packaged data is bound to the specific evolution tree. 

`--load(-l) [list|OTU|ascii|binary] [path]`
### Package data:
Meta prism 2.0 can package texa data with by ascii or binary. Ascii format packaged data is easy to read and edit, while binary format data take less space and can be load fast.

`--package(-p) [ascii|binary] [path]`

Note that the packaged data is bound to the evolution tree and cannot be used on another evolution tree.
### Calculate similarity matrix:
Meta prism 2.0 can load the data of microbiome samples and calculate the similarity matrix between samples. 
```
--matrix(-m) ## Calculate similarity matrix
--output(-o) [path] ## Result output path
--threads(-T) [Number] ## Threads for calculathon
```

For example:

`./bin/Meta-Prism2.0 --tree [TreePath] -l OTU [OTUPath] -m -o [ResultPath]`

This command will open OTU file and read all samples, calculate the similarity matrix, and output it at ResultPath.
### Database search:
> Meta prism 2.0 can search one or more microbial samples for another group of microbial samples, and return the top n sample name with the highest similarity and similarity. Relevant args are:  
```
--search(-s) [single|list|OTU|ascii|binary] [search sample path] [select number] ## Load and search samples then return top N (default=5) similar samples
--output(-o) [path] ## Result output path
--threads(-T) [Number] ## Threads for calculathon
```

For example:

`./bin/Meta-Prism_2.0 --tree [TreePath] -l ascii [PdataPath] -s single [FilePath] 3 -o [ResultPath]`

Multiple samples can be read from the packaged file with ascii type as the database, searched with one sample in FilePath, returned the three best matched samples, and output to ResultPath. 
## Supplement
We provide Silva tree data, SSU_ TSV data, ascii packaged data for example.

[Silva phylogenetic tree](https://www.arb-silva.de/fileadmin/silva_databases/living_tree/LTP_release_132/LTPs132_SSU_tree.newick) (need to delete description head first)

[SSU_OTUs data](https://github.com/HUST-NingKang-Lab/Meta-Prism-2.0/releases/download/v1.0-with-data/MGYS00000337-ERR358543.tsv)

[100 samples ascii packaged data](https://github.com/HUST-NingKang-Lab/Meta-Prism-2.0/releases/download/datas/100samples.ascii_packaged.pdata)

[1000 samples ascii packaged data](https://github.com/HUST-NingKang-Lab/Meta-Prism-2.0/releases/download/datas/1000samples.ascii_packaged.pdata)

[file list for example](https://github.com/HUST-NingKang-Lab/Meta-Prism-2.0/releases/download/datas/file_list_for_example.txt)

Following are the datasets we used for evaluate our method:

[combined dataset binary packaged](https://github.com/HUST-NingKang-Lab/Meta-Prism-2.0/releases/download/datas/Full_Combined_data.binary_packaged.pdata.zip)

[FEAST dataset ascii packaged](https://github.com/HUST-NingKang-Lab/Meta-Prism-2.0/releases/download/datas/FEAST_dataset.ascii_packaged.pdata)

## Author
康凯 Kai Kang  kang_kai_kk@icloud.com

宁康 Kang Ning  ningkang@hust.edu.cn
