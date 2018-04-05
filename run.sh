sudo rm -rf /boot/*-3.16.0*
make -j 6
sudo make modules_instll
sudo make install
sudo shutdown -r now
