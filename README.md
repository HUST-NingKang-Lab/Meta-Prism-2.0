# Meta-Prism 2.0
## Introduction
Microbial community samples have been accumulated at a speed faster than ever, with hundreds of thousands of samples been sequenced each year. Mining such a huge amount of multi-source heterogeneous data is becoming an increasingly difficult challenge. Among several data mining bottlenecks, efficient and accurate search of samples is one of the most prominent: Faced with millions of samples in the data repository, traditional sample comparison and search approaches fall short in speed and accuracy.

Here we proposed Meta-Prism 2.0, a microbial community sample search method based on smart pair-wise sample comparison, which pushed the time and memory efficiency to a new limit without compromising accuracy. Based on index-free and memory-saving data structure, time-saving instruction pipeline, and boost scheme optimization, Meta-Prism 2.0 has enabled ultra-fast, memory-efficient, flexible and accurate search among millions of samples. Meta-Prism 2.0 has been put to test on several datasets, with the largest containing one million samples. Results have shown that Meta-Prism 2.0’s 0.00001s per sample pair search speed and 8GB memory needs for searching against one million samples have made it the most efficient sample search method. Additionally, Meta-Prism 2.0 could achieve the accuracy comparable with or better than other contemporary methods. Thirdly, Meta-Prism 2.0 can precisely identify the original biome for samples, thus enabling sample source tracking. It default use [Silva phylogeny tree](https://www.arb-silva.de) and support [EBI MGnify data](https://www.ebi.ac.uk/metagenomics/). It also supports custom sequencing evolutionary trees. It can calculate the similarity matrix of microbiome samples and search the microbiome database. Because of the improvement of calculation performance compared with [Meta-Prism 1.0](https://github.com/HUST-NingKang-Lab/metaPrism), 2.0 doesn’t need index system but instead execute exhaustive search.

![The_Meta-Prism_2.0_pipeline](https://www.biorxiv.org/content/biorxiv/early/2020/11/20/2020.11.17.387811/F1.large.jpg?width=800&height=600&carousel=1)

Meta-Prism 2.0 has been put to test on several datasets, with largest containing one million samples. Results have shown that firstly, as a distance-based method, Meta-Prism 2.0 is not only faster than other distance-based methods, but also faster than unsupervised methods. Its 0.00001s per sample pair search speed, as well as 8GB memory needs for searching against one million samples, have enabled it to be the most efficient method for sample comparison. Additionally, Meta-Prism 2.0 could achieve the comparison accuracy and search precision that are comparable or better than other contemporary methods. Thirdly, Meta-Prism 2.0 can precisely identify the original biome for samples, thus enabling sample source tracking.

![Time_and_memory_usage](https://www.biorxiv.org/content/biorxiv/early/2020/11/20/2020.11.17.387811/F4.large.jpg?width=800&height=600&carousel=1)

In summary, Meta-Prism 2.0 has changed the resource-intensive sample search scheme to an effective procedure, which could be conducted by researchers everyday even on a laptop, for insightful sample search and knowledge discovery. Our work is preprinted at [biorxiv](https://www.biorxiv.org/content/10.1101/2020.11.17.387811v1).

## Requirement
We successfully compiled on CentOS 7.6 by gcc 4.8.5 and macOS 10.15 by clang 11.0.0.

## Installation

- to clone a local repository, run
```bash
git clone https://github.com/HUST-NingKang-Lab/Meta-Prism-2.0.git
```
- to install, enter repository folder and run
```bash
make
```

- to uninstall, run
```bash
make clean
```

## QuickStart
Here are three commands to start using Meta-Prism 2.0. We provided example datasets to help you startup, but please note that this tutorial needs you have [wget](https://www.gnu.org/software/wget/) and the [Meta-Prism 2.0](https://github.com/HUST-NingKang-Lab/Meta-Prism-2.0#Installation) softwares installed first.

- download example datasets
```bash
wget -c https://github.com/HUST-NingKang-Lab/Meta-Prism-2.0/releases/download/datas/Skin_samples.ascii_packaged.pdata
wget -c https://github.com/HUST-NingKang-Lab/Meta-Prism-2.0/releases/download/datas/Oral_samples.ascii_packaged.pdata
```

- download phylogenetic tree
```bash
wget -c https://github.com/HUST-NingKang-Lab/Meta-Prism-2.0/releases/download/datas/LTPs132_SSU_tree.txt
```

- search a dataset against another in search mode
```bash
./bin/Meta-Prism2.0 --tree LTPs132_SSU_tree.txt -l ascii Skin_samples.ascii_packaged.pdata -s ascii Oral_samples.ascii_packaged.pdata 5 -o Skin_against_Oral.txt
```

- N-against-N comparison in matrix mode
```bash
./bin/Meta-Prism2.0 -m --tree LTPs132_SSU_tree.txt -l ascii Oral_samples.ascii_packaged.pdata -o Oral_SimMatrix.txt
```

## Resources

We provide the SILVA phylogenetic tree, an example taxa abundance data, and an example ascii packaged data.

| Data                | Example                                                      |
| :------------------ | :----------------------------------------------------------- |
| Phylogenetic tree   | [at SILVA (with header)](https://www.arb-silva.de/fileadmin/silva_databases/living_tree/LTP_release_132/LTPs132_SSU_tree.newick)  or [our local backup (without header)](https://github.com/HUST-NingKang-Lab/Meta-Prism-2.0/releases/download/datas/LTPs132_SSU_tree.txt). |
| taxa abundance data | [at MGnify](https://www.ebi.ac.uk/metagenomics/api/v1/analyses/MGYA00001775/file/ERR358543_FASTQ_otu.tsv) or [our local backup](https://github.com/HUST-NingKang-Lab/Meta-Prism-2.0/releases/download/v1.0-with-data/MGYS00000337-ERR358543.tsv). |
| ascii packaged data | [100 samples](https://github.com/HUST-NingKang-Lab/Meta-Prism-2.0/releases/download/datas/100samples.ascii_packaged.pdata) or [1000 samples](https://github.com/HUST-NingKang-Lab/Meta-Prism-2.0/releases/download/datas/1000samples.ascii_packaged.pdata) |

Datasets used  in our study:

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

Feel free to contact us if you have any question. Thank you for using Meta-Prism 2.0.

   Name   |      Email      |      Organization
:--------:|-----------------|--------------------------------------------------------------------------------------------------------------------------------
Kai Kang |kang_kai_kk@icloud.com|Research Assistant, School of Life Science and Technology, Huazhong University of Science & Technology
Hui Chong|huichong.me@gmail.com|Research Assistant, School of Life Science and Technology, Huazhong University of Science & Technology
Kang Ning|ningkang@hust.edu.cn|Professor, School of Life Science and Technology, Huazhong University of Science & Technology
