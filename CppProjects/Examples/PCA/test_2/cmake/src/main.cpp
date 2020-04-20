#include "include/sample.h"

/** @function main */
int main( int argc, char** argv )
{
    String defaultFolder;
    if ( argc != 2 ) {
        cout << "usage: add Folder as Arg -- <Path>" << endl;
        cout << "usage as default: PATH: ./Bilden/test3" << endl;
   	defaultFolder = "./Bilden/test3";
    } else {
        defaultFolder = argv[1];
    }

    vector<String> fileNames;
    read_files(fileNames, defaultFolder); ///Read the images into the current file

    //PCA
    PCA_image(fileNames);

    destroyAllWindows();

    return 0;
}


