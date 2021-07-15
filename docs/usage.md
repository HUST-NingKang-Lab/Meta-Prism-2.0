# Usage
### Prepare phylogeny tree:
Meta prism 2.0 needs the evolutionary tree of `Newick` format as the basis of calculation. It is recommended to use `SILVA` phylogenetic tree, or set the phylogenetic tree according to the sequencing results or use requirements. Use  `  -t [path]` or `--tree [path] `   to select the phylogenetic tree path.
### Load data:
Meta prism 2.0 supports taxa abundance data, and provides special packaged data format with two types (ascii and binary) to store the abundance more efficiently. Note that packaged data is bound to the specific phylogenetic tree.

`--load(-l) [list|mat|ascii|binary] [path]`

### Package data:
Meta prism 2.0 can package the abundance data to ascii or binary format for efficient storage and fast loading.
Ascii format packaged data is easy to read and edit, while binary format data is more efficient in terms of space and time usage.

`--package(-p) [ascii|binary] [path]`

Note that the packaged data is bound to the phylogenetic tree and cannot be used with another phylogenetic tree.

### Merge data:
Meta prism 2.0 can load multiple database and merge these together by command:

`--merge + [[single|list|mat|ascii|binary] + [sample path],...]`

For example:

`./bin/Meta-Prism2.0 --tree [TreePath] -l mat [matPath] -merge ascii [Path1] binnary [Path2] -p [PackagePath]`

### Calculate similarity matrix:
Meta prism 2.0 can load the data of microbial community samples and calculate the similarity matrix between samples.
```
--matrix(-m) ## Calculate similarity matrix
--output(-o) [path] ## Result output path
--threads(-T) [Number] ## Threads for calculathon
```

For example:

`./bin/Meta-Prism2.0 --tree [TreePath] -l mat [matPath] -m -o [ResultPath]`

This command will open mat file and read all samples, calculate the similarity matrix, and save it to ResultPath.
### Database search:
> Meta prism 2.0 can search one or more microbial samples for another group of microbial samples, and return the top n sample name with the highest similarity and similarity. Relevant options are:  
```
--search(-s) [single|list|mat|ascii|binary] [search sample path] [select number|f] ## Load and search samples then return top N (default=5, input char 'f' will return full result as matrix) similar samples
--output(-o) [path] ## Result output path
--threads(-T) [Number] ## Threads for calculathon
```

For example:

`./bin/Meta-Prism_2.0 --tree [TreePath] -l ascii [PdataPath] -s single [FilePath] 3 -o [ResultPath]`

Multiple samples can be read from the packaged file with ascii type as the database, searched with one sample in FilePath, returned the three best matched samples, and output to ResultPath.


## Note

* Many phylogenetic tree has notation in file, including the SILVA phylogenetic tree. You may want to delete the notations to use the program.
* Our packaged data format is combined with the phylogenetic tree. After you packaged samples from one tree, you cannot perform calculation using the data with another phylogenetic tree.
