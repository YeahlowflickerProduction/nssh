cd src
echo "Compiling and exporting NSSH..."
gcc -o nssh nssh.c nssh_lib.c db.c
mv nssh ../nssh
cd ../
echo "Copying to /usr/bin..."
sudo cp nssh /usr/bin/nssh
echo "Complete."
