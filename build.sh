if [ ! -d ./build ]
then
    mkdir ./build
fi

if [ ! -d ./sockpp/cmake-build-debug ]
then
    mkdir ./sockpp/build
fi

echo "Compiling sockpp library..."

cd ./sockpp/build/
cmake -DCMAKE_BUILD_TYPE=debug ../
make
cd ../../

echo "Compiling the main project..."
#
cd ./build 
cmake -DCMAKE_BUILD_TYPE=debug ../
make

echo compilation finished!