# Meta-Prism 2.0
## Introduction
Microbial community samples have been accumulated at a speed faster than ever, with hundreds of thousands of samples been sequenced each year. Mining such a huge amount of multi-source heterogeneous data is becoming an increasingly difficult challenge. Among several data mining bottlenecks, efficient and accurate search of samples is one of the most prominent: Faced with millions of samples in the data repository, traditional sample comparison and search approaches fall short in speed and accuracy.

Here we proposed Meta-Prism 2.0, a microbial community sample search method based on smart pair-wise sample comparison, which pushed the time and memory efficiency to a new limit without compromising accuracy. Based on index-free and memory-saving data structure, time-saving instruction pipeline, and boost scheme optimization, Meta-Prism 2.0 has enabled ultra-fast, memory-efficient, flexible and accurate search among millions of samples. Meta-Prism 2.0 has been put to test on several datasets, with the largest containing one million samples. Results have shown that Meta-Prism 2.0’s 0.00001s per sample pair search speed and 8GB memory needs for searching against one million samples have made it the most efficient sample search method. Additionally, Meta-Prism 2.0 could achieve the accuracy comparable with or better than other contemporary methods. Thirdly, Meta-Prism 2.0 can precisely identify the original biome for samples, thus enabling sample source tracking. It default use [Silva phylogeny tree](https://www.arb-silva.de) and support [EBI MGnify data](https://www.ebi.ac.uk/metagenomics/). It also supports custom sequencing evolutionary trees. It can calculate the similarity matrix of microbiome samples and search the microbiome database. Because of the improvement of calculation performance compared with [Meta-Prism 1.0](https://github.com/HUST-NingKang-Lab/metaPrism), 2.0 doesn’t need index system but instead execute exhaustive search.

![The_Meta-Prism_2.0_pipeline](https://www.biorxiv.org/content/biorxiv/early/2020/11/20/2020.11.17.387811/F1.large.jpg?width=800&height=600&carousel=1)

Meta-Prism 2.0 has been put to test on several datasets, with largest containing one million samples. Results have shown that firstly, as a distance-based method, Meta-Prism 2.0 is not only faster than other distance-based methods, but also faster than unsupervised methods. Its 0.00001s per sample pair search speed, as well as 8GB memory needs for searching against one million samples, have enabled it to be the most efficient method for sample comparison. Additionally, Meta-Prism 2.0 could achieve the comparison accuracy and search precision that are comparable or better than other contemporary methods. Thirdly, Meta-Prism 2.0 can precisely identify the original biome for samples, thus enabling sample source tracking.

![Time_and_memory_usage](https://www.biorxiv.org/content/biorxiv/early/2020/11/20/2020.11.17.387811/F4.large.jpg?width=800&height=600&carousel=1)

In summary, Meta-Prism 2.0 has changed the resource-intensive sample search scheme to an effective procedure, which could be conducted by researchers everyday even on a laptop, for insightful sample search and knowledge discovery. Our work is preprinted at [biorxiv](https://www.biorxiv.org/content/10.1101/2020.11.17.387811v1).

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

### Merge data:
Meta prism 2.0 can load mutiple database and merge these together by command:

`--merge + [[single|list|OTU|ascii|binary] + [sample path],...]`

For example:

`./bin/Meta-Prism2.0 --tree [TreePath] -l OTU [OTUPath] -merge ascii [Path1] binnary [Path2] -p [PackagePath]`

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


## Note

* Many phylogenetic tree has notation in file, including silva phylogenetic tree. You may want to delete the notations to use the program.
* Our packaged data format is combined with the phylogenetic tree. After you packaged samples from one tree, you cannot perform calculation using the data with another phylogenetic tree.

### Formats
1. Format of phylogeny tree

Newick format: `(left_node: Phylogeny_distance, right_node: Phylogeny_distance)`

Both left or right node are recursable. For example

```
((NodeA:0.2,(NodeB:0.1,NodeC:0.2):0.2):0.3,(((NodeD:0.15,NodeE:0.23):0.66,NodeF:0.4)NodeH:0.7,NodeG:0.5):0.2)
```

2. Format of tsv data

Each line contains abundance and taxonomy. For example

<table><thead><tr><th colspan="3"># Constructed from biom file</th></tr></thead><tbody><tr><td># OTU ID</td><td>ERR1754760</td><td>taxonomy</td></tr><tr><td>207119</td><td>19.0</td><td>sk__Archaea</td></tr><tr><td>118090</td><td>45.0</td><td>sk__Archaea;k__;p__Thaumarchaeota;c__;o__Nitrosopumilales;f__Nitro...</td></tr><tr><td>153156</td><td>38.0</td><td>sk__Archaea;k__;p__Thaumarchaeota;c__;o__Nitrosopumilales;f__Nitro...</td></tr><tr><td>131704</td><td>1.0</td><td>sk__Archaea;k__;p__Thaumarchaeota;c__Nitrososphaeria;o__Nitrososp...</td></tr><tr><td>103181</td><td>5174.0</td><td>sk__Bacteria</td></tr><tr><td>157361</td><td>9.0</td><td>sk__Bacteria;k__;p__;c__;o__;f__;g__;s__agricultural_soil_bacterium_SC-I-11</td></tr></tbody></table>

3. Format of abundance data

Matrix of relative abundance for different samples and taxonomy. For example
```
#Taxonomy     Sample1     Sample2     Sample3
sk__Bacteria;k__;p__Actinobacteria;c__Actinobacteria        0.3     0.1     0.5
sk__Bacteria;k__;p__Actinobacteria;c__Actinobacteria;o__Actinomycetales;f__Actinomycetaceae;g__Actinotignum     0.4     0.9     0
sk__Bacteria;k__;p__Proteobacteria;c__Gammaproteobacteria;o__Xanthomonadales;f__Xanthomonadaceae        0.3     0       0.5
```

4. Format of ascii packaged data

Samples are split by '{' or '}', in each sample contains relative abundance and taxon_id (identical to taxon name) at phylogeny tree. For example
```
#Generated By Prism2.0,Data number: 2
{
MGYS00002173-SRR2086747.tsv
98:557 4.06554e-05,816 0.0060983,2968 4.06554e-05,3617 0.00422816,4050 4.06554e-05,4158 0.000243932,4229 0.00959467,4494 0.0446802,4610 0.00195146,4616 0.000162621,4621 4.06554e-05,4662 4.06554e-05,4686 0.000121966,4756 0.000284588,5231 4.06554e-05,5478 8.13107e-05,5561 0.000731797,5818 0.00166687,6464 4.06554e-05,8678 0.000121966,9419 4.06554e-05,9422 0.00060983,9563 0.069602,9633 0.000487864,9634 0.00126032,9692 0.133675,9725 0.0473228,9726 0.0133756,9925 0.000569175,9999 0.044355,10095 0.000365898,10515 0.0015449,11693 0.00378095,12211 0.053909,12245 4.06554e-05,12262 0.00138228,12268 4.06554e-05,12274 0.00317112,12276 0.000243932,12312 0.000121966,12424 0.113672,12441 4.06554e-05,12479 0.0197585,12507 0.0566329,12627 4.06554e-05,12645 0.0921657,12862 0.000121966,13867 0.00390291,13939 4.06554e-05,14921 0.0015449,14971 0.000325243,14984 0.000650486,14988 0.000203277,14995 8.13107e-05,15047 4.06554e-05,15932 0.00134163,16149 0.015571,18277 0.00556978,20690 0.000447209,20797 0.00536651,20848 0.000325243,20940 8.13107e-05,20955 8.13107e-05,20976 0.000487864,21023 0.0207749,21609 8.13107e-05,21627 0.00101638,21646 4.06554e-05,21988 0.0373216,22051 0.00101638,22101 0.000121966,22118 0.00113835,22119 0.00439078,22400 0.000121966,22541 0.000243932,22581 0.00898484,22780 0.000121966,24879 0.00162621,24940 0.0045534,25074 4.06554e-05,25144 0.0123592,25537 0.00430947,25593 0.000975729,25597 4.06554e-05,25737 0.00150425,25800 0.000691141,25939 0.00187015,25970 0.00357767,25972 0.000203277,25996 0.000162621,26023 0.00573241,26078 0.000691141,26123 0.0428508,26134 0.000121966,26399 0.00101638,26575 0.0079278,26699 0.00939139,26810 0.0603326,
}
{
MGYS00002173-SRR2086906.tsv
28:4050 0.0247525,4158 0.618812,4610 0.00990099,9286 0.019802,9422 0.00495049,9563 0.039604,9692 0.019802,9725 0.0148515,9726 0.0148515,9999 0.00990099,12211 0.00495049,12239 0.00495049,12274 0.00495049,12424 0.039604,12507 0.0247525,12645 0.0445545,13867 0.00990099,20976 0.00495049,21023 0.00495049,21627 0.00495049,21988 0.00990099,22119 0.00495049,22541 0.00495049,22581 0.00495049,26023 0.00495049,26123 0.029703,26575 0.00990099,26810 0.00495049,
}
```
5. Format of binnary packaged data

Binnary packaged data is mainly similar to ascii packaged data. The structure is (little endian)

```
20 Byte char Source
4 Byte float Version
8 Byte unsigned_int Size # Count of samples

# At each sample:
38 Byte char Sample_Name
8 Byte unsigned_int SampleSize # Count of sample's abundance
Repeat of 4 Byte unsigned_int Taxa_ID and 4 Byte float abundance pair
```
6. Format of matrix mode result

Result of matrix mode calculation is ascii packaged. The structure is
```
N # count of samples
SampleName1 SampleName2 SampleName3 ... SampleNameN # N samples' name
value11 value12 ... value1N # first line of similarity matrix
...
valueN1 valueN2 ... ValueNN # last line of similarityt matrix
```

7. Format of search mode result

Top N result of  search mode calculation is ascii packaged. The structure is
```
QuerySample1: 1st_Matched_Target_Sample:Similarity, 2nd_Matched_Target_Sample:Similarity, ... Nth_Matched_Target_Sample:Similarity,
QuerySample2: 1st_Matched_Target_Sample:Similarity, 2nd_Matched_Target_Sample:Similarity, ... Nth_Matched_Target_Sample:Similarity,
...
```
Full result of search mode calculation is ascii packaged. The structure is
```
X # Count of Query samples
TargetSampleName1 TargetSampleName2 ... TargetSampleNameN
QuerySample1 SimilarityValue11 SimilarityValue12 ... SimilarityValue1N
QuerySample2 SimilarityValue21 SimilarityValue22 ... SimilarityValue2N
...
QuerySampleX SimilarityValueX1 SimilarityValueX2 ... SimilarityValueXN
```

## Supplement

### Datas

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

## Contact

   Name   |      Email      |      Organization
:--------:|-----------------|--------------------------------------------------------------------------------------------------------------------------------
Kai Kang |kang_kai_kk@icloud.com|Research Assistant, School of Life Science and Technology, Huazhong University of Science & Technology
Hui Chong|huichong.me@gmail.com|Research Assistant, School of Life Science and Technology, Huazhong University of Science & Technology
Kang Ning|ningkang@hust.edu.cn|Professor, School of Life Science and Technology, Huazhong University of Science & Technology
