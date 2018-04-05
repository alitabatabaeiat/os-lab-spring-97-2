sudo rm -rf /boot/*-3.16.0*
make -j 6
sudo make -j 6 modules_install
sudo make -j 6 install
sudo shutdown -r now
