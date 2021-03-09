# D-Bus service example with object-level permissions

This service allows to filter method return values and objects listing
based on client process credentials.

# HOW TO USE

In docker container, run:

```
mkdir build && cd build && cmake .. && make && sudo make install

./dbus-service/dbus-service &
# Shows only one string for all clients
../send-dbus-request.sh
# Shows also string special for "testuser"
sudo -u testuser ../send-dbus-request.sh
```
