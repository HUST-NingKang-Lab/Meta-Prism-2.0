# Meta-Prism 2.0
## Introduction
Microbial community samples and sequencing data have been accumulated at a speed faster than ever, with tens of thousands of samples been sequenced each year. Mining such a huge amount of multi-source heterogeneous data is becoming more and more difficult. Among several sample mining bottlenecks, efficient and accurate search of samples is one of the most prominent: Faced with millions of samples in the data repository, traditional sample comparison and search approaches fall short in speed and accuracy.

Here we proposed Meta-Prism 2.0, a microbial community sample search method based on smart pair-wise sample comparison, which pushed the time and memory efficiency to a new limit, without the compromise of accuracy. Based on memory-saving data structure, time-saving instruction pipeline, and boost scheme optimization, Meta-Prism 2.0 has enabled ultra-fast, accurate and memory-efficient search among millions of samples. Meta-prism 2.0 is a faster microbiome sample comparison and search software. It default use [Silva phylogeny tree](https://www.arb-silva.de) and support [EBI MGNify data](https://www.ebi.ac.uk/metagenomics/). It also supports custom sequencing evolutionary trees. It can calculate the similarity matrix of microbiome samples and search the microbiome database. Because of the improvement of calculation performance compared with [Meta-Prism 1.0](https://github.com/HUST-NingKang-Lab/metaPrism), 2.0 doesn’t need index system but instead execute exhaustive search.

In summary, Meta-Prism 2.0 can perform accurate searches among millions of samples with very low memory cost and fast speed, enabling knowledge discovery from samples at a massive scale. It has changed the traditional resource-intensive sample comparison and search scheme to a cheap and effective procedure, which could be conducted by researchers everyday even on a laptop, for insightful sample search and knowledge discovery.

## Requirement
We successfully compiled on CentOS 7.6 by gcc 4.8.5 and macOS 10.15 by clang 11.0.0
## Install and Build
### Use git clone to download the source code:
`git clone https://github.com/HUST-NingKang-Lab/Meta-Prism-2.0.git`
### Build the software:
`make`

### Uninstall the software:
`make clean`
## Usage
### Prepare phylogeny tree:
Meta prism 2.0 needs the evolutionary tree of Newick format as the basis of calculation. It is recommended to use Silva evolutionary tree, or set the evolutionary tree according to the sequencing results or use requirements. Use  `  -t [path]` or `--tree [path] `   to select the evolutionary tree path.
### Load data:
Meta prism 2.0 supports SSU abundance data files in EBI MGNify and other databases as input, and provides special packaged data format with two types: ascii and binary to store abundance more efficiently. Note that packaged data is bound to the specific evolution tree. 

`--load(-l) [list|OTU|ascii|binary] [path]`
### Package data:
Meta prism 2.0 can package texa data with by ascii or binary for space efficient storage and fast loading.
Ascii format packaged data is easy to read and edit, while binary format data is more efficient at space and time.

`--package(-p) [ascii|binary] [path]`

Note that the packaged data is bound to the evolution tree and cannot be used with another evolution tree.
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
--search(-s) [single|list|OTU|ascii|binary] [search sample path] [select number|f] ## Load and search samples then return top N (default=5, input char 'f' will return full result as matrix) similar samples
--output(-o) [path] ## Result output path
--threads(-T) [Number] ## Threads for calculathon
```

For example:

`./bin/Meta-Prism_2.0 --tree [TreePath] -l ascii [PdataPath] -s single [FilePath] 3 -o [ResultPath]`

Multiple samples can be read from the packaged file with ascii type as the database, searched with one sample in FilePath, returned the three best matched samples, and output to ResultPath. 
## Supplement
We provide Silva tree data, SSU_ TSV data, ascii packaged data for example.

[Silva phylogenetic tree](https://www.arb-silva.de/fileadmin/silva_databases/living_tree/LTP_release_132/LTPs132_SSU_tree.newick) (need to delete description head first)
  or [local backup](https://github.com/HUST-NingKang-Lab/Meta-Prism-2.0/releases/download/datas/LTPs132_SSU_tree.txt)

[SSU_OTUs data](https://www.ebi.ac.uk/metagenomics/api/v1/analyses/MGYA00001775/file/ERR358543_FASTQ_otu.tsv)
or [local backup](https://github.com/HUST-NingKang-Lab/Meta-Prism-2.0/releases/download/v1.0-with-data/MGYS00000337-ERR358543.tsv)

[100 samples ascii packaged data](https://github.com/HUST-NingKang-Lab/Meta-Prism-2.0/releases/download/datas/100samples.ascii_packaged.pdata)

[1000 samples ascii packaged data](https://github.com/HUST-NingKang-Lab/Meta-Prism-2.0/releases/download/datas/1000samples.ascii_packaged.pdata)

[file list for example](https://github.com/HUST-NingKang-Lab/Meta-Prism-2.0/releases/download/datas/file_list_for_example.txt)

- Datasets used  in our study:

| Dataset                              | Description                                                  | Source                                        | Link                                                         | Format            |
| ------------------------------------ | ------------------------------------------------------------ | --------------------------------------------- | ------------------------------------------------------------ | ----------------- |
| Combined dataset                     | 126,727 samples curated from MGnify database                 | [MGnify](https://www.ebi.ac.uk/metagenomics/) | [download](https://github.com/HUST-NingKang-Lab/Meta-Prism-2.0/releases/download/datas/Full_Combined_data.binary_packaged.pdata.zip) | packaged (binary) |
| FEAST dataset                        | 10,270 samples curated from MGnify database, also used in FEAST study | [MGnify](https://www.ebi.ac.uk/metagenomics/) | [download](https://github.com/HUST-NingKang-Lab/Meta-Prism-2.0/releases/download/datas/FEAST_dataset.ascii_packaged.pdata) | packaged (ascii)  |
| skin samples                         | 1,261 skin metagenomic samples                               | [MGnify](https://www.ebi.ac.uk/metagenomics/) | [download](https://github.com/HUST-NingKang-Lab/Meta-Prism-2.0/releases/download/datas/Skin_samples.ascii_packaged.pdata) | packaged (ascii)  |
| oral samples                         | 70 oral metagenomic samples                                  | [MGnify](https://www.ebi.ac.uk/metagenomics/) | [download](https://github.com/HUST-NingKang-Lab/Meta-Prism-2.0/releases/download/datas/Oral_samples.ascii_packaged.pdata) | packaged (ascii)  |
| groundwater samples                  | 11 Query samples curated from groundwater in Saudi Arabian   | [MGnify](https://www.ebi.ac.uk/metagenomics/) | [download](https://www.ebi.ac.uk/metagenomics/api/v1/studies/MGYS00001601/pipelines/3.0/file/ERP010611_taxonomy_abundances_v3.0.tsv) | abundance matrix  |
| dataset for detecting contaminations | 611 Query samples from indoor house surfaces +<br />6,285 Target samples | [MGnify](https://www.ebi.ac.uk/metagenomics/) | [download](https://github.com/HUST-NingKang-Lab/Meta-Prism-2.0/releases/download/datas/contamination_study_related_RunIDs.txt) | Run IDs list      |

Their related meta data are available from [MGnify](https://www.ebi.ac.uk/metagenomics/) database.

## Note

* We don't provide our synthesized dataset (with one-million samples) here. But you can easily synthesize the dataset by following these [steps]().
* Many phylogenetic tree has notation in file, including silva phylogenetic tree. You need to delete notations before running program.
* Our packaged data format is combined with phylogenetic tree. After you packaged samples from one tree, you can't calculate this data with another tree.

## Contact
康凯 Kai Kang 

kang_kai_kk@icloud.com

宁康 Kang Ning

ningkang@hust.edu.cn
