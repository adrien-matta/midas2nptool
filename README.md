# midas2nptool
A framework for convertion of MIDAS (Daresbury) data file to nptool ROOT tree. For any question, contact me at matta@lpccaen.in2p3.fr

## installation:
The only prerequisit is to have root and nptool installed and configured first.

    > git clone http://github.com/adrien-matta/midas2nptool
    > cd midas2nptool
    > cmake ./
    > make install

Don't forget to source path/to/midas2nptool/midas2nptool.sh from your bashrc file or equivalent.

## basic running:
To run the package:

    - Once your source midas2nptool.sh the executable are available from any place

    - to show the help :
            > midas2nptool -h

    - to convert a file:
        midas2nptool configuration.txt path/to/midasfile path/to/rootfile.root TreeName

    - to convert a folder:
        m2n\_folder configuration,txt path/to/midasfolder/ parth/to/root/folder/ TreeName

NB : m2n\_folder skip file that are already converted, and unzip them if necessary.

## Running a quick test:
You can test the code by running the programme on the provided sample files:
  
    > cd $M2N 
    > midas2nptool  test.txt sample/R40\_0  R40\_0.root TestTree
    > mkdir root
    > m2n\_folder test.txt sample/ root/ TestTree

Have Fun!
