# cJSON_demo
##Install

Macos

```
https://github.com/DaveGamble/cJSON.git
mkdir build
cd build
cmake ..
make
make install
```



Linux

```
https://github.com/DaveGamble/cJSON.git
mkdir build
cd build
cmake .. -DENABLE_CJSON_UTILS=On -DENABLE_CJSON_TEST=Off -DCMAKE_INSTALL_PREFIX=/usr
make
make DESTDIR=$pkgdir install
```

##Usage
```
make
./demo 1.json 
```
