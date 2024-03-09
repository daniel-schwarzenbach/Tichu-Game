if [ ! -d ./build ]
then
    mkdir ./build
fi

if [ ! -d ./sockpp/cmake-build-debug ]
then
    mkdir ./sockpp/build
fi

cd ./sockpp/build/
cmake -DCMAKE_BUILD_TYPE=debug ../
make
cd ../../

#
cd ./build 
cmake -DCMAKE_BUILD_TYPE=debug ../
make

echo compilation finished!