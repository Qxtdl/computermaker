rm -rf release

mkdir -p release
make TARGET=linux
rm -rf build/src
cp build -r release/linux

make TARGET=windows
rm -rf build/src
cp build -r release/windows
