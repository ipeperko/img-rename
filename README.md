# img_rename

Command line tool for renaming images.

```shell script
$ img_rename --src-dir=/home/user/myimages --format="%D_%T_%m"
``` 

Format options:
- %D capture date (e.g. 20190529)
- %T capture time (e.g. 071542)
- %M manufacturer (e.g. Apple)
- %m model (e.g. iPhone_7)

### Compile

```shell script
$ mkdir build
$ cd build
$ cmake ..
$ make 
$ make install
```
Requires libraries boost, ImageMagick