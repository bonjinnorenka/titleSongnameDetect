# titleSongnameDetect

This library improves on the content of the URL and supports multiple languages

[https://qiita.com/miyatsuki/items/cb0ce6ae0e48cf42b1c8](https://qiita.com/miyatsuki/items/cb0ce6ae0e48cf42b1c8)
This qiita article is not my article.

## support language

C++
Python
JavaScript

### c++ information

this library depend on boost library.

this library use codecvt when run on windows.

THIS LIBRARY IS NOT HEADER ONLY.

So if you use this library, you should build this library.


## accuracy

this library is not perfect.

test/testdata_large.csv(testdata_large include some mistake data so real accuracy is higher than this data)

c++ 82.8%(6708/8100)

js 80.79%(6542/8097)

python 80.43%(6513/8097)

This data tested from test/testdata_mini.csv(this data is not include mistake data)

C++ 93.2%(1155/1239)

js 94.1%(1166/1238)

python 92.65%(1148/1239)


## advice

if you use this library, tou should longest song name first in past musicnames data in title mathcing.

titleSongname.csv is dataset of this generated from example data and checked correct data.
