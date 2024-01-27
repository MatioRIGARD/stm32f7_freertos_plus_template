# Mosquitto usage

This document purpose is to detail a basic usage of mosquitto to be able to run the MQTT example of this template.

## Basics

Install:

```bash
sudo apt-get update
sudo apt-get install mosquitto mosquitto-clients
```

The daemon should start immediately. Check the status with:

```bash
sudo systemctl status mosquitto.service
```

If it's not started, you can use this command:

```bash
sudo systemctl start mosquitto.service
```

Once started, you can sub to a topic with:

```bash
mosquitto_sub -h localhost -t "my/topic"
```

Next you can publish on a topic:

```bash
mosquitto_pub -h localhost -t "my/topic" -m "Hello, MQTT!"
```

Finaly, you can stop the broker with:

```bash
sudo systemctl stop mosquitto.service
```

Personnaly, I prefer disable the daemon and start it manually when needed when developping:

```bash
sudo systemctl disable mosquitto.service
```

It is also possible to run mosquitto directly using:

```bash
sudo mosquitto
```

## Just what you need for this example

TODO

## About the configuration file

The default conf file is located in `/etc/mosquitto/mosquitto.conf`, but you can use your own conf file by using:

```bash
mosquitto -c /path/to/my_own_mosquitto.conf
```

You can have more infos about available fields using:

```bash
man mosquitto.conf
```

## Use a password

You may need to use a password file for more secure connexions to broker. To create a new password file, use:

```bash
touch /path/to/passwordfile.pass
mosquitto_passwd -c /path/to/passwordfile.pass user_name
```


## Configuration example

For this example, I changed default port to 44444 port number, created a password file and added its path to my own conf file, and chage log destination:

```bash
# Create password file
touch user01_password.pass
# create password for user01 (pass: password)
mosquitto_passwd mosquitto_passwords.pass user01
# Then, the tool will ask you to enter password and confirm password
# Same thing for user02 (pass: password2)
mosquitto_passwd mosquitto_passwords.pass user02
# Now the mosquitto_passwords.pass file contains the hash of the password for each user
# Create the mosquitto conf file:
touch mosquitto.conf
# Create the mosquitto log file:
touch mosquitto.log
```

Here is the `mosquitto.conf` file I use:

```conf
###############################
## Custom conf file for demo ##
###############################

# default path for pid
pid_file /run/mosquitto/mosquitto.pid
# disable persistance for light disk size 
persistence false
# will write logs here
log_dest file ./mosquitto.log
# changed port
listener 44444
# anonymous not allowed to connect to broker
allow_anonymous false
# password file path
password_file ./mosquitto_passwords.pass
```

To run the Mosquitto server with this conf:

```bash
sudo mosquitto -c ./mosquitto.conf
```

To send message to a topic with this config:

```bash
mosquitto_pub -h localhost -p 44444 -u user01 -P password -t test_topic -m "new message"
```

To subscribe to a topic with this config:

```bash
mosquitto_sub -h localhost -p 44444 -t test_topic -u "user02" -P "password2"
```


